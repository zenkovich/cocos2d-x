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
#include "CCO2Application.h"

#include "base/CCDirector.h"
#include "platform/CCGLView.h"

#include "o2/Utils/System/Time/Time.h"

class O2GLView : public cocos2d::GLView
{
public:
	O2GLView(CocosO2Application& app):
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
	CocosO2Application& mApplication;
};

CocosO2Application::CocosO2Application(o2::RefCounter* refCounter) :
	o2::Application(refCounter)
{
}

CocosO2Application::~CocosO2Application()
{
}

void CocosO2Application::BasicInitialize()
{
	o2::Application::BasicInitialize();

	mCocosDirector = cocos2d::Director::getInstance();
	mCocosDirector->setOpenGLView(new O2GLView(*this));
}

void CocosO2Application::Deinitialize()
{
	o2::Application::Deinitialize();
}

void CocosO2Application::ProcessFrame()
{
	float dt = o2Time.GetDeltaTime();

	PreCocosUpdate(dt);

	o2::Application::ProcessFrame();

	PostCocosUpdate(dt);
}

void CocosO2Application::PreCocosUpdate(float dt)
{
	mCocosDirector->mainLoop();
}

void CocosO2Application::PostCocosUpdate(float dt)
{
}

void CocosO2Application::OnStarted()
{
	o2::Application::OnStarted();
}

void CocosO2Application::OnClosing()
{
	o2::Application::OnClosing();
}


