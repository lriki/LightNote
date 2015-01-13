//==============================================================================
// TransitionImageFilter 
//------------------------------------------------------------------------------
///**
//  @file       TransitionImageFilter.h
//  @brief      TransitionImageFilter
//  @author     Riki
//*/
//==============================================================================

#pragma once

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
// TransitionImageFilter
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class TransitionImageFilter
    : public ImageFilter
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
	TransitionImageFilter();
	virtual ~TransitionImageFilter();

public:
	/// �쐬
	void create( Manager* manager );

	/// ��ʃt���[�Y
	void freeze();

	/// �g�����W�V�������s
	void transition( double duration, Graphics::Texture* mask, int vague );

public:
	virtual void updateFrame(float elapsedTime);
	
private:
	class DrawContext;

	bool							mFreezeRequested;
    Animation::FadeValue<lnFloat>	mTransitionFactor;  ///< �g�����W�V�����̉��͈͂̍Œ�l (-mTransitionVague�`1.0)
	Graphics::Texture*				mTransitionMaskTexture;
	lnFloat							mTransitionVague;
};

} // namespace Scene
} // namespace Core
} // namespace LNote
