//@CODETOOL_NON_EXCLUDE
#pragma once

#include "platform/CCPlatformConfig.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC

// Mac Metal bridge for rendering cocos inside an o2-hosted window.
// Implemented in CCO2MetalBridge.mm; see there for the frame-splitting scheme.

// Registers o2's CAMetalLayer and command queue with the cocos Metal device.
// Must run after o2 render initialization and before the cocos renderer
// creates its device (Director::setOpenGLView)
void O2CocosRegisterMetalDevice();

// Splits o2's frame so cocos can render next; returns false when there is
// nothing to render into (threaded o2 render or outside of a frame)
bool O2CocosBeginExternalRender();

// Ends the cocos slot of the frame; o2 continues into the same target
void O2CocosEndExternalRender();

#endif
