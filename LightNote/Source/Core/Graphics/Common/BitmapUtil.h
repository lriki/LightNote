//=============================================================================
//【 BitmapUtil 】
//-----------------------------------------------------------------------------
///**
//  @file       BitmapUtil.h
//  @brief      BitmapUtil
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Types.h"
#include "../../Geometry/DrawingShapes.h"
#include "../../Geometry/Geometry.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

#if 0
//=============================================================================
// ■ Bitmap
//-----------------------------------------------------------------------------
///**
//  @brief		
//  @note
//*/
//=============================================================================
class Bitmap
{
public:

    lnByte*         Buffer;
    int             Width;
    int             Height;
    int             Pitch;
    LNBitmapFormat  Format;
};
#endif

//=============================================================================
// ■ BitmapUtil
//-----------------------------------------------------------------------------
///**
//  @brief		
//  @note
//              dest 側は必ず FORMAT_A8R8G8B8 であること。
//*/
//=============================================================================
class BitmapUtil
{
public:

    struct BitmapData
    {
        lnByte*         Buffer;
        int             Width;
        int             Height;     ///< 負の値の場合は上下反転
        int             Pitch;
        LNBitmapFormat  Format;

    public:

        void            setTargetRect( const LRect& rect ) { mTargetRect = rect; }
        inline lnByte   getPixelU8( int x, int y ) { return Buffer[Height * (mTargetRect.y + y) + mTargetRect.x + x]; }
        inline lnU32    getPixelU32( int x, int y ) { return ((lnU32*)Buffer)[Height * (mTargetRect.y + y) + mTargetRect.x + x]; }
        inline void     setPixelU32( int x, int y, lnU32 p ) { (((lnU32*)Buffer)[Height * (mTargetRect.y + y) + mTargetRect.x + x]) = p; }

    private:

        LRect   mTargetRect;
    };

public:

	/// (mul_color のフォーマットは FORMAT_A8R8G8B8。エンディアン関係なし)
    static void bitblt(
        BitmapData* dest_bitmap,
        const LRect& dest_rect,
        const BitmapData* src_bitmap,
        const LRect& src_rect,
        lnU32 mul_color,
        bool alpha_blend );
   
private:

    struct BitmapWrapper
    {
    public:
        BitmapWrapper( BitmapData* bitmap, const LRect& rect )
            : mDestBitmap       ( bitmap )
            , mSrcBitmap        ( NULL )
            , mReadLine         ( NULL )
            , mTargetRect       ( rect )
            , mReadBottomLine   ( 0 )
            , mUpFlow           ( false )
        {}
        BitmapWrapper( const BitmapData* bitmap, const LRect& rect, bool up_flow )
            : mDestBitmap       ( NULL )
            , mSrcBitmap        ( bitmap )
            , mReadLine         ( NULL )
            , mTargetRect       ( rect )
            , mReadBottomLine   ( mTargetRect.y + mTargetRect.h - 1 )
            , mUpFlow           ( up_flow )
        {}

        inline void     setReadLineU1( int y )
        {
            if ( mUpFlow )
                mReadLine = &mSrcBitmap->Buffer[mSrcBitmap->Pitch * (mTargetRect.y + y)];
            else
                mReadLine = &mSrcBitmap->Buffer[mSrcBitmap->Pitch * (mReadBottomLine -  y)];
        }
        inline void     setReadLineU8( int y )
        {
            if ( mUpFlow )
                mReadLine = &mSrcBitmap->Buffer[mSrcBitmap->Width * (mTargetRect.y + y)];
            else
                mReadLine = &mSrcBitmap->Buffer[mSrcBitmap->Width * (mReadBottomLine -  y)];
        }
        inline void     setReadLineU32( int y ) { mReadLine = (lnByte*)&(((lnU32*)mSrcBitmap->Buffer)[mSrcBitmap->Height * (mTargetRect.y + y)]); }
        
        inline lnByte   getPixelU1( int x ) const
        {
            int byte = (mTargetRect.x + x) >> 3;//(mTargetRect.x + x) / 8;
            int bit  = (mTargetRect.x + x) & 7;//(mTargetRect.x + x) % 8;
            return mReadLine[byte] & ( 0x80 >> bit );
        }
        inline lnByte   getPixelU8( int x ) const { return mReadLine[mTargetRect.x + x]; }
        inline lnU32    getPixelU32( int x ) const { return ((lnU32*)mReadLine)[mTargetRect.x + x]; }
        
        inline void     setPixelU32( int x, int y, lnU32 p ) { (((lnU32*)mDestBitmap->Buffer)[mDestBitmap->Width * (mTargetRect.y + y) + mTargetRect.x + x]) = p; }
        inline lnU32    getDestPixelU32( int x, int y ) { return (((lnU32*)mDestBitmap->Buffer)[mDestBitmap->Width * (mTargetRect.y + y) + mTargetRect.x + x]); }

    private:

        BitmapData*         mDestBitmap;
        const BitmapData*   mSrcBitmap;
        const lnByte*       mReadLine;
        LRect               mTargetRect;
        lnU32               mReadBottomLine;
        bool                mUpFlow;
    };
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