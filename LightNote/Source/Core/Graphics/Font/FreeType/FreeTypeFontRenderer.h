//=============================================================================
//【 FreeTypeFontRenderer 】
//-----------------------------------------------------------------------------
///**
//  @file       FreeTypeFontRenderer.h
//  @brief      FreeTypeFontRenderer
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
class FreeTypeFont;

struct TargetBitmap
{
    lnByte* Bitmap;
    int     Width;
    int     Height;
};

//=============================================================================
// ■ FreeTypeFontRenderer
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class FreeTypeFontRenderer
{
public:

    

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