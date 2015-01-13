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
	/// 作成
	void create( Manager* manager );

	/// 画面フリーズ
	void freeze();

	/// トランジション実行
	void transition( double duration, Graphics::Texture* mask, int vague );

public:
	virtual void updateFrame(float elapsedTime);
	
private:
	class DrawContext;

	bool							mFreezeRequested;
    Animation::FadeValue<lnFloat>	mTransitionFactor;  ///< トランジションの可視範囲の最低値 (-mTransitionVague～1.0)
	Graphics::Texture*				mTransitionMaskTexture;
	lnFloat							mTransitionVague;
};

} // namespace Scene
} // namespace Core
} // namespace LNote
