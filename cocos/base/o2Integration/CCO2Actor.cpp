#include "base/o2Integration/CCO2Actor.h"

#include "2d/CCNode.h"

O2CocosActor::O2CocosActor(o2::RefCounter* refCounter, cocos2d::Node* node) :
    o2::Actor(refCounter)
{
    SetNode(node);
}

void O2CocosActor::SetNode(cocos2d::Node* node)
{
    mNode = node;
}

cocos2d::Node* O2CocosActor::GetNode() const
{
    return mNode;
}
// --- META ---

DECLARE_CLASS(O2CocosActor, O2CocosActor);
// --- END META ---
