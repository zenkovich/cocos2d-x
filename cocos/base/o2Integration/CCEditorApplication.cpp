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

#include "o2/Render/Render.h"

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
	HWND getWin32Window() override { return mApplication.GetWindowHandle(); }

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

	PreCocosUpdate(dt);

	Editor::EditorApplication::ProcessFrame();

	PostCocosUpdate(dt);
}

void CocosEditorApplication::PreCocosUpdate(float dt)
{
	mCocosDirector->mainLoop();

	o2Render.ResetState();
}

void CocosEditorApplication::PostCocosUpdate(float dt)
{
}

void CocosEditorApplication::OnStarted()
{
	Editor::EditorApplication::OnStarted();
}

void CocosEditorApplication::OnClosing()
{
	Editor::EditorApplication::OnClosing();
}