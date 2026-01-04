#include "TestsO2Application.h"

#include "base/CCConfiguration.h"
#include "controller.h"
#include "platform/CCFileUtils.h"

TestsO2Application::TestsO2Application(o2::RefCounter* refCounter) :
	CocosO2Application(refCounter)
{
}

void TestsO2Application::OnStarted()
{
	CocosO2Application::OnStarted();

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


