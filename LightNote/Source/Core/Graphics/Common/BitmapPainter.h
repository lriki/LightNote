//==============================================================================
// BitmapPainter 
//------------------------------------------------------------------------------
///**
//  @file       BitmapPainter.h
//  @brief      BitmapPainter
//*/
//==============================================================================

#pragma once

#include "../../Base/NonCopyable.h"
#include "Bitmap.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// BitmapPainter
//------------------------------------------------------------------------------
///**
//  @brief		Scoped
//*/
////==============================================================================
class BitmapPainter
	: public Base::NonCopyable
{
public:
	BitmapPainter(Texture* texture);
	~BitmapPainter();
	
public:

	/// ‹éŒ`“h‚è‚Â‚Ô‚µ
	void drawRectangle(const LRect& rect, const LColor& color);

public:
	Bitmap*		mBitmap;
	Texture*	mTexture;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
