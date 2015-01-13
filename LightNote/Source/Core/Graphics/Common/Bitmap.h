//==============================================================================
// Bitmap 
//------------------------------------------------------------------------------
///**
//  @file       Bitmap.h
//  @brief      Bitmap
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "Common.h"
#include "../../Base/ReferenceBuffer.h"
#include "../../Geometry/DrawingShapes.h"
#include "../../FileIO/StreamObject.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ Bitmap
//------------------------------------------------------------------------------
///**
//  @brief	
//*/
////==============================================================================
class Bitmap
	: public Base::ReferenceObject
{
public:
	Bitmap();
	virtual ~Bitmap();
	
public:

	/// ビットマップ作成
	void create( FileIO::Stream* stream );

	/// ファイルから読み込む
	void create( FileIO::Manager* fileManager, const lnChar* filePath );

	/// 空のビットマップ作成
	void create( int width, int height, LNBitmapFormat format );

	/// メモリ上のデータから作成 (メモリコピーは行わないため、データは使用終了まで解放しないこと)
	void create( lnByte* data, size_t dataSize, int width, int height, LNBitmapFormat format, bool copy );

	/// ビットマップデータ本体の取得
	Base::ReferenceBuffer* getBitmapData() const { return mBitmapData; }
	
	/// 幅(px)の取得
	int getWidth() const { return mWidth; }
	
	/// 高さ(px)の取得
	int getHeight() const { return mHeight; }

	/// ピクセルフォーマットの取得
	LNBitmapFormat getPixelFormat() const { return mFormat; }

	/// ピクセルフォーマットの変換
	void convertPixelFormat( LNBitmapFormat newFormat );

	/// ビットマップのコピーを作成する
	Bitmap* copy() const;

public:

	/// フォーマット毎のバイト数を求める (A1 の場合は 1 を返す)
	static int getPixelFormatByteCount( LNBitmapFormat format );

	/// フォーマット毎のバイト数を求める
	static int getPixelFormatByteCount( LNBitmapFormat format, int width, int height );

	/// ビットマップ転送
	static bool bitblt(Bitmap* dest, const LRect& destRect, const Bitmap* src, const LRect& srcRect, lnU32 mulColor, bool alphaBlend);

private:
	void _dispose();

private:
	static void _convertPixelFormat( 
		const lnByte* input, size_t inputSize, LNBitmapFormat inputFormat,
		lnByte* output, size_t outputSize, LNBitmapFormat outputFormat );	

	static inline lnU32 blendColor(lnU32 destColor, lnU32 srcColor)
	{
		lnU32 destAlpha = (destColor >> 24) & 0xff;
		lnU32 srcAlpha = (srcColor >> 24) & 0xff;
		lnU32 a, r, g, b;

		// photoshop 等のツール系の計算式ではやや時間がかかるため、
		// DirectX 同様、dest のアルファは無視する方向で実装する。
		// ただし、このままだと dest(0, 0, 0, 0) に半透明の色を合成する場合、
		// 黒ずみが発生してしまう。テクスチャのデフォルトはこの状態。
		// dest(1, 0, 0, 0) とかなら、ユーザーが黒と合成されることを意図していると考えられるが、
		// 流石に完全に透明なのに黒ずむのはどうかと…。
		// というわけで、dest_alpha == 0 なら src が 100% になるように細工している。
		if (destAlpha == 0)
			a = 0xff;
		else
		{
			a = destAlpha * srcAlpha;
			a >>= 8;
		}

		r = ((((destColor >> 16) & 0xff) * (0xff - a)) >> 8) +
			((((srcColor >> 16) & 0xff) * a) >> 8);

		g = ((((destColor >> 8) & 0xff) * (0xff - a)) >> 8) +
			((((srcColor >> 8) & 0xff) * a) >> 8);

		b = ((((destColor)& 0xff) * (0xff - a)) >> 8) +
			((((srcColor)& 0xff) * a) >> 8);

		// 書き込み用に再計算。
		// 乗算だと、半透明を重ねるごとに薄くなってしまう。
		// イメージとしては、重ねるごとに濃くなる加算が適切だと思う。
		a = (destAlpha + srcAlpha);
		a = (a > 255) ? 255 : a;

		return (a << 24) | (r << 16) | (g << 8) | (b);
	}

	static inline lnU32 getAlpha(lnU32 color)	{ return (color >> 24) & 0xFF; }
	static inline lnU32 getRed(lnU32 color)		{ return (color >> 16) & 0xFF; }
	static inline lnU32 getGreen(lnU32 color)	{ return (color >> 8) & 0xFF; }
	static inline lnU32 getBlue(lnU32 color)	{ return (color) & 0xFF; }
	static inline lnU32 makeColorRGBA(lnU32 a, lnU32 r, lnU32 g, lnU32 b) { return (((lnU8)a) << 24) | (((lnU8)r) << 16) | (((lnU8)g) << 8) | ((lnU8)b); }

	static inline lnU32 multiplyColor(lnU32 destColor, lnU32 srcColor)
	{
		return makeColorRGBA(
			(getAlpha(destColor) * getAlpha(srcColor)) >> 8,
			(getRed(destColor)   * getRed(srcColor)) >> 8,
			(getGreen(destColor) * getGreen(srcColor)) >> 8,
			(getBlue(destColor)  * getBlue(srcColor)) >> 8);
	}

public:
    Base::ReferenceBuffer*	mBitmapData;
	int						mWidth;
    int						mHeight;
	int						mPitch;		///< フォーマット A1 時の、row バイト数。(FreeTypeからだと、必ず width / 8 + 1 にならないので)
	LNBitmapFormat			mFormat;
	bool					mUpFlow;	///< 上下逆のイメージの場合は false になる

private:

	/// Dest Utils
	class DestBitmapWrapper
    {
	public:

		DestBitmapWrapper( Bitmap* bitmap, const LRect& rect )
			: mBitmap			( bitmap )
			, mBitmapData		( bitmap->mBitmapData->getPointer() )
            , mTargetRect       ( rect )
        {}
		
	public:

		inline void     setPixelU32( int x, int y, lnU32 p ) 
		{ 
			(((lnU32*)mBitmapData)[mBitmap->mWidth * (mTargetRect.y + y) + mTargetRect.x + x]) = p; 
		}

        inline lnU32    getDestPixelU32( int x, int y ) 
		{ 
			return (((lnU32*)mBitmapData)[mBitmap->mWidth * (mTargetRect.y + y) + mTargetRect.x + x]);
		}

	public:
		Bitmap*			mBitmap;
		lnByte*			mBitmapData;
		const LRect&	mTargetRect;
	};
	
	/// Src Utils
	class SecBitmapWrapper
    {
	public:

        SecBitmapWrapper( const Bitmap* bitmap, const LRect& rect )
            : mBitmap			( bitmap )
			, mBitmapData		( bitmap->mBitmapData->getPointer() )
            , mCurrentReadLine	( NULL )
            , mTargetRect       ( rect )
            , mReadBottomLine	( mTargetRect.y + mTargetRect.h - 1 )
			, mUpFlow			( bitmap->mUpFlow )
        {}
		
	public:

		inline void     setReadLineU1( int y )
        {
            if ( mUpFlow )
                mCurrentReadLine = &mBitmapData[mBitmap->mPitch * (mTargetRect.y + y)];
            else
                mCurrentReadLine = &mBitmapData[mBitmap->mPitch * (mReadBottomLine -  y)];
        }

		inline lnByte   getPixelU1( int x ) const
        {
            int byte = (mTargetRect.x + x) >> 3;//(mTargetRect.x + x) / 8;
            int bit  = (mTargetRect.x + x) & 7;//(mTargetRect.x + x) % 8;
            return mCurrentReadLine[byte] & ( 0x80 >> bit );
        }

		inline void     setReadLineU8( int y )
        {
            if ( mUpFlow )
                mCurrentReadLine = &mBitmapData[mBitmap->mWidth * (mTargetRect.y + y)];
            else
                mCurrentReadLine = &mBitmapData[mBitmap->mWidth * (mReadBottomLine - y)];
        }

		inline lnByte   getPixelU8( int x ) const 
		{ 
			return mCurrentReadLine[mTargetRect.x + x]; 
		}

		inline void     setReadLineU32(int y)
		{
			if (mUpFlow)
				mCurrentReadLine = (const lnByte*)(&((const lnU32*)mBitmapData)[mBitmap->mWidth * (mTargetRect.y + y)]);
			else
				mCurrentReadLine = (const lnByte*)(&((const lnU32*)mBitmapData)[mBitmap->mWidth * (mReadBottomLine - y)]);
		}

		inline lnU32   getPixelU32(int x) const
		{
			return ((const lnU32*)mCurrentReadLine)[mTargetRect.x + x];
		}

	public:
		const Bitmap*	mBitmap;
		const LRect&	mTargetRect;
		const lnByte*	mBitmapData;
        const lnByte*	mCurrentReadLine;
        lnU32			mReadBottomLine;
		bool			mUpFlow;
	};

#if 0
	struct BitmapWrapper
    {
    public:
		/// Dest
        BitmapWrapper( Bitmap* bitmap, const LRect& rect )
            : mDestBitmap       ( bitmap )
            , mSrcBitmap        ( NULL )
            , mReadLine         ( NULL )
            , mTargetRect       ( rect )
            , mReadBottomLine   ( 0 )
            , mUpFlow           ( false )
        {}

		/// Src
        BitmapWrapper( const Bitmap* bitmap, const LRect& rect/*, bool up_flow*/ )
            : mDestBitmap       ( NULL )
            , mSrcBitmap        ( bitmap )
            , mReadLine         ( NULL )
            , mTargetRect       ( rect )
            , mReadBottomLine   ( mTargetRect.y + mTargetRect.h - 1 )
            , mUpFlow           ( false/*up_flow*/ )
        {}

        //inline void     setReadLineU1( int y )
        //{
        //    if ( mUpFlow )
        //        mReadLine = &mSrcBitmap->Buffer[mSrcBitmap->Pitch * (mTargetRect.y + y)];
        //    else
        //        mReadLine = &mSrcBitmap->Buffer[mSrcBitmap->Pitch * (mReadBottomLine -  y)];
        //}
        inline void     setReadLineU8( int y )
        {
            if ( mUpFlow )
                mReadLine = &mSrcBitmap->Buffer[mSrcBitmap->Width * (mTargetRect.y + y)];
            else
                mReadLine = &mSrcBitmap->Buffer[mSrcBitmap->Width * (mReadBottomLine -  y)];
        }
        inline void     setReadLineU32( int y ) { mReadLine = (lnByte*)&(((lnU32*)mSrcBitmap->Buffer)[mSrcBitmap->Height * (mTargetRect.y + y)]); }
        
        //inline lnByte   getPixelU1( int x ) const
        //{
        //    int byte = (mTargetRect.x + x) >> 3;//(mTargetRect.x + x) / 8;
        //    int bit  = (mTargetRect.x + x) & 7;//(mTargetRect.x + x) % 8;
        //    return mReadLine[byte] & ( 0x80 >> bit );
        //}
        inline lnByte   getPixelU8( int x ) const { return mReadLine[mTargetRect.x + x]; }
        inline lnU32    getPixelU32( int x ) const { return ((lnU32*)mReadLine)[mTargetRect.x + x]; }
        
        inline void     setPixelU32( int x, int y, lnU32 p ) { (((lnU32*)mDestBitmap->Buffer)[mDestBitmap->Width * (mTargetRect.y + y) + mTargetRect.x + x]) = p; }
        inline lnU32    getDestPixelU32( int x, int y ) { return (((lnU32*)mDestBitmap->Buffer)[mDestBitmap->Width * (mTargetRect.y + y) + mTargetRect.x + x]); }

    private:

        Bitmap*			mDestBitmap;
        const Bitmap*	mSrcBitmap;
        const lnByte*	mReadLine;
        LRect			mTargetRect;
        lnU32			mReadBottomLine;
        bool			mUpFlow;
    };

	friend class BitmapWrapper;
#endif
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================