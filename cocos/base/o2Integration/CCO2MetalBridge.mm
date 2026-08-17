#include "platform/CCPlatformConfig.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC

#import <MetalKit/MetalKit.h>
#import <QuartzCore/CAMetalLayer.h>

#include "renderer/backend/metal/DeviceMTL.h"
#include "renderer/backend/metal/Utils.h"

#include "o2/Application/Mac/ApplicationPlatformWrapper.h"
#include "o2/Render/Render.h"
#include "o2/Render/Mac/MetalWrappers.h"

// ---------------------------------------------------------------------------
// Mac Metal bridge for hosting the cocos renderer inside the o2 editor.
//
// Both engines render in turns within one frame, into the same texture (o2's
// currently bound render target — the Scene window texture) and on the same
// MTLCommandQueue, so GPU passes execute strictly in submission order:
//   o2 pass(es) -> commit -> cocos passes (own command buffer) -> commit ->
//   o2 continues on a fresh command buffer with LoadAction=Load.
// No intermediate render texture is introduced by the integration itself.
// ---------------------------------------------------------------------------

// Registers o2's CAMetalLayer and command queue with the cocos Metal device.
// Must run after o2's render initialization and before the cocos renderer
// creates its device (Director::setOpenGLView).
void O2CocosRegisterMetalDevice()
{
    cocos2d::backend::DeviceMTL::setCAMetalLayer((CAMetalLayer*)o2::ApplicationPlatformWrapper::view.layer);
    cocos2d::backend::DeviceMTL::setExternalCommandQueue(o2::RenderDevice::commandQueue);
}

// Splits o2's frame so cocos can render next: flushes and commits everything o2
// has encoded so far, opens a fresh o2 command buffer for the rest of the frame,
// and points the cocos "screen" attachment at o2's current render target.
// Returns false when there is nothing to render into (recording/threaded mode).
bool O2CocosBeginExternalRender()
{
    using namespace o2;

    if (o2Render.IsMultithreadedRenderEnabled())
        return false;

    if (!RenderDevice::commandBuffer)
        return false;

    // The target: o2's currently bound render target (the Scene window texture),
    // or the frame drawable when no target is bound
    id<MTLTexture> color = nil;
    if (TextureRef renderTarget = o2Render.GetRenderTexture())
        color = renderTarget->GetPlatformTextureImpl()->texture;
    else if (RenderDevice::view.currentDrawable)
        color = RenderDevice::view.currentDrawable.texture;

    if (!color)
        return false;

    // Close the open o2 pass and commit its command buffer: the cocos command
    // buffer created on the shared queue must land after everything o2 drew so far
    if (RenderDevice::renderEncoder)
    {
        [RenderDevice::renderEncoder endEncoding];
        RenderDevice::renderEncoder = nil;
    }

    [RenderDevice::commandBuffer commit];
    RenderDevice::commandBuffer = [RenderDevice::commandQueue commandBuffer];
    RenderDevice::commandBuffer.label = @"AfterExternal";

    // cocos always attaches depth+stencil to its default target; keep one sized
    // to the color target (o2 render target depth is depth-only, format differs)
    static id<MTLTexture> depthStencilTexture = nil;
    if (!depthStencilTexture ||
        depthStencilTexture.width != color.width ||
        depthStencilTexture.height != color.height)
    {
        [depthStencilTexture release];

        MTLTextureDescriptor* descriptor = [MTLTextureDescriptor
            texture2DDescriptorWithPixelFormat:cocos2d::backend::Utils::getDefaultDepthStencilAttachmentPixelFormat()
                                         width:color.width
                                        height:color.height
                                     mipmapped:NO];
        descriptor.usage = MTLTextureUsageRenderTarget;
        descriptor.storageMode = MTLStorageModePrivate;
        depthStencilTexture = [RenderDevice::device newTextureWithDescriptor:descriptor];
    }

    cocos2d::backend::DeviceMTL::setExternalRenderTarget(color, depthStencilTexture);
    return true;
}

// Ends the cocos slot of the frame; o2 lazily opens a new pass on the same
// attachments with LoadAction=Load, so the cocos content stays in the target
void O2CocosEndExternalRender()
{
    cocos2d::backend::DeviceMTL::setExternalRenderTarget(nil, nil);
}

#endif
