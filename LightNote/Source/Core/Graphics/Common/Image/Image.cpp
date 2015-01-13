﻿//=============================================================================
//【 Image 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "PngImage.h"
#include "Image.h"

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
// ■ IImage
//=============================================================================

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	//IImage* IImage::createImage( void* data_, lnU32 size_, bool swap_height_ )
	//{
	//    PngImage* png_img = LN_NEW PngImage();
	//    if ( png_img->initialize( data_, size_, swap_height_ ) )
	//    {
	//        return png_img;
	//    }

	//    LN_THROW_FileFormat( 0 );
	//    return NULL;
	//}

//=============================================================================
// ■ Image
//=============================================================================

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    bool Image::createFromBytes( void* data_, lnU32 size_, bool swap_height_ )
    {
        return createPngFromBytes( data_, size_, swap_height_ );
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