//==============================================================================
// BitmapPainter 
//==============================================================================

#include "stdafx.h"
#include "../Interface.h"
#include "GraphicsUtil.h"
#include "BitmapPainter.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// BitmapPainter
////==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	BitmapPainter::BitmapPainter(Texture* texture)
	{
		mTexture = texture;
		if (mTexture->getSurfaceFormat() != SurfaceFormat_A8R8G8B8 &&
			mTexture->getSurfaceFormat() != SurfaceFormat_X8R8G8B8)
		{
			// 許可するのは以上のフォーマット
			LN_ERR2_ASSERT_S(0);
		}

		const LVector2& size = mTexture->getSize();
		lnByte* data = (lnByte*)mTexture->lock();
		size_t dataSize = Util::getSurfaceFormatPixelByteCount(mTexture->getSurfaceFormat()) * (int)size.X * (int)size.Y;

		mBitmap = LN_NEW Bitmap();
		mBitmap->create(
			data, dataSize,
			(int)size.X, (int)size.Y,
			Util::convertSurfaceFormatToBitmapFormat(mTexture->getSurfaceFormat()),
			false);

	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	BitmapPainter::~BitmapPainter()
	{
		if (mTexture)
		{
			mTexture->unlock();
			SAFE_DELETE(mBitmap);
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void BitmapPainter::drawRectangle(const LRect& rect, const LColor& color)
	{
		lnU32* data = (lnU32*)mBitmap->getBitmapData()->getPointer();

		LRect destRect = rect;
		destRect.clip(LRect(0, 0, mBitmap->getWidth(), mBitmap->getHeight()));

		lnU32 c = color.toU32ARGB();
		for (int y = 0; y < destRect.h; y++)
		{
			for (int x = 0; x < destRect.w; x++)
			{
				data[(destRect.y + y) * mBitmap->getWidth() + (destRect.x + x)] = c;
			}
		}
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote
