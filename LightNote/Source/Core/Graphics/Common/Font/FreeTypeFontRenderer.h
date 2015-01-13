//==============================================================================
// FreeTypeFontRenderer 
//------------------------------------------------------------------------------
///**
//  @file       FreeTypeFontRenderer.h
//  @brief      FreeTypeFontRenderer
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Bitmap.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{
#if 1
#else
class FreeTypeFont;

struct TargetBitmap
{
    lnByte* Bitmap;
    int     Width;
    int     Height;
};
#endif

//==============================================================================
// ■ FreeTypeFontRenderer
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class FreeTypeFontRenderer
{


#if 1
public:
	FreeTypeFontRenderer( Font* font, Bitmap* targetBitmap );
	~FreeTypeFontRenderer();

	/// 文字列描画
	void drawText(const Base::UnicodeString& text, int len, const LRect& rect, TextAlign align);

	void drawLine(const wchar_t* text, int len, const LRect& rect, int* lineHeight, TextAlign align);

private:
	//FreeTypeFont*	mFont;
	Font*			mFont;
	Bitmap*			mTargetBitmap;

#else
    

public:
    FreeTypeFontRenderer();
    ~FreeTypeFontRenderer();

public:

    void setFont( IFont* font );

    void setTargetBitmap( TargetBitmap* bitmap ) { mTargetBitmap = bitmap; }

    void drawCharacter( const LRect& rect, lnU32 utf32code, lnU32 mul_color );

    void drawString( const LRect& rect, lnU32* utf32string, int len, lnU32 mul_color );

	/// テスト用
    void drawString( const LRect& rect, const char* sjis_string, int len, lnU32 mul_color );

	//void drawText( TargetBitmap* bitmap, FreeTypeFont* font,  );

private:

    FreeTypeFont*   mFont;
    TargetBitmap*   mTargetBitmap;
#endif
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================