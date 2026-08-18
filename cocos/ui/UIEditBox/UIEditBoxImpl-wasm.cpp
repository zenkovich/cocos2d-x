/****************************************************************************
 Edit box implementation for WebAssembly builds.

 The desktop implementations open a native text dialog (GTK on linux, NSTextField
 on mac); the browser has no equivalent to open from wasm, so the widget draws and
 behaves like a label until a web based input is wired up.
 ****************************************************************************/

#include "ui/UIEditBox/UIEditBoxImpl-common.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX) && defined(__EMSCRIPTEN__)

NS_CC_BEGIN

namespace ui {

class EditBoxImplWasm : public EditBoxImplCommon
{
public:
    EditBoxImplWasm(EditBox* pEditText) : EditBoxImplCommon(pEditText) {}

    void createNativeControl(const Rect& frame) override {}
    void setNativeFont(const char* pFontName, int fontSize) override {}
    void setNativeFontColor(const Color4B& color) override {}
    void setNativePlaceholderFont(const char* pFontName, int fontSize) override {}
    void setNativePlaceholderFontColor(const Color4B& color) override {}
    void setNativeInputMode(EditBox::InputMode inputMode) override {}
    void setNativeInputFlag(EditBox::InputFlag inputFlag) override {}
    void setNativeReturnType(EditBox::KeyboardReturnType returnType) override {}
    void setNativeTextHorizontalAlignment(cocos2d::TextHAlignment alignment) override {}
    void setNativeText(const char* pText) override {}
    void setNativePlaceHolder(const char* pText) override {}
    void setNativeVisible(bool visible) override {}
    void updateNativeFrame(const Rect& rect) override {}
    const char* getNativeDefaultFontName() override { return "arial"; }
    void nativeOpenKeyboard() override {}
    void nativeCloseKeyboard() override {}
    void setNativeMaxLength(int maxLength) override {}
    bool isEditing() override { return false; }
};

EditBoxImpl* __createSystemEditBox(EditBox* pEditBox)
{
    return new EditBoxImplWasm(pEditBox);
}

}

NS_CC_END

#endif
