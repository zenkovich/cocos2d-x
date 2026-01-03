#include "base/o2Integration/CCO2Actor.h"

#include "2d/CCNode.h"
#include "2d/CCSprite.h"

O2CocosActor::O2CocosActor(o2::RefCounter* refCounter, cocos2d::Node* node) :
    o2::Actor(refCounter)
{
    SetNode(node);
    CreateTestSprite();
}

void O2CocosActor::SetNode(cocos2d::Node* node)
{
    if (mSprite && mSprite->getParent() && mSprite->getParent() != node)
        mSprite->removeFromParent();

    mNode = node;

    if (mNode && mSprite && !mSprite->getParent())
        mNode->addChild(mSprite);
}

cocos2d::Node* O2CocosActor::GetNode() const
{
    return mNode;
}

void O2CocosActor::Update(float dt)
{
    o2::Actor::Update(dt);

	mSpinAngle += dt * 90.0f;
	mSprite->setRotation(mSpinAngle);
}

void O2CocosActor::CreateTestSprite()
{
    mNode = cocos2d::Node::create();
	mSprite = cocos2d::Sprite::create();

	mSprite->setTextureRect(cocos2d::Rect(0, 0, 64, 64));
	mSprite->setColor(cocos2d::Color3B::GREEN);
	mSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));

	mNode->addChild(mSprite);
}

void O2CocosActor::OnDraw()
{
	mSprite->cocos2d::Node::draw();
}

void O2CocosActor::OnTransformUpdated()
{
	o2::Actor::OnTransformUpdated();

	const auto pos = transform->GetWorldPosition();
	const auto scale = transform->GetScale();
	const float angle = transform->GetAngleDegrees();

	mNode->setPosition(cocos2d::Vec2(pos.x, pos.y));
	mNode->setScale(scale.x, scale.y);
	mNode->setRotation(angle);
}
// --- META ---

DECLARE_CLASS(O2CocosActor, O2CocosActor);
// --- END META ---
