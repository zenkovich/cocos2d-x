//@CODETOOL_NON_EXCLUDE
#pragma once
#include "o2/Application/Application.h"

namespace cocos2d
{
	class Director;
	class GLView;
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

	// Process input events and forward them to Cocos2d-x
	void ProcessInputEvents();

	// Convert O2 coordinate system to Cocos coordinate system
	// O2: center = (0,0), Y up
	// Cocos: bottom-left = (0,0), Y up
	o2::Vec2F O2ToCocosCoords(const o2::Vec2F& o2Pos) const;

protected:
	cocos2d::Director* mCocosDirector = nullptr;
	cocos2d::GLView* mCocosGLView = nullptr;

	// Mouse state tracking
	bool mLeftButtonPressed = false;
	bool mRightButtonPressed = false;
	bool mMiddleButtonPressed = false;
	float mLastMouseX = 0.0f;
	float mLastMouseY = 0.0f;
};
