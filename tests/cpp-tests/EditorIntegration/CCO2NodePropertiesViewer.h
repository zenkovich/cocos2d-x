#pragma once

// The editor headers assume the o2Editor pch; pull their prerequisites first
#include "o2/Scene/UI/Widgets/Label.h"

#include "o2Editor/Windows/PropertiesWindow/IPropertiesViewer.h"
#include "o2/Utils/ValueProxy.h"

namespace cocos2d
{
	class Node;
}

namespace o2
{
	class VerticalLayout;
	class Widget;
}

namespace Editor
{
	class SpoilerWithHead;
	class IObjectPropertiesViewer;
}

// ---------------------------------------------------------------------------
// Properties viewer for cocos2d nodes. Picked automatically by the o2
// properties window for any selected cocos2d::Node (through the o2 reflection
// of the node types). Structured like the actor viewer: a header with enable
// toggle and name, a transform spoiler, and per-node-type property sections.
// All edits go through the node getters/setters
// ---------------------------------------------------------------------------
class CocosNodeViewer : public Editor::IPropertiesViewer
{
public:
	// Default constructor, builds the layout
	CocosNodeViewer(o2::RefCounter* refCounter);

	// Returns viewing object type — cocos2d::Node
	const o2::Type* GetViewingObjectType() const override;

	// Updates property values from the target nodes
	void Refresh() override;

	IOBJECT(CocosNodeViewer);

protected:
	o2::Vector<cocos2d::Node*> mNodes; // Currently viewed nodes

	// Header
	o2::Ref<o2::Widget>             mHeader;          // Header container
	o2::Ref<Editor::IPropertyField> mEnableProperty;  // Visible toggle
	o2::Ref<Editor::IPropertyField> mNameProperty;    // Node name

	// Transform rows, part of the header block
	o2::Ref<Editor::IPropertyField>  mPositionProperty;
	o2::Ref<Editor::IPropertyField>  mSizeProperty;
	o2::Ref<Editor::IPropertyField>  mScaleProperty;
	o2::Ref<Editor::IPropertyField>  mRotationProperty;
	o2::Ref<Editor::IPropertyField>  mAnchorProperty;
	o2::Ref<Editor::IPropertyField>  mZOrderProperty;
	o2::Ref<Editor::IPropertyField>  mColorProperty;

	// Reflection-driven section for the concrete node type: builds fields from the o2
	// reflection of the type, so new node types need no viewer code at all. One section per
	// type, kept alive and just hidden when another type is selected: the viewer owns the
	// spoiler widget, releasing it back to the pool would detach it from this viewer's layout
	struct TypeSection
	{
		o2::Ref<Editor::SpoilerWithHead>         spoiler;
		o2::Ref<Editor::IObjectPropertiesViewer> viewer;

		bool operator==(const TypeSection& other) const { return spoiler == other.spoiler && viewer == other.viewer; }
	};

	o2::Ref<o2::VerticalLayout>    mRootLayout;               // Holds the header, transform and type sections
	o2::Map<const o2::Type*, TypeSection> mTypeSections;      // Built sections by node type
	const o2::Type*                mViewedNodeType = nullptr; // Type of the shown section

	o2::Vector<o2::Ref<Editor::IPropertyField>> mAllFields; // Every field for batch refresh

protected:
	// Sets target objects and rebinds the value proxies to them
	void SetTargets(const o2::Vector<o2::IObject*>& targets) override;

	// Builds the header (enable + name), actor-header styled
	void BuildHeader();

	// Builds the transform spoiler with icon rows
	void BuildTransform(const o2::Ref<o2::VerticalLayout>& rootLayout);

	// Rebuilds the reflection-driven section for the common type of the targets
	void RefreshTypeViewer();

	// Rebinds base node field proxies to the current nodes
	void BindNodeProxies();

	// Reflection writes the node fields directly, bypassing the cocos setters: the nodes are
	// asked to refresh their derived state after every edit in the type section
	void OnTypePropertyChanged(const o2::Ref<Editor::IPropertyField>& field, bool byUser);

	// Same for a completed change, an undo step is recorded from it
	void OnTypePropertyChangeCompleted(const o2::String& path, const o2::Vector<o2::DataDocument>& before,
									   const o2::Vector<o2::DataDocument>& after);
};
// --- META ---

CLASS_BASES_META(CocosNodeViewer)
{
    BASE_CLASS(Editor::IPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(CocosNodeViewer)
{
    FIELD().PROTECTED().NAME(mNodes);
    FIELD().PROTECTED().NAME(mHeader);
    FIELD().PROTECTED().NAME(mEnableProperty);
    FIELD().PROTECTED().NAME(mNameProperty);
    FIELD().PROTECTED().NAME(mPositionProperty);
    FIELD().PROTECTED().NAME(mSizeProperty);
    FIELD().PROTECTED().NAME(mScaleProperty);
    FIELD().PROTECTED().NAME(mRotationProperty);
    FIELD().PROTECTED().NAME(mAnchorProperty);
    FIELD().PROTECTED().NAME(mZOrderProperty);
    FIELD().PROTECTED().NAME(mColorProperty);
    FIELD().PROTECTED().NAME(mRootLayout);
    FIELD().PROTECTED().NAME(mTypeSections);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mViewedNodeType);
    FIELD().PROTECTED().NAME(mAllFields);
}
END_META;
CLASS_METHODS_META(CocosNodeViewer)
{

    FUNCTION().PUBLIC().SIGNATURE(const o2::Type*, GetViewingObjectType);
    FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
    FUNCTION().PROTECTED().SIGNATURE(void, SetTargets, const o2::Vector<o2::IObject*>&);
    FUNCTION().PROTECTED().SIGNATURE(void, BuildHeader);
    FUNCTION().PROTECTED().SIGNATURE(void, BuildTransform, const o2::Ref<o2::VerticalLayout>&);
    FUNCTION().PROTECTED().SIGNATURE(void, RefreshTypeViewer);
    FUNCTION().PROTECTED().SIGNATURE(void, BindNodeProxies);
    FUNCTION().PROTECTED().SIGNATURE(void, OnTypePropertyChanged, const o2::Ref<Editor::IPropertyField>&, bool);
    FUNCTION().PROTECTED().SIGNATURE(void, OnTypePropertyChangeCompleted, const o2::String&, const o2::Vector<o2::DataDocument>&, const o2::Vector<o2::DataDocument>&);
}
END_META;
// --- END META ---
