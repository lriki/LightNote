//==============================================================================
// Image 
//==============================================================================

#include "stdafx.h"
#include <algorithm>
#include <libpng/png.h>
#include <libpng/pnginfo.h>
#include "../../FileIO/Manager.h"
#include "Bitmap.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ PngLoader
//==============================================================================
class PngLoader
{
public:

	PngLoader()
		: mBitmapData	( NULL )
		, mPngStruct	( NULL )
		, mPngInfo		( NULL )
	{
	}

	~PngLoader()
	{
		if ( mPngStruct ) {
			png_destroy_read_struct( &mPngStruct, &mPngInfo, NULL ); 
		}
		LN_SAFE_RELEASE( mBitmapData );
	}


public:
	int						mWidth;
	int						mHeight;
	Base::ReferenceBuffer*	mBitmapData;
	LNBitmapFormat			mFormat;



	#define PNG_BYTES_TO_CHECK ( 4 )	// png 識別用の、ファイルの先頭バイト数

	/// 読み込みコールバックに渡すデータ
    struct PngData
    {
		FileIO::Stream* Stream;
        //unsigned char* Data;
        //unsigned long  Length;
        //unsigned long  Offset;
    };

	png_structp	mPngStruct;
	png_infop	mPngInfo;


public:

	/// 読み取り
	bool load( /*Bitmap* bitmap, */FileIO::Stream* stream, bool swapHeight )
	{
		png_byte	sig[PNG_BYTES_TO_CHECK];
		//size_t		size;
		int			res;

		size_t dataSize = stream->getSize();

        // サイズチェック
		if ( dataSize < PNG_BYTES_TO_CHECK ) return false;

		// データが png かどうか調べる
		stream->read( sig, 4 );
		stream->seek( 0, SEEK_SET );
		res = png_check_sig( sig, PNG_BYTES_TO_CHECK ); 
		if ( !res ) return false;

		// png_struct構造体を確保・初期化する
		mPngStruct = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
		if ( !mPngStruct ) return false;

		// png_info構造体を確保・初期化する
		mPngInfo = png_create_info_struct( mPngStruct );
		if ( !mPngInfo ) return false;

        if (setjmp(png_jmpbuf(mPngStruct))) return false;

        // png データ、読み込みコールバック設定
        PngData png_data;
        //png_data.Data   = (unsigned char*)data;
        //png_data.Offset = 0;
        //png_data.Length = dataSize;
		png_data.Stream = stream;
        png_set_read_fn( mPngStruct, (void *)&png_data, _readDataForPng );

		// シグネチャの確認で読み飛ばしたバイト数を知らせる
		//png_set_sig_bytes( mPngStruct, PNG_BYTES_TO_CHECK );

        // PNGファイルのヘッダ情報を読み込む
        png_read_png( mPngStruct, mPngInfo, PNG_TRANSFORM_EXPAND, NULL );
		//png_read_info( mPngStruct, mPngInfo );

		// IHDRチャンク情報を取得する
		//png_get_IHDR(
  //          mPngStruct, mPngInfo, &mWidth, &mHeight,
		//	&mBitDepth, &mColorType, &mInterlaceType, NULL, NULL);

		mWidth  = mPngInfo->width;
		mHeight = mPngInfo->height;

        

        // 必ず1色 8 ビットで
        if ( mPngInfo->bit_depth != 8 ) return false;

        // パレットモードは非対応
        if ( mPngInfo->color_type & PNG_COLOR_MASK_PALETTE ) return false;

        //unsigned int row_bytes = png_get_rowbytes( mPngStruct, mPngInfo );
        //mImageData = (unsigned char*) malloc( row_bytes * mHeight );

        //png_bytepp row_pointers = png_get_rows( mPngStruct, mPngInfo );

        //for (int i = 0; i < mHeight; i++) {
        //    memcpy(mImageData+(row_bytes * (i)), row_pointers[i], row_bytes);
        //}

        //printf("%x\n", *((lnU32*)mImageData));
        //printf("%x\n", *(((lnU32*)mImageData) + 1));

		//-----------------------------------------------------
        // ビットマップ格納
		
        unsigned int	row_bytes = png_get_rowbytes( mPngStruct, mPngInfo );	// 横幅のバイト数 (例えば ABGR の時は 横幅 * 4)
        png_bytepp		row_pointers = png_get_rows( mPngStruct, mPngInfo );	// ビットマップデータ

		int sign = ( swapHeight ) ? -1 : 1;			// 反転するか？
        int unit = ( swapHeight ) ? mHeight - 1 : 0;	// イテレート開始行 (一番上か、一番下か)

		// ABGR (ピクセルフォーマットは AA BB GG RR)
		// (R155, G128, B0, A78) のとき、U32 で 4e0080ff となる
        if ( mPngInfo->color_type == PNG_COLOR_TYPE_RGB_ALPHA && mPngInfo->pixel_depth == 32 )
        {
			mFormat = LN_BITMAPFORMAT_A8B8G8R8;
			mBitmapData = LN_NEW Base::ReferenceBuffer();
			mBitmapData->reserve( mWidth * mHeight * 4 );

			lnByte* bitmap = (lnByte*)mBitmapData->getPointer();

			for ( int h = 0; h < mHeight; ++h ) {
                memcpy( &bitmap[row_bytes * (unit + (sign * h))], row_pointers[h], row_bytes );
			}
		}
		// BGR
		// ABGR に拡張して読み込む
        else if ( mPngInfo->color_type == PNG_COLOR_TYPE_RGB && mPngInfo->pixel_depth == 24 )
        {
			mFormat = LN_BITMAPFORMAT_A8B8G8R8;
			mBitmapData = LN_NEW Base::ReferenceBuffer();
			mBitmapData->reserve( mWidth * mHeight * 4 );

            lnU32* bitmap = (lnU32*)mBitmapData->getPointer();
            lnByte* row;
            lnByte* b;
            for ( int y = 0; y < mHeight; ++y )
            {
                row = row_pointers[unit + (sign * y)];
                for ( int x = 0; x < mWidth; ++x )
                {
                    b = &row[x * 3];
                    bitmap[x + mWidth * y] = 
                        0xff000000           |
                        ( *( b + 0 ) )       |
                        ( *( b + 1 ) << 8 )  |
                        ( *( b + 2 ) << 16 );
                }
            }
        }
        // Gray
        else if ( mPngInfo->color_type == PNG_COLOR_TYPE_GRAY && mPngInfo->pixel_depth == 8 )
        {
			mFormat = LN_BITMAPFORMAT_A8;
			mBitmapData = LN_NEW Base::ReferenceBuffer();
			mBitmapData->reserve( mWidth * mHeight * 1 );

			lnByte* bitmap = (lnByte*)mBitmapData->getPointer();

			for ( int h = 0; h < mHeight; ++h ) {
                memcpy( &bitmap[row_bytes * (unit + (sign * h))], row_pointers[h], row_bytes );
			}
        }
		else {
			return false;
		}

		//PNG_COLOR_TYPE_GRAY// (ビット深度 1, 2, 4, 8, 16)
		//PNG_COLOR_TYPE_GRAY_ALPHA// (ビット深度 8, 16)
		//PNG_COLOR_TYPE_PALETTE// (ビット深度 1, 2, 4, 8)
		//PNG_COLOR_TYPE_RGB// (ビット深度 8, 16)
		//PNG_COLOR_TYPE_RGB_ALPHA// (ビット深度 8, 16)

		//PNG_COLOR_MASK_PALETTE
		//PNG_COLOR_MASK_COLOR
		//PNG_COLOR_MASK_ALPHA

		//if(png_get_valid(pPng, pInfo, PNG_INFO_tRNS))
		//	png_set_expand(pPng);
		//if(ColorType == PNG_COLOR_TYPE_PALETTE)
		//	png_set_expand(pPng);
		//if(ColorType == PNG_COLOR_TYPE_GRAY && bpp < 8)
		//	png_set_expand(pPng);
		//if(bpp > 8)
		//	png_set_strip_16(pPng);
		//if(ColorType == PNG_COLOR_TYPE_GRAY)
		//	png_set_gray_to_rgb(pPng);
		return true;
	}

private:

	/// ストリーム抽象化用
    static void _readDataForPng( png_structp png_ptr, png_bytep data, png_size_t length )
    {
#if 1
		PngData* png_data = (PngData*)png_get_io_ptr( png_ptr );
		int validSize = png_data->Stream->read( data, length );
		if ( validSize != length ) {
			png_error( png_ptr, "_readPngData failed" );
		}
#else
        PngData* buffer = (PngData*)png_get_io_ptr( png_ptr_ );

        if ( buffer->Offset + length_ <= buffer->Length )
        {
            memcpy( data_, buffer->Data + buffer->Offset, length_ );
            buffer->Offset += length_;
        }
        else
        {
            printf("buffer->Offset:%d length_:%d buffer->Length:%d", buffer->Offset, length_, buffer->Length);
            png_error( png_ptr_, "_readPngData failed" );
        }
#endif
    }
};

//==============================================================================
// ■ Bitmap
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Bitmap::Bitmap()
		: mBitmapData		( NULL )
		, mWidth			( 0 )
		, mHeight			( 0 )
		, mPitch			( 0 )
		, mFormat			( LN_BITMAPFORMAT_UNKNOWN )
		, mUpFlow			( true )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Bitmap::~Bitmap()
    {
		_dispose();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Bitmap::create( FileIO::Stream* stream )
	{
		_dispose();
		PngLoader pngLoader;
		if ( pngLoader.load( stream, false ) ) {
			mWidth = pngLoader.mWidth;
			mHeight = pngLoader.mHeight;
			mFormat = pngLoader.mFormat;
			LN_REFOBJ_SET( mBitmapData, pngLoader.mBitmapData );
			return;
		}
		LN_THROW_InvalidFormat(0);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Bitmap::create( FileIO::Manager* fileManager, const lnChar* filePath )
	{
		_dispose();
		LRefPtr<FileIO::InFile> file(
			fileManager->createInFile(filePath, FileIO::FileType_Binary));
		create( file );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Bitmap::create( int width, int height, LNBitmapFormat format )
	{
		_dispose();
		mBitmapData = LN_NEW Base::ReferenceBuffer();
		mBitmapData->reserve( width * height * getPixelFormatByteCount( format ) );
		mWidth = width;
		mHeight = height;
		mFormat = format;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Bitmap::create(lnByte* data, size_t dataSize, int width, int height, LNBitmapFormat format, bool copy)
	{
		_dispose();
		mBitmapData = LN_NEW Base::ReferenceBuffer( data, dataSize, true );
		mWidth = width;
		mHeight = height;
		mFormat = format;

		if (copy){
			mBitmapData->reserve(data, dataSize);
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Bitmap::convertPixelFormat( LNBitmapFormat newFormat )
	{
		_convertPixelFormat( 
			mBitmapData->getPointer(), mBitmapData->getSize(), mFormat,
			mBitmapData->getPointer(), mBitmapData->getSize(), newFormat );
		mFormat = newFormat;
	}


	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Bitmap* Bitmap::copy() const
	{
		Bitmap* newBitmap = LN_NEW Bitmap();
		newBitmap->create(mWidth, mHeight, mFormat);
		newBitmap->mPitch = mPitch;
		newBitmap->mUpFlow = mUpFlow;
		newBitmap->mBitmapData->copy(mBitmapData->getPointer(), mBitmapData->getSize());
		return newBitmap;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	int Bitmap::getPixelFormatByteCount( LNBitmapFormat format )
	{
		switch ( format )
		{
			case LN_BITMAPFORMAT_A1:
				return 1;
			case LN_BITMAPFORMAT_A8:
				return 1;
			case LN_BITMAPFORMAT_A8R8G8B8:
				return 4;
			case LN_BITMAPFORMAT_A8B8G8R8:
				return 4;
			default:
				LN_THROW_Argument(0);
				break;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	int Bitmap::getPixelFormatByteCount( LNBitmapFormat format, int width, int height )
	{
		switch ( format )
		{
			case LN_BITMAPFORMAT_A1:
				return std::max(((width + 7) * (height + 7)) / 8, 1);//(((width / 8) + 1) * ((height / 8) + 1));
			case LN_BITMAPFORMAT_A8:
				return width * height;
			case LN_BITMAPFORMAT_A8R8G8B8:
				return width * height * 4;
			case LN_BITMAPFORMAT_A8B8G8R8:
				return width * height * 4;
			default:
				LN_THROW_Argument(0);
				break;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//inline lnU32 alphaBlendColor( lnU32 destColor, lnU32 srcColor,  )

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Bitmap::bitblt( Bitmap* dest, const LRect& destRect, const Bitmap* src, const LRect& srcRect, lnU32 mulColor, bool alphaBlend )
	{
		LRect dest_all( 0, 0, dest->mWidth, dest->mHeight );
        LRect targetRect( destRect.x, destRect.y, srcRect.w, srcRect.h );
        targetRect.clip( dest_all );

		DestBitmapWrapper destWrapper( dest, targetRect );
        SecBitmapWrapper srcWrapper( src, srcRect );


		//-----------------------------------------------------------
		// dest:A8R8G8B8 ← src:A8
		if ( dest->mFormat == LN_BITMAPFORMAT_A8R8G8B8 && src->mFormat == LN_BITMAPFORMAT_A8 )
        {
            if ( alphaBlend )
            {
                for ( int y = 0; y < targetRect.h; ++y )
                {
                    srcWrapper.setReadLineU8( y );
                    for ( int x = 0; x < targetRect.w; ++x )
                    {
                        lnU32 src_alpha  = srcWrapper.getPixelU8( x );
                        if ( src_alpha == 0 ) continue;     // フォントならコレでかなり高速化できるはず
                        src_alpha = ( src_alpha * ( ( mulColor >> 24 ) & 0xff ) ) >> 8;    // >> 8 は / 256。乗算で最大 65535 になるのを 0～ 255 に直している

                        lnU32 dest_color = destWrapper.getDestPixelU32( x, y );
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
                            ( ( ( ( mulColor   >> 16 ) & 0xff ) * a ) >> 8 );

                        g = ( ( ( ( dest_color >> 8  ) & 0xff ) * ( 0xff - a ) ) >> 8 ) +
                            ( ( ( ( mulColor   >> 8  ) & 0xff ) * a ) >> 8 );

                        b = ( ( ( ( dest_color       ) & 0xff ) * ( 0xff - a ) ) >> 8 ) +
                            ( ( ( ( mulColor         ) & 0xff ) * a ) >> 8 );

                        // 書き込み用に再計算。
                        // 乗算だと、半透明を重ねるごとに薄くなってしまう。
                        // イメージとしては、重ねるごとに濃くなる加算が適切だと思う。
                        a = ( dest_alpha + src_alpha );
                        a = ( a > 255 ) ? 255 : a;

                        destWrapper.setPixelU32( x, y, ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | ( b ) );
                    }
                }
            }
            else
            {
                for ( int y = 0; y < targetRect.h; ++y )
                {
                    srcWrapper.setReadLineU8( y );
                    for ( int x = 0; x < targetRect.w; ++x )
                    {
                        lnByte p = srcWrapper.getPixelU8( x );
                        destWrapper.setPixelU32( x, y,
                            ( ( ( ( ( mulColor >> 24 ) & 0xff ) * p ) >> 8 ) << 24 ) |
                            ( ( ( ( ( mulColor >> 16 ) & 0xff ) * p ) >> 8 ) << 16 ) |
                            ( ( ( ( ( mulColor >> 8  ) & 0xff ) * p ) >> 8 ) <<  8 ) |
                            ( ( ( ( ( mulColor       ) & 0xff ) * p ) >> 8 )       )
                        ); 
                    }
                }
            }
        }
		//-----------------------------------------------------------
		// dest:A8R8G8B8 ← src:A1
		else if ( dest->mFormat == LN_BITMAPFORMAT_A8R8G8B8 && src->mFormat == LN_BITMAPFORMAT_A1 )
        {
			if ( alphaBlend )
            {
                for ( int y = 0; y < targetRect.h; ++y )
                {
                    srcWrapper.setReadLineU1( y );
                    
                    for ( int x = 0; x < targetRect.w; ++x )
                    {
                        lnByte bit  = srcWrapper.getPixelU1( x );
                        if ( bit == 0 ) continue;
                        lnU32 src_alpha = ( ( mulColor >> 24 ) & 0xff );

                        lnU32 dest_color = destWrapper.getDestPixelU32( x, y );
                        lnU32 dest_alpha = ( dest_color >> 24 ) & 0xff;

                        lnU32 a, r, g, b;

                        if ( dest_alpha == 0 ) a = 0xff;
                        else a = src_alpha;    

                        r = ( ( ( ( dest_color >> 16 ) & 0xff ) * ( 0xff - a ) ) >> 8 ) +
                            ( ( ( ( mulColor  >> 16 ) & 0xff ) * a ) >> 8 );

                        g = ( ( ( ( dest_color >> 8  ) & 0xff ) * ( 0xff - a ) ) >> 8 ) +
                            ( ( ( ( mulColor  >> 8  ) & 0xff ) * a ) >> 8 );

                        b = ( ( ( ( dest_color       ) & 0xff ) * ( 0xff - a ) ) >> 8 ) +
                            ( ( ( ( mulColor        ) & 0xff ) * a ) >> 8 );

                        a = ( dest_alpha + src_alpha );
                        a = ( a > 255 ) ? 255 : a;

                        destWrapper.setPixelU32( x, y, ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | ( b ) );
                    }
                }
            }
            // アルファブレンドなし
            else
			{
				LN_THROW_NotImpl(0);
            }
		}
		//-----------------------------------------------------------
		// dest:A8R8G8B8 ← src:A8R8G8B8
		else if (dest->mFormat == LN_BITMAPFORMAT_A8R8G8B8 && src->mFormat == LN_BITMAPFORMAT_A8R8G8B8)
		{
			if (alphaBlend)
			{
				for (int y = 0; y < targetRect.h; ++y)
				{
					srcWrapper.setReadLineU32(y);
					for (int x = 0; x < targetRect.w; ++x)
					{
						lnU32 destColor = destWrapper.getDestPixelU32(x, y);
						lnU32 srcColor = srcWrapper.getPixelU32(x);
						lnU32 resultColor = blendColor(destColor, srcColor);


						// 乗算するべき色があれば
						if (mulColor != 0xFFFFFFFF)
							resultColor = multiplyColor(resultColor, mulColor);

						//printf("%x\n", resultColor);
						destWrapper.setPixelU32(x, y, resultColor);
					}
				}
			}
			else
			{
				for (int y = 0; y < targetRect.h; ++y)
				{
					srcWrapper.setReadLineU32(y);
					for (int x = 0; x < targetRect.w; ++x)
					{
						lnU32 resultColor = srcWrapper.getPixelU32(x);

						// 乗算するべき色があれば
						if (mulColor != 0xFFFFFFFF)
							resultColor = multiplyColor(resultColor, mulColor);

						destWrapper.setPixelU32(x, y, resultColor);
					}
				}
			}
		}
		else {
			LN_THROW_NotImpl(0);
		}

		return true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Bitmap::_dispose()
	{
		LN_SAFE_RELEASE( mBitmapData );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Bitmap::_convertPixelFormat( 
		const lnByte* input, size_t inputSize, LNBitmapFormat inputFormat,
		lnByte* output, size_t outputSize, LNBitmapFormat outputFormat )
	{
		// ABGR → ARGB
		if ( inputFormat == LN_BITMAPFORMAT_A8B8G8R8 && outputFormat == LN_BITMAPFORMAT_A8R8G8B8 )
		{
			const lnU32* in = (const lnU32*)input;
			lnU32* out = (lnU32*)output;
			size_t count = inputSize / sizeof(lnU32);
			for ( size_t i = 0; i < count; ++i ) {
				lnU32 c = in[i];
				out[i] = 
					c & 0xff000000 |
					( ( c & 0x000000ff ) << 16 ) |
					( ( c & 0x0000ff00 )       ) |
					( ( c & 0x00ff0000 ) >> 16 ) ;
			}
		}
		else {
			LN_THROW_InvalidFormat(0);
		}
	}


} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================