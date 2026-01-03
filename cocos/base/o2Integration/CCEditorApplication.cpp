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

#include "base/CCDirector.h"
#include "platform/CCGLView.h"

class EditorGLView : public cocos2d::GLView
{
public:
	EditorGLView()
	{
	}


	void end() override
	{
	}


	bool isOpenGLReady() override
	{
		return true;
	}


	void swapBuffers() override
	{
	}


	void setIMEKeyboardState(bool open) override
	{
	}


	HWND getWin32Window() override
	{
		return o2Application.GetWindowHandle();
	}
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

	cocos2d::Director::initializeO2Integration = false;
	mCocosDirector = cocos2d::Director::getInstance();
	mCocosDirector->setOpenGLView(new EditorGLView());
}

void CocosEditorApplication::Deinitialize()
{
	// Deinitialize o2 Editor
	::Editor::EditorApplication::Deinitialize();
}

void CocosEditorApplication::ProcessFrame()
{
	float dt = o2Time.GetDeltaTime();

	PreCocosUpdate(dt);

	::Editor::EditorApplication::ProcessFrame();

	PostCocosUpdate(dt);
}

void CocosEditorApplication::PreCocosUpdate(float dt)
{
	// TODO: Prepare Cocos2d-x subsystems before o2 update
	// - Update Director mainLoop (partial)
	// - Process Cocos events
	// - Update Cocos scheduler
}

void CocosEditorApplication::PostCocosUpdate(float dt)
{
	// TODO: Finalize Cocos2d-x subsystems after o2 update
	// - Render Cocos scene graph
	// - Swap buffers
	// - Cleanup temporary objects
}

void CocosEditorApplication::OnStarted()
{
	o2Debug.Log("CocosEditorApplication: Application started");

	// Call parent implementation
	::Editor::EditorApplication::OnStarted();

	// TODO: Additional Cocos-specific startup
	// - Load Cocos resources
	// - Setup initial Cocos scene
}

void CocosEditorApplication::OnClosing()
{
	o2Debug.Log("CocosEditorApplication: Application closing");

	// TODO: Cocos-specific cleanup
	// - Save Cocos state
	// - Unload Cocos resources

	// Call parent implementation
	::Editor::EditorApplication::OnClosing();
}

