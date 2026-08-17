//@CODETOOL_NON_EXCLUDE
// pragma once is required: the generated reflection META sits after the include
// guard's #endif and would be compiled twice otherwise
#pragma once
/****************************************************************************
 Copyright (c) 2013      Zynga Inc.
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

#ifndef _COCOS2D_CCLABEL_H_
#define _COCOS2D_CCLABEL_H_

#include "2d/CCNode.h"
#include "renderer/CCCustomCommand.h"
#include "renderer/CCQuadCommand.h"
#include "2d/CCFontAtlas.h"
#include "base/ccTypes.h"

NS_CC_BEGIN

/**
 * @addtogroup _2d
 * @{
 */

#define CC_DEFAULT_FONT_LABEL_SIZE  12

/**
 * @struct TTFConfig
 * @see `GlyphCollection`
 */
typedef struct _ttfConfig
{
    std::string fontFilePath;
    float fontSize;

    GlyphCollection glyphs;
    const char *customGlyphs;

    bool distanceFieldEnabled;
    int outlineSize;

    bool italics;
    bool bold;
    bool underline;
    bool strikethrough;

    _ttfConfig(const std::string& filePath = "",float size = CC_DEFAULT_FONT_LABEL_SIZE, const GlyphCollection& glyphCollection = GlyphCollection::DYNAMIC,
        const char *customGlyphCollection = nullptr, bool useDistanceField = false, int outline = 0,
               bool useItalics = false, bool useBold = false, bool useUnderline = false, bool useStrikethrough = false)
        : fontFilePath(filePath)
        , fontSize(size)
        , glyphs(glyphCollection)
        , customGlyphs(customGlyphCollection)
        , distanceFieldEnabled(useDistanceField)
        , outlineSize(outline)
        , italics(useItalics)
        , bold(useBold)
        , underline(useUnderline)
        , strikethrough(useStrikethrough)
    {
        if(outline > 0)
        {
            distanceFieldEnabled = false;
        }
    }
} TTFConfig;

class Sprite;
class SpriteBatchNode;
class DrawNode;
class EventListenerCustom;
class TextureAtlas;

/**
 * @brief Label is a subclass of Node that knows how to render text labels.
 *
 * Label can be created with:
 * - A true type font file.
 * - A bitmap font file.
 * - A char map file.
 * - The built in system font.
 *
 * Bitmap Font supported editors:
 * - http://glyphdesigner.71squared.com/ (Commercial, Mac OS X)
 * - http://www.n4te.com/hiero/hiero.jnlp (Free, Java)
 * - http://slick.cokeandcode.com/demos/hiero.jnlp (Free, Java)
 * - http://www.angelcode.com/products/bmfont/ (Free, Windows only)
 * @js NA
 */
class CC_DLL Label : public Node, public LabelProtocol, public BlendProtocol
{
public:
    IOBJECT(Label);


    enum class Overflow
    {
        //In NONE mode, the dimensions is (0,0) and the content size will change dynamically to fit the label.
        NONE,
        /**
         *In CLAMP mode, when label content goes out of the bounding box, it will be clipped.
         */
        CLAMP,
        /**
         * In SHRINK mode, the font size will change dynamically to adapt the content size.
         */
        SHRINK,
        /**
         *In RESIZE_HEIGHT mode, you can only change the width of label and the height is changed automatically.
         */
        RESIZE_HEIGHT
    };
    
    enum class LabelType {
        TTF,
        BMFONT,
        CHARMAP,
        STRING_TEXTURE
    };
    
    /// @name Creators
    /// @{

    /**
    * Allocates and initializes a Label, with default settings.
    *
    * @return An automatically released Label object.
    */
    static Label* create();

    /**
     * Allocates and initializes a Label, base on platform-dependent API.
     *
     * @param text The initial text.
     * @param font A font file or a font family name.
     * @param fontSize The font size. This value must be > 0.
     * @param dimensions
     * @param hAlignment The text horizontal alignment.
     * @param vAlignment The text vertical alignment.
     *
     * @warning It will generate texture by the platform-dependent code.
     *
     * @return An automatically released Label object.
     */
    static Label* createWithSystemFont(const std::string& text, const std::string& font, float fontSize,
        const Size& dimensions = Size::ZERO, TextHAlignment hAlignment = TextHAlignment::LEFT,
        TextVAlignment vAlignment = TextVAlignment::TOP);

    /**
    * Allocates and initializes a Label, base on FreeType2.
    *
    * @param text The initial text.
    * @param fontFilePath A font file.
    * @param fontSize The font size. This value must be > 0.
    * @param dimensions
    * @param hAlignment The text horizontal alignment.
    * @param vAlignment The text vertical alignment.
    *
    * @return An automatically released Label object.
    */
    static Label * createWithTTF(const std::string& text, const std::string& fontFilePath, float fontSize,
        const Size& dimensions = Size::ZERO, TextHAlignment hAlignment = TextHAlignment::LEFT,
        TextVAlignment vAlignment = TextVAlignment::TOP);

    /**
    * Allocates and initializes a Label, base on FreeType2.
    *
    * @param ttfConfig A pointer to a TTFConfig object.
    * @param text The initial text.
    * @param hAlignment The text horizontal alignment.
    * @param maxLineWidth The max line width.
    *
    * @return An automatically released Label object.
    * @see TTFConfig setTTFConfig setMaxLineWidth
    */
    static Label* createWithTTF(const TTFConfig& ttfConfig, const std::string& text, 
        TextHAlignment hAlignment = TextHAlignment::LEFT, int maxLineWidth = 0);

    /**
    * Allocates and initializes a Label, with a bitmap font file.
    *
    * @param bmfontPath A bitmap font file, it's a FNT format.
    * @param text The initial text.
    * @param hAlignment Text horizontal alignment.
    * @param maxLineWidth The max line width.
    *
    * @return An automatically released Label object.
    * @see setBMFontFilePath setMaxLineWidth
    */
    static Label* createWithBMFont(const std::string& bmfontPath, const std::string& text,
        const TextHAlignment& hAlignment = TextHAlignment::LEFT, int maxLineWidth = 0);

    /**
    * Allocates and initializes a Label, with a bitmap font file.
    *
    * @param bmfontPath A bitmap font file, it's a FNT format.
    * @param text The initial text.
    * @param hAlignment Text horizontal alignment.
    * @param maxLineWidth The max line width.
    * @param imageRect
    * @param imageRotated
    *
    * @return An automatically released Label object.
    * @see setBMFontFilePath setMaxLineWidth
    */
    static Label* createWithBMFont(const std::string& bmfontPath, const std::string& text,
        const TextHAlignment& hAlignment, int maxLineWidth, const Rect& imageRect, bool imageRotated);

    /**
    * Allocates and initializes a Label, with a bitmap font file.
    *
    * @param bmfontPath A bitmap font file, it's a FNT format.
    * @param text The initial text.
    * @param hAlignment Text horizontal alignment.
    * @param maxLineWidth The max line width.
    * @param subTextureKey Name of entry in PLIST texture atlas/sprite sheet
    *
    * @return An automatically released Label object.
    * @see setBMFontFilePath setMaxLineWidth
    */
    static Label* createWithBMFont(const std::string& bmfontPath, const std::string& text,
        const TextHAlignment& hAlignment, int maxLineWidth, const std::string& subTextureKey);

    /**
    * Allocates and initializes a Label, with a bitmap font file.
    *
    * @param bmfontPath A bitmap font file, it's a FNT format.
    * @param text The initial text.
    * @param hAlignment Text horizontal alignment.
    * @param maxLineWidth The max line width.
    * @param imageOffset Offset into larger texture
    *
    * @return An automatically released Label object.
    * @see setBMFontFilePath setMaxLineWidth
    */
    CC_DEPRECATED_ATTRIBUTE static Label* createWithBMFont(const std::string& bmfontPath, const std::string& text,
            const TextHAlignment& hAlignment, int maxLineWidth, const Vec2& imageOffset);

    /**
    * Allocates and initializes a Label, with char map configuration.
    *
    * @param charMapFile A char map file, it's a PNG format.
    * @param itemWidth The width in points of each element.
    * @param itemHeight The height in points of each element.
    * @param startCharMap The starting char of the char map.
    *
    * @return An automatically released Label object.
    */
    static Label * createWithCharMap(const std::string& charMapFile, int itemWidth, int itemHeight, int startCharMap);

    /**
    * Allocates and initializes a Label, with char map configuration.
    *
    * @param texture A pointer to an existing Texture2D object.
    * @param itemWidth The width in points of each element.
    * @param itemHeight The height in points of each element.
    * @param startCharMap The starting char of the char map.
    *
    * @return An automatically released Label object.
    */
    static Label * createWithCharMap(Texture2D* texture, int itemWidth, int itemHeight, int startCharMap);

    /**
    * Allocates and initializes a Label, with char map configuration.
    *
    * @param plistFile A configuration file of char map.
    *
    * @return An automatically released Label object.
    */
    static Label * createWithCharMap(const std::string& plistFile);

    //  end of creators group
    /// @}

    /// @{
    /// @name Font methods

    /**
     * Sets a new TTF configuration to Label.
     * @see `TTFConfig`
     */
    virtual bool setTTFConfig(const TTFConfig& ttfConfig);

    /**
     * Returns the TTF configuration object used by the Label.
     * @see `TTFConfig`
     */
    virtual const TTFConfig& getTTFConfig() const { return _fontConfig;}

    /** Sets a new bitmap font to Label */
    virtual bool setBMFontFilePath(const std::string& bmfontFilePath, float fontSize = 0);

    /** Sets a new bitmap font to Label */
    virtual bool setBMFontFilePath(const std::string& bmfontFilePath, const Rect& imageRect, bool imageRotated, float fontSize = 0);

    /** Sets a new bitmap font to Label */
    virtual bool setBMFontFilePath(const std::string& bmfontFilePath, const std::string& subTextureKey, float fontSize = 0);

    /** Sets a new bitmap font to Label */
    CC_DEPRECATED_ATTRIBUTE virtual bool setBMFontFilePath(const std::string& bmfontFilePath, const Vec2& imageOffset, float fontSize = 0);

    /** Returns the bitmap font used by the Label.*/
    const std::string& getBMFontFilePath() const { return _bmFontPath;}

    /**
     * Sets a new char map configuration to Label.
     *
     * @see `createWithCharMap(const std::string&,int,int,int)`
     */
    virtual bool setCharMap(const std::string& charMapFile, int itemWidth, int itemHeight, int startCharMap);

    /**
     * Sets a new char map configuration to Label.
     *
     * @see `createWithCharMap(Texture2D*,int,int,int)`
     */
    virtual bool setCharMap(Texture2D* texture, int itemWidth, int itemHeight, int startCharMap);

    /**
     * Sets a new char map configuration to Label.
     *
     * @see `createWithCharMap(const std::string&)`
     */
    virtual bool setCharMap(const std::string& plistFile);

    /**
     * Sets a new system font to Label.
     *
     * @param font A font file or a font family name.
     * @warning
     */
    virtual void setSystemFontName(const std::string& font);

    /** Returns the system font used by the Label.*/
    virtual const std::string& getSystemFontName() const { return _systemFont;}

    /* Sets the system font size of Label.*/
    virtual void setSystemFontSize(float fontSize);

    /** Returns the bitmap font path used by the Label.*/
    virtual float getSystemFontSize() const { return _systemFontSize;}

    /**
     * @warning This method is not recommended for game developers.
     */
    virtual void requestSystemFontRefresh() { _systemFontDirty = true;}

    //  end of font methods
    /// @}

    /** Sets the text that this Label is to display.*/
    virtual void setString(const std::string& text) override;

    /** Return the text the Label is currently displaying.*/
    virtual const std::string& getString() const override {  return _utf8Text; }

    /**
     * Return the number of lines of text.
     */
    int getStringNumLines();

    /**
     * Return length of string.
     */
    int getStringLength();

    /**
     * Sets the text color of Label.
     *
     * The text color is different from the color of Node.
     *
     * @warning Limiting use to only when the Label created with true type font or system font.
     */
    virtual void setTextColor(const Color4B &color);

    /** Returns the text color of the Label.*/
    const Color4B& getTextColor() const { return _textColor;}

    /**
     * Enable shadow effect to Label.
     *
     * @todo Support blur for shadow effect.
     */
    virtual void enableShadow(const Color4B& shadowColor = Color4B::BLACK,const Size &offset = Size(2,-2), int blurRadius = 0);

    /**
     * Enable outline effect to Label.
     * @warning Limiting use to only when the Label created with true type font or system font.
     */
    virtual void enableOutline(const Color4B& outlineColor,int outlineSize = -1);

    /**
    * Enable glow effect to Label.
    * @warning Limiting use to only when the Label created with true type font.
    */
    virtual void enableGlow(const Color4B& glowColor);

    /**
     * Enable italics rendering
     */
    void enableItalics();

    /**
     * Enable bold rendering
     */
    void enableBold();

    /**
     * Enable underline
     */
    void enableUnderline();

    /**
     * Enables strikethrough.
     * Underline and Strikethrough cannot be enabled at the same time.
     * Strikethrough is like an underline but at the middle of the glyph
     */
    void enableStrikethrough();
    /**
     * Disable all effect applied to Label.
     * @warning Please use disableEffect(LabelEffect::ALL) instead of this API.
     */
    virtual void disableEffect();

    /**
     * Disable effect to Label.
     *
     * @see `LabelEffect`
     */
    virtual void disableEffect(LabelEffect effect);

    /**
    * Return whether the shadow effect is enabled.
    */
    bool isShadowEnabled() const { return _shadowEnabled; }

    /**
    * Return shadow effect offset value.
    */
    Size getShadowOffset() const { return _shadowOffset; }

    /**
    * Return the shadow effect blur radius.
    */
    float getShadowBlurRadius() const { return _shadowBlurRadius; }

    /**
    * Return the shadow effect color value.
    */
    Color4F getShadowColor() const { return _shadowColor4F; }

    /**
    * Return the outline effect size value.
    */
    float getOutlineSize() const { return _outlineSize; }

    /**
    * Return current effect type.
    */
    LabelEffect getLabelEffectType() const { return _currLabelEffect; }

    /**
    * Return current effect color value.
    */
    Color4F getEffectColor() const { return _effectColorF; }

    /** Sets the Label's text horizontal alignment.*/
    void setAlignment(TextHAlignment hAlignment) { setAlignment(hAlignment,_vAlignment);}

    /** Returns the Label's text horizontal alignment.*/
    TextHAlignment getTextAlignment() const { return _hAlignment;}

    /** Sets the Label's text alignment.*/
    void setAlignment(TextHAlignment hAlignment,TextVAlignment vAlignment);

    /** Sets the Label's text horizontal alignment.*/
    void setHorizontalAlignment(TextHAlignment hAlignment) { setAlignment(hAlignment,_vAlignment); }

    /** Returns the Label's text horizontal alignment.*/
    TextHAlignment getHorizontalAlignment() const { return _hAlignment; }

    /** Sets the Label's text vertical alignment.*/
    void setVerticalAlignment(TextVAlignment vAlignment) { setAlignment(_hAlignment,vAlignment); }

    /** Returns the Label's text vertical alignment.*/
    TextVAlignment getVerticalAlignment() const { return _vAlignment; }

    /**
     * Specify what happens when a line is too long for Label.
     *
     * @param breakWithoutSpace Lines are automatically broken between words if this value is false.
     */
    void setLineBreakWithoutSpace(bool breakWithoutSpace);

    /**
     * Makes the Label at most this line untransformed width.
     * The Label's max line width be used for force line breaks if the value not equal zero.
     */
    void setMaxLineWidth(float maxLineWidth);
    float getMaxLineWidth() { return _maxLineWidth; }
    /**
     * Change font size of label type BMFONT
     * Note: This function only scale the BMFONT letter to mimic the font size change effect.
     *
     * @param fontSize The desired font size in float.
     */
    void setBMFontSize(float fontSize);

    /**
     * Return the user define BMFont size.
     *
     * @return The BMFont size in float value.
     */
    float getBMFontSize()const;

    /**
     * Toggle wrap option of the label.
     * Note: System font doesn't support manually toggle wrap.
     *
     * @param enable Set true to enable wrap and false to disable wrap.
     */
    void enableWrap(bool enable);

    /**
     * Query the wrap is enabled or not.
     * Note: System font will always return true.
     */
    bool isWrapEnabled()const;

    /**
     * Change the label's Overflow type, currently only TTF and BMFont support all the valid Overflow type.
     * Char Map font supports all the Overflow type except for SHRINK, because we can't measure it's font size.
     * System font only support Overflow::Normal and Overflow::RESIZE_HEIGHT.
     *
     * @param overflow   see `Overflow`
     */
    void setOverflow(Overflow overflow);

    /**
     * Query the label's Overflow type.
     *
     * @return see `Overflow`
     */
    Overflow getOverflow()const;
    /**
     * Makes the Label exactly this untransformed width.
     *
     * The Label's width be used for text align if the value not equal zero.
                                                           */
    void setWidth(float width) { setDimensions(width,_labelHeight);}
    float getWidth() const { return _labelWidth; }

    /**
     * Makes the Label exactly this untransformed height.
     *
     * The Label's height be used for text align if the value not equal zero.
     * The text will display incomplete if the size of Label is not large enough to display all text.
     */
    void setHeight(float height){ setDimensions(_labelWidth, height); }
    float getHeight() const { return _labelHeight; }

    /** Sets the untransformed size of the Label in a more efficient way. */
    void setDimensions(float width, float height);
    const Size& getDimensions() const{ return _labelDimensions;}

    /** Update content immediately.*/
    virtual void updateContent();

    /**
     * Provides a way to treat each character like a Sprite.
     * @warning No support system font.
     */
    virtual Sprite * getLetter(int lettetIndex);

    /** Clips upper and lower margin to reduce height of Label.*/
    void setClipMarginEnabled(bool clipEnabled) { _clipEnabled = clipEnabled; }

    bool isClipMarginEnabled() const { return _clipEnabled; }

    /** Sets the line height of the Label.
     * @warning Not support system font.
     * @since v3.2.0
     */
    void setLineHeight(float height);

    /**
     * Returns the line height of this Label.
     * @warning Not support system font.
     * @since v3.2.0
     */
    float getLineHeight() const;

    void setLineSpacing(float height);
    float getLineSpacing() const;
    
    /**
     Returns type of label
     
     @warning Not support system font.
     @return the type of label
     @since v3.18.0
     */
    LabelType getLabelType() const { return _currentLabelType; }
    
    /**
     Returns font size
     */
    float getRenderingFontSize()const;

    /**
     * Sets the additional kerning of the Label.
     *
     * @warning Not support system font.
     * @since v3.2.0
     */
    void setAdditionalKerning(float space);

    /**
     * Returns the additional kerning of the Label.
     *
     * @warning Not support system font.
     * @since v3.2.0
     */
    float getAdditionalKerning() const;

    /**
    * set ProgramState of current render command
    */
    virtual void setProgramState(backend::ProgramState *programState) override;

    FontAtlas* getFontAtlas() { return _fontAtlas; }

    virtual const BlendFunc& getBlendFunc() const override { return _blendFunc; }
    virtual void setBlendFunc(const BlendFunc &blendFunc) override;

    virtual bool isOpacityModifyRGB() const override { return _isOpacityModifyRGB; }
    virtual void setOpacityModifyRGB(bool isOpacityModifyRGB) override;
    virtual void updateDisplayedColor(const Color3B& parentColor) override;
    virtual void updateDisplayedOpacity(uint8_t parentOpacity) override;

    virtual std::string getDescription() const override;

    virtual const Size& getContentSize() const override;
    virtual Rect getBoundingBox() const override;

    virtual void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

    virtual void setCameraMask(unsigned short mask, bool applyChildren = true) override;

    virtual void removeAllChildrenWithCleanup(bool cleanup) override;
    virtual void removeChild(Node* child, bool cleanup = true) override;
    virtual void setGlobalZOrder(float globalZOrder) override;

CC_CONSTRUCTOR_ACCESS:
    /**
     * Constructor of Label.
     * @js NA
     */
    Label(TextHAlignment hAlignment = TextHAlignment::LEFT,
      TextVAlignment vAlignment = TextVAlignment::TOP);

    /**
     * Destructor of Label.
     * @js NA
     * @lua NA
     */
    virtual ~Label();

    bool initWithTTF(const std::string& text, const std::string& fontFilePath, float fontSize,
                     const Size& dimensions = Size::ZERO, TextHAlignment hAlignment = TextHAlignment::LEFT,
                     TextVAlignment vAlignment = TextVAlignment::TOP);

    bool initWithTTF(const TTFConfig& ttfConfig, const std::string& text,
                     TextHAlignment hAlignment = TextHAlignment::LEFT, int maxLineWidth = 0);

protected:
    struct LetterInfo
    {
        char32_t utf32Char;
        bool valid;
        float positionX;
        float positionY;
        int atlasIndex;
        int lineIndex;
    };

    struct BatchCommand {
        BatchCommand();
        ~BatchCommand();

        CustomCommand textCommand;
        CustomCommand outLineCommand;
        CustomCommand shadowCommand;

        std::array<CustomCommand*, 3> getCommandArray();
    };

    virtual void setFontAtlas(FontAtlas* atlas, bool distanceFieldEnabled = false, bool useA8Shader = false);
    bool getFontLetterDef(char32_t character, FontLetterDefinition& letterDef) const;

    void computeStringNumLines();

    void drawSelf(bool visibleByCamera, Renderer* renderer, uint32_t flags);

    bool multilineTextWrapByChar();
    bool multilineTextWrapByWord();
    bool multilineTextWrap(const std::function<int(const std::u32string&, int, int)>& lambda);
    void shrinkLabelToContentSize(const std::function<bool(void)>& lambda);
    bool isHorizontalClamp();
    bool isVerticalClamp();
    void rescaleWithOriginalFontSize();

    void updateLabelLetters();
    virtual bool alignText();
    void computeAlignmentOffset();
    bool computeHorizontalKernings(const std::u32string& stringToRender);

    void recordLetterInfo(const cocos2d::Vec2& point, char32_t utf32Char, int letterIndex, int lineIndex);
    void recordPlaceholderInfo(int letterIndex, char32_t utf16Char);
    
    bool updateQuads();

    void createSpriteForSystemFont(const FontDefinition& fontDef);
    void createShadowSpriteForSystemFont(const FontDefinition& fontDef);

    virtual void updateShaderProgram();
    void updateBMFontScale();
    void scaleFontSizeDown(float fontSize);
    bool setTTFConfigInternal(const TTFConfig& ttfConfig);
    void setBMFontSizeInternal(float fontSize);
    bool isHorizontalClamped(float letterPositionX, int lineIndex);
    void restoreFontSize();
    void updateLetterSpriteScale(Sprite* sprite);
    int getFirstCharLen(const std::u32string& utf32Text, int startIndex, int textLen) const;
    int getFirstWordLen(const std::u32string& utf32Text, int startIndex, int textLen) const;

    void reset();

    FontDefinition _getFontDefinition() const;

    virtual void updateColor() override;
    
    void updateUniformLocations();
    void setVertexLayout(PipelineDescriptor& vertexLayout);
    void updateBlendState();
    void updateEffectUniforms(BatchCommand &batch, TextureAtlas* textureAtlas, Renderer *renderer, const Mat4 &transform);
    void updateBuffer(TextureAtlas* textureAtlas, CustomCommand& customCommand);

    void updateBatchCommand(BatchCommand &batch);

    LabelType _currentLabelType; // @EDITOR_PROPERTY
    bool _contentDirty;
    std::u32string _utf32Text;
    std::string _utf8Text;
    int _numberOfLines; // @EDITOR_PROPERTY

    std::string _bmFontPath;
    std::string _bmSubTextureKey;
    Rect _bmRect;
    bool _bmRotated;

    TTFConfig _fontConfig;
    float _outlineSize; // @EDITOR_PROPERTY

    bool _systemFontDirty;
    std::string _systemFont;
    float _systemFontSize; // @EDITOR_PROPERTY
    Sprite* _textSprite;
    Sprite* _shadowNode;

    FontAtlas* _fontAtlas;
    Vector<SpriteBatchNode*> _batchNodes;
    std::vector<LetterInfo> _lettersInfo;

    //! used for optimization
    Sprite *_reusedLetter;
    Rect _reusedRect;
    int _lengthOfString;

    //layout relevant properties.
    float _lineHeight; // @EDITOR_PROPERTY
    float _lineSpacing; // @EDITOR_PROPERTY
    float _additionalKerning; // @EDITOR_PROPERTY
    int* _horizontalKernings;
    bool _lineBreakWithoutSpaces;
    float _maxLineWidth; // @EDITOR_PROPERTY
    Size _labelDimensions;
    float _labelWidth;
    float _labelHeight;
    TextHAlignment _hAlignment;
    TextVAlignment _vAlignment;

    float _textDesiredHeight;
    std::vector<float> _linesWidth;
    std::vector<float> _linesOffsetX;
    float _letterOffsetY;
    float _tailoredTopY;
    float _tailoredBottomY;

    LabelEffect _currLabelEffect;
    Color4F _effectColorF;
    Color4B _textColor;
    Color4F _textColorF;

    QuadCommand _quadCommand;

    std::vector<BatchCommand> _batchCommands;
    
    Mat4  _shadowTransform;
    int  _uniformEffectColor;
    int  _uniformEffectType; // 0: None, 1: Outline, 2: Shadow; Only used when outline is enabled.
    int  _uniformTextColor;
    bool _useDistanceField;
    bool _useA8Shader;

    bool _shadowDirty;
    bool _shadowEnabled; // @EDITOR_PROPERTY
    Size _shadowOffset;
    
    Color4F _shadowColor4F;
    Color3B _shadowColor3B;
    uint8_t _shadowOpacity;
    float _shadowBlurRadius; // @EDITOR_PROPERTY

    bool _clipEnabled; // @EDITOR_PROPERTY
    bool _blendFuncDirty;
    BlendFunc _blendFunc;

    /// whether or not the label was inside bounds the previous frame
    bool _insideBounds;

    bool _isOpacityModifyRGB;

    std::unordered_map<int, Sprite*> _letters;

    EventListenerCustom* _purgeTextureListener;
    EventListenerCustom* _resetTextureListener;

#if CC_LABEL_DEBUG_DRAW
    DrawNode* _debugDrawNode;
#endif

    bool _enableWrap; // @EDITOR_PROPERTY
    float _bmFontSize;
    float _bmfontScale;
    Overflow _overflow; // @EDITOR_PROPERTY
    float _originalFontSize;

    bool _boldEnabled; // @EDITOR_PROPERTY
    DrawNode* _underlineNode;
    bool _strikethroughEnabled; // @EDITOR_PROPERTY
    
    backend::UniformLocation _mvpMatrixLocation;
    backend::UniformLocation _textureLocation;
    backend::UniformLocation _alphaTextureLocation;
    backend::UniformLocation _textColorLocation;
    backend::UniformLocation _effectColorLocation;
    backend::UniformLocation _effectTypeLocation;
    
private:
    CC_DISALLOW_COPY_AND_ASSIGN(Label);
};

// end group
/// @}

NS_CC_END

#endif /*__COCOS2D_CCLABEL_H */
// --- META ---

PRE_ENUM_META(cocos2d::Label::Overflow);

PRE_ENUM_META(cocos2d::Label::LabelType);

CLASS_BASES_META(cocos2d::Label)
{
    BASE_CLASS(cocos2d::Node);
    BASE_CLASS(LabelProtocol);
    BASE_CLASS(BlendProtocol);
}
END_META;
CLASS_FIELDS_META(cocos2d::Label)
{
    FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().NAME(_currentLabelType);
    FIELD().PROTECTED().NAME(_contentDirty);
    FIELD().PROTECTED().NAME(_utf32Text);
    FIELD().PROTECTED().NAME(_utf8Text);
    FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().NAME(_numberOfLines);
    FIELD().PROTECTED().NAME(_bmFontPath);
    FIELD().PROTECTED().NAME(_bmSubTextureKey);
    FIELD().PROTECTED().NAME(_bmRect);
    FIELD().PROTECTED().NAME(_bmRotated);
    FIELD().PROTECTED().NAME(_fontConfig);
    FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().NAME(_outlineSize);
    FIELD().PROTECTED().NAME(_systemFontDirty);
    FIELD().PROTECTED().NAME(_systemFont);
    FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().NAME(_systemFontSize);
    FIELD().PROTECTED().NAME(_textSprite);
    FIELD().PROTECTED().NAME(_shadowNode);
    FIELD().PROTECTED().NAME(_fontAtlas);
    FIELD().PROTECTED().NAME(_batchNodes);
    FIELD().PROTECTED().NAME(_lettersInfo);
    FIELD().PROTECTED().NAME(_reusedLetter);
    FIELD().PROTECTED().NAME(_reusedRect);
    FIELD().PROTECTED().NAME(_lengthOfString);
    FIELD().PROTECTED().NAME(_lineHeight);
    FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().NAME(_lineSpacing);
    FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().NAME(_additionalKerning);
    FIELD().PROTECTED().NAME(_horizontalKernings);
    FIELD().PROTECTED().NAME(_lineBreakWithoutSpaces);
    FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().NAME(_maxLineWidth);
    FIELD().PROTECTED().NAME(_labelDimensions);
    FIELD().PROTECTED().NAME(_labelWidth);
    FIELD().PROTECTED().NAME(_labelHeight);
    FIELD().PROTECTED().NAME(_hAlignment);
    FIELD().PROTECTED().NAME(_vAlignment);
    FIELD().PROTECTED().NAME(_textDesiredHeight);
    FIELD().PROTECTED().NAME(_linesWidth);
    FIELD().PROTECTED().NAME(_linesOffsetX);
    FIELD().PROTECTED().NAME(_letterOffsetY);
    FIELD().PROTECTED().NAME(_tailoredTopY);
    FIELD().PROTECTED().NAME(_tailoredBottomY);
    FIELD().PROTECTED().NAME(_currLabelEffect);
    FIELD().PROTECTED().NAME(_effectColorF);
    FIELD().PROTECTED().NAME(_textColor);
    FIELD().PROTECTED().NAME(_textColorF);
    FIELD().PROTECTED().NAME(_quadCommand);
    FIELD().PROTECTED().NAME(_batchCommands);
    FIELD().PROTECTED().NAME(_shadowTransform);
    FIELD().PROTECTED().NAME(_uniformEffectColor);
    FIELD().PROTECTED().NAME(_uniformEffectType);
    FIELD().PROTECTED().NAME(_uniformTextColor);
    FIELD().PROTECTED().NAME(_useDistanceField);
    FIELD().PROTECTED().NAME(_useA8Shader);
    FIELD().PROTECTED().NAME(_shadowDirty);
    FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().NAME(_shadowEnabled);
    FIELD().PROTECTED().NAME(_shadowOffset);
    FIELD().PROTECTED().NAME(_shadowColor4F);
    FIELD().PROTECTED().NAME(_shadowColor3B);
    FIELD().PROTECTED().NAME(_shadowOpacity);
    FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().NAME(_shadowBlurRadius);
    FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().NAME(_clipEnabled);
    FIELD().PROTECTED().NAME(_blendFuncDirty);
    FIELD().PROTECTED().NAME(_blendFunc);
    FIELD().PROTECTED().NAME(_insideBounds);
    FIELD().PROTECTED().NAME(_isOpacityModifyRGB);
    FIELD().PROTECTED().NAME(_letters);
    FIELD().PROTECTED().NAME(_purgeTextureListener);
    FIELD().PROTECTED().NAME(_resetTextureListener);
#if  CC_LABEL_DEBUG_DRAW
    FIELD().PROTECTED().NAME(_debugDrawNode);
#endif
    FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().NAME(_enableWrap);
    FIELD().PROTECTED().NAME(_bmFontSize);
    FIELD().PROTECTED().NAME(_bmfontScale);
    FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().NAME(_overflow);
    FIELD().PROTECTED().NAME(_originalFontSize);
    FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().NAME(_boldEnabled);
    FIELD().PROTECTED().NAME(_underlineNode);
    FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().NAME(_strikethroughEnabled);
    FIELD().PROTECTED().NAME(_mvpMatrixLocation);
    FIELD().PROTECTED().NAME(_textureLocation);
    FIELD().PROTECTED().NAME(_alphaTextureLocation);
    FIELD().PROTECTED().NAME(_textColorLocation);
    FIELD().PROTECTED().NAME(_effectColorLocation);
    FIELD().PROTECTED().NAME(_effectTypeLocation);
}
END_META;
CLASS_METHODS_META(cocos2d::Label)
{

    typedef const std::function<int(const std::u32string&, int, int)>& _tmp1;

    FUNCTION().PUBLIC().SIGNATURE_STATIC(Label*, create);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Label*, createWithSystemFont, const std::string&, const std::string&, float, const Size&, TextHAlignment, TextVAlignment);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Label*, createWithTTF, const std::string&, const std::string&, float, const Size&, TextHAlignment, TextVAlignment);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Label*, createWithTTF, const TTFConfig&, const std::string&, TextHAlignment, int);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Label*, createWithBMFont, const std::string&, const std::string&, const TextHAlignment&, int);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Label*, createWithBMFont, const std::string&, const std::string&, const TextHAlignment&, int, const Rect&, bool);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Label*, createWithBMFont, const std::string&, const std::string&, const TextHAlignment&, int, const std::string&);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Label*, createWithBMFont, const std::string&, const std::string&, const TextHAlignment&, int, const Vec2&);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Label*, createWithCharMap, const std::string&, int, int, int);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Label*, createWithCharMap, Texture2D*, int, int, int);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Label*, createWithCharMap, const std::string&);
    FUNCTION().PUBLIC().SIGNATURE(bool, setTTFConfig, const TTFConfig&);
    FUNCTION().PUBLIC().SIGNATURE(const TTFConfig&, getTTFConfig);
    FUNCTION().PUBLIC().SIGNATURE(bool, setBMFontFilePath, const std::string&, float);
    FUNCTION().PUBLIC().SIGNATURE(bool, setBMFontFilePath, const std::string&, const Rect&, bool, float);
    FUNCTION().PUBLIC().SIGNATURE(bool, setBMFontFilePath, const std::string&, const std::string&, float);
    FUNCTION().PUBLIC().SIGNATURE(bool, setBMFontFilePath, const std::string&, const Vec2&, float);
    FUNCTION().PUBLIC().SIGNATURE(const std::string&, getBMFontFilePath);
    FUNCTION().PUBLIC().SIGNATURE(bool, setCharMap, const std::string&, int, int, int);
    FUNCTION().PUBLIC().SIGNATURE(bool, setCharMap, Texture2D*, int, int, int);
    FUNCTION().PUBLIC().SIGNATURE(bool, setCharMap, const std::string&);
    FUNCTION().PUBLIC().SIGNATURE(void, setSystemFontName, const std::string&);
    FUNCTION().PUBLIC().SIGNATURE(const std::string&, getSystemFontName);
    FUNCTION().PUBLIC().SIGNATURE(void, setSystemFontSize, float);
    FUNCTION().PUBLIC().SIGNATURE(float, getSystemFontSize);
    FUNCTION().PUBLIC().SIGNATURE(void, requestSystemFontRefresh);
    FUNCTION().PUBLIC().SIGNATURE(void, setString, const std::string&);
    FUNCTION().PUBLIC().SIGNATURE(const std::string&, getString);
    FUNCTION().PUBLIC().SIGNATURE(int, getStringNumLines);
    FUNCTION().PUBLIC().SIGNATURE(int, getStringLength);
    FUNCTION().PUBLIC().SIGNATURE(void, setTextColor, const Color4B&);
    FUNCTION().PUBLIC().SIGNATURE(const Color4B&, getTextColor);
    FUNCTION().PUBLIC().SIGNATURE(void, enableShadow, const Color4B&, const Size&, int);
    FUNCTION().PUBLIC().SIGNATURE(void, enableOutline, const Color4B&, int);
    FUNCTION().PUBLIC().SIGNATURE(void, enableGlow, const Color4B&);
    FUNCTION().PUBLIC().SIGNATURE(void, enableItalics);
    FUNCTION().PUBLIC().SIGNATURE(void, enableBold);
    FUNCTION().PUBLIC().SIGNATURE(void, enableUnderline);
    FUNCTION().PUBLIC().SIGNATURE(void, enableStrikethrough);
    FUNCTION().PUBLIC().SIGNATURE(void, disableEffect);
    FUNCTION().PUBLIC().SIGNATURE(void, disableEffect, LabelEffect);
    FUNCTION().PUBLIC().SIGNATURE(bool, isShadowEnabled);
    FUNCTION().PUBLIC().SIGNATURE(Size, getShadowOffset);
    FUNCTION().PUBLIC().SIGNATURE(float, getShadowBlurRadius);
    FUNCTION().PUBLIC().SIGNATURE(Color4F, getShadowColor);
    FUNCTION().PUBLIC().SIGNATURE(float, getOutlineSize);
    FUNCTION().PUBLIC().SIGNATURE(LabelEffect, getLabelEffectType);
    FUNCTION().PUBLIC().SIGNATURE(Color4F, getEffectColor);
    FUNCTION().PUBLIC().SIGNATURE(void, setAlignment, TextHAlignment);
    FUNCTION().PUBLIC().SIGNATURE(TextHAlignment, getTextAlignment);
    FUNCTION().PUBLIC().SIGNATURE(void, setAlignment, TextHAlignment, TextVAlignment);
    FUNCTION().PUBLIC().SIGNATURE(void, setHorizontalAlignment, TextHAlignment);
    FUNCTION().PUBLIC().SIGNATURE(TextHAlignment, getHorizontalAlignment);
    FUNCTION().PUBLIC().SIGNATURE(void, setVerticalAlignment, TextVAlignment);
    FUNCTION().PUBLIC().SIGNATURE(TextVAlignment, getVerticalAlignment);
    FUNCTION().PUBLIC().SIGNATURE(void, setLineBreakWithoutSpace, bool);
    FUNCTION().PUBLIC().SIGNATURE(void, setMaxLineWidth, float);
    FUNCTION().PUBLIC().SIGNATURE(float, getMaxLineWidth);
    FUNCTION().PUBLIC().SIGNATURE(void, setBMFontSize, float);
    FUNCTION().PUBLIC().SIGNATURE(float, getBMFontSize);
    FUNCTION().PUBLIC().SIGNATURE(void, enableWrap, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, isWrapEnabled);
    FUNCTION().PUBLIC().SIGNATURE(void, setOverflow, Overflow);
    FUNCTION().PUBLIC().SIGNATURE(Overflow, getOverflow);
    FUNCTION().PUBLIC().SIGNATURE(void, setWidth, float);
    FUNCTION().PUBLIC().SIGNATURE(float, getWidth);
    FUNCTION().PUBLIC().SIGNATURE(void, setHeight, float);
    FUNCTION().PUBLIC().SIGNATURE(float, getHeight);
    FUNCTION().PUBLIC().SIGNATURE(void, setDimensions, float, float);
    FUNCTION().PUBLIC().SIGNATURE(const Size&, getDimensions);
    FUNCTION().PUBLIC().SIGNATURE(void, updateContent);
    FUNCTION().PUBLIC().SIGNATURE(Sprite*, getLetter, int);
    FUNCTION().PUBLIC().SIGNATURE(void, setClipMarginEnabled, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, isClipMarginEnabled);
    FUNCTION().PUBLIC().SIGNATURE(void, setLineHeight, float);
    FUNCTION().PUBLIC().SIGNATURE(float, getLineHeight);
    FUNCTION().PUBLIC().SIGNATURE(void, setLineSpacing, float);
    FUNCTION().PUBLIC().SIGNATURE(float, getLineSpacing);
    FUNCTION().PUBLIC().SIGNATURE(LabelType, getLabelType);
    FUNCTION().PUBLIC().SIGNATURE(float, getRenderingFontSize);
    FUNCTION().PUBLIC().SIGNATURE(void, setAdditionalKerning, float);
    FUNCTION().PUBLIC().SIGNATURE(float, getAdditionalKerning);
    FUNCTION().PUBLIC().SIGNATURE(void, setProgramState, backend::ProgramState*);
    FUNCTION().PUBLIC().SIGNATURE(FontAtlas*, getFontAtlas);
    FUNCTION().PUBLIC().SIGNATURE(const BlendFunc&, getBlendFunc);
    FUNCTION().PUBLIC().SIGNATURE(void, setBlendFunc, const BlendFunc&);
    FUNCTION().PUBLIC().SIGNATURE(bool, isOpacityModifyRGB);
    FUNCTION().PUBLIC().SIGNATURE(void, setOpacityModifyRGB, bool);
    FUNCTION().PUBLIC().SIGNATURE(void, updateDisplayedColor, const Color3B&);
    FUNCTION().PUBLIC().SIGNATURE(void, updateDisplayedOpacity, uint8_t);
    FUNCTION().PUBLIC().SIGNATURE(std::string, getDescription);
    FUNCTION().PUBLIC().SIGNATURE(const Size&, getContentSize);
    FUNCTION().PUBLIC().SIGNATURE(Rect, getBoundingBox);
    FUNCTION().PUBLIC().SIGNATURE(void, visit, Renderer*, const Mat4&, uint32_t);
    FUNCTION().PUBLIC().SIGNATURE(void, draw, Renderer*, const Mat4&, uint32_t);
    FUNCTION().PUBLIC().SIGNATURE(void, setCameraMask, unsigned short, bool);
    FUNCTION().PUBLIC().SIGNATURE(void, removeAllChildrenWithCleanup, bool);
    FUNCTION().PUBLIC().SIGNATURE(void, removeChild, Node*, bool);
    FUNCTION().PUBLIC().SIGNATURE(void, setGlobalZOrder, float);
    FUNCTION().PUBLIC().CONSTRUCTOR(TextHAlignment, TextVAlignment);
    FUNCTION().PUBLIC().SIGNATURE(bool, initWithTTF, const std::string&, const std::string&, float, const Size&, TextHAlignment, TextVAlignment);
    FUNCTION().PUBLIC().SIGNATURE(bool, initWithTTF, const TTFConfig&, const std::string&, TextHAlignment, int);
    FUNCTION().PROTECTED().SIGNATURE(void, setFontAtlas, FontAtlas*, bool, bool);
    FUNCTION().PROTECTED().SIGNATURE(bool, getFontLetterDef, char32_t, FontLetterDefinition&);
    FUNCTION().PROTECTED().SIGNATURE(void, computeStringNumLines);
    FUNCTION().PROTECTED().SIGNATURE(void, drawSelf, bool, Renderer*, uint32_t);
    FUNCTION().PROTECTED().SIGNATURE(bool, multilineTextWrapByChar);
    FUNCTION().PROTECTED().SIGNATURE(bool, multilineTextWrapByWord);
    FUNCTION().PROTECTED().SIGNATURE(bool, multilineTextWrap, _tmp1);
    FUNCTION().PROTECTED().SIGNATURE(void, shrinkLabelToContentSize, const std::function<bool(void)>&);
    FUNCTION().PROTECTED().SIGNATURE(bool, isHorizontalClamp);
    FUNCTION().PROTECTED().SIGNATURE(bool, isVerticalClamp);
    FUNCTION().PROTECTED().SIGNATURE(void, rescaleWithOriginalFontSize);
    FUNCTION().PROTECTED().SIGNATURE(void, updateLabelLetters);
    FUNCTION().PROTECTED().SIGNATURE(bool, alignText);
    FUNCTION().PROTECTED().SIGNATURE(void, computeAlignmentOffset);
    FUNCTION().PROTECTED().SIGNATURE(bool, computeHorizontalKernings, const std::u32string&);
    FUNCTION().PROTECTED().SIGNATURE(void, recordLetterInfo, const cocos2d::Vec2&, char32_t, int, int);
    FUNCTION().PROTECTED().SIGNATURE(void, recordPlaceholderInfo, int, char32_t);
    FUNCTION().PROTECTED().SIGNATURE(bool, updateQuads);
    FUNCTION().PROTECTED().SIGNATURE(void, createSpriteForSystemFont, const FontDefinition&);
    FUNCTION().PROTECTED().SIGNATURE(void, createShadowSpriteForSystemFont, const FontDefinition&);
    FUNCTION().PROTECTED().SIGNATURE(void, updateShaderProgram);
    FUNCTION().PROTECTED().SIGNATURE(void, updateBMFontScale);
    FUNCTION().PROTECTED().SIGNATURE(void, scaleFontSizeDown, float);
    FUNCTION().PROTECTED().SIGNATURE(bool, setTTFConfigInternal, const TTFConfig&);
    FUNCTION().PROTECTED().SIGNATURE(void, setBMFontSizeInternal, float);
    FUNCTION().PROTECTED().SIGNATURE(bool, isHorizontalClamped, float, int);
    FUNCTION().PROTECTED().SIGNATURE(void, restoreFontSize);
    FUNCTION().PROTECTED().SIGNATURE(void, updateLetterSpriteScale, Sprite*);
    FUNCTION().PROTECTED().SIGNATURE(int, getFirstCharLen, const std::u32string&, int, int);
    FUNCTION().PROTECTED().SIGNATURE(int, getFirstWordLen, const std::u32string&, int, int);
    FUNCTION().PROTECTED().SIGNATURE(void, reset);
    FUNCTION().PROTECTED().SIGNATURE(FontDefinition, _getFontDefinition);
    FUNCTION().PROTECTED().SIGNATURE(void, updateColor);
    FUNCTION().PROTECTED().SIGNATURE(void, updateUniformLocations);
    FUNCTION().PROTECTED().SIGNATURE(void, setVertexLayout, PipelineDescriptor&);
    FUNCTION().PROTECTED().SIGNATURE(void, updateBlendState);
    FUNCTION().PROTECTED().SIGNATURE(void, updateEffectUniforms, BatchCommand&, TextureAtlas*, Renderer*, const Mat4&);
    FUNCTION().PROTECTED().SIGNATURE(void, updateBuffer, TextureAtlas*, CustomCommand&);
    FUNCTION().PROTECTED().SIGNATURE(void, updateBatchCommand, BatchCommand&);
}
END_META;
// --- END META ---
