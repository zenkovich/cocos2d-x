#pragma once
#include "base/o2Integration/CCEditorApplication.h"

namespace cocos2d
{
	class Director;
}

class TestController;

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

protected:
	TestController* _testController;
};
