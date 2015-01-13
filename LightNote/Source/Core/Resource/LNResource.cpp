//==============================================================================
// Resource 
//==============================================================================

#include "stdafx.h"
#include "../FileIO/FileUtil.h"
#include "../FileIO/StreamObject.h"
#include "LNResource.h"

// 配列の要素数を返すマクロ
#define ARRAY_SIZE_OF( ary_ )     ( sizeof( ary_ ) / sizeof( ary_[ 0 ] ) )

namespace LNote
{
namespace Core
{
namespace Resource
{

//==============================================================================
// ■ StaticResourceManager
//==============================================================================

	static const lnChar* gStringResources[String::String_MAX] =
	{
		NULL,
		_T( "XAudio の初期化に失敗しました。\nDirectX がインストールされていない可能性があります。\n音声無しで続行します。" ),
	};

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	const lnChar* StaticResourceManager::getString( int resourceID )
	{
		LN_ERR2_ASSERT_S( 0 < resourceID && resourceID < String::String_MAX );
		return gStringResources[resourceID];
	}


/// シェーダリソース
namespace Shader
{
	/// Pane マスクなしトランジション
    const unsigned char BasicPane_Transition[] =
	{
		#include "Shader/BasicPane_Transition.lnfx.h"
	};
    const unsigned int BasicPane_Transition_Size = ARRAY_SIZE_OF( BasicPane_Transition );

	/// Pane マスク付きトランジション
    const unsigned char BasicPane_TransitionWithMask[] =
	{
		#include "Shader/BasicPane_TransitionWithMask.lnfx.h"
	};
    const unsigned int BasicPane_TransitionWithMask_Size = ARRAY_SIZE_OF( BasicPane_TransitionWithMask );

	/// Pane ブラ―
    const unsigned char BasicPane_BufferingBlar[] =
	{
		#include "Shader/BasicPane_BufferingBlar.lnfx.h"
	};
    const unsigned int BasicPane_BufferingBlar_Size = ARRAY_SIZE_OF( BasicPane_BufferingBlar );
    
	/// Pane PostDraw
    const unsigned char BasicPane_PostDraw[] =
	{
		#include "Shader/BasicPane_PostDraw.lnfx.h"
	};
    const unsigned int BasicPane_PostDraw_Size = ARRAY_SIZE_OF( BasicPane_PostDraw );
    

	/// 基本形状描画用
    const unsigned char GeometryRenderer[] =
	{
		#include "Shader/GeometryRenderer.lnfx.h"
	};
    const unsigned int  GeometryRenderer_Size = ARRAY_SIZE_OF( GeometryRenderer );

	/// スプライト描画用
    const unsigned char SpriteRenderer[] =
	{
		#include "Shader/SpriteRenderer.lnfx.h"
	};
    const unsigned int  SpriteRenderer_Size = ARRAY_SIZE_OF( SpriteRenderer );

	/// PointParticle
    const unsigned char PointParticle[] =
	{
		#include "Shader/PointParticle.lnfx.h"
	};
    const unsigned int PointParticle_Size = ARRAY_SIZE_OF( PointParticle );

	/// Tilemap
    const unsigned char Tilemap[] =
	{
		#include "Shader/Tilemap.lnfx.h"
	};
    const unsigned int Tilemap_Size = ARRAY_SIZE_OF( Tilemap );


	/// SceneShader 2D 基本
    extern const unsigned char SSBasic2D[] =
	{
		#include "Shader/SSBasic2D.lnfx.h"
	};
    extern const unsigned int SSBasic2D_Size = ARRAY_SIZE_OF( SSBasic2D );

	/// SceneShader 3D 基本
    extern const unsigned char SSBasic3D[] =
	{
		#include "Shader/SSBasic3D.lnfx.h"
	};
    extern const unsigned int SSBasic3D_Size = ARRAY_SIZE_OF( SSBasic3D );

	/// SSBasic3DSkinning
    extern const unsigned char SSBasic3DSkinning[] =
	{
		#include "Shader/SSBasic3DSkinning.lnfx.h"
	};
    extern const unsigned int SSBasic3DSkinning_Size = ARRAY_SIZE_OF( SSBasic3DSkinning );

	/// SceneShader ライティングなし
    const unsigned char SSNoLighting[] =
	{
		#include "Shader/SSNoLighting.lnfx.h"
	};
    const unsigned int  SSNoLighting_Size = ARRAY_SIZE_OF( SSNoLighting );
    
	/// MikuMikuMoving ヘッダ
	const unsigned char MMM_EffectHeader[] =
	{
		#include "Shader/MMM_EffectHeader.fxh.h"
	};
    const unsigned int  MMM_EffectHeader_Size = ARRAY_SIZE_OF( MMM_EffectHeader );

	/// ImageFilter_Tone
	const unsigned char ImageFilter_Tone[] =
	{
		#include "Shader/ImageFilter_Tone.fx.h"
	};
    const unsigned int  ImageFilter_Tone_Size = ARRAY_SIZE_OF( ImageFilter_Tone );

	/// ImageFilter_TransitionWithoutMask
	const unsigned char ImageFilter_TransitionWithoutMask[] =
	{
		#include "Shader/ImageFilter_TransitionWithoutMask.fx.h"
	};
    const unsigned int  ImageFilter_TransitionWithoutMask_Size = ARRAY_SIZE_OF( ImageFilter_TransitionWithoutMask );

	/// ImageFilter_TransitionWithMask
	const unsigned char ImageFilter_TransitionWithMask[] =
	{
		#include "Shader/ImageFilter_TransitionWithMask.fx.h"
	};
    const unsigned int  ImageFilter_TransitionWithMask_Size = ARRAY_SIZE_OF( ImageFilter_TransitionWithMask );

	/// ImageFilter_BufferingBlar
	const unsigned char ImageFilter_BufferingBlar[] =
	{
		#include "Shader/ImageFilter_BufferingBlar.fx.h"
	};
    const unsigned int  ImageFilter_BufferingBlar_Size = ARRAY_SIZE_OF( ImageFilter_BufferingBlar );
}

/// 画像リソース
namespace Image
{
	/// DebugFont 用フォントテクスチャ
    const unsigned char DebugFont[] =
	{
		#include "Image/DebugFont.png.h"
	};
    const unsigned int DebugFont_Size = ARRAY_SIZE_OF( DebugFont );

	/// GUI スキンテクスチャ
    const unsigned char GUISkin[] =
	{
		#include "Image/GUISkin.png.h"
	};
    const unsigned int GUISkin_Size = ARRAY_SIZE_OF( GUISkin );

	/// toon01.bmp
    const unsigned char toon01[] =
	{
		#include "Image/toon01.bmp.h"
	};
    const unsigned int toon01_Size = ARRAY_SIZE_OF( toon01 );

	/// toon02.bmp
    const unsigned char toon02[] =
	{
		#include "Image/toon02.bmp.h"
	};
    const unsigned int toon02_Size = ARRAY_SIZE_OF( toon02 );

	/// toon03.bmp
    const unsigned char toon03[] =
	{
		#include "Image/toon03.bmp.h"
	};
    const unsigned int toon03_Size = ARRAY_SIZE_OF( toon03 );

	/// toon04.bmp
    const unsigned char toon04[] =
	{
		#include "Image/toon04.bmp.h"
	};
    const unsigned int toon04_Size = ARRAY_SIZE_OF( toon04 );

	/// toon05.bmp
    const unsigned char toon05[] =
	{
		#include "Image/toon05.bmp.h"
	};
    const unsigned int toon05_Size = ARRAY_SIZE_OF( toon05 );

	/// toon06.bmp
    const unsigned char toon06[] =
	{
		#include "Image/toon06.bmp.h"
	};
    const unsigned int toon06_Size = ARRAY_SIZE_OF( toon06 );

	/// toon07.bmp
    const unsigned char toon07[] =
	{
		#include "Image/toon07.bmp.h"
	};
    const unsigned int toon07_Size = ARRAY_SIZE_OF( toon07 );

	/// toon08.bmp
    const unsigned char toon08[] =
	{
		#include "Image/toon08.bmp.h"
	};
    const unsigned int toon08_Size = ARRAY_SIZE_OF( toon08 );

	/// toon09.bmp
    const unsigned char toon09[] =
	{
		#include "Image/toon09.bmp.h"
	};
    const unsigned int toon09_Size = ARRAY_SIZE_OF( toon09 );

	/// toon10.bmp
    const unsigned char toon10[] =
	{
		#include "Image/toon10.bmp.h"
	};
    const unsigned int toon10_Size = ARRAY_SIZE_OF( toon10 );

	/// BitmapFont1_h7_6x15.png
	const unsigned char BitmapFont1_h7_6x15[] =
	{
		#include "Image/BitmapFont1_h7_6x15.png.h"
	};
	const unsigned int BitmapFont1_h7_6x15_Size = ARRAY_SIZE_OF(BitmapFont1_h7_6x15);
}

/// GUI 系リソース
namespace GUI
{
	/// 基本テーマ
    const unsigned char BasicTheme[] =
	{
		#include "GUI/BasicTheme.xml.h"
	};
    const unsigned int BasicTheme_Size = ARRAY_SIZE_OF( BasicTheme );

	/// Window イメージ
    const unsigned char Window[] =
	{
		#include "GUI/Window.png.h"
	};
    const unsigned int Window_Size = ARRAY_SIZE_OF( Window );
}


//==============================================================================
// ■ HardCodeResourceArchive
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
 //   HardCodeResourceArchive::HardCodeResourceArchive()
 //   {

 //   }

 //   //---------------------------------------------------------------------
	////
 //   //---------------------------------------------------------------------
 //   HardCodeResourceArchive::~HardCodeResourceArchive()
 //   {
 //   }

//	//----------------------------------------------------------------------
//	//
//	//----------------------------------------------------------------------
//#define SELECT_RESOURCE( key, buf, size ) \
//	if ( wcsicmp( filePath, L##key ) == 0 ) \
//	{ \
//		st = FileIO::File::createInStreamFromMemoryUnManaged( buf, size ); \
//		return st; \
//	}
//
//    FileIO::IInStream* HardCodeResourceArchive::createStream( const wchar_t* filePath, FileIO::FileOpenMode mode )
//    {
//		if ( checkResourceRootPath( filePath ) == false ) return NULL;
//
//        FileIO::IInStream* st = NULL;
//
//		//-----------------------------------------------------
//        // Shader
//		SELECT_RESOURCE( 
//			"LNResource/Shader/MMM_EffectHeader.h",
//			Shader::MMM_EffectHeader, Shader::MMM_EffectHeader_Size );
//
//		SELECT_RESOURCE( 
//			"LNResource/Shader/GeometryRenderer.lnfx",
//			Shader::GeometryRenderer, Shader::GeometryRenderer_Size );
//		SELECT_RESOURCE( 
//			"LNResource/Shader/SpriteRenderer.lnfx",
//			Shader::SpriteRenderer, Shader::SpriteRenderer_Size );
//		SELECT_RESOURCE( 
//			"LNResource/Shader/SSBasic2D.lnfx",
//			Shader::SSBasic2D, Shader::SSBasic2D_Size );
//        SELECT_RESOURCE( 
//			"LNResource/Shader/SSBasic3D.lnfx",
//			Shader::SSBasic3D, Shader::SSBasic3D_Size );
//		SELECT_RESOURCE( 
//			"LNResource/Shader/SSBasic3DSkinning.lnfx",
//			Shader::SSBasic3DSkinning, Shader::SSBasic3DSkinning_Size );
//		SELECT_RESOURCE( 
//			"LNResource/Shader/SSNoLighting.lnfx",
//			Shader::SSNoLighting, Shader::SSNoLighting_Size );
//        SELECT_RESOURCE( 
//			"LNResource/Shader/BasicPane_Transition.lnfx",
//			Shader::BasicPane_Transition, Shader::BasicPane_Transition_Size );
//		SELECT_RESOURCE( 
//			"LNResource/Shader/BasicPane_TransitionWithMask.lnfx" ,
//			Shader::BasicPane_TransitionWithMask, Shader::BasicPane_TransitionWithMask_Size );
//		SELECT_RESOURCE( 
//			"LNResource/Shader/BasicPane_BufferingBlar.lnfx",
//			Shader::BasicPane_BufferingBlar, Shader::BasicPane_BufferingBlar_Size );
//		SELECT_RESOURCE( 
//			"LNResource/Shader/BasicPane_PostDraw.lnfx",
//			Shader::BasicPane_PostDraw, Shader::BasicPane_PostDraw_Size );
//		SELECT_RESOURCE( 
//			"LNResource/Shader/PointParticle.lnfx",
//			Shader::PointParticle, Shader::PointParticle_Size );
//		SELECT_RESOURCE( 
//			"LNResource/Shader/Tilemap.lnfx",
//			Shader::Tilemap, Shader::Tilemap_Size );
//		
//
//        //-----------------------------------------------------
//        // Image
//        SELECT_RESOURCE(
//			"LNResource/Image/DebugFont.png",
//			Image::DebugFont, Image::DebugFont_Size );
//        SELECT_RESOURCE(
//			"LNResource/Image/GUISkin.png",
//			Image::GUISkin, Image::GUISkin_Size );
//		SELECT_RESOURCE(
//			"LNResource/Image/toon01.bmp",
//			Image::toon01, Image::toon01_Size );
//		SELECT_RESOURCE(
//			"LNResource/Image/toon02.bmp",
//			Image::toon02, Image::toon02_Size );
//		SELECT_RESOURCE(
//			"LNResource/Image/toon03.bmp",
//			Image::toon03, Image::toon03_Size );
//		SELECT_RESOURCE(
//			"LNResource/Image/toon04.bmp",
//			Image::toon04, Image::toon04_Size );
//		SELECT_RESOURCE(
//			"LNResource/Image/toon05.bmp",
//			Image::toon05, Image::toon05_Size );
//		SELECT_RESOURCE(
//			"LNResource/Image/toon06.bmp",
//			Image::toon06, Image::toon06_Size );
//		SELECT_RESOURCE(
//			"LNResource/Image/toon07.bmp",
//			Image::toon07, Image::toon07_Size );
//		SELECT_RESOURCE(
//			"LNResource/Image/toon08.bmp",
//			Image::toon08, Image::toon08_Size );
//		SELECT_RESOURCE(
//			"LNResource/Image/toon09.bmp",
//			Image::toon09, Image::toon09_Size );
//		SELECT_RESOURCE(
//			"LNResource/Image/toon10.bmp",
//			Image::toon10, Image::toon10_Size );
//
//		//-----------------------------------------------------
//        // GUI
//        SELECT_RESOURCE(
//			"LNResource/GUI/BasicTheme.xml",
//			GUI::BasicTheme, GUI::BasicTheme_Size );
//        SELECT_RESOURCE(
//			"LNResource/GUI/Window.png",
//			GUI::Window, GUI::Window_Size );
//
//        return NULL;
//    }
//#undef SELECT_RESOURCE
//	//----------------------------------------------------------------------
//	//
//	//----------------------------------------------------------------------
//	bool HardCodeResourceArchive::checkResourceRootPath( const wchar_t* path )
//	{
//		if ( wcslen( path ) < LN_RESOURCE_ROOT_PATH_LENGTH ) return false;
//		return ( wcsncmp( path, LN_RESOURCE_ROOT_PATH_W, LN_RESOURCE_ROOT_PATH_LENGTH ) == 0 );
//	}

//==============================================================================
// StaticResource
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void StaticResource::initialize()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void StaticResource::finalize()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void StaticResource::loadData( int dataResourceID, const void** data, size_t* byteCount )
	{
		*data = NULL;
		*byteCount = 0;

#define SELECT_RESOURCE( resID, buf, size ) \
		if ( dataResourceID == resID ) \
		{ \
			*data = buf; \
			*byteCount = size; \
			return; \
		}

		//-----------------------------------------------------
        // Shader
		SELECT_RESOURCE( 
			ID_Shader_MMM_EffectHeader,
			Shader::MMM_EffectHeader, Shader::MMM_EffectHeader_Size );

		SELECT_RESOURCE( 
			ID_Shader_GeometryRenderer,
			Shader::GeometryRenderer, Shader::GeometryRenderer_Size );
		SELECT_RESOURCE( 
			ID_Shader_SpriteRenderer,
			Shader::SpriteRenderer, Shader::SpriteRenderer_Size );
		SELECT_RESOURCE( 
			ID_Shader_SSBasic2D,
			Shader::SSBasic2D, Shader::SSBasic2D_Size );
        SELECT_RESOURCE( 
			ID_Shader_SSBasic3D,
			Shader::SSBasic3D, Shader::SSBasic3D_Size );
		SELECT_RESOURCE( 
			ID_Shader_SSBasic3DSkinning,
			Shader::SSBasic3DSkinning, Shader::SSBasic3DSkinning_Size );
		SELECT_RESOURCE( 
			ID_Shader_SSNoLighting,
			Shader::SSNoLighting, Shader::SSNoLighting_Size );
		SELECT_RESOURCE( 
			ID_Shader_ImageFilter_Tone,
			Shader::ImageFilter_Tone, Shader::ImageFilter_Tone_Size );
		SELECT_RESOURCE( 
			ID_Shader_ImageFilter_TransitionWithoutMask,
			Shader::ImageFilter_TransitionWithoutMask, Shader::ImageFilter_TransitionWithoutMask_Size );
		SELECT_RESOURCE( 
			ID_Shader_ImageFilter_TransitionWithMask,
			Shader::ImageFilter_TransitionWithMask, Shader::ImageFilter_TransitionWithMask_Size );
		SELECT_RESOURCE( 
			ID_Shader_ImageFilter_BufferingBlar,
			Shader::ImageFilter_BufferingBlar, Shader::ImageFilter_BufferingBlar_Size );

        SELECT_RESOURCE( 
			ID_Shader_BasicPane_Transition,
			Shader::BasicPane_Transition, Shader::BasicPane_Transition_Size );
		SELECT_RESOURCE( 
			ID_Shader_BasicPane_TransitionWithMask,
			Shader::BasicPane_TransitionWithMask, Shader::BasicPane_TransitionWithMask_Size );
		SELECT_RESOURCE( 
			ID_Shader_BasicPane_BufferingBlar,
			Shader::BasicPane_BufferingBlar, Shader::BasicPane_BufferingBlar_Size );
		SELECT_RESOURCE( 
			ID_Shader_BasicPane_PostDraw,
			Shader::BasicPane_PostDraw, Shader::BasicPane_PostDraw_Size );
		SELECT_RESOURCE( 
			ID_Shader_PointParticle,
			Shader::PointParticle, Shader::PointParticle_Size );
		SELECT_RESOURCE( 
			ID_Shader_Tilemap,
			Shader::Tilemap, Shader::Tilemap_Size );
		

        //-----------------------------------------------------
        // Image
        SELECT_RESOURCE(
			ID_Image_DebugFont,
			Image::DebugFont, Image::DebugFont_Size );
        SELECT_RESOURCE(
			ID_Image_GUISkin,
			Image::GUISkin, Image::GUISkin_Size );
		SELECT_RESOURCE(
			ID_Image_toon01,
			Image::toon01, Image::toon01_Size );
		SELECT_RESOURCE(
			ID_Image_toon02,
			Image::toon02, Image::toon02_Size );
		SELECT_RESOURCE(
			ID_Image_toon03,
			Image::toon03, Image::toon03_Size );
		SELECT_RESOURCE(
			ID_Image_toon04,
			Image::toon04, Image::toon04_Size );
		SELECT_RESOURCE(
			ID_Image_toon05,
			Image::toon05, Image::toon05_Size );
		SELECT_RESOURCE(
			ID_Image_toon06,
			Image::toon06, Image::toon06_Size );
		SELECT_RESOURCE(
			ID_Image_toon07,
			Image::toon07, Image::toon07_Size );
		SELECT_RESOURCE(
			ID_Image_toon08,
			Image::toon08, Image::toon08_Size );
		SELECT_RESOURCE(
			ID_Image_toon09,
			Image::toon09, Image::toon09_Size );
		SELECT_RESOURCE(
			ID_Image_toon10,
			Image::toon10, Image::toon10_Size );
		SELECT_RESOURCE(
			ID_Image_BitmapFont1_h7_6x15,
			Image::BitmapFont1_h7_6x15, Image::BitmapFont1_h7_6x15_Size);

		//-----------------------------------------------------
        // GUI
        SELECT_RESOURCE(
			ID_GUI_BasicTheme,
			GUI::BasicTheme, GUI::BasicTheme_Size );
        SELECT_RESOURCE(
			ID_GUI_Window,
			GUI::Window, GUI::Window_Size );

#undef SELECT_RESOURCE
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	FileIO::Stream* StaticResource::createStream( int dataResourceID )
	{
		const void* buf;
		size_t size;
		loadData( dataResourceID, &buf, &size );
		return FileIO::FileUtils::createInStreamFromMemoryUnManaged(buf, size);
	}

} // namespace Resource
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================