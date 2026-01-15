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

#include <tchar.h>

#include "o2/stdafx.h"
#include "o2/O2.h"
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

int WINAPI _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    o2::MemoryAnalyzer::enabledObjectsTracking = false;
    InitializeTypeso2Editor();
    InitializeTypeso2Integration();
	INITIALIZE_O2;
    o2::MemoryAnalyzer::enabledObjectsTracking = true;

	auto app = mmake<TestsO2EditorApplication>();
    app->Initialize();
    app->Launch();

	return 0;
}