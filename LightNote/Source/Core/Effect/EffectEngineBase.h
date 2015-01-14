//==============================================================================
// EffectEngineBase 
//------------------------------------------------------------------------------
///**
//  @file       EffectEngineBase.h
//  @brief      EffectEngineBase
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "Common.h"
#include "../Math/LMath.h"
#include "../Base/Cache.h"

namespace LNote
{

namespace Core
{
namespace Effect
{
//==============================================================================
// �� EffectEngineBase
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class EffectEngineBase
    : public Base::ReferenceObject
{
public:
	EffectEngineBase();
	virtual ~EffectEngineBase();

public:

	/// ������
	void initialize( Manager* manager, int cacheSize );

	/// �I������
	virtual void finalize();

	/// Manager �擾
	Manager* getManager() { return mManager; }

	/// EffectCore ����
	Base::ICacheObject* findEffectCoreCache( const lnSharingKey& key );

	/// �L���b�V���N���A
	void clearEffectCoreCache();

	/// �r���[�E�v���W�F�N�V�����s��̐ݒ�
	virtual void setViewProjection( const LMatrix& view, const LMatrix& proj ) = 0;

	/// �`����̍X�V
	virtual void updateRenderContents() = 0;

protected:
	Manager*				mManager;
	Base::CacheManager		mEffectCoreCache;
};

} // namespace Effect
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================