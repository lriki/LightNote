//=============================================================================
//【 FreeTypeFont 】
//-----------------------------------------------------------------------------
///**
//  @file       FreeTypeFont.h
//  @brief      FreeTypeFont
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../../Base/RefString.h"
#include "../../Interface.h"
#include "../../Common/BitmapUtil.h"
#include "FreeTypeFontManager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

//=============================================================================
// ■ FreeTypeFont
//-----------------------------------------------------------------------------
///**
//  @brief     
//*/
//=============================================================================
class FreeTypeFont
    : public Base::ReferenceObject
    , public IFont
{
public:

    FreeTypeFont( FreeTypeFontManager* manager );
    virtual ~FreeTypeFont();
    LN_REFOBJ_METHODS;

public:

    /// 初期化
    LNRESULT initialize(
        const lnChar* fontname_,
        lnU32 size_,
        const LColor& color_,
        bool bold_,
        bool italic_ );

    // ※戻り値変更
    virtual LNRESULT        setName( const lnChar* fontname ) { mFontName = fontname; mModified = true; return LN_OK; }
    virtual const lnChar*   getName() const { return mFontName.c_str(); }

    virtual void            setSize( lnU32 size ) { mFontSize = size; mModified = true; }
    virtual lnU32           getSize() const { return mFontSize; }

    virtual void            setColor( const LColor& color ) { mColor = color; mModified = true; }
    virtual const LColor&   getColor() const { return mColor; }

    virtual void            setEdgeColor( const LColor& color ) { mEdgeColor = color; mModified = true; }
    virtual const LColor&   getEdgeColor() const { return mEdgeColor; }

	virtual void            setEdgeSize( lnU32 size ) { mEdgeSize = size; mModified = true; }
	virtual lnU32           getEdgeSize() const { return mEdgeSize; }

    virtual void            setBold( bool flag ) { mIsBold = flag; mModified = true; }
    virtual bool            isBold() const { return mIsBold; }

    virtual void            setItalic( bool flag ) { mIsItalic = flag; mModified = true; }
    virtual bool            isItalic() const { return mIsItalic; }

    virtual void            setAntiAlias( bool flag ) { mIsAntiAlias = flag; mModified = true; }
    virtual bool            isAntiAlias() const { return mIsAntiAlias; }

    virtual LNRESULT getTextSize( const lnChar* text_, int len_, Geometry::Rect* rect_ );

    virtual LNFontGlyphData* makeGlyphData( lnU32 utf32code, LNFontGlyphData* prev_data );
    virtual void postGlyphData( LNFontGlyphData* glyph_data );






    virtual LNRESULT beginGetGlyphOutline();
    virtual LNRESULT endGetGlyphOutline();




    /// フォント文字のビットマップ情報を取得する (char)
    virtual LNRESULT getGlyphOutline( const char* code_, LNGlyphOutlineData** data_ );

    /// フォント文字のビットマップ情報を取得する (wchar_t)
    virtual LNRESULT getGlyphOutline( const wchar_t* code_, LNGlyphOutlineData** data_ );

    /// フォントのコピーを作成する
    virtual LNRESULT deepCopy( IFont** font_ );
   
public:

    //bool isOutlineFont() const { return ( mEdgeSize > 0 ); }



private:

    ///// mDIBBitmapBuffer から mGlyphOutlineData.Bitmap に ARGB ビットマップを作成する
    ////      glyph_w_, glyph_h_ : 文字部分のサイズ
    //LNRESULT _makeGlyphFromDIBBitmap( lnU32 glyph_w_, lnU32 glyph_h_ );

    /// 現在の状態からフォントを更新する (再構築)
    void updateFont();

    /// リソース解放
    void dispose();

    void renderSpans( FT_OutlineGlyph glyph );

    static void rasterCallback(
        const int y,
        const int count,
        const FT_Span * const spans,
        void * const user);


private:

    struct RasterData
    {
        Graphics::Bitmap*   Bitmap;
        int                 MinX;
        int                 MinY;
    };

    struct PixelData
    {
        lnS16               X;
        lnS16               Y;
        lnByte              Coverage;
    };

    FreeTypeFontManager*    mManager;
    FTC_FaceID              mFTFaceID;
    FT_Face                 mFTFace;
    FTC_ScalerRec           mFTScaler;
    FTC_ImageTypeRec        mFTImageType;
    FT_Int                  mFTCacheMapIndex;

    bool                    mModified;
    lnRefString             mFontName;
    int                     mFontSize;
    LColor                  mColor;
    LColor                  mEdgeColor;
    lnU32                   mEdgeSize;
    bool                    mIsBold;
    bool                    mIsItalic;
    bool                    mIsAntiAlias;
    
    LNGlyphOutlineData      mGlyphOutlineData;
    FT_Glyph                mCopyGlyph;
    
    LNFontGlyphData         mFontGlyphData;
    Graphics::Bitmap        mGlyphBitmap;
    int                     mBitmapRealDataBytes;
    PixelData*              mPixelList;
    int                     mPixelCount;
    int                     mPixelMinX;
    int                     mPixelMinY;
    int                     mPixelMaxX;
    int                     mPixelMaxY;
    FT_Stroker              mFTStroker;

    bool                    mtest;

    //GlyphType               mGlyphType;

    //
    //lnU32                     mRealEdgeSize;
    //lnU32                     mOutlineQuality;    ///< 最低 1
    //HDC                     mMemDC;
    //HBITMAP                 mDIBBitmap;
    //lnU8*                     mDIBBitmapBuffer;
    //lnU32                     mDIBBitmapWidth;
    //lnU32                     mDIBBitmapHeight;
    //HPEN                    mOutlinePen;
    //HBRUSH                  mOutlineBrush;
    //HBRUSH                  mOutlineBGBrush;    ///< 青ブラシ

  

    //LNGlyphOutlineData      mGlyphOutlineData;  ///< グリフデータ
    //lnU32                     mBitmapBufferSize;
    //lnU32                     mTMAscent;
    //HDC                     mHDC;
    //

    //HFONT                   mOldFont;
    //
    //
    //bool                    mNeedUpdate;
    //bool                    mBeganGetGlyphOutline;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//
//=============================================================================