// Win32: Ensure stable WinSock2/Windows include order before any other headers pull them in.
// This prevents winsock type redefinition errors and macro collisions in headers included later.
#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <WinSock2.h>
#include <Windows.h>
#endif

#include "platform/CCGL.h"
#include "CCEditorApplication.h"

#include "base/o2Integration/CCO2MetalBridge.h"
#include "base/o2Integration/CCO2Integration.h"
#include "base/o2Integration/CCO2SceneActor.h"

#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventKeyboard.h"
#include "platform/CCGLView.h"

#include "o2/Application/Input.h"
#include "o2/Render/Render.h"

namespace Editor
{
	// Defined in o2Editor SceneEditScreen.cpp; plain prototypes avoid pulling
	// editor headers that assume the o2Editor pch
	void RequestSceneViewRedraw();
	bool TryGetSceneCursorWorldPoint(o2::Vec2F& worldPoint);
}

class EditorGLView : public cocos2d::GLView
{
public:
	EditorGLView(CocosEditorApplication& app) :
		mApplication(app)
	{
		_designResolutionSize.width = app.GetContentSize().x;
		_designResolutionSize.height = app.GetContentSize().y;
	}

	void end() override {}
	bool isOpenGLReady() override { return true; }
	void swapBuffers() override {}
	void setIMEKeyboardState(bool open) override {}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	HWND getWin32Window() override { return mApplication.GetWindowHandle(); }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	// o2 owns the native window on Mac; cocos never touches it directly here
	id getCocoaWindow() override { return nullptr; }
	id getNSGLContext() override { return nullptr; }
#endif

protected:
	CocosEditorApplication& mApplication;
};

CocosEditorApplication::CocosEditorApplication(o2::RefCounter* refCounter) :
	::Editor::EditorApplication(refCounter)
{
}

CocosEditorApplication::~CocosEditorApplication()
{
}

void CocosEditorApplication::BasicInitialize()
{
	::Editor::EditorApplication::BasicInitialize();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	// The frame-splitting bridge needs o2's command buffer on the main thread;
	// with the threaded render o2 only records on the main thread and submits
	// elsewhere, so hosted cocos rendering requires the single-threaded path
	o2Render.SetMultithreadedRenderEnabled(false);

	// Must happen before setOpenGLView: the cocos Metal device is created lazily
	// from the registered CAMetalLayer when the renderer initializes
	O2CocosRegisterMetalDevice();
#endif

	mCocosDirector = cocos2d::Director::getInstance();
	mCocosDirector->setOpenGLView(new EditorGLView(*this));
}

void CocosEditorApplication::Deinitialize()
{
	Editor::EditorApplication::Deinitialize();
}

void CocosEditorApplication::ProcessFrame()
{
	float dt = o2Time.GetDeltaTime();

	ProcessCocosInput();

	PreCocosUpdate(dt);

	Editor::EditorApplication::ProcessFrame();

	PostCocosUpdate(dt);
}

void CocosEditorApplication::PreCocosUpdate(float dt)
{
	// The cocos scheduler is pumped by O2CocosSceneActor::Update — the scene root
	// of the integration — so cocos only advances while the editor is in play mode

#if (CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
	// On Mac the cocos scene is drawn by O2CocosSceneActor inside the o2 scene
	// drawing pass; rendering here would fight the shared Metal frame layout
	mCocosDirector->mainLoopRender();

	o2Render.ResetState();
#endif
}

void CocosEditorApplication::PostCocosUpdate(float dt)
{
	// While playing, cocos animates through O2CocosSceneActor::Update — the scene
	// view redraws on demand, so keep it following the live content
	if (IsPlaying())
		::Editor::RequestSceneViewRedraw();
}

void CocosEditorApplication::ProcessCocosInput()
{
	using namespace o2;

	if (!mCocosDirector)
		return;

	auto glView = mCocosDirector->getOpenGLView();
	auto eventDispatcher = mCocosDirector->getEventDispatcher();
	if (!glView || !eventDispatcher)
		return;

	bool playing = IsPlaying();

	// Keyboard: forwarded entirely while playing
	if (playing)
	{
		for (const auto& key : o2Input.GetPressedKeys())
		{
			auto cocosKey = MapO2KeyToCocosKeyCode(key.keyCode);
			if (cocosKey != cocos2d::EventKeyboard::KeyCode::KEY_NONE)
			{
				cocos2d::EventKeyboard event(cocosKey, true);
				eventDispatcher->dispatchEvent(&event);
			}
		}

		for (const auto& key : o2Input.GetReleasedKeys())
		{
			auto cocosKey = MapO2KeyToCocosKeyCode(key.keyCode);
			if (cocosKey != cocos2d::EventKeyboard::KeyCode::KEY_NONE)
			{
				cocos2d::EventKeyboard event(cocosKey, false);
				eventDispatcher->dispatchEvent(&event);
			}
		}
	}

	// Cursor events are handled per-node: interactive cocos nodes register as
	// CursorAreaEventsListener areas in the game camera's listeners layer (see
	// O2CocosSceneActor::RegisterInteractiveNodes), so clicks work only in the
	// Game view, don't leak through editor UI, and are converted into cocos
	// touches by the node listeners themselves
}

void CocosEditorApplication::OnStarted()
{
	Editor::EditorApplication::OnStarted();
}

void CocosEditorApplication::OnClosing()
{
	Editor::EditorApplication::OnClosing();
}