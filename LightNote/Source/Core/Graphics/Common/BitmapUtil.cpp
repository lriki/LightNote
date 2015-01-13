//=============================================================================
//【 BitmapUtil 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "BitmapUtil.h"

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
// ■ BitmapUtil
//=============================================================================

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void BitmapUtil::bitblt(
        BitmapData* dest_bitmap,
        const LRect& dest_rect,
        const BitmapData* src_bitmap,
        const LRect& src_rect,
        lnU32 mul_color,
        bool alpha_blend )
    {
        LRect dest_all( 0, 0, dest_bitmap->Width, dest_bitmap->Height );
        LRect target_rect( dest_rect.x, dest_rect.y, src_rect.w, src_rect.h );
        target_rect.clip( dest_all );

        BitmapWrapper dest_wrapper( dest_bitmap, target_rect );
        BitmapWrapper src_wrapper( src_bitmap, src_rect, true );

        if ( dest_bitmap->Format == LN_BITMAPFORMAT_A8R8G8B8 && src_bitmap->Format == LN_BITMAPFORMAT_A8 )
        {
            if ( alpha_blend )
            {
                for ( int y = 0; y < target_rect.h; ++y )
                {
                    src_wrapper.setReadLineU8( y );
                    for ( int x = 0; x < target_rect.w; ++x )
                    {
                        lnU32 src_alpha  = src_wrapper.getPixelU8( x );
                        if ( src_alpha == 0 ) continue;     // フォントならコレでかなり高速化できるはず
                        src_alpha = ( src_alpha * ( ( mul_color >> 24 ) & 0xff ) ) >> 8;    // >> 8 は / 256。乗算で最大 65535 になるのを 0～ 255 に直している

                        lnU32 dest_color = dest_wrapper.getDestPixelU32( x, y );
                        lnU32 dest_alpha = ( dest_color >> 24 ) & 0xff;

                        lnU32 a, r, g, b;

                        // photoshop 等のツール系の計算式ではやや時間がかかるため、
                        // DirectX 同様、dest のアルファは無視する方向で実装する。
                        // ただし、このままだと dest(0, 0, 0, 0) に半透明の色を合成する場合、
                        // 黒ずみが発生してしまう。テクスチャのデフォルトはこの状態。
                        // dest(1, 0, 0, 0) とかなら、ユーザーが黒と合成されることを意図していると考えられるが、
                        // 流石に完全に透明なのに黒ずむのはどうかと…。
                        // というわけで、dest_alpha == 0 なら src が 100% になるように細工している。
                        if ( dest_alpha == 0 ) a = 0xff;
                        else a = src_alpha;    

                        r = ( ( ( ( dest_color >> 16 ) & 0xff ) * ( 0xff - a ) ) >> 8 ) +
                            ( ( ( ( mul_color  >> 16 ) & 0xff ) * a ) >> 8 );

                        g = ( ( ( ( dest_color >> 8  ) & 0xff ) * ( 0xff - a ) ) >> 8 ) +
                            ( ( ( ( mul_color  >> 8  ) & 0xff ) * a ) >> 8 );

                        b = ( ( ( ( dest_color       ) & 0xff ) * ( 0xff - a ) ) >> 8 ) +
                            ( ( ( ( mul_color        ) & 0xff ) * a ) >> 8 );

                        // 書き込み用に再計算。
                        // 乗算だと、半透明を重ねるごとに薄くなってしまう。
                        // イメージとしては、重ねるごとに濃くなる加算が適切だと思う。
                        a = ( dest_alpha + src_alpha );
                        a = ( a > 255 ) ? 255 : a;

                        dest_wrapper.setPixelU32( x, y, ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | ( b ) );
                    }
                }
            }
            else
            {
                for ( int y = 0; y < target_rect.h; ++y )
                {
                    src_wrapper.setReadLineU8( y );
                    for ( int x = 0; x < target_rect.w; ++x )
                    {
                        lnByte p = src_wrapper.getPixelU8( x );
                        dest_wrapper.setPixelU32( x, y,
                            ( ( ( ( ( mul_color >> 24 ) & 0xff ) * p ) >> 8 ) << 24 ) |
                            ( ( ( ( ( mul_color >> 16 ) & 0xff ) * p ) >> 8 ) << 16 ) |
                            ( ( ( ( ( mul_color >> 8  ) & 0xff ) * p ) >> 8 ) <<  8 ) |
                            ( ( ( ( ( mul_color       ) & 0xff ) * p ) >> 8 )       )
                        ); 
                    }
                }
            }
        }
        if ( dest_bitmap->Format == LN_BITMAPFORMAT_A8R8G8B8 && src_bitmap->Format == LN_BITMAPFORMAT_A1 )
        {
            if ( alpha_blend )
            {
                for ( int y = 0; y < target_rect.h; ++y )
                {
                    src_wrapper.setReadLineU1( y );
                    
                    for ( int x = 0; x < target_rect.w; ++x )
                    {
                        lnByte bit  = src_wrapper.getPixelU1( x );
                        if ( bit == 0 ) continue;
                        lnU32 src_alpha = ( ( mul_color >> 24 ) & 0xff );

                        lnU32 dest_color = dest_wrapper.getDestPixelU32( x, y );
                        lnU32 dest_alpha = ( dest_color >> 24 ) & 0xff;

                        lnU32 a, r, g, b;

                        if ( dest_alpha == 0 ) a = 0xff;
                        else a = src_alpha;    

                        r = ( ( ( ( dest_color >> 16 ) & 0xff ) * ( 0xff - a ) ) >> 8 ) +
                            ( ( ( ( mul_color  >> 16 ) & 0xff ) * a ) >> 8 );

                        g = ( ( ( ( dest_color >> 8  ) & 0xff ) * ( 0xff - a ) ) >> 8 ) +
                            ( ( ( ( mul_color  >> 8  ) & 0xff ) * a ) >> 8 );

                        b = ( ( ( ( dest_color       ) & 0xff ) * ( 0xff - a ) ) >> 8 ) +
                            ( ( ( ( mul_color        ) & 0xff ) * a ) >> 8 );

                        a = ( dest_alpha + src_alpha );
                        a = ( a > 255 ) ? 255 : a;

                        dest_wrapper.setPixelU32( x, y, ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | ( b ) );
                    }
                }
            }
            // アルファブレンドなし
            else
            {
            }
        }
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