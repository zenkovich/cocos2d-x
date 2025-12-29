#pragma once

#include "platform/CCPlatformMacros.h"
#include "o2/Integration.h"

NS_CC_BEGIN

class Director;

class O2Integration : public o2::Integration
{
public:
    O2Integration(o2::RefCounter* refCounter, Director* director);

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
    Director* mDirector = nullptr;
};

NS_CC_END



