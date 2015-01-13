//=============================================================================
//【 FreeTypeFontRenderer 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <ft2build.h>
#include FT_FREETYPE_H	/* <freetype/freetype.h> */
#include FT_CACHE_H	/* <freetype/ftcache.h> */
#include FT_TRUETYPE_TAGS_H	/* <freetype/tttags.h> */
#include FT_TRUETYPE_TABLES_H	/* <freetype/tttables.h> */
#include FT_SFNT_NAMES_H
#include "../../../Base/StringUtil.h"
#include "../../../FileIO/Interface.h"
#include "../../../FileIO/Manager.h"
#include "../../Common/BitmapUtil.h"
#include "FreeTypeFont.h"
#include "FreeTypeFontRenderer.h"

//#include "../../../System/Common/PerfStopwatch.h"
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{


    //void bitbltLine( TargetBitmap* dest, int x, int y, lnByte* line, int line_width )
    //{
    //    // 32 bit
    //    if ( 1 )
    //    {
    //        int dx = // 転送開始 x 座標
    //        int count = 
    //    }
    //}

    void bitblt( TargetBitmap* dest, const LRect& dest_rect, TargetBitmap* src, bool up_flow )
    {
        LRect target_rect = dest_rect;
        target_rect.clip( LRect( 0, 0, dest->Width, dest->Height ) );

        target_rect.w = ( src->Width  < target_rect.w ) ? src->Width  : target_rect.w;
        target_rect.h = ( src->Height < target_rect.h ) ? src->Height : target_rect.h;

        // dest は 32 bit、src は 8bit
        if ( 1 )
        {
            lnU32* dest_bitmap = (lnU32*)dest->Bitmap;

            for ( int y = 0; y < target_rect.h; ++y )
            {
                lnByte* line = up_flow ?
                    &src->Bitmap[(src->Width * src->Height) - src->Width * y] :
                    &src->Bitmap[src->Width * y];

                for ( int x = 0; x < target_rect.w; ++x )
                {
                    lnByte c = line[x];

                    dest_bitmap[y * 256 + x] = //c | 0xff000000;//c || 0xff0000ff;
                        ( ( ( ( ( 0xff00ffff >> 24 ) & 0xff ) * c ) >> 8 ) << 24 ) |
                        ( ( ( ( ( 0xff00ffff >> 16 ) & 0xff ) * c ) >> 8 ) << 16 ) |
                        ( ( ( ( ( 0xff00ffff >> 8  ) & 0xff ) * c ) >> 8 ) <<  8 ) |
                        ( ( ( ( ( 0xff00ffff       ) & 0xff ) * c ) >> 8 )       ); 
                }
            }
        }
    }

//=============================================================================
// ■ FreeTypeFontRenderer クラス
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    FreeTypeFontRenderer::FreeTypeFontRenderer()
        : mFont         ( NULL )
        , mTargetBitmap ( NULL )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    FreeTypeFontRenderer::~FreeTypeFontRenderer()
    {
    }

    void FreeTypeFontRenderer::setFont( IFont* font ) { mFont = dynamic_cast<FreeTypeFont*>(font); }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void FreeTypeFontRenderer::drawCharacter( const LRect& rect, lnU32 utf32code, lnU32 mul_color )
    {
        LNFontGlyphData* data = mFont->makeGlyphData( utf32code, NULL );

        BitmapUtil::BitmapData dest;
        dest.Buffer = mTargetBitmap->Bitmap;
        dest.Width  = mTargetBitmap->Width;
        dest.Height = mTargetBitmap->Height;
        dest.Pitch  = 0;
        dest.Format = LN_BITMAPFORMAT_A8R8G8B8;

        BitmapUtil::BitmapData src;
        src.Buffer  = data->Bitmap->Buffer;
        src.Width   = data->Bitmap->Width;
        src.Height  = data->Bitmap->Height;
        src.Pitch   = data->Bitmap->Pitch;
        src.Format  = data->Bitmap->Format;

        BitmapUtil::bitblt(
            &dest,
            LRect( rect.x + data->GlyphOffsetX, rect.y + data->GlyphOffsetY, rect.w - data->GlyphOffsetX, rect.h - data->GlyphOffsetY ),
            &src,
            LRect( 0, 0, src.Width, src.Height ),
            mul_color,
            true );

        mFont->postGlyphData( data );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void FreeTypeFontRenderer::drawString( const LRect& rect, lnU32* utf32string, int len, lnU32 mul_color )
    {
        int i = 0;
        LNFontGlyphData* data = NULL;

        while ( 1 )
        {
            if ( len < 0 )
            {
                if ( utf32string[i] == '\0' ) break;
            }
            else
            {
                if ( i >= len ) break;
            }

            data = mFont->makeGlyphData( utf32string[i], data );

            BitmapUtil::BitmapData dest;
            dest.Buffer = mTargetBitmap->Bitmap;
            dest.Width  = mTargetBitmap->Width;
            dest.Height = mTargetBitmap->Height;
            dest.Pitch  = 0;
            dest.Format = LN_BITMAPFORMAT_A8R8G8B8;

            BitmapUtil::BitmapData src;
            src.Buffer  = data->Bitmap->Buffer;
            src.Width   = data->Bitmap->Width;
            src.Height  = data->Bitmap->Height;
            src.Pitch   = data->Bitmap->Pitch;
            src.Format  = data->Bitmap->Format;

            BitmapUtil::bitblt(
                &dest,
                LRect( rect.x + data->GlyphOffsetX, rect.y + data->GlyphOffsetY, rect.w - data->GlyphOffsetX, rect.h - data->GlyphOffsetY ),
                &src,
                LRect( 0, 0, src.Width, src.Height ),
                mul_color,
                true );

            ++i;
        }

        mFont->postGlyphData( data );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void FreeTypeFontRenderer::drawString( const LRect& rect, const char* sjis_string, int len, lnU32 mul_color )
    {
        int bytes = strlen( sjis_string );
        lnU8* utf8str = LN_NEW lnU8[bytes * 6];
        memset(utf8str, 0, sizeof(lnU8) * bytes * 6);
        Base::StringUtil::convertSJISToUTF8N( sjis_string, bytes, utf8str, bytes * 6 );

        int i = 0;
        int count = 0;
        LNFontGlyphData* data = NULL;

        while ( 1 )
        {
            if ( len < 0 )
            {
                if ( utf8str[i] == '\0' ) break;
            }
            else
            {
                if ( count >= len ) break;
            }

            lnU32 utf32code;
            int read_len = 0;
            Base::StringUtil::convertCodeUTF8ToUTF32BE( &utf8str[i], &read_len, &utf32code );
            i += read_len;

            data = mFont->makeGlyphData( utf32code, data );

            BitmapUtil::BitmapData dest;
            dest.Buffer = mTargetBitmap->Bitmap;
            dest.Width  = mTargetBitmap->Width;
            dest.Height = mTargetBitmap->Height;
            dest.Pitch  = 0;
            dest.Format = LN_BITMAPFORMAT_A8R8G8B8;

            BitmapUtil::BitmapData src;
            src.Buffer  = data->Bitmap->Buffer;
            src.Width   = data->Bitmap->Width;
            src.Height  = data->Bitmap->Height;
            src.Pitch   = data->Bitmap->Pitch;
            src.Format  = data->Bitmap->Format;

            BitmapUtil::bitblt(
                &dest,
                LRect( rect.x + data->GlyphOffsetX, rect.y + data->GlyphOffsetY, rect.w - data->GlyphOffsetX, rect.h - data->GlyphOffsetY ),
                &src,
                LRect( 0, 0, src.Width, src.Height ),
                mul_color,
                true );

            ++count;
        }

        mFont->postGlyphData( data );


        SAFE_DELETE_ARRAY( utf8str );
    }
  
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//
//=============================================================================