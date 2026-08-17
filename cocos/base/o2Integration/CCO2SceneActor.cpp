#include "base/o2Integration/CCO2SceneActor.h"
#include "base/o2Integration/CCO2MetalBridge.h"

#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "platform/CCGLView.h"
#include "2d/CCNode.h"
#include "2d/CCScene.h"

#include "o2/Render/Render.h"
#include "o2/Scene/Scene.h"
#include "o2/Scene/CameraActor.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2/Utils/System/Time/Time.h"

#include <functional>

O2CocosSceneActor* O2CocosSceneActor::sInstance = nullptr;
o2::Basis O2CocosSceneActor::sCocosToWorld;
bool O2CocosSceneActor::sMappingReady = false;

const o2::Basis& O2CocosSceneActor::GetCocosToWorldBasis()
{
	return sCocosToWorld;
}

bool O2CocosSceneActor::IsMappingReady()
{
	return sMappingReady;
}

void O2CocosSceneActor::ForwardTouchToCocos(const o2::Vec2F& worldPoint, TouchPhase phase)
{
	auto instance = sInstance;
	if (!instance || !instance->mDirector)
		return;

	auto glView = instance->mDirector->getOpenGLView();
	if (!glView)
		return;

	o2::Vec2F designPoint = worldPoint * sCocosToWorld.Inverted();

	// handleTouches* expect view coordinates with y pointing down
	auto designSize = glView->getDesignResolutionSize();
	float touchX = designPoint.x;
	float touchY = designSize.height - designPoint.y;
	intptr_t touchId = 0;

	switch (phase)
	{
		case TouchPhase::Began: glView->handleTouchesBegin(1, &touchId, &touchX, &touchY); break;
		case TouchPhase::Moved: glView->handleTouchesMove(1, &touchId, &touchX, &touchY); break;
		case TouchPhase::Ended: glView->handleTouchesEnd(1, &touchId, &touchX, &touchY); break;
	}
}

O2CocosSceneActor::O2CocosSceneActor(o2::RefCounter* refCounter) :
	o2::Actor(refCounter)
{
	SetName("Cocos2d");
	mDirector = cocos2d::Director::getInstance();
	sInstance = this;
}

O2CocosSceneActor::~O2CocosSceneActor()
{
	if (sInstance == this)
		sInstance = nullptr;
}

O2CocosSceneActor* O2CocosSceneActor::Instance()
{
	return sInstance;
}

cocos2d::Director* O2CocosSceneActor::GetDirector() const
{
	return mDirector;
}

void O2CocosSceneActor::Update(float dt)
{
	o2::Actor::Update(dt);

	if (mDirector)
		mDirector->mainLoopUpdate(dt);
}

o2::Vector<o2::Ref<o2::SceneEditableObject>> O2CocosSceneActor::GetEditableChildren() const
{
	auto children = o2::Actor::GetEditableChildren();

	if (mDirector)
	{
		if (auto scene = mDirector->getRunningScene())
			children.Add(o2::Ref<o2::SceneEditableObject>(scene));
	}

	return children;
}

void O2CocosSceneActor::OnDraw()
{
	using namespace o2;

	if (!mDirector || !mDirector->getOpenGLView())
		return;

	// A camera's Clear is applied lazily when o2 opens its next pass. If nothing was
	// drawn yet (cocos is the first drawable, as in the Game window), the pending
	// clear would land on the pass AFTER cocos and wipe its content — so force a
	// no-op primitive: the flush below opens the o2 pass and consumes the clear
	o2Render.DrawLine(Vec2F(), Vec2F(), Color4(0, 0, 0, 0));

	// Flush pending o2 batches into the current pass before cocos takes over
	o2Render.BeginCustomRender();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	// On Windows both engines share one GL context, so cocos just draws into it.
	// On Mac the frame is split: o2 commits its Metal work, cocos renders its own
	// passes into the same target on the shared queue, then o2 continues
	if (!O2CocosBeginExternalRender())
	{
		o2Render.EndCustomRender();
		return;
	}
#endif

	// Map the cocos design-resolution box into the current o2 camera space; the
	// camera here is whatever view is being drawn — the editor scene view camera
	// or a game CameraActor — so the same math serves both windows
	Vec2I currentResolutionI = o2Render.GetCurrentResolution();
	Vec2F currentResolution = (Vec2F)currentResolutionI;

	Basis defaultCameraBasis(currentResolution * -0.5f, Vec2F::Right() * currentResolution.x, Vec2F().Up() * currentResolution.y);
	Basis camTransf = o2Render.GetCamera().GetBasis().Inverted() * defaultCameraBasis;

	auto cocosDesignResolution = mDirector->getOpenGLView()->getDesignResolutionSize();
	Vec2F cocosResolution(cocosDesignResolution.width, cocosDesignResolution.height);
	Basis cocosResolutionBasis(Vec2F(), Vec2F::Right() * cocosResolution.x, Vec2F().Up() * cocosResolution.y);
	Basis cocosResolutionCorrection = defaultCameraBasis.Inverted() * cocosResolutionBasis;

	Basis finalCamTransf = camTransf * cocosResolutionCorrection;

	// Refresh the cocos → o2 world mapping for editor transforms: run design-space
	// points through the REAL director projection (may be perspective 3D) composed
	// with the unflipped camera mapping, then NDC → camera unit box → o2 world.
	// Camera terms cancel, so the result is camera-independent and stable
	{
		cocos2d::Mat4 unflippedTransform(
			finalCamTransf.xv.x, finalCamTransf.yv.x, 0.0f, finalCamTransf.origin.x,
			finalCamTransf.xv.y, finalCamTransf.yv.y, 0.0f, finalCamTransf.origin.y,
			0.0f,                0.0f,                0.0f, 0.0f,
			0.0f,                0.0f,                0.0f, 1.0f
		);

		cocos2d::Mat4 clipFromDesign =
			mDirector->getMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION) * unflippedTransform;

		auto designToWorld = [&](float x, float y)
		{
			cocos2d::Vec4 clip = clipFromDesign * cocos2d::Vec4(x, y, 0.0f, 1.0f);
			float invW = clip.w != 0.0f ? 1.0f / clip.w : 1.0f;
			Vec2F unit(clip.x * invW * 0.5f + 0.5f, clip.y * invW * 0.5f + 0.5f);
			return o2Render.GetCamera().GetBasis() * unit;
		};

		Vec2F worldOrigin = designToWorld(0.0f, 0.0f);
		sCocosToWorld = Basis(worldOrigin,
		                      designToWorld(1.0f, 0.0f) - worldOrigin,
		                      designToWorld(0.0f, 1.0f) - worldOrigin);
		sMappingReady = true;

		// Give the actor a meaningful frame once: the cocos design box in world
		// space, so selecting it shows a real selection frame instead of a point
		if (!mFrameInitialized)
		{
			mFrameInitialized = true;
			Basis designBox(Vec2F(), Vec2F(cocosResolution.x, 0.0f), Vec2F(0.0f, cocosResolution.y));
			transform->SetSize2D(cocosResolution);
			transform->SetWorldBasis(designBox * sCocosToWorld);
		}
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	// The cocos Metal backend renders y-flipped relative to o2's render target
	// orientation. Mirror the OUTPUT y axis (applied after the whole camera
	// mapping): fixes both the upside-down image and inverted vertical panning
	finalCamTransf.xv.y = -finalCamTransf.xv.y;
	finalCamTransf.yv.y = -finalCamTransf.yv.y;
	finalCamTransf.origin.y = cocosResolution.y - finalCamTransf.origin.y;
#endif

	cocos2d::Mat4 transform(
		finalCamTransf.xv.x, finalCamTransf.yv.x, 0.0f, finalCamTransf.origin.x,
		finalCamTransf.xv.y, finalCamTransf.yv.y, 0.0f, finalCamTransf.origin.y,
		0.0f,                0.0f,                0.0f, 0.0f,
		0.0f,                0.0f,                0.0f, 1.0f
	);

	cocos2d::Viewport viewport{ 0, 0, (unsigned int)currentResolutionI.x, (unsigned int)currentResolutionI.y };
	mDirector->customLoopRender(viewport, transform);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	O2CocosEndExternalRender();
#endif

	o2Render.EndCustomRender();

	RegisterInteractiveNodes();
}

void O2CocosSceneActor::RegisterInteractiveNodes()
{
	using namespace o2;

	// Only while the editor plays
	if (!o2Scene.IsEditorPlaying())
		return;

	// Strictly the game camera pass: the current render camera must belong to one
	// of the scene camera actors. The editor Scene view draws with its own camera
	// (and sometimes with the editor scope force-popped), so scope checks are not
	// reliable — camera identity is
	bool gameCameraPass = false;
	for (auto& camera : o2Scene.GetCameras())
	{
		if (auto cameraActor = camera.Lock())
		{
			if (cameraActor->GetRenderCamera() == o2Render.GetCamera())
			{
				gameCameraPass = true;
				break;
			}
		}
	}

	if (!gameCameraPass)
		return;

	// The same camera can be set up twice per frame (the scene view pipeline and
	// the Game view both draw through it); a second registration in the same
	// listeners layer would double-deliver every cursor event
	static int sLastRegisteredFrame = -1;
	int currentFrame = o2Time.GetCurrentFrame();
	if (currentFrame == sLastRegisteredFrame)
		return;
	sLastRegisteredFrame = currentFrame;

	auto scene = mDirector->getRunningScene();
	if (!scene)
		return;

	auto eventDispatcher = mDirector->getEventDispatcher();

	// OnDrawn registers a listener in the current layer — the game camera's
	// listenersLayer here. Pre-order traversal: children register after the parent
	// and win the pick, matching the draw order on top
	std::function<void(cocos2d::Node*)> visit = [&visit, eventDispatcher](cocos2d::Node* node)
	{
		if (!node->isVisible())
			return;

		// Interactive: explicitly clickable classes (MenuItem, ui::Widget) plus any
		// node with live touch/mouse listeners attached (TableView, custom nodes)
		bool interactive = node->isO2InteractiveNode() ||
			(eventDispatcher && eventDispatcher->hasTouchOrMouseEventListenersForTarget(node));

		if (interactive)
		{
			// The listener must live in the game (non-editor) input scope regardless
			// of where the node was constructed
			node->SetEditorMode(false);
			node->OnDrawn();
		}

		for (const auto& child : node->getChildren())
			visit(child);
	};

	visit(scene);

	// One-time confirmation that the game-pass registration actually happens
	static bool sLoggedOnce = false;
	if (!sLoggedOnce)
	{
		sLoggedOnce = true;
		CCLOG("[O2CocosSceneActor] interactive nodes registered in the game camera pass");
	}
}
// --- META ---

ENUM_META(O2CocosSceneActor::TouchPhase, O2CocosSceneActor__TouchPhase)
{
    ENUM_ENTRY(Began);
    ENUM_ENTRY(Ended);
    ENUM_ENTRY(Moved);
}
END_ENUM_META;

DECLARE_CLASS(O2CocosSceneActor, O2CocosSceneActor);
// --- END META ---
