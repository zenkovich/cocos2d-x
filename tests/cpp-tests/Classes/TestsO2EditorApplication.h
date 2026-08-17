#pragma once
#include "base/o2Integration/CCEditorApplication.h"

namespace cocos2d
{
	class Director;
}

namespace o2
{
	class CameraActor;
}

class TestController;
class O2CocosSceneActor;

// ---------------------------------------------------------
// Application with Cocos2d-x integration
// Combines o2 Application functionality with Cocos2d-x subsystems
// ---------------------------------------------------------
class TestsO2EditorApplication : public CocosEditorApplication
{
public:
	// Constructor
	TestsO2EditorApplication(o2::RefCounter* refCounter);

protected:
	// Calling when application is starting
	void OnStarted() override;

	// Fits the game camera to the cocos content once the mapping is known
	void ProcessFrame() override;

protected:
	TestController* _testController;

	o2::Ref<O2CocosSceneActor>  _cocosSceneActor; // Cocos integration entry point on the o2 scene
	o2::Ref<o2::CameraActor>    _gameCamera;      // Camera for the Game window view

	bool _gameCameraFitted = false; // Camera framed to the cocos design box once
};
