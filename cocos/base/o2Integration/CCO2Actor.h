#pragma once

#include "base/CCRefPtr.h"

#include "o2/Utils/Serialization/DataValue.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Scene/Actor.h"

namespace cocos2d
{
    class Node;
    class Sprite;
}

// Actor wrapper that owns a cocos node
class O2CocosActor : public o2::Actor
{
public:
    O2CocosActor(o2::RefCounter* refCounter, cocos2d::Node* node = nullptr);

    void SetNode(cocos2d::Node* node);
	cocos2d::Node* GetNode() const;

	void Update(float dt) override;

	SERIALIZABLE(O2CocosActor);

private:
	void CreateTestSprite();

	// Called when actor is drawing
	void OnDraw() override;

	// Called when transformation was updated
	void OnTransformUpdated() override;

private:
    cocos2d::Node* mNode = nullptr;
    cocos2d::Sprite* mSprite = nullptr;
    float mSpinAngle = 0.0f;
};
// --- META ---

CLASS_BASES_META(O2CocosActor)
{
    BASE_CLASS(o2::Actor);
}
END_META;
CLASS_FIELDS_META(O2CocosActor)
{
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mNode);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mSprite);
    FIELD().PRIVATE().DEFAULT_VALUE(0.0f).NAME(mSpinAngle);
}
END_META;
CLASS_METHODS_META(O2CocosActor)
{

    FUNCTION().PUBLIC().SIGNATURE(void, SetNode, cocos2d::Node*);
    FUNCTION().PUBLIC().SIGNATURE(cocos2d::Node*, GetNode);
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
    FUNCTION().PRIVATE().SIGNATURE(void, CreateTestSprite);
    FUNCTION().PRIVATE().SIGNATURE(void, OnDraw);
    FUNCTION().PRIVATE().SIGNATURE(void, OnTransformUpdated);
}
END_META;
// --- END META ---
