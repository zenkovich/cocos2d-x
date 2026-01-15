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
#include "TestsO2EditorApplication.h"

#include "base/CCConfiguration.h"
#include "controller.h"
#include "platform/CCFileUtils.h"

TestsO2EditorApplication::TestsO2EditorApplication(o2::RefCounter* refCounter) :
	CocosEditorApplication(refCounter)
{
}

void TestsO2EditorApplication::OnStarted()
{
	CocosEditorApplication::OnStarted();

	using namespace cocos2d;

	// As an example, load config file
	// FIXME:: This should be loaded before the Director is initialized,
	// FIXME:: but at this point, the director is already initialized
	Configuration::getInstance()->loadConfigFile("configs/config-example.plist");

	// initialize director
	auto director = Director::getInstance();
	director->setDisplayStats(true);
	//director->setAnimationInterval(1.0f / 60);

	auto designSize = Size(480, 320);

	auto fileUtils = FileUtils::getInstance();
	std::vector<std::string> searchPaths;

	if (true)
	{
		auto resourceSize = Size(960, 640);
		searchPaths.push_back("hd");
		searchPaths.push_back("ccs-res/hd");
		searchPaths.push_back("ccs-res");
		searchPaths.push_back("Manifests");
		director->setContentScaleFactor(resourceSize.height / designSize.height);

		searchPaths.push_back("hd/ActionTimeline");
	}
	else
	{
		searchPaths.push_back("ccs-res");

		searchPaths.push_back("ActionTimeline");
	}

	fileUtils->setSearchPaths(searchPaths);

	// Enable Remote Console
	auto console = director->getConsole();
	console->listenOnTCP(5678);

	_testController = TestController::getInstance();
}


