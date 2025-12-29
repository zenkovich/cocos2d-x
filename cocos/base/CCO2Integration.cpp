/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2013 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2016 Chukong Technologies Inc.
Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "base/CCDirector.h"
#include "base/CCO2Integration.h"

#include "o2/Render/Sprite.h"

NS_CC_BEGIN

O2Integration::O2Integration(o2::RefCounter* refCounter, Director* director) :
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
    auto size = mDirector->_openGLView->getFrameSize();
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

NS_CC_END



