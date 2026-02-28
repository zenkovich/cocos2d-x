//@CODETOOL_NON_EXCLUDE
#pragma once
#include "o2Editor/EditorApplication.h"

namespace cocos2d
{
	class Director;
}

// ---------------------------------------------------------
// Editor application with Cocos2d-x integration
// Combines o2 Editor functionality with Cocos2d-x subsystems
// ---------------------------------------------------------
class CocosEditorApplication : public Editor::EditorApplication
{
public:
	// Constructor
	CocosEditorApplication(o2::RefCounter* refCounter);

	// Destructor
	~CocosEditorApplication();

	// Draws external renderers
	void DrawExternal();

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
