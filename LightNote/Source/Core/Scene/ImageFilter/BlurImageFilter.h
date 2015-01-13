//==============================================================================
// BlurImageFilter 
//------------------------------------------------------------------------------
///**
//  @file       BlurImageFilter.h
//  @brief      BlurImageFilter
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../Animation/FadeValue.h"
#include "../../Graphics/Interface.h"
#include "../../Graphics/DeviceObjects.h"
#include "ImageFilter.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// BlurImageFilter
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class BlurImageFilter
    : public ImageFilter
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
	BlurImageFilter();
	virtual ~BlurImageFilter();

public:
	/// 作成
	void create( Manager* manager );

	/// ブラ―適用 (center はスクリーン座標空間で指定する(-1.0～1.0) )
    void blur( double duration, lnFloat power, lnFloat scale, const LVector3& center, Camera* baseCamera );

public:
	virtual void updateFrame(float elapsedTime);
	
private:
	class DrawContext;

	LVector4                            mBlurCenter;
    LVector4                            mBlurColor;
    LMatrix                             mBlurMatrix;
    Animation::FixedLengthScalar        mBlurPower;
    double                              mBlarTime;              ///< ブラー適用開始からの累計時間 (-1.0 の場合はアニメーションせずに固定する)
};

} // namespace Scene
} // namespace Core
} // namespace LNote
