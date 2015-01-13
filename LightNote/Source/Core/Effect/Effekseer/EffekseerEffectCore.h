//==============================================================================
// EffekseerEffectCore 
//------------------------------------------------------------------------------
///**
//  @file       EffekseerEffectCore.h
//  @brief      EffekseerEffectCore
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <Effekseer/Effekseer.h>
#include "../Common.h"
#include "../../Base/Cache.h"

namespace LNote
{
namespace Core
{
namespace Effect
{
//==============================================================================
// Å° EffekseerEffectCore
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class EffekseerEffectCore
    : public Base::ReferenceObject
	, public Base::ICacheObject
{
	LN_CACHE_OBJECT_DECL;
public:
	EffekseerEffectCore( EffekseerEffectEngine* engine, ::Effekseer::Effect* efkEffeect );
	virtual ~EffekseerEffectCore();

public:
	EffekseerEffectEngine*	getEffectEngine() { return mEffectEngine; }
	::Effekseer::Effect*	getEffekseerEffect() { return mEffekseerEffect; }

public:
	EffekseerEffectEngine*	mEffectEngine;
	::Effekseer::Effect*	mEffekseerEffect;
};

} // namespace Effect
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================