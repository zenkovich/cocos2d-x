#pragma once
#include "o2/Application/Application.h"

namespace cocos2d
{
	class Director;
}

// ---------------------------------------------------------
// Application with Cocos2d-x integration
// Combines o2 Application functionality with Cocos2d-x subsystems
// ---------------------------------------------------------
class CocosO2Application : public o2::Application
{
public:
	// Constructor
	CocosO2Application(o2::RefCounter* refCounter);

	// Destructor
	~CocosO2Application();

protected:
	// Initialize Cocos2d-x systems alongside o2 systems
	void BasicInitialize() override;

	// Deinitialize application
	void Deinitialize() override;

	// Cocos-specific update before o2 update
	void PreCocosUpdate(float dt);

	// Cocos-specific update after o2 update
	void PostCocosUpdate(float dt);

	// Override process frame to integrate Cocos main loop
	void ProcessFrame() override;

	// Calling when application is starting
	void OnStarted() override;

	// Calling when application is closing
	void OnClosing() override;

protected:
	cocos2d::Director* mCocosDirector = nullptr;
};
