//==============================================================================
// FreeTypeFont 
// http://code.google.com/p/freetype-gl/source/browse/trunk/texture-font.c?r=105
/*
	太字 斜体
	http://hp.vector.co.jp/authors/VA028002/freetype.html


	テキストレイアウト
	http://e-class.center.yuge.ac.jp/jdk_docs/ja/technotes/guides/2d/spec/j2d-fonts.html

	縦書き
	http://d.hatena.ne.jp/project_the_tower2/20100509/1273370298
*/
//==============================================================================

#include "stdafx.h"
#include <algorithm>
#include "../../../FileIO/Interface.h"
#include "../../../Base/Unicode.h"
#include "../../../Base/StringUtils.h"
#include "FreeTypeFont.h"

#define FT_PIX_FLOOR( x )     ( (x) & ~63 )
#define FT_PIX_ROUND( x )     FT_PIX_FLOOR( (x) + 32 )
#define FT_PIX_CEIL( x )      FT_PIX_FLOOR( (x) + 63 )

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ FreeTypeFont
//==============================================================================

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

	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
    FreeTypeFont::FreeTypeFont( FreeTypeFontManager* manager )
        : mManager      ( manager )
        , mFTFace       ( NULL )
        , mFTCacheMapIndex  ( 0 )
        , mModified     ( false )
        , mFontName     ()
        , mFontSize     ( 0 )
		, mColor(LColor::Gray)
        , mEdgeColor    ( 0, 0, 0, 1 )
        , mEdgeSize     ( 0 )
        , mIsBold       ( false )
        , mIsItalic     ( false )
        , mIsAntiAlias  ( false )
        //, mCopyGlyph    ( NULL )
        //, mBitmapRealDataBytes   ( 0 )
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
        //memset( &mGlyphBitmap, 0, sizeof( mGlyphBitmap ) );
        //mFontGlyphData.Bitmap = &mGlyphBitmap;

		mFontGlyphData.GlyphBitmap = &mGlyphBitmap;
		mFontGlyphData.CopyGlyph = NULL;

        LN_SAFE_ADDREF( mManager );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    FreeTypeFont::~FreeTypeFont()
    {
        dispose();
        LN_SAFE_RELEASE( mManager );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
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
		mIsAntiAlias = true;
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
		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void FreeTypeFont::getTextSize( const char* text, int len, Geometry::Rect* rect )
    {
		Base::UnicodeString unistr;
		unistr.setDecodeBytes( text, len );
		getTextSize(unistr.getString(), unistr.getCharCount(), rect);
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void FreeTypeFont::getTextSize(const wchar_t* text, int length, Geometry::Rect* outRect)
	{
		updateFont();

		FT_Bool useKerning = FT_HAS_KERNING(mFTFace);
		FT_UInt previous = 0;

		int w = 0;
		const wchar_t* code = text;
		int count = 0;
		for (; *code; ++code)
		{
			FT_UInt glyph_index = FTC_CMapCache_Lookup(
				mManager->getFTCacheMapCache(),
				mFTFaceID,
				mFTCacheMapIndex,
				*code);

			// カーニング
			if (useKerning && previous != 0 && glyph_index)
			{
				FT_Vector delta;
				FT_Get_Kerning(
					mFTFace,
					previous,
					glyph_index,
					ft_kerning_default,
					&delta);
				w += delta.x >> 6;
			}
			previous = glyph_index;

			FT_Glyph glyph;
			FT_Error err = FTC_ImageCache_Lookup(
				mManager->getFTCImageCache(),
				&mFTImageType,
				glyph_index,
				&glyph,
				NULL);
			LN_THROW_InvalidOperation(err == 0, "failed FTC_ImageCache_Lookup : %d\n", err);

			w += glyph->advance.x >> 16;

			//FT_Size_Metrics
			//_p(mFTFace->size->metrics.height >> 6);
			count++;
			if (count >= length)
				break;
		}

		//glyphData->NextBaseY = (mFTFace->height + mFTFace->descender) *
		//							mFTFace->size->metrics.y_ppem / mFTFace->units_per_EM;
		//glyphData->GlyphOffsetX = glyphData->NextBaseX + glyph_bitmap->left;
		//glyphData->GlyphOffsetY = glyphData->NextBaseY - glyph_bitmap->top;

		outRect->set(0, 0, w, mFTFace->size->metrics.height >> 6);//mFTImageType.height/*mFTFace->size->metrics.height >> 6*/ );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Font* FreeTypeFont::copy()
    {
		LRefPtr<FreeTypeFont> font( LN_NEW FreeTypeFont( mManager ) );
		font->initialize( mFontName.c_str(), mFontSize, mColor, mIsBold, mIsItalic );
		font->setEdgeColor( mEdgeColor );
		font->setEdgeSize( mEdgeSize );
		font->setAntiAlias( mIsAntiAlias );
		font->addRef();
		return font;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    FontGlyphData* FreeTypeFont::makeGlyphData( lnU32 utf32code, FontGlyphData* prevData_ )
	{
		FreeTypeFontGlyphData* prevData = static_cast<FreeTypeFontGlyphData*>(prevData_);
		FreeTypeFontGlyphData* glyphData;

		// ひとつ前のデータがあればそれをベースに
        if ( prevData )
        {
			glyphData = prevData;

			if ( glyphData->CopyGlyph ) {
				FT_Done_Glyph( glyphData->CopyGlyph );
				glyphData->CopyGlyph = NULL;
			}
			if ( glyphData->CopyOutlineGlyph ) {
				FT_Done_Glyph( glyphData->CopyOutlineGlyph );
				glyphData->CopyOutlineGlyph = NULL;
			}
        }
		// 最初の文字であればデータリセット
        else
        {
			updateFont();

            glyphData = &mFontGlyphData;
            glyphData->NextBaseX      = 0;
			glyphData->Previous = 0;
			glyphData->CopyGlyph = NULL;
			glyphData->CopyOutlineGlyph = NULL;
        }

		//-----------------------------------------------------
		// グリフ取得

		/* キャッシュを使うときは…
			FT_Set_Char_Size のかわりに FTC_Manager_LookupSize
			FT_Get_Char_Index のかわりに FTC_CMapCache_Lookup
			FT_Load_Glyph のかわりに FTC_ImageCache_Lookup
		*/

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
        LN_THROW_InvalidOperation( err == 0, "failed FTC_ImageCache_Lookup : %d\n", err );

		// 太字フォント
		if ( mIsBold )
		{
			// アウトラインフォントである必要がある
			LN_THROW_InvalidOperation( (glyph->format == FT_GLYPH_FORMAT_OUTLINE), "glyph->format != FT_GLYPH_FORMAT_OUTLINE" );

			FT_Pos strength = (800 - 400) / 8;	// 太さ
			err = FT_Outline_Embolden( &mFTFace->glyph->outline, strength );
			LN_THROW_InvalidOperation( err == 0, "failed FT_Outline_Embolden : %d\n", err );
		}
		
		FT_Render_Mode renderMode = ( mIsAntiAlias ) ? FT_RENDER_MODE_NORMAL : FT_RENDER_MODE_MONO;

		FT_BitmapGlyph glyph_bitmap;
		if ( glyph->format == FT_GLYPH_FORMAT_BITMAP ) {
			// FT_LOAD_NO_BITMAP が OFF だとここに入ってくる
			glyph_bitmap = (FT_BitmapGlyph)glyph;
		}
		else
		{
			err = FT_Glyph_Copy( glyph, &glyphData->CopyGlyph );
			LN_THROW_InvalidOperation( err == 0, "failed FT_Glyph_Copy : %d\n", err );

			/* FT_RENDER_MODE_NORMALで普通の(256階調)アンチエイリアス
				* FT_RENDER_MODE_LCDで液晶用アンチエイリアス(サブピクセルレンダリング)
				*/
			err = FT_Glyph_To_Bitmap( &glyphData->CopyGlyph, renderMode, NULL, 1 );
			LN_THROW_InvalidOperation( err == 0, "failed FT_Glyph_To_Bitmap : %d\n", err );

			glyph_bitmap = (FT_BitmapGlyph)glyphData->CopyGlyph;
		}

		FT_Bitmap* ft_bitmap = &glyph_bitmap->bitmap;
		_refreshBitmap(&mGlyphBitmap, ft_bitmap);

		if ( mFTStroker )
		{
			err = FT_Glyph_Copy( glyph, &glyphData->CopyOutlineGlyph );
			LN_THROW_InvalidOperation( err == 0, "failed FT_Glyph_Copy : %d\n", err );

			err = FT_Glyph_StrokeBorder( &glyphData->CopyOutlineGlyph, mFTStroker, 0, 1 );
			LN_THROW_InvalidOperation( err == 0, "failed FT_Glyph_StrokeBorder : %d\n", err );

			err = FT_Glyph_To_Bitmap( &glyphData->CopyOutlineGlyph, renderMode, NULL, 1 );
			LN_THROW_InvalidOperation( err == 0, "failed FT_Glyph_To_Bitmap : %d\n", err );

			glyph_bitmap = (FT_BitmapGlyph)glyphData->CopyOutlineGlyph;



			FT_Bitmap* ft_bitmap = &glyph_bitmap->bitmap;
			_refreshBitmap( &mOutlineBitmap, ft_bitmap );

			mFontGlyphData.OutlineBitmap = &mOutlineBitmap;
			mFontGlyphData.OutlineOffset = mEdgeSize;
		}
		else 
		{
			mFontGlyphData.OutlineBitmap = NULL;
			mFontGlyphData.OutlineOffset = 0;
		}

		//-----------------------------------------------------
		// 位置調整

		// 転送座標オフセットを進める
		glyphData->NextBaseY = (mFTFace->height + mFTFace->descender) *
									mFTFace->size->metrics.y_ppem / mFTFace->units_per_EM;
		glyphData->GlyphOffsetX = glyphData->NextBaseX + glyph_bitmap->left;
		glyphData->GlyphOffsetY = glyphData->NextBaseY - glyph_bitmap->top;

		// ひとつ前のデータがある場合、ひとつ前の文字からの位置を求める
		if ( prevData )
		{
			// カーニング
			if( (FT_HAS_KERNING(mFTFace)) && prevData->Previous && glyph_index)
			{
				FT_Vector delta;
				FT_Get_Kerning(mFTFace,
					prevData->Previous, glyph_index,
					ft_kerning_default, &delta);
				glyphData->NextBaseX += delta.x >> 6;
			}
		}
		// 初回文字 (リセット)
		else
		{
			// 初回の生成で負の値は許さない (j 等が -1 となってしまい、想定した描画領域からはみ出す)
			//if ( glyphData->GlyphOffsetX < 0 ) glyphData->GlyphOffsetX = 0;
			//if ( glyphData->GlyphOffsetY < 0 ) glyphData->GlyphOffsetY = 0;
		}

		// 直前の glyph_index として記憶
		glyphData->Previous = glyph_index;

		// 次の文字へ
		glyphData->NextBaseX += glyph_bitmap->root.advance.x >> 16;

		// 行高さ
		glyphData->MetricsHeight = mFTFace->size->metrics.height >> 6;

        return glyphData;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void FreeTypeFont::postGlyphData(FontGlyphData* glyphData_)
	{
		FreeTypeFontGlyphData* glyphData = static_cast<FreeTypeFontGlyphData*>(glyphData_);

		if ( glyphData )
		{
			if ( glyphData->CopyGlyph ) {
				FT_Done_Glyph( glyphData->CopyGlyph );
				glyphData->CopyGlyph = NULL;
			}
			if ( glyphData->CopyOutlineGlyph ) {
				FT_Done_Glyph( glyphData->CopyOutlineGlyph );
				glyphData->CopyOutlineGlyph = NULL;
			}
		}
        //if ( glyph_data )
        //{
        //    //SAFE_DELETE_ARRAY( glyph_data->Bitmap->Buffer );
        //    //SAFE_DELETE( glyph_data->Bitmap );
        //    //SAFE_DELETE( glyph_data );
        //}
    }

    

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void FreeTypeFont::updateFont()
    {
        
            
        mFTFaceID = (FTC_FaceID)Base::Hash::calcHash( mFontName.c_str() );//(void*)1;//
        FTC_Manager ftc_manager = mManager->getFTCacheManager();
		mManager->mRequesterFaceName = mFontName.c_str();

        FT_Error err = FTC_Manager_LookupFace( ftc_manager, mFTFaceID, &mFTFace );
        LN_THROW_InvalidOperation( err == 0, "failed FTC_Manager_LookupFace : %d\n", err );

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

        mFTCacheMapIndex = FT_Get_Charmap_Index( mFTFace->charmap );

		
#define RESOLUTION_X 72
#define RESOLUTION_Y 72

		FT_Size ft_size;
		FTC_ScalerRec scaler;
		scaler.face_id = mFTFaceID;
		scaler.width = 0;
		scaler.height = mFontSize << 6;
		scaler.pixel = 0;
		scaler.x_res = RESOLUTION_X;
		scaler.y_res = RESOLUTION_Y;
		err = FTC_Manager_LookupSize( ftc_manager, &scaler, &ft_size);
		LN_THROW_InvalidOperation( err == 0, "failed FTC_Manager_LookupSize : %d\n", err );


		if ( mModified )
        {
            dispose();
            if ( mEdgeSize > 0 )
            {
                FT_Stroker_New(mManager->getFTLibrary(), &mFTStroker);
                FT_Stroker_Set(mFTStroker,
                       (int)(mEdgeSize * 64),
                       FT_STROKER_LINECAP_ROUND,
                       FT_STROKER_LINEJOIN_ROUND,
                       0 );
            }


            // font_typeを設定
	        mFTImageType.face_id = mFTFaceID;
	        mFTImageType.width   = 0;
	        mFTImageType.height  = mFontSize;
	        /* ビットマップまでキャッシュする場合はFT_LOAD_RENDER | FT_LOAD_TARGET_*
	         * とする。ただし途中でTARGETを変更した場合等はキャッシュが邪魔する。
	         * そういう時はFT_LOAD_DEFAULTにしてFTC_ImageCache_Lookup後に
	         * FT_Glyph_To_Bitmapしたほうが都合がいいと思う。
	         */
			//mFTImageType.flags = FT_LOAD_NO_BITMAP;

			/* mFTImageType.flags に　FT_LOAD_FORCE_AUTOHINT を使用すると、
			 * 一部のフォント(VL ゴシック等)で次の文字までのオフセットが正しく取れないことがある。
			 */
			 
			// アウトライン ON
			if ( mEdgeSize > 0 || mIsBold ) {
				mFTImageType.flags = FT_LOAD_NO_BITMAP;
			}
			// アウトライン OFF
			else {
				mFTImageType.flags = FT_LOAD_RENDER;
			}
			// アンチエイリアス ON
			if ( mIsAntiAlias ) {
				//mFTImageType.flags = ; そのまま
			}
			// アンチエイリアス OFF
			else {
				mFTImageType.flags |= FT_LOAD_MONOCHROME;
			}
			


            mPixelList = LN_NEW PixelData[mFontSize * mFontSize * 4];

			// グリフ格納用ビットマップ (ダミー確保)
			mGlyphBitmap.create(mFontSize, mFontSize, LN_BITMAPFORMAT_A8);
			mOutlineBitmap.create( mFontSize, mFontSize, LN_BITMAPFORMAT_A8 );

            mModified = false;
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void FreeTypeFont::dispose()
    {
        //SAFE_DELETE_ARRAY( mGlyphBitmap.Buffer );
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

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void FreeTypeFont::_refreshBitmap( Bitmap* bitmap, FT_Bitmap* ftBitmap )
	{
		int width = ftBitmap->width;
		int height = ftBitmap->rows;

		// サイズ
		bitmap->mWidth = width;
		bitmap->mHeight = height;
		bitmap->mPitch = ftBitmap->pitch;
		if ( ftBitmap->pitch < 0 ) {
			bitmap->mUpFlow = false;
		}

		// フォーマット
		if ( ftBitmap->pixel_mode == FT_PIXEL_MODE_MONO ) {
			bitmap->mFormat = LN_BITMAPFORMAT_A1;
		}
		else if ( ftBitmap->pixel_mode == FT_PIXEL_MODE_GRAY ) {
			bitmap->mFormat = LN_BITMAPFORMAT_A8;
		}
		else {
			LN_THROW_InvalidOperation( 0 );
		}

		// ビットマップデータ
		bitmap->mBitmapData->setBytesBuffer( 
			ftBitmap->buffer, 
			Bitmap::getPixelFormatByteCount( bitmap->mFormat, width, height ),
			true );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void FreeTypeFont::_reserveBitmapBuffer( int newWidth, int newHeight, int newEdgeSize )
	{
		// ビットマップを格納するバッファサイズが足りなければ再作成
		int w = (newWidth + newEdgeSize * 2);
		int h = (newWidth + newEdgeSize * 2);
		
		Base::ReferenceBuffer* bitmapData = mGlyphBitmap.getBitmapData();
        if ( w * h > bitmapData->getSize() )
        {
			bitmapData->reserve( w * h );
			bitmapData->clear();
        }

		//bitmapData = mOutlineBitmap.getBitmapData();
		//if ( w * h > bitmapData->getSize() )
  //      {
		//	bitmapData->reserve( w * h );
		//	bitmapData->clear();
  //      }
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void FreeTypeFont::_copyBitmap( Base::ReferenceBuffer* dest, FT_Bitmap* ftBitmap )
	{
		int width = ftBitmap->width;
		int height = ftBitmap->rows;

		// ビットマップバッファのコピー
		dest->clear();
		lnByte* buf = dest->getPointer();
		for (int y = 0; y < height; ++y )
		{
			lnByte* line = ftBitmap->pitch < 0 ?
				&ftBitmap->buffer[(-ftBitmap->pitch * height) - ftBitmap->pitch * y] :
				&ftBitmap->buffer[ftBitmap->pitch * y];
			for ( int x = 0; x < width; ++x )
			{
				buf[y * width + x] = line[x];
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void FreeTypeFont::renderSpans( FT_OutlineGlyph glyph )
    {
        mPixelCount = 0;

        FT_Raster_Params params;
        memset(&params, 0, sizeof(params));
        params.gray_spans = rasterCallback;
        params.user = this;
        if ( mIsAntiAlias ) params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
        else                params.flags = FT_RASTER_FLAG_DIRECT;
		// ↑FT_RASTER_FLAG_DIRECT を使用する場合はコールバックを登録する必要があるが、
		// 2.4現在、アンチエイリアス時のみしかサポートしていない。

        FT_Outline_Render( mManager->getFTLibrary(), &glyph->outline, &params );

        int width  = mPixelMaxX - mPixelMinX + 1;
        int height = mPixelMaxY - mPixelMinY + 1;
#if 0
		if ( width  != mFontGlyphData.GlyphBitmap.mWidth ||
			 height != mFontGlyphData.GlyphBitmap.mHeight )
		{
			mFontGlyphData.GlyphBitmap.create( width, height, LN_BITMAPFORMAT_A8 );
			mFontGlyphData.GlyphBitmap.mWidth  = width;
			mFontGlyphData.GlyphBitmap.mHeight = height;
		}
#else
		// ビットマップを格納するバッファサイズが足りなければ再作成
		_reserveBitmapBuffer( width, height, mEdgeSize );

		mGlyphBitmap.mWidth = width;
		mGlyphBitmap.mHeight = height;
#endif
		// ビットマップバッファに格納
		Base::ReferenceBuffer* bitmapData = mGlyphBitmap.getBitmapData();
		lnByte* buf = bitmapData->getPointer();
        for ( int i = 0; i < mPixelCount; ++i )
        {
            //printf("%d ", (mGlyphBitmap.Height - 1 - (mPixelList[i].Y - mPixelMinY) ));
            //printf( "x:%d y:%d w:%d h:%d minX:%d minY:%d\n",
            //    (mPixelList[i].X - mPixelMinX), 
            //    (mGlyphBitmap.Height - 1 - (mPixelList[i].Y - mPixelMinY) ),
            //    width, height,
            //    mPixelMinX, mPixelMinY);

            buf[
                (width * (height - 1 - (mPixelList[i].Y - mPixelMinY) )) +
                (mPixelList[i].X - mPixelMinX)
            ] = (mtest)? 0 : mPixelList[i].Coverage;
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
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

} // namespace Graphics
} // namespace Core
} // namespace LNote
