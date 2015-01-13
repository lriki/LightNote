//==============================================================================
// EffectBase 
//------------------------------------------------------------------------------
///**
//  @file       EffectBase.h
//  @brief      EffectBase
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "Common.h"
#include "Interface.h"

namespace LNote
{
namespace Core
{
namespace Effect
{
//==============================================================================
// �� EffectBase
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class EffectBase
    : /*public Base::ReferenceObject
	, */public Effect
{
public:
	EffectBase( Manager* manager );
	virtual ~EffectBase();
	LN_REFOBJ_METHODS;

public:
	virtual void setMatrix( const LMatrix& matrix ) { mMatrix = matrix; }
	virtual void advanceTime( lnFloat deltaTime );

public:

	/// �`����̍X�V
	virtual void updateContext();

	/// �G�t�F�N�g�X�V (�X�V�X���b�h����Ă΂��)
	virtual void update() = 0;


protected:
	Manager*	mManager;
	LMatrix		mMatrix;
	lnFloat		mDeltaTime;

	struct
	{
		LMatrix	mMatrix;
		lnFloat	mDeltaTime;
	} mContext;
};

} // namespace Effect
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================