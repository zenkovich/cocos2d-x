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
#include "base/o2Integration/CCO2SceneActor.h"
#include "controller.h"
#include "platform/CCFileUtils.h"

#include "o2/Scene/CameraActor.h"

#include <filesystem>

TestsO2EditorApplication::TestsO2EditorApplication(o2::RefCounter* refCounter) :
	CocosEditorApplication(refCounter)
{
}

void TestsO2EditorApplication::OnStarted()
{
	CocosEditorApplication::OnStarted();

	using namespace cocos2d;

#if defined(__EMSCRIPTEN__)
	// Everything the tests read is packed into the page and mounted at /Resources
	FileUtils::getInstance()->setDefaultResourceRootPath("/Resources/");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	// On Windows the build copies cpp-tests Resources next to the executable; on Mac
	// nothing is copied, so point cocos at the sources. Uses the same working-directory
	// convention as the o2 editor asset paths (cwd is five levels below the repo root).
	// The path must be absolute: the apple FileUtils resolves relative roots against
	// the app bundle, not the working directory
	{
		std::error_code pathError;
		auto resourcesPath = std::filesystem::canonical("../../../../../tests/cpp-tests/Resources", pathError);
		if (!pathError)
			FileUtils::getInstance()->setDefaultResourceRootPath(resourcesPath.string() + "/");
	}
#endif

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

	// Enable Remote Console. The browser has no listening sockets to offer
#if !defined(__EMSCRIPTEN__)
	auto console = director->getConsole();
	console->listenOnTCP(5678);
#endif

	_testController = TestController::getInstance();

	// The integration entry point: draws the cocos scene as an o2 scene drawable
	// (with layers/depth support) and exposes the cocos hierarchy to the editor
	_cocosSceneActor = mmake<O2CocosSceneActor>();

	// A camera so the Game window has a view to render the scene (cocos included).
	// Dark background: the cocos test scenes are mostly white-on-transparent and
	// would blend into a white fill
	_gameCamera = mmake<o2::CameraActor>();
	_gameCamera->SetName("GameCamera");
	_gameCamera->fillColor = o2::Color4(45, 45, 48, 255);
}

void TestsO2EditorApplication::ProcessFrame()
{
	CocosEditorApplication::ProcessFrame();

	// Frame the whole cocos design box in the Game view. The cocos→world mapping
	// is known only after the first draw, so the camera is fitted lazily
	if (!_gameCameraFitted && _gameCamera && O2CocosSceneActor::IsMappingReady())
	{
		_gameCameraFitted = true;

		auto designSize = cocos2d::Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
		const o2::Basis& toWorld = O2CocosSceneActor::GetCocosToWorldBasis();

		o2::Vec2F worldSize(toWorld.xv.Length() * designSize.width, toWorld.yv.Length() * designSize.height);
		o2::Vec2F worldCenter = toWorld * o2::Vec2F(designSize.width * 0.5f, designSize.height * 0.5f);

		_gameCamera->SetFittedSize(worldSize);
		_gameCamera->transform->SetWorldPosition2D(worldCenter);
	}
}


