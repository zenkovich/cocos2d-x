//@CODETOOL_NON_EXCLUDE
#pragma once

#include "platform/CCPlatformMacros.h"
#include "base/CCEventKeyboard.h"
#include "o2/Integration.h"
#include "o2/Application/VKCodes.h"

NS_CC_BEGIN
class Director;
NS_CC_END

// Maps an o2 virtual key code to the cocos key code (shared by the demo
// application loop and the editor input forwarding)
cocos2d::EventKeyboard::KeyCode MapO2KeyToCocosKeyCode(o2::KeyboardKey key);

class O2Integration : public o2::Integration
{
public:
    O2Integration(o2::RefCounter* refCounter, cocos2d::Director* director);

    void InitializeBeforeRender();
    void InitializeAfterRender();

    void ProcessFrame();

    o2::Vec2I GetContentSize() const;
    float GetGraphicsScale() const;

    // Returns is platform-specific initialization needed
    bool IsNeedPlatformInitialization() const override { return false; }

    using o2::Integration::CalculateAndSyncFPS;
    using o2::Integration::PreUpdateFrame;
    using o2::Integration::MainUpdateFrame;
    using o2::Integration::UpdateFrameFixed;
    using o2::Integration::PreDrawFrame;
    using o2::Integration::DrawFrame;
    using o2::Integration::PostDrawFrame;
    using o2::Integration::PostUpdateFrame;

    void OnDraw() override;

private:
    cocos2d::Director* mDirector = nullptr;
};
