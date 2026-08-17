#include "CCO2NodePropertiesViewer.h"

#include "2d/CCNode.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Image.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Scene/UI/Widgets/ScrollArea.h"
#include "o2/Utils/Editor/EditorScope.h"

#include "o2Editor/UI/SpoilerWithHead.h"
#include "o2Editor/Properties/IObjectPropertiesViewer.h"
#include "o2Editor/Properties/Properties.h"
#include "o2Editor/Properties/Basic/BooleanProperty.h"
#include "o2Editor/Properties/Basic/StringProperty.h"

using namespace o2;
using namespace Editor;

CocosNodeViewer::CocosNodeViewer(o2::RefCounter* refCounter) :
	IPropertiesViewer(refCounter)
{
	PushEditorScopeOnStack scope;

	auto scrollArea = o2UI.CreateScrollArea("backless");
	*scrollArea->layout = WidgetLayout::BothStretch(0, 0, 15, 0);
	scrollArea->SetViewLayout(Layout::BothStretch());
	scrollArea->SetClippingLayout(Layout::BothStretch());
	scrollArea->name = "scroll area";
	mContentWidget = scrollArea;

	auto rootLayout = o2UI.CreateVerLayout();
	*rootLayout->layout = WidgetLayout::BothStretch(0, 0, 0, 0);
	rootLayout->spacing = 0.0f;
	rootLayout->expandHeight = false;
	rootLayout->expandWidth = true;
	rootLayout->fitByChildren = true;
	rootLayout->baseCorner = BaseCorner::Top;
	scrollArea->AddChild(rootLayout);

	BuildHeader();
	rootLayout->AddChild(mHeader);

	BuildTransform(rootLayout);

	// Reflection-driven section for the concrete node type
	mTypeSpoiler = o2UI.CreateWidget<SpoilerWithHead>();
	mTypeSpoiler->borderBottom = 5;
	mTypeSpoiler->SetCaption("Node");
	mTypeSpoiler->GetIcon()->SetImageName("ui/UI4_component_icon.png");
	mTypeSpoiler->SetExpanded(true);
	rootLayout->AddChild(mTypeSpoiler);
}

void CocosNodeViewer::BuildHeader()
{
	mHeader = mmake<Widget>();
	mHeader->name = "cocos node head";
	mHeader->layout->minHeight = 42;

	mEnableProperty = o2UI.CreateWidget<BooleanProperty>("actor head enable");
	*mEnableProperty->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(1, 0));
	mHeader->AddChild(DynamicCast<Widget>(mEnableProperty));

	mNameProperty = o2UI.CreateWidget<StringProperty>("actor head name");
	*mNameProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 21, 15, 17, 2);
	mHeader->AddChild(DynamicCast<Widget>(mNameProperty));

	mAllFields.Add(mEnableProperty);
	mAllFields.Add(mNameProperty);
}

void CocosNodeViewer::BuildTransform(const o2::Ref<o2::VerticalLayout>& rootLayout)
{
	mTransformSpoiler = o2UI.CreateWidget<SpoilerWithHead>();
	mTransformSpoiler->borderBottom = 5;
	mTransformSpoiler->SetCaption("Transform");
	mTransformSpoiler->GetIcon()->SetImageName("ui/UI4_transform_icon_white.png");
	mTransformSpoiler->SetExpanded(true);
	rootLayout->AddChild(mTransformSpoiler);

	auto addIconRow = [&](const String& iconPath, const Type* type, const String& caption) -> Ref<IPropertyField>
	{
		auto container = mmake<Widget>();
		container->name = caption;
		container->layout->minHeight = 20;
		mTransformSpoiler->AddChild(container);

		auto icon = o2UI.CreateImage(iconPath);
		*icon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		container->AddChild(icon);

		auto field = o2EditorProperties.CreateRegularField(type, caption);
		*DynamicCast<Widget>(field)->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		container->AddChild(DynamicCast<Widget>(field));

		mAllFields.Add(field);
		return field;
	};

	mPositionProperty = addIconRow("ui/UI4_position_icon.png", &TypeOf(Vec2F), "Position");
	mSizeProperty     = addIconRow("ui/UI4_icon_size.png", &TypeOf(Vec2F), "Size");
	mScaleProperty    = addIconRow("ui/UI4_scale_icon.png", &TypeOf(Vec2F), "Scale");
	mRotationProperty = addIconRow("ui/UI4_rotate_icon.png", &TypeOf(float), "Rotation");
	mAnchorProperty   = addIconRow("ui/UI4_pivot_icon.png", &TypeOf(Vec2F), "Anchor");

	// Rows without icons: sorting and color
	auto addPlainRow = [&](const Type* type, const String& caption) -> Ref<IPropertyField>
	{
		auto field = o2EditorProperties.CreateRegularField(type, caption);
		mTransformSpoiler->AddChild(DynamicCast<Widget>(field));
		mAllFields.Add(field);
		return field;
	};

	mZOrderProperty = addPlainRow(&TypeOf(int), "Z order");
	mColorProperty  = addPlainRow(&TypeOf(Color4), "Color");
}

void CocosNodeViewer::RefreshTypeViewer()
{
	// The common type of all targets: the concrete node type when they match,
	// nothing when the selection is mixed
	const Type* commonType = nullptr;
	for (auto node : mNodes)
	{
		const Type* nodeType = &node->GetType();
		if (!commonType)
			commonType = nodeType;
		else if (commonType != nodeType)
		{
			commonType = nullptr;
			break;
		}
	}

	// Nothing to show for a plain Node: its properties are in the header and transform
	if (commonType == &TypeOf(cocos2d::Node))
		commonType = nullptr;

	mTypeSpoiler->SetEnabledForcible(commonType != nullptr);

	if (!commonType)
		return;

	// Rebuild the reflection-driven viewer when the type changes: it creates the
	// fields from the type's reflected properties, so new node types work without
	// any viewer code here
	if (mViewedNodeType != commonType)
	{
		if (mTypeViewer)
			o2EditorProperties.FreeObjectViewer(mTypeViewer);

		mViewedNodeType = commonType;
		mTypeSpoiler->SetCaption(commonType->GetName());

		mTypeViewer = o2EditorProperties.CreateObjectViewer(commonType, "");
		mTypeViewer->CheckCreateSpoiler(mTypeSpoiler);
		mTypeViewer->SetHeaderEnabled(false);
	}

	if (mTypeViewer)
	{
		mTypeViewer->Refresh(mNodes.Convert<Pair<IObject*, IObject*>>([](cocos2d::Node* node)
		{
			return Pair<IObject*, IObject*>(dynamic_cast<IObject*>(node), nullptr);
		}));
	}
}

const o2::Type* CocosNodeViewer::GetViewingObjectType() const
{
	return &TypeOf(cocos2d::Node);
}

void CocosNodeViewer::SetTargets(const o2::Vector<o2::IObject*>& targets)
{
	IPropertiesViewer::SetTargets(targets);

	mNodes = targets.Convert<cocos2d::Node*>([](IObject* object) { return dynamic_cast<cocos2d::Node*>(object); });
	mNodes.RemoveAll([](cocos2d::Node* node) { return node == nullptr; });

	BindNodeProxies();

	RefreshTypeViewer();

	Refresh();
}

void CocosNodeViewer::BindNodeProxies()
{
	auto proxiesFor = [this](auto makeProxy)
	{
		Vector<Ref<IAbstractValueProxy>> proxies;
		for (auto node : mNodes)
			proxies.Add(makeProxy(node));

		return proxies;
	};

	mEnableProperty->SetValueProxy(proxiesFor([](cocos2d::Node* node) -> Ref<IAbstractValueProxy>
	{
		return mmake<FunctionalValueProxy<bool>>(
			[node](bool value) { node->setVisible(value); },
			[node]() { return node->isVisible(); });
	}));

	mNameProperty->SetValueProxy(proxiesFor([](cocos2d::Node* node) -> Ref<IAbstractValueProxy>
	{
		return mmake<FunctionalValueProxy<String>>(
			[node](String value) { node->setName((std::string)value); },
			[node]() { return String(node->getName().c_str()); });
	}));

	mPositionProperty->SetValueProxy(proxiesFor([](cocos2d::Node* node) -> Ref<IAbstractValueProxy>
	{
		return mmake<FunctionalValueProxy<Vec2F>>(
			[node](Vec2F value) { node->setPosition(value.x, value.y); },
			[node]() { auto p = node->getPosition(); return Vec2F(p.x, p.y); });
	}));

	mSizeProperty->SetValueProxy(proxiesFor([](cocos2d::Node* node) -> Ref<IAbstractValueProxy>
	{
		return mmake<FunctionalValueProxy<Vec2F>>(
			[node](Vec2F value) { node->setContentSize(cocos2d::Size(value.x, value.y)); },
			[node]() { auto s = node->getContentSize(); return Vec2F(s.width, s.height); });
	}));

	mScaleProperty->SetValueProxy(proxiesFor([](cocos2d::Node* node) -> Ref<IAbstractValueProxy>
	{
		return mmake<FunctionalValueProxy<Vec2F>>(
			[node](Vec2F value) { node->setScale(value.x, value.y); },
			[node]() { return Vec2F(node->getScaleX(), node->getScaleY()); });
	}));

	mRotationProperty->SetValueProxy(proxiesFor([](cocos2d::Node* node) -> Ref<IAbstractValueProxy>
	{
		return mmake<FunctionalValueProxy<float>>(
			[node](float value) { node->setRotation(value); },
			[node]() { return node->getRotation(); });
	}));

	mAnchorProperty->SetValueProxy(proxiesFor([](cocos2d::Node* node) -> Ref<IAbstractValueProxy>
	{
		return mmake<FunctionalValueProxy<Vec2F>>(
			[node](Vec2F value) { node->setAnchorPoint(cocos2d::Vec2(value.x, value.y)); },
			[node]() { auto p = node->getAnchorPoint(); return Vec2F(p.x, p.y); });
	}));

	mZOrderProperty->SetValueProxy(proxiesFor([](cocos2d::Node* node) -> Ref<IAbstractValueProxy>
	{
		return mmake<FunctionalValueProxy<int>>(
			[node](int value) { node->setLocalZOrder(value); },
			[node]() { return (int)node->getLocalZOrder(); });
	}));

	mColorProperty->SetValueProxy(proxiesFor([](cocos2d::Node* node) -> Ref<IAbstractValueProxy>
	{
		return mmake<FunctionalValueProxy<Color4>>(
			[node](Color4 value)
			{
				node->setColor(cocos2d::Color3B((uint8_t)value.r, (uint8_t)value.g, (uint8_t)value.b));
				node->setOpacity((uint8_t)value.a);
			},
			[node]()
			{
				auto c = node->getColor();
				return Color4((int)c.r, (int)c.g, (int)c.b, (int)node->getOpacity());
			});
	}));
}

void CocosNodeViewer::Refresh()
{
	for (auto& field : mAllFields)
		field->Refresh();
}
// --- META ---

DECLARE_CLASS(CocosNodeViewer, CocosNodeViewer);
// --- END META ---
