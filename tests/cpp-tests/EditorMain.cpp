#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <WinSock2.h>
#include <Windows.h>
#include <tchar.h>
#endif

#include "o2/stdafx.h"
#include "o2/O2.h"
// Complete types for Ref<> instantiations pulled in through the editor headers
#include "o2/Assets/Assets.h"
#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Scene/CameraActor.h"
#include "base/o2Integration/CCO2SceneActor.h"
#include "o2/Utils/Memory/MemoryAnalyzer.h"
#include "o2Editor/EditorApplication.h"
#include "o2Editor/EditorConfig.h"
#include "o2Editor/ToolsPanel.h"
#include "o2Editor/Windows/WindowsManager.h"

#include "Classes/TestsO2EditorApplication.h"

using namespace o2;

DECLARE_SINGLETON(Editor::WindowsManager);
DECLARE_SINGLETON(Editor::EditorConfig);
DECLARE_SINGLETON(Editor::ToolsPanel);

extern void InitializeTypeso2Editor();
extern void InitializeTypeso2Integration();

namespace
{
	int RunEditor()
	{
		o2::MemoryAnalyzer::enabledObjectsTracking = false;
		// INITIALIZE_O2 must run before project type registration (o2 master contract)
		INITIALIZE_O2;
		InitializeTypeso2Editor();
		InitializeTypeso2Integration();
		o2::MemoryAnalyzer::enabledObjectsTracking = true;

		auto app = mmake<TestsO2EditorApplication>();
		app->Initialize();
		app->Launch();

		return 0;
	}
}

#if defined(_WIN32)
int WINAPI _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	return RunEditor();
}
#else
int main(int argc, char** argv)
{
	return RunEditor();
}
#endif
