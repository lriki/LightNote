//==============================================================================
// FreeTypeFont 
//------------------------------------------------------------------------------
///**
//  @file       FreeTypeFont.h
//  @brief      FreeTypeFont
//  @author     Riki
//*/
//==============================================================================

#pragma once


#include <ft2build.h>
#include FT_FREETYPE_H	/* <freetype/freetype.h> */
#include FT_CACHE_H	/* <freetype/ftcache.h> */
#include FT_TRUETYPE_TAGS_H	/* <freetype/tttags.h> */
#include FT_TRUETYPE_TABLES_H	/* <freetype/tttables.h> */
#include FT_BITMAP_H            /* freetype/ftbitmap.h */
#include FT_OUTLINE_H
#include FT_SYNTHESIS_H
#include FT_STROKER_H

#include "../../../Base/RefString.h"
#include "../../Interface.h"
#include "../../Common/Bitmap.h"
#include "Font.h"
#include "FreeTypeFontManager.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

struct FreeTypeFontGlyphData
	: public FontGlyphData
{
	//int             BitmapRealDataSize;    ///< 内部データ
	int				NextBaseX;      ///< 内部データ
	int				NextBaseY;      ///< 内部データ
	lnUInt			Previous;       ///< 内部データ
	FT_Glyph		CopyGlyph;			///< GlyphBitmap のバッファ本体はこれが持っている
	FT_Glyph		CopyOutlineGlyph;	///< OutlineBitmap のバッファ本体はこれが持っている
};

//==============================================================================
// FreeTypeFont
//------------------------------------------------------------------------------
///**
//  @brief     
//*/
//==============================================================================
class FreeTypeFont
    : /*public Base::ReferenceObject
    , */public Font
{
public:

	//struct FontGlyphData
	//{
	//	Bitmap			GlyphBitmap;
	//	Bitmap*			OutlineBitmap;	///< 縁どり線用 (NULL でなければ先にこれをbltし、その上に GlyphBitmap を blt する)
	//	int				GlyphOffsetX;   ///< ビットマップを転送するべき座標へのXオフセット
	//	int				GlyphOffsetY;   ///< ビットマップを転送するべき座標へのYオフセット
	//	int				OutlineOffset;	///< 縁どり時は、線の分だけサイズが大きくなる。

	//	int				MetricsHeight;	///< 行高さ

	//	//int             BitmapRealDataSize;    ///< 内部データ
	//	int				NextBaseX;      ///< 内部データ
	//	int				NextBaseY;      ///< 内部データ
	//	lnUInt			Previous;       ///< 内部データ
	//	FT_Glyph		CopyGlyph;			///< GlyphBitmap のバッファ本体はこれが持っている
	//	FT_Glyph		CopyOutlineGlyph;	///< OutlineBitmap のバッファ本体はこれが持っている

	//	/* 描画先座標は GlyphOffsetX, GlyphOffsetY に格納されている。
	//	 * これは makeGlyphData() を呼ぶたびに送り方向へ加算されていくため、
	//	 * 描画側は座標を加算し続ける必要はない。
	//	 * 常に 描画領域の左上 + GlyphOffsetX(Y) でOK。
	//	 */
	//};

public:
    FreeTypeFont( FreeTypeFontManager* manager );
    virtual ~FreeTypeFont();
    //LN_REFOBJ_METHODS;

public:

	/// 初期化
    LNRESULT initialize(
        const lnChar* fontname_,
        lnU32 size_,
        const LColor& color_,
        bool bold_,
        bool italic_ );

    virtual void			setName( const lnChar* fontname ) { mFontName = fontname; mModified = true; }
    virtual const lnChar*   getName() const { return mFontName.c_str(); }

    virtual void            setSize( lnU32 size ) { mFontSize = size; mModified = true; }
    virtual lnU32           getSize() const { return mFontSize; }

	virtual void            setColor(const LColor& color) { mColor = color; mModified = true; }
	virtual const LColor&	getColor() const { return mColor; }

	virtual void            setEdgeColor(const LColor& color) { mEdgeColor = color; mModified = true; }
	virtual const LColor&	getEdgeColor() const { return mEdgeColor; }

	virtual void            setEdgeSize( lnU32 size ) { mEdgeSize = size; mModified = true; }
	virtual lnU32           getEdgeSize() const { return mEdgeSize; }

    virtual void            setBold( bool flag ) { mIsBold = flag; mModified = true; }
    virtual bool            isBold() const { return mIsBold; }

    virtual void            setItalic( bool flag ) { mIsItalic = flag; mModified = true; }
    virtual bool            isItalic() const { return mIsItalic; }

    virtual void            setAntiAlias( bool flag ) { mIsAntiAlias = flag; mModified = true; }
    virtual bool            isAntiAlias() const { return mIsAntiAlias; }

	virtual void			getTextSize(const char* text, int length, Geometry::Rect* outRect);
	virtual void			getTextSize(const wchar_t* text, int length, Geometry::Rect* outRect);

	virtual Font*			copy();

public:

	/// グリフデータの取得 (最初の文字の場合、prevData に NULL を渡す。以降は戻り値を渡し続ける。非スレッドセーフ)
	virtual FontGlyphData* makeGlyphData(lnU32 utf32code, FontGlyphData* prevData);

	/// グリフデータの取得を終了する (メモリ解放。一連の makeGlyphData() を呼び終わった後、最後に呼ぶ)
	virtual void postGlyphData(FontGlyphData* glyphData);


   
public:

	//bool isOutlineFont() const { return ( mEdgeSize > 0 ); }



private:

	//void makeGlyphDataForMono( FT_GRyph );

	///// mDIBBitmapBuffer から mGlyphOutlineData.Bitmap に ARGB ビットマップを作成する
	////      glyph_w_, glyph_h_ : 文字部分のサイズ
	//LNRESULT _makeGlyphFromDIBBitmap( lnU32 glyph_w_, lnU32 glyph_h_ );

	/// 現在の状態からフォントを更新する (再構築)
    void updateFont();

	/// リソース解放
    void dispose();

	void _refreshBitmap( Bitmap* bitmap, FT_Bitmap* ftBitmap );

	void _reserveBitmapBuffer( int newWidth, int newHeight, int newEdgeSize );
	void _copyBitmap( Base::ReferenceBuffer* dest, FT_Bitmap* ftBitmap );

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
    FTC_ImageTypeRec        mFTImageType;		// フォント設定 (ID, サイズ, キャッシュ有無など)
    FT_Int                  mFTCacheMapIndex;

    bool                    mModified;
    lnRefString             mFontName;
    int                     mFontSize;
	LColor				mColor;
	LColor				mEdgeColor;
    lnU32                   mEdgeSize;
    bool                    mIsBold;
    bool                    mIsItalic;
    bool                    mIsAntiAlias;
    
	//LNGlyphOutlineData      mGlyphOutlineData;
    
	// グリフ生成関連
	FreeTypeFontGlyphData	mFontGlyphData;
	Bitmap					mGlyphBitmap;
	Bitmap					mOutlineBitmap;
	//Graphics::Bitmap        mGlyphBitmap;
	//int                     mBitmapRealDataBytes;
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

} // namespace Graphics
} // namespace Core
} // namespace LNote
