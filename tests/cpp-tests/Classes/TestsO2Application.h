#pragma once
#include "base/o2Integration/CCO2Application.h"

namespace cocos2d
{
	class Director;
}

class TestController;

// ---------------------------------------------------------
// Application with Cocos2d-x integration
// Combines o2 Application functionality with Cocos2d-x subsystems
// ---------------------------------------------------------
class TestsO2Application : public CocosO2Application
{
public:
	// Constructor
	TestsO2Application(o2::RefCounter* refCounter);

protected:
	// Calling when application is starting
	void OnStarted() override;

protected:
	TestController* _testController;
};
