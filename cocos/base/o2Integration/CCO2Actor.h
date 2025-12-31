#pragma once

#include "base/CCRefPtr.h"

#include "o2/Utils/Serialization/DataValue.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Scene/Actor.h"

namespace cocos2d
{
    class Node;
}

// Actor wrapper that owns a cocos node
class O2CocosActor : public o2::Actor
{
public:
    O2CocosActor(o2::RefCounter* refCounter, cocos2d::Node* node = nullptr);

    void SetNode(cocos2d::Node* node);
    cocos2d::Node* GetNode() const;

	SERIALIZABLE(O2CocosActor);

private:
    cocos2d::Node* mNode;
};
// --- META ---

CLASS_BASES_META(O2CocosActor)
{
    BASE_CLASS(o2::Actor);
}
END_META;
CLASS_FIELDS_META(O2CocosActor)
{
    FIELD().PRIVATE().NAME(mNode);
}
END_META;
CLASS_METHODS_META(O2CocosActor)
{

    FUNCTION().PUBLIC().SIGNATURE(void, SetNode, cocos2d::Node*);
    FUNCTION().PUBLIC().SIGNATURE(cocos2d::Node*, GetNode);
}
END_META;
// --- END META ---
