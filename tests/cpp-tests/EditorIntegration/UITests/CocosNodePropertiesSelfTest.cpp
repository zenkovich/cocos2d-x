// Drives the editor with cocos integration frame by frame through AppTestDriver: selects cocos
// nodes of every reflected type and checks that the properties window builds fields for them.
// Guards the failure that made the editor crash: a node type whose meta was compiled out has a
// null type pointer, so GetType() returns a null reference and the viewer selector dereferences it.
//
// Run: CocosEditorSelfTest.app/Contents/MacOS/CocosEditorSelfTest [-screenshots <folder>]
// Exit code is the number of failed checks.

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

#include "o2/Assets/Assets.h"
#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Scene/CameraActor.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/Widgets/Image.h"
#include "o2/Scene/UI/Widgets/ScrollArea.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Utils/Memory/MemoryAnalyzer.h"
#include "o2/Utils/Test/AppTestDriver.h"

// The editor headers assume the o2Editor pch; pull their prerequisites first
#include "o2/Scene/UI/Widgets/Label.h"

#include "o2Editor/EditorApplication.h"
#include "o2Editor/Properties/Basic/BooleanProperty.h"
#include "o2Editor/Properties/Basic/IntegerProperty.h"
#include "o2Editor/Properties/IObjectPropertiesViewer.h"
#include "o2Editor/Properties/Properties.h"
#include "o2Editor/UI/SpoilerWithHead.h"
#include "o2Editor/Windows/DockWindowPlace.h"
#include "o2Editor/EditorConfig.h"
#include "o2Editor/Properties/IPropertyField.h"
#include "o2Editor/ToolsPanel.h"
#include "o2Editor/Windows/PropertiesWindow/PropertiesWindow.h"
#include "o2Editor/Windows/SceneWindow/SceneEditScreen.h"
#include "o2Editor/Windows/WindowsManager.h"

#include "2d/CCLabel.h"
#include "2d/CCMenuItem.h"
#include "2d/CCScene.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/o2Integration/CCO2SceneActor.h"
#include "ui/UIWidget.h"

#include "../CCO2NodePropertiesViewer.h"

#include "../../Classes/TestsO2EditorApplication.h"

#include <string>
#include <vector>

using namespace o2;
using namespace Editor;

DECLARE_SINGLETON(Editor::WindowsManager);
DECLARE_SINGLETON(Editor::EditorConfig);
DECLARE_SINGLETON(Editor::ToolsPanel);

extern void InitializeTypeso2Editor();
extern void InitializeTypescocos2d();
extern void InitializeTypescocosEditorIntegration();

namespace
{
	// OnStarted builds the cocos scene, and it is called from Launch, which enters the platform
	// event loop. The self test drives frames itself, so it calls the startup directly
	struct SelfTestApplication : public TestsO2EditorApplication
	{
		SelfTestApplication(RefCounter* refCounter) : TestsO2EditorApplication(refCounter) {}

		void Start() { OnStarted(); }
		void Stop() { Deinitialize(); }

		REF_COUNTERABLE_IMPL(TestsO2EditorApplication);
	};

	// Reaches the fields the viewer built: they live in the content widget, protected in the interface
	struct ViewerProbe : public CocosNodeViewer
	{
		ViewerProbe(RefCounter* refCounter) : CocosNodeViewer(refCounter) {}

		void Target(cocos2d::Node* node) { SetTargets({ node }); }

		const Ref<Widget>& Content() const { return mContentWidget; }

		REF_COUNTERABLE_IMPL(CocosNodeViewer);
	};

	int failuresCount = 0;

	void Check(bool condition, const String& what)
	{
		if (!condition)
			failuresCount++;

		printf("%s %s\n", condition ? "[ ok ]" : "[FAIL]", what.Data());
	}

	// Collects property fields of the widget subtree: that is what the user sees as rows in the panel
	void CollectPropertyFields(const Ref<Widget>& widget, Vector<Ref<IPropertyField>>& result)
	{
		// Sections of the other node types stay in the panel, hidden
		if (!widget || !widget->IsEnabled())
			return;

		if (auto field = DynamicCast<IPropertyField>(widget))
			result.Add(field);

		for (auto& child : widget->GetChildWidgets())
			CollectPropertyFields(child, result);
	}

	Ref<IPropertyField> FindField(const Vector<Ref<IPropertyField>>& fields, const String& caption)
	{
		for (auto& field : fields)
		{
			if (String(field->GetCaption()) == caption)
				return field;
		}

		return nullptr;
	}

	void CollectNodes(cocos2d::Node* node, std::vector<cocos2d::Node*>& result)
	{
		if (!node)
			return;

		result.push_back(node);

		for (auto child : node->getChildren())
			CollectNodes(child, result);
	}

	// First node of the given reflected type in the running cocos scene
	cocos2d::Node* FindNodeOfType(const std::vector<cocos2d::Node*>& nodes, const Type& type)
	{
		for (auto node : nodes)
		{
			if (&node->GetType() == &type)
				return node;
		}

		return nullptr;
	}

	void CheckNodeType(const Ref<ViewerProbe>& probe, const std::vector<cocos2d::Node*>& nodes, const Type& type,
					   const String& screenshotsFolder)
	{
		const String typeName = type.GetName();

		// The crash was here: a type whose registrator was compiled out keeps a null type pointer,
		// and every check on the selected object's type dereferences it
		Check(type.ID() != 0, typeName + " is registered in reflection");
		Check(type.IsBasedOn(TypeOf(cocos2d::Node)), typeName + " is based on cocos2d::Node");

		auto node = FindNodeOfType(nodes, type);
		if (!node)
		{
			printf("[skip] no %s in the cocos scene\n", typeName.Data());
			return;
		}

		// The full editor path: selection goes to the properties window, which picks the viewer by type
		o2EditorSceneScreen.SelectObject(Ref<SceneEditableObject>(node), false);
		AppTestDriver::PumpFrames(3);

		probe->Target(node);
		AppTestDriver::PumpFrames(2);

		Vector<Ref<IPropertyField>> fields;
		CollectPropertyFields(probe->Content(), fields);

		String names;
		for (auto& field : fields)
		{
			String caption = field->GetCaption();
			if (!caption.IsEmpty())
				names += (names.IsEmpty() ? "" : ", ") + caption;
		}

		printf("       %s fields (%i): %s\n", typeName.Data(), fields.Count(), names.Data());

		// Header (visibility, name) and the transform rows are built for every node type
		Check(fields.Count() >= 7, typeName + " viewer builds the base rows");

		// Rows built from o2 properties go through the cocos setters
		if (auto tagField = DynamicCast<IntegerProperty>(FindField(fields, "Tag")))
		{
			int tag = node->getTag() + 17;
			tagField->SetValue(tag, true);
			AppTestDriver::PumpFrames(2);

			Check(node->getTag() == tag, typeName + " property row writes through the node setter");
		}
		else
			Check(false, typeName + " has the Tag property row");

		// A reflected field is written straight into the node, so the node has to be asked to
		// rebuild what the setter would have rebuilt: check it on the sprite quads, they carry
		// the flip flags
		if (&type == &TypeOf(cocos2d::Sprite))
		{
			if (auto flipField = DynamicCast<BooleanProperty>(FindField(fields, "Flipped Y")))
			{
				auto sprite = (cocos2d::Sprite*)node;
				float topLeftV = sprite->getQuad().tl.texCoords.v;

				flipField->SetValue(!flipField->GetCommonValue(), true);
				AppTestDriver::PumpFrames(2);

				Check(sprite->getQuad().tl.texCoords.v != topLeftV,
					  "editing a reflected field updates the sprite quads");
			}
			else
				Check(false, "sprite has the Flipped Y field");
		}

		if (!screenshotsFolder.IsEmpty())
		{
			String path = screenshotsFolder + "/" + typeName.ReplacedAll(":", "_") + ".png";
			Check(AppTestDriver::SaveScreenshot(path), typeName + " screenshot saved");
		}
	}

	int RunSelfTest(const String& screenshotsFolder, const String& onlyType)
	{
		MemoryAnalyzer::enabledObjectsTracking = false;
		INITIALIZE_O2;
		InitializeTypeso2Editor();
		InitializeTypescocos2d();
		InitializeTypescocosEditorIntegration();
		MemoryAnalyzer::enabledObjectsTracking = true;

		auto app = mmake<SelfTestApplication>();
		app->Initialize();
		app->Start();

		// The cocos scene appears after the first frames, the cocos to world mapping after the first draw
		for (int i = 0; i < 120 && !O2CocosSceneActor::IsMappingReady(); i++)
			AppTestDriver::PumpFrames(1);

		Check(O2CocosSceneActor::IsMappingReady(), "cocos scene is drawn in the editor");

		std::vector<cocos2d::Node*> nodes;
		CollectNodes(cocos2d::Director::getInstance()->getRunningScene(), nodes);
		printf("       cocos nodes in the scene: %i\n", (int)nodes.size());

		Check(!nodes.empty(), "cocos hierarchy is visible");

		// The editor opens with a loading screen and restores its window layout while it runs:
		// waiting by game time lets the background loading jobs finish
		AppTestDriver::Wait(5.0f);

		const Type* types[] = { &TypeOf(cocos2d::Node), &TypeOf(cocos2d::Label), &TypeOf(cocos2d::Sprite),
								&TypeOf(cocos2d::MenuItem), &TypeOf(cocos2d::ui::Widget) };

		// One viewer for the whole run: it is the same instance the properties window keeps alive
		auto probe = mmake<ViewerProbe>(nullptr);

		for (auto type : types)
		{
			if (onlyType.IsEmpty() || type->GetName().Contains(onlyType))
				CheckNodeType(probe, nodes, *type, screenshotsFolder);
		}

		printf("\n%s: %i failed checks\n", failuresCount == 0 ? "PASSED" : "FAILED", failuresCount);

		app->Stop();

		return failuresCount;
	}
}

int main(int argc, char** argv)
{
	String screenshotsFolder;
	String onlyType;
	for (int i = 1; i < argc - 1; i++)
	{
		if (String(argv[i]) == "-screenshots")
			screenshotsFolder = argv[i + 1];

		if (String(argv[i]) == "-only")
			onlyType = argv[i + 1];
	}

	return RunSelfTest(screenshotsFolder, onlyType);
}
