//==============================================================================
// AnimationClip 
//------------------------------------------------------------------------------
///**
//  @file       AnimationClip.h
//  @brief      AnimationClip
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "Types.h"
#include "AnimationTimelineBase.h"

namespace LNote
{
namespace Core
{
namespace Animation
{

/// �A�j���[�V�����Đ��̏������@
enum WrapMode
{
	WrapMode_Once = 0,		///< �Ō�܂ōĐ����ꂽ��A��~����
	WrapMode_Loop,			///< �Ō�܂ōĐ����ꂽ��A�擪�ɖ߂��ă��[�v����
	//WrapMode_PingPong,		///< �Ō�܂ōĐ����ꂽ��A�t�����ɖ߂��ă��[�v����
	//WrapMode_Default,
	//WrapMode_ClampForever,
};

//==============================================================================
// AnimationClip
//------------------------------------------------------------------------------
///**
//  @brief	�^�C�����C���̏W�� (3D���f���p���ƁA���s�A�W�����v�Ȃǂ�1���[�V������\��)
//*/
//==============================================================================
//class AnimationClip2
//	: public Core::Base::ReferenceObject
//{
//public:
//    AnimationClip2();
//    virtual ~AnimationClip2();
//
//public:
//
//	/// �A�j���[�V�����N���b�v���̐ݒ�
//	void setName( const lnRefStringW& name ) { mName = name; }
//
//	/// �A�j���[�V�����N���b�v���擾
//	const lnRefStringW& getName() const { return mName; }
//
//	/// �I�[�t���[���ʒu
//	double getLastFramePos() { return mLastFramePosition; }
//
//	/// �Đ����@�̐ݒ�
//	void setWrapMode( WrapMode mode ) { mWrapMode = mode; }
//
//	/// �Đ����@�̎擾
//	WrapMode getWrapMode() { return mWrapMode; }
//
//	/// �A�j���[�V�����̒ǉ� (3D���f�����[�V�����ł���΁A�{�[����, SQTTransformAnimation�BEvent�A�j���ł���� NULL)
//	void addAnimationCurve( const wchar_t* relativePath, AnimationTimelineBase* curve );
//
//	/// ���Ԃ�i�߂� (�S�ẴA�j���[�V������ advanceTime() ���Ă΂��)
//	void advanceTime( double currentTime, lnFloat deltaTime );
//
//public:
//
//	struct AnimationCurveState
//	{
//		lnRefStringW			RelativePath;
//		AnimationTimelineBase*	AnimationCurve;
//	};
//
//	typedef std::vector<AnimationCurveState>	AnimationCurveStateArray;
//
//	AnimationCurveStateArray& getAnimationCurveStateArray() { return mAnimationCurveStateArray; }
//
//private:
//	lnRefStringW				mName;
//	double						mLastFramePosition;
//	WrapMode					mWrapMode;
//	AnimationCurveStateArray	mAnimationCurveStateArray;
//};




//==============================================================================
// �� AnimationClip
//------------------------------------------------------------------------------
///**
//  @brief	AnimationTimelineBase �̏W�� (3D���f���p���ƁA���s�A�W�����v�Ȃǂ�1���[�V�����P��)
//*/
//==============================================================================
class AnimationClip
	: public Core::Base::ReferenceObject
{
public:

public:
    AnimationClip();
    virtual ~AnimationClip();

public:

	/// �A�j���[�V�����N���b�v���̐ݒ�
	void setName( const lnRefString& name ) { mName = name; }

	/// �A�j���[�V�����N���b�v���擾
	const lnRefKeyString& getName() const { return mName; }

	/// �I�[�t���[���ʒu
	double getLastFramePos() { return mLastFramePosition; }

	/// �Đ����@�̐ݒ�
	void setWrapMode( WrapMode mode ) { mWrapMode = mode; }

	/// �Đ����@�̎擾
	WrapMode getWrapMode() { return mWrapMode; }

	/// �A�j���[�V�����̒ǉ� (3D���f�����[�V�����ł���΁A�{�[����, SQTTransformAnimation�BEvent�A�j���ł���� NULL)
	void addAnimationCurve( const lnKeyChar* relativePath, AnimationTimelineBase* curve );

	/// ���Ԃ�i�߂� (�S�ẴA�j���[�V������ advanceTime() ���Ă΂��)
	void advanceTime( double currentTime, lnFloat deltaTime );

public:

	struct AnimationCurveState
	{
		lnRefKeyString				RelativePath;
		AnimationTimelineBase*	AnimationCurve;
	};

	typedef std::vector<AnimationCurveState>	AnimationCurveStateArray;

	AnimationCurveStateArray& getAnimationCurveStateArray() { return mAnimationCurveStateArray; }

private:

	
	
	lnRefKeyString				mName;
	double						mLastFramePosition;
	WrapMode					mWrapMode;
	AnimationCurveStateArray	mAnimationCurveStateArray;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================