#include "base/CCDirector.h"
#include "base/o2Integration/CCO2Integration.h"

#include "o2/Render/Sprite.h"
#include "platform/CCGLView.h"

O2Integration::O2Integration(o2::RefCounter* refCounter, cocos2d::Director* director) :
    o2::Integration(refCounter), mDirector(director)
{}

void O2Integration::InitializeBeforeRender()
{
    o2::Integration::InitalizeSystems();
    o2::Integration::InitializePlatform();
}

void O2Integration::InitializeAfterRender()
{
    o2::Integration::InitiazeRender();
    o2::Integration::InitilizeUIStyles();

    o2::Integration::mReady = true;
}

void O2Integration::ProcessFrame()
{
    o2::Integration::ProcessFrame();
}

o2::Vec2I O2Integration::GetContentSize() const
{
    auto size = mDirector->getOpenGLView()->getFrameSize();
    return o2::Vec2I((int)size.width, (int)size.height);
}

float O2Integration::GetGraphicsScale() const
{
    return 1.0f;
}

void O2Integration::OnDraw()
{
    static float angle = 0.0f;

    o2::Sprite sprt;
    sprt.SetSize(o2::Vec2F(10000, 10));

    sprt.SetAngleDegrees(angle);
    angle += 1.0f;

    sprt.Draw();
}

