//==============================================================================
// RenderState 
//------------------------------------------------------------------------------
///**
//  @file       RenderState.h
//  @brief      RenderState
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Types.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ LRenderState クラス
//------------------------------------------------------------------------------
///**
//  @brief      レンダリングステート
//*/
//==============================================================================
class LRenderState
{
public:

	BlendMode			Blend;
	CullingMode			Culling;
    FillMode			Fill;			///< (Opengl ES では無効)
	bool        	    AlphaTest;      ///< (Opengl ES では無効)
	bool        	    DepthTest;
	bool        	    DepthWrite;
    bool                PointSprite;

	//Geometry::Rect             ScissorRect;
	//bool	            ScissorTest;

public:

    static const LRenderState BUFFER_COPY_STATE;    ///< 単純に画面全体を別の RT にコピーする目的で使うステート
	static const LRenderState BUFFER_COPY_STATE_WITH_ALPHA;
    static const LRenderState SMOOTH_ALPHA;         ///< 通常のアルファブレンド。bool フラグはすべて false

public:

	/// デフォルト値
	LRenderState()
		: Blend(BlendMode_Alpha)
		, Culling(CullingMode_Front)
		, Fill(FillMode_Solid)
		, AlphaTest(true)
		, DepthTest(true)
		, DepthWrite(true)
		, PointSprite(false)
	{}

	/// 値を指定して作成
	LRenderState(BlendMode blend_mode_, CullingMode culling_, FillMode fill_mode_, bool alpha_test_, bool depth_test_, bool depth_write_, bool point_sprite_)
		: Blend(blend_mode_)
		, Culling(culling_)
		, Fill(fill_mode_)
		, AlphaTest(alpha_test_)
		, DepthTest(depth_test_)
		, DepthWrite(depth_write_)
	{}

    lnU32 getHashCode() const
	{
		return
			((int)Blend) * 1000000 + 
			((int)Culling) * 100000 + 
			((int)Fill) * 10000 + 
			((int)AlphaTest) * 1000 + 
			((int)DepthTest) * 100 + 
			((int)DepthWrite) * 10 + 
			((int)PointSprite);
	}

	bool operator == ( const LRenderState& state ) const
	{
		return
			Blend == state.Blend &&
			Culling == state.Culling &&
			Fill == state.Fill &&
			AlphaTest == state.AlphaTest &&
			DepthTest == state.DepthTest &&
			DepthWrite == state.DepthWrite &&
			PointSprite == state.PointSprite;
	}

};


//==============================================================================
// ■ LSamplerState クラス
//------------------------------------------------------------------------------
///**
//  @brief      サンプラステート
//*/
//==============================================================================
class LSamplerState
{
public:

    TextureFilter     MagFilter;  ///< LN_TEXFILTER_POINT, LN_TEXFILTER_LINEAR のいずれか
    TextureFilter     MinFilter;  ///< LN_TEXFILTER_POINT, LN_TEXFILTER_LINEAR のいずれか
    TextureFilter     MipFilter;  ///< LN_TEXFILTER_NONE, LN_TEXFILTER_LINEAR のいずれか
    TextureAddress    AddressU;
    TextureAddress    AddressV;

public:

	/// デフォルト値
    LSamplerState()
		: MagFilter(TextureFilter_Linear)
		, MinFilter(TextureFilter_Linear)
		, MipFilter(TextureFilter_None)
		, AddressU(TextureAddress_Repeat)
		, AddressV(TextureAddress_Repeat)
    {}
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================