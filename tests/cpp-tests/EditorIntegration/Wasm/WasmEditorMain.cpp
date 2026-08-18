// Entry point of the editor page: the same o2 editor with the cocos2d integration, running in the
// browser. Everything it reads is packed into the page and mounted at the root of the virtual file
// system, so unlike the desktop build there is no working directory convention to satisfy.

#include <cstdio>

#include <emscripten/emscripten.h>

#include "o2/stdafx.h"
#include "o2/O2.h"

#include "o2/Assets/Assets.h"
#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Scene/CameraActor.h"
#include "o2/Utils/Memory/MemoryAnalyzer.h"

#include "o2Editor/EditorApplication.h"
#include "o2Editor/EditorConfig.h"
#include "o2Editor/ToolsPanel.h"
#include "o2Editor/Windows/WindowsManager.h"

#include "base/CCDirector.h"
#include "base/o2Integration/CCO2SceneActor.h"
#include "base/o2Integration/CCEditorApplication.h"

#include "../../Classes/TestsO2EditorApplication.h"
#include "platform/CCFileUtils.h"

using namespace o2;

DECLARE_SINGLETON(Editor::WindowsManager);
DECLARE_SINGLETON(Editor::EditorConfig);
DECLARE_SINGLETON(Editor::ToolsPanel);

extern void InitializeTypeso2Editor();
extern void InitializeTypescocos2d();
extern void InitializeTypescocosEditorIntegration();

namespace
{
    // /config is the only writable place that survives a reload; the first run starts from the
    // layout shipped with the page
    void PrepareEditorConfig()
    {
        const char* configPath = "/config/EditorConfig.json";
        const char* defaultConfigPath = "/EditorConfigDefault.json";

        if (FILE* existing = fopen(configPath, "rb"))
        {
            fclose(existing);
            return;
        }

        FILE* source = fopen(defaultConfigPath, "rb");
        if (!source)
        {
            printf("editor: no default config to start from\n");
            return;
        }

        FILE* target = fopen(configPath, "wb");
        if (target)
        {
            char buffer[4096];
            size_t read = 0;
            while ((read = fread(buffer, 1, sizeof(buffer), source)) > 0)
                fwrite(buffer, 1, read, target);

            fclose(target);
            printf("editor: config initialized from the packaged default\n");
        }

        fclose(source);
    }
}

int main()
{
    // Unbuffered output: a crash must not swallow the log lines that explain it
    setvbuf(stdout, nullptr, _IONBF, 0);
    setvbuf(stderr, nullptr, _IONBF, 0);

    // Object tracking roughly doubles the allocation cost and pushes the editor past the wasm
    // memory ceiling
    MemoryAnalyzer::enabledObjectsTracking = false;

    PrepareEditorConfig();

    INITIALIZE_O2;
    InitializeTypeso2Editor();
    InitializeTypescocos2d();
    InitializeTypescocosEditorIntegration();

    auto app = mmake<TestsO2EditorApplication>();
    app->Initialize();
    app->Launch();

    return 0;
}
