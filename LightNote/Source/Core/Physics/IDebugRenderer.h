//==============================================================================
// IDebugRenderer 
//------------------------------------------------------------------------------
///**
//  @file       IDebugRenderer.h
//  @brief      IDebugRenderer
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Math/LMath.h"
#include "../Graphics/Common/Common.h"

class btIDebugDraw;

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Physics
{

//==============================================================================
// ■ IDebugRenderer クラス
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class IDebugRenderer
    //: public Base::Interface
{
public:

	/// 座標変換行列の設定 (ワールド行列)
    virtual void setMatrix( const LMatrix& matrix_ ) = 0;

	/// 球の描画
	virtual void drawSphere(const LVector3& pos_, lnFloat radius_, const LColorF& color_) = 0;

	/// ボックスの描画
	virtual void drawBox(const LVector3& bb_min_, const LVector3& bb_max_, const LColorF& color_) = 0;
    
	/// カプセルの描画
	virtual void drawCapsule(lnFloat radius_, lnFloat half_height_, const LColorF& color_) = 0;



    virtual void drawLine(const LVector3& from, const LVector3& to, const LColorF& from_color, const LColorF& to_color ) = 0;

protected:

    virtual ~IDebugRenderer() {}
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================