//==============================================================================
// EffekseerEffectInstance 
//------------------------------------------------------------------------------
///**
//  @file       EffekseerEffectInstance.h
//  @brief      EffekseerEffectInstance
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <Effekseer/Effekseer.h>
#include "../EffectBase.h"

namespace LNote
{
namespace Core
{
namespace Effect
{
//==============================================================================
// Å° EffekseerEffectInstance
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class EffekseerEffectInstance
	: public EffectBase
{
public:
	EffekseerEffectInstance( EffekseerEffectCore* core );
	virtual ~EffekseerEffectInstance();
	
public:
	EffekseerEffectCore*	getEffectCore() { return mEffectCore; }

public:
	virtual void play( bool overlap );
	virtual void stop();
	virtual bool isPlaying();
	virtual void updateContext();
	virtual void update();
	virtual void draw();

private:
	void LNToEFKMatrix43( const LMatrix& mat, ::Effekseer::Matrix43* efkMat );

private:
	EffekseerEffectCore*				mEffectCore;
	::Effekseer::Handle					mCurrentHandle;
	std::vector<::Effekseer::Handle>	mOldHandleArray;

	std::vector<::Effekseer::Handle>	mDrawHandleArray;
};

} // namespace Effect
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================