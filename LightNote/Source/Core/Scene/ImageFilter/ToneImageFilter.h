//==============================================================================
// ToneImageFilter 
//------------------------------------------------------------------------------
///**
//  @file       ToneImageFilter.h
//  @brief      ToneImageFilter
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
// ToneImageFilter
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class ToneImageFilter
    : public ImageFilter
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
	ToneImageFilter();
	virtual ~ToneImageFilter();

public:
	/// çÏê¨
	void create( Manager* manager );

	/// êFí≤ÇÃê›íË
	void setTone( const LTone& tone, lnFloat duration );

public:
	virtual void updateFrame(float elapsedTime) { mTone.advanceTime(elapsedTime); }
	
private:
	class DrawContext;

	Animation::FadeValue<LVector4, lnFloat>	mTone;
	Graphics::IShader*						mShader;
    Graphics::IShaderVariable*				mVarTone;
    Graphics::IShaderVariable*				mVarScreenTexture;
};

} // namespace Scene
} // namespace Core
} // namespace LNote
