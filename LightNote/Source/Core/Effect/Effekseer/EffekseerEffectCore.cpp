//==============================================================================
// EffekseerEffectCore 
//==============================================================================

#include "stdafx.h"
#include "EffekseerEffectEngine.h"
#include "EffekseerEffectCore.h"

namespace LNote
{
namespace Core
{
namespace Effect
{
//==============================================================================
// Å° EffekseerEffectEngine
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	EffekseerEffectCore::EffekseerEffectCore( EffekseerEffectEngine* engine, Effekseer::Effect* efkEffeect )
		: mEffectEngine		( engine )
		, mEffekseerEffect	( efkEffeect )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	EffekseerEffectCore::~EffekseerEffectCore() 
	{
		ES_SAFE_RELEASE( mEffekseerEffect );
	}

} // namespace Effect
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================