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
	/// �쐬
	void create( Manager* manager );

	/// �u���\�K�p (center �̓X�N���[�����W��ԂŎw�肷��(-1.0�`1.0) )
    void blur( double duration, lnFloat power, lnFloat scale, const LVector3& center, Camera* baseCamera );

public:
	virtual void updateFrame(float elapsedTime);
	
private:
	class DrawContext;

	LVector4                            mBlurCenter;
    LVector4                            mBlurColor;
    LMatrix                             mBlurMatrix;
    Animation::FixedLengthScalar        mBlurPower;
    double                              mBlarTime;              ///< �u���[�K�p�J�n����̗݌v���� (-1.0 �̏ꍇ�̓A�j���[�V���������ɌŒ肷��)
};

} // namespace Scene
} // namespace Core
} // namespace LNote
