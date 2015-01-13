//=============================================================================
//【 FreeTypeFont 】
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
#include FT_BITMAP_H            /* freetype/ftbitmap.h */
#include FT_OUTLINE_H
#include FT_SYNTHESIS_H
#include FT_STROKER_H
#include "../../../FileIO/Interface.h"
#include "../../../Base/StringUtil.h"
#include "FreeTypeFont.h"

#include "../../../System/Common/PerfStopwatch.h"

#define FT_PIX_FLOOR( x )     ( (x) & ~63 )
#define FT_PIX_ROUND( x )     FT_PIX_FLOOR( (x) + 32 )
#define FT_PIX_CEIL( x )      FT_PIX_FLOOR( (x) + 63 )


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
// ■ FreeTypeFont クラス
//=============================================================================

    /*
        時間計測…
            FTC_CMapCache_Lookup, FTC_ImageCache_Lookup
            文字設定              サイズ設定・グリフ取得
            
            同じフォントサイズ・同じ文字
                4, 1522
                1, 1
                0, 0
                0, 0
                0, 0

            同じフォントサイズ・文字++      2 週目はすべて 0ns
                5, 1167
                1, 78
                1, 52
                1, 46
                1, 33

            フォントサイズ++・同じ文字      2週目も同様
                4, 1147 (ns)
                0, 60
                0, 62
                0, 66
                0, 61

            フォントサイズ++・文字++        2週目も同様
                5, 1158
                1, 85
                1, 81
                1, 55
                1, 40

            サイズ、文字どちらか一方でも新しいものが来るといろいろ再構築されるみたい。
            ただし、上の各条件でさらにもう1周させた場合、
            フォントサイズが同じであれば 0ns だった。
            キャッシュの中ではサイズの方が偉いみたい。

            また、Face を作るのに必要な時間は FT_Open_Face で 300ns 程度。
            ファイルロード・メモリ確保入れると 3000ns くらいになる。

        設計方針…
            ・フォントファイルひとつ分のメモリ領域は、同名フォントで共有
            ・FreeTypeFont クラスひとつに対して FT_Face はひとつ。
                
    */

    //---------------------------------------------------------------------
    // 
    //---------------------------------------------------------------------
    FreeTypeFont::FreeTypeFont( FreeTypeFontManager* manager )
        : mManager      ( manager )
        , mFTFace       ( NULL )
        , mFTCacheMapIndex  ( 0 )
        , mModified     ( false )
        , mFontName     ()
        , mFontSize     ( 0 )
        , mColor        ()
        , mEdgeColor    ( 0, 0, 0, 1 )
        , mEdgeSize     ( 0 )
        , mIsBold       ( false )
        , mIsItalic     ( false )
        , mIsAntiAlias  ( true )
        , mCopyGlyph    ( NULL )
        , mBitmapRealDataBytes   ( 0 )
        , mPixelList    ( NULL )
        , mFTStroker            ( NULL )
    
    /*
    mFontHandle           ( NULL )
        , mFontName             ( LN_DEFAULT_FONT_NAME )
        , mFontSize             ( 22 )
        , mGlyphType            ( LN_GT_NOEDGE_NOANTI )
        , mEdgeColor            ( 0, 0, 0, 1 )
        , mEdgeSize             ( 0 )
        , mRealEdgeSize         ( 0 )
        , mOutlineQuality       ( FONT_ANTI_QUALITY )
        , mMemDC                ( NULL )
        , mDIBBitmap            ( NULL )
        , mDIBBitmapBuffer      ( NULL )
        , mDIBBitmapWidth       ( 0 )
        , mDIBBitmapHeight      ( 0 )
        , mOutlinePen           ( NULL )
        , mOutlineBrush         ( NULL )
        , mOutlineBGBrush       ( NULL )
        
        , mBitmapBufferSize     ( 0 )
        , mHDC                  ( NULL )
        
        , mOldFont              ( NULL )
        
        , mNeedUpdate           ( true )
        , mBeganGetGlyphOutline ( false )
        */
    {
        //memset( &mGlyphOutlineData, 0, sizeof( mGlyphOutlineData ) );
        memset( &mGlyphBitmap, 0, sizeof( mGlyphBitmap ) );
        mFontGlyphData.Bitmap = &mGlyphBitmap;

        LN_SAFE_ADDREF( mManager );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    FreeTypeFont::~FreeTypeFont()
    {
        dispose();
        LN_SAFE_RELEASE( mManager );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    LNRESULT FreeTypeFont::initialize(
        const lnChar* fontname,
        lnU32 size,
        const LColor& color,
        bool bold,
        bool italic )
    {
        mFontName = fontname;
        mFontSize = 22;
        mEdgeSize = 0;
  //      mColor = color_;

  //      mLogFont.lfHeight = size_ * mOutlineQuality;
		//mLogFont.lfWidth = 0;
		//mLogFont.lfEscapement = 0;
		//mLogFont.lfOrientation = 0;
		//mLogFont.lfWeight = ( ( bold_ ) ? FW_BOLD : FW_NORMAL );
		//mLogFont.lfItalic = ( ( italic_ ) ? TRUE : FALSE );
		//mLogFont.lfUnderline = FALSE;
		//mLogFont.lfStrikeOut = FALSE;
		//mLogFont.lfCharSet = SHIFTJIS_CHARSET;
		//mLogFont.lfOutPrecision = OUT_TT_ONLY_PRECIS;
		//mLogFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		//mLogFont.lfQuality = ANTIALIASED_QUALITY | PROOF_QUALITY;	// PROOF_QUALITY;
		//mLogFont.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;
  //      //wcscat_s( mLogFont.lfFaceName, LF_FACESIZE, mFontName.c_str() );
  //      _tcscat( mLogFont.lfFaceName, mFontName.c_str() );

        //_updateFont();
        mModified = true;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    LNRESULT FreeTypeFont::getTextSize( const lnChar* text_, int len_, Geometry::Rect* rect_ )
    {
        return LN_OK;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    LNFontGlyphData* FreeTypeFont::makeGlyphData( lnU32 utf32code, LNFontGlyphData* prev_data )
    {


        LNFontGlyphData* glyph_data;

        updateFont();

        if ( prev_data )
        {
            glyph_data = prev_data;
        }
        else
        {
            glyph_data = &mFontGlyphData;
            mGlyphBitmap.Format = LN_BITMAPFORMAT_A8;
            //glyph_data = LN_NEW LNFontGlyphData;
            glyph_data->NextBaseX      = 0;
            //glyph_data->BitmapRealDataSize = mFontSize * mFontSize;
            //glyph_data->Bitmap = LN_NEW Graphics::Bitmap;
            //glyph_data->Bitmap->Buffer = LN_NEW lnByte[glyph_data->BitmapRealDataSize];
            //glyph_data->Bitmap->Pitch  = 0;
            //glyph_data->Bitmap->Format = LN_BITMAPFORMAT_A8;
        }
        FT_UInt glyph_index = FTC_CMapCache_Lookup(
			mManager->getFTCacheMapCache(),
			mFTFaceID,
			mFTCacheMapIndex,
			utf32code );

        FT_Glyph glyph;
        FT_Error err = FTC_ImageCache_Lookup(
			mManager->getFTCImageCache(),
			&mFTImageType,
			glyph_index,
			&glyph,
			NULL );
        LN_THROW_InvalidCall( err == 0, "failed FTC_ImageCache_Lookup : %d\n", err );

        {
            int strength = (700 - 400) / 8;	// 太さ (700 = FW_BOLD)
            //FT_Outline_Embolden( &((FT_OutlineGlyph)glyph)->outline, strength );
        }

        memset( mGlyphBitmap.Buffer, 0, mBitmapRealDataBytes );
        mPixelMinX = mFontSize;
        mPixelMinY = mFontSize;
        mPixelMaxX = -mFontSize;
        mPixelMaxY = -mFontSize;

        if ( mFTStroker )
        {
            FT_Glyph_StrokeBorder(&glyph, mFTStroker, 0, 0);
            mtest = false;
            renderSpans( (FT_OutlineGlyph)glyph );

            // 内側部分のレンダリング用に再取得
            err = FTC_ImageCache_Lookup(
			    mManager->getFTCImageCache(),
			    &mFTImageType,
			    glyph_index,
			    &glyph,
			    NULL );
            LN_THROW_InvalidCall( err == 0, "failed FTC_ImageCache_Lookup : %d\n", err );
        }
        
        //mtest = true;
        mtest = false;
        renderSpans( (FT_OutlineGlyph)glyph );

        FT_BBox cbox;
        FT_Outline_Get_CBox( &((FT_OutlineGlyph)glyph)->outline, &cbox );

        // 転送座標オフセットを進める
        glyph_data->NextBaseY = (mFTFace->height + mFTFace->descender) *
		                         mFTFace->size->metrics.y_ppem / mFTFace->units_per_EM;
        glyph_data->GlyphOffsetX = glyph_data->NextBaseX + (FT_Int)( cbox.xMin >> 6 );
        glyph_data->GlyphOffsetY = glyph_data->NextBaseY - (FT_Int)( cbox.yMax >> 6 );

        // 直前の glyph_index として記憶
        glyph_data->Previous = glyph_index;

        if ( prev_data )
        {
            // カーニング
		    if( (FT_HAS_KERNING(mFTFace)) && prev_data->Previous && glyph_index)
            {
			    FT_Vector delta;
			    FT_Get_Kerning(mFTFace,
				    prev_data->Previous, glyph_index,
				    ft_kerning_default, &delta);
			    glyph_data->NextBaseX += delta.x >> 6;
		    }
        }
        else
        {
            // 初回の生成で負の値は許さない (j 等が -1 となってしまい、想定した描画領域からはみ出す)
            if ( glyph_data->GlyphOffsetX < 0 ) glyph_data->GlyphOffsetX = 0;
            if ( glyph_data->GlyphOffsetY < 0 ) glyph_data->GlyphOffsetY = 0;
        }

        glyph_data->NextBaseX += glyph->advance.x >> 16;
        return glyph_data;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void FreeTypeFont::postGlyphData( LNFontGlyphData* glyph_data )
    {
        if ( glyph_data )
        {
            //SAFE_DELETE_ARRAY( glyph_data->Bitmap->Buffer );
            //SAFE_DELETE( glyph_data->Bitmap );
            //SAFE_DELETE( glyph_data );
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    LNRESULT FreeTypeFont::beginGetGlyphOutline()
    {
        updateFont();

      
        return LN_OK;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    LNRESULT FreeTypeFont::endGetGlyphOutline()
    {
        if ( mCopyGlyph )
			FT_Done_Glyph( mCopyGlyph );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● フォント文字のビットマップ情報を取得する (char)
    //---------------------------------------------------------------------
    LNRESULT FreeTypeFont::getGlyphOutline( const char* code_, LNGlyphOutlineData** data_ )
    {
        
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● フォント文字のビットマップ情報を取得する (wchar_t)
    //---------------------------------------------------------------------
    LNRESULT FreeTypeFont::getGlyphOutline( const wchar_t* code, LNGlyphOutlineData** data )
    {
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● フォントのコピーを作成する
    //---------------------------------------------------------------------
    LNRESULT FreeTypeFont::deepCopy( IFont** font_ )
    {
        return LN_OK;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void FreeTypeFont::updateFont()
    {
        if ( mModified )
        {
            dispose();
            
            mFTFaceID = (FTC_FaceID)mFontName.c_str();
            FTC_Manager ftc_manager = mManager->getFTCacheManager();
            FT_Error err = FTC_Manager_LookupFace( ftc_manager, mFTFaceID, &mFTFace );
            LN_THROW_InvalidCall( err == 0, "failed FTC_Manager_LookupFace : %d\n", err );

            if ( mIsItalic )
            {
                FT_Vector transform = { 0, 0 };
                FT_Matrix matrix;
                matrix.xx = 1 << 16;
                matrix.xy = 0x5800;
                matrix.yx = 0;
                matrix.yy = 1 << 16;
                FT_Set_Transform( mFTFace, &matrix, &transform );
            }
            else
            {
                FT_Set_Transform( mFTFace, NULL, NULL );
            }

            if ( mEdgeSize > 0 )
            {
                FT_Stroker_New(mManager->getFTLibrary(), &mFTStroker);
                FT_Stroker_Set(mFTStroker,
                       (int)(mEdgeSize * 64),
                       FT_STROKER_LINECAP_ROUND,
                       FT_STROKER_LINEJOIN_ROUND,
                       0 );
            }

#define RESOLUTION_X 72
#define RESOLUTION_Y 72

            // font_typeを設定
	        mFTImageType.face_id = mFTFaceID;
	        mFTImageType.width   = 0;
	        mFTImageType.height  = mFontSize;
	        /* ビットマップまでキャッシュする場合はFT_LOAD_RENDER | FT_LOAD_TARGET_*
	         * とする。ただし途中でTARGETを変更した場合等はキャッシュが邪魔する。
	         * そういう時はFT_LOAD_DEFAULTにしてFTC_ImageCache_Lookup後に
	         * FT_Glyph_To_Bitmapしたほうが都合がいいと思う。
	         */

            mFTImageType.flags = FT_LOAD_NO_BITMAP;

            mFTCacheMapIndex = FT_Get_Charmap_Index( mFTFace->charmap );


            mPixelList = LN_NEW PixelData[mFontSize * mFontSize * 4];

            mModified = false;
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void FreeTypeFont::dispose()
    {
        SAFE_DELETE_ARRAY( mGlyphBitmap.Buffer );
        SAFE_DELETE_ARRAY( mPixelList );
        if ( mFTStroker )
        {
            FT_Stroker_Done( mFTStroker );
            mFTStroker = NULL;
        }

        // キャッシュを使用している場合、解放の必要はない。
        // 解放してしまうと、CacheManager の解放時に NULL アクセスが起こる。
        //if ( mFTFace )
        //{
        //    FT_Done_Face( mFTFace );
        //    mFTFace = NULL;
        //}
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void FreeTypeFont::renderSpans( FT_OutlineGlyph glyph )
    {
        mPixelCount = 0;

        FT_Raster_Params params;
        memset(&params, 0, sizeof(params));
        params.gray_spans = rasterCallback;
        params.user = this;
        if ( mIsAntiAlias ) params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
        else                params.flags = FT_RASTER_FLAG_DIRECT;

        FT_Outline_Render( mManager->getFTLibrary(), &glyph->outline, &params );

        int width  = mPixelMaxX - mPixelMinX + 1;
        int height = mPixelMaxY - mPixelMinY + 1;
        mGlyphBitmap.Width  = width;
        mGlyphBitmap.Height = height;

        if ( width * height > mBitmapRealDataBytes )
        {
            SAFE_DELETE_ARRAY( mGlyphBitmap.Buffer );
            mBitmapRealDataBytes = width * height;
            mGlyphBitmap.Buffer = LN_NEW lnByte[mBitmapRealDataBytes];
            memset( mGlyphBitmap.Buffer, 0, mBitmapRealDataBytes );
        }

        for ( int i = 0; i < mPixelCount; ++i )
        {
            //printf("%d ", (mGlyphBitmap.Height - 1 - (mPixelList[i].Y - mPixelMinY) ));
            //printf( "x:%d y:%d w:%d h:%d minX:%d minY:%d\n",
            //    (mPixelList[i].X - mPixelMinX), 
            //    (mGlyphBitmap.Height - 1 - (mPixelList[i].Y - mPixelMinY) ),
            //    width, height,
            //    mPixelMinX, mPixelMinY);

            mGlyphBitmap.Buffer[
                (mGlyphBitmap.Width * (mGlyphBitmap.Height - 1 - (mPixelList[i].Y - mPixelMinY) )) +
                (mPixelList[i].X - mPixelMinX)
            ] = (mtest)? 0 : mPixelList[i].Coverage;
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void FreeTypeFont::rasterCallback(
        const int y,
        const int count,
        const FT_Span * const spans,
        void * const user)
    {
        FreeTypeFont* font = (FreeTypeFont*)user;

        font->mPixelMinY = std::min( y, font->mPixelMinY );
        font->mPixelMaxY = std::max( y, font->mPixelMaxY );

        for (int i = 0; i < count; ++i) 
        {
            for (int cx = 0; cx < spans[i].len; ++cx) 
            {
                font->mPixelList[font->mPixelCount].X = spans[i].x + cx;
                font->mPixelList[font->mPixelCount].Y = y;
                font->mPixelList[font->mPixelCount].Coverage = spans[i].coverage;
                font->mPixelMinX = std::min( (int)font->mPixelList[font->mPixelCount].X, font->mPixelMinX );
                font->mPixelMaxX = std::max( (int)font->mPixelList[font->mPixelCount].X, font->mPixelMaxX );
                ++font->mPixelCount;
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