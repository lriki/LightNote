//==============================================================================
// AnimationController 
//------------------------------------------------------------------------------
///**
//  @file       AnimationController.h
//  @brief      AnimationController
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "Types.h"

namespace LNote
{
namespace Core
{
namespace Animation
{
class AnimationClip;

enum PlayMode
{
	StopSameLayer = 0,	///< �����C���[�̃A�j���[�V���������ׂĒ�~����
	StopAll,			///< �S�ẴA�j���[�V�������~����
};

enum QueueMode
{
	CompleteOthers = 0,	///< ���̂��ׂẴA�j���[�V��������~������ɍĐ����J�n����
	PlayNow				///< �����ɍĐ�����
};

/// �A�j���[�V�����l�X�V�ʒm���󂯎��I�u�W�F�N�g
class IAnimationApplyTarget2
{
public:
	virtual ~IAnimationApplyTarget2() {}

public:

	/// AnimationClip �ɃZ�b�g�����A�j���[�V�����Ɗ֘A�t���邽�߂̃L�[ (�ʏ�A�{�[����)
	virtual const lnRefStringW& getAnimationTargetName() const = 0;

	/// �l���X�V���ꂽ���ɌĂ΂��
	virtual void onUpdateAnimationValue( ValueType type, void* value ) = 0;
};




















class IAnimationApplyTarget
{
public:
	virtual ~IAnimationApplyTarget() {}

public:

	/// AnimationClip �ɃZ�b�g�����A�j���[�V�����Ɗ֘A�t���邽�߂̃L�[ (�ʏ�A�{�[����)
	virtual const lnRefKeyString& getAnimationTargetName() const = 0;

	/// �l���X�V���ꂽ���ɌĂ΂��
	virtual void onUpdateAnimationValue( ValueType type, void* value ) = 0;
};


/// �A�j���[�V�����K�p�l���ꎞ�I�Ɋi�[����ꏊ�ƁA�����ݒ肷��^�[�Q�b�g�̃Z�b�g
struct AnimationApplyTargetState
{
	enum ValueType			ValueType;
	lnFloat					Value[16];	// advanceTime() �ł͂����ɒl������āA
	IAnimationApplyTarget*	Target;		// �ŏI�I�ɂ����ɐݒ肷��B
};

struct AnimationState
{
	AnimationState()
	{
		mTime = 0;
	}

	class AnimationClip*	AnimationClip;


	/// (������Clip�Ԃł̓C���f�b�N�X�ƃ{�[��������v���Ă���Ƃ͌���Ȃ��̂� State �Ɏ������Ă���)
	struct AnimationTargetSet
	{
		//IAnimationApplyTarget*	Target;
		AnimationTimelineBase*		Animation;		// ���̃A�j���[�V������
		AnimationApplyTargetState*	TargetState;	// TargetState.Value �ɒl�����
	};
	typedef std::vector<AnimationTargetSet>	AnimationTargetSetArray;
	AnimationTargetSetArray		mAnimationTargetSetArray;
	double						mTime;
};

//==============================================================================
// �� AnimationController
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class AnimationController
	: public Core::Base::ReferenceObject
{
public:

public:
    AnimationController();
    ~AnimationController();

public:

	/// �Đ����ł��邩�𒲂ׂ�
	bool isPlaying() const;
    
	/// �Đ� (���̃A�j���[�V�����͂��ׂĒ�~����)
	void play( const lnKeyChar* animName, PlayMode mode = StopSameLayer );

	/// �u�����h (�A�j���[�V�����̍Đ��ɂ͉e�����Ȃ��B��~���̃A�j���[�V���������̊֐��ɂ���čĐ��J�n����邱�Ƃ͂Ȃ�)
	void blend( const lnKeyChar* animName, lnFloat targetWeight, lnFloat fadeLength );

	/// �N���X�t�F�[�h
	void crossFade( const lnKeyChar* animName, lnFloat fadeLength, PlayMode mode = StopSameLayer );

	/// �O�̃A�j���[�V�������I��������A�Đ����J�n����
	void playQueued( const lnKeyChar* animName, QueueMode queueMode = CompleteOthers, PlayMode playMode = StopSameLayer );

	/// �O�̃A�j���[�V�������I������Ƃ��A�N���X�t�F�[�h�ōĐ����J�n����
	void crossFadeQueued( const lnKeyChar* animName, lnFloat fadeLength, QueueMode queueMode = CompleteOthers, PlayMode playMode = StopSameLayer );

	/// �����C���[���̃A�j���[�V�����Đ����x�̓���
	void syncLayer( int layer );

	/// �A�j���[�V������i�߂�
	virtual void advanceTime( lnFloat deltaTime );

public:

	/// �A�j���[�V�����N���b�v�̒ǉ�   TODO: ���O atach
	void addAnimationClip( AnimationClip* animationClip );

	/// �A�j���[�V�����N���b�v�̍폜
	void removeAnimationClip( const lnKeyChar* clipName );

	/// AnimationState �̌���
	AnimationState* findAnimationState( const lnKeyChar* clipName );

	/// AnimationApplyTarget �̒ǉ� (�ł��邾�� AnimationController �쐬����ɐݒ肷�邱�ƁB�d���֎~)
	void addAnimationApplyTarget( IAnimationApplyTarget* target );

private:


	/// �ǉ��ς� AnimationApplyTarget �̌���
	AnimationApplyTargetState* findAnimationApplyTargetState( const lnKeyChar* naem );

	typedef std::vector<AnimationState*>			AnimationStateArray;
	typedef std::vector<AnimationApplyTargetState>	AnimationApplyTargetStateArray;

	AnimationStateArray				mAnimationStateArray;
	AnimationApplyTargetStateArray	mAnimationApplyTargetStateArray;
};










} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================