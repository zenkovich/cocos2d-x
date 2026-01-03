#include "o2/stdafx.h"
#include "o2/O2.h"
#include "o2/Utils/Memory/MemoryAnalyzer.h"
#include "o2Editor/EditorApplication.h"
#include "o2Editor/EditorConfig.h"
#include "o2Editor/ToolsPanel.h"
#include "o2Editor/Windows/WindowsManager.h"
#include "../../cocos/base/o2Integration/CCEditorApplication.h"

using namespace o2;

DECLARE_SINGLETON(Editor::WindowsManager);
DECLARE_SINGLETON(Editor::EditorConfig);
DECLARE_SINGLETON(Editor::ToolsPanel);

extern void InitializeTypeso2Editor();
extern void InitializeTypeso2Integration();

int main()
{
    o2::MemoryAnalyzer::enabledObjectsTracking = false;
    InitializeTypeso2Editor();
    InitializeTypeso2Integration();
	INITIALIZE_O2;
    o2::MemoryAnalyzer::enabledObjectsTracking = true;

	auto app = mmake<CocosEditorApplication>();
    app->Initialize();
    app->Launch();

	return 0;
}
