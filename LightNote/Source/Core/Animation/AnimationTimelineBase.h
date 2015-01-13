//==============================================================================
// AnimationTimelineBase 
//------------------------------------------------------------------------------
///**
//  @file       AnimationTimelineBase.h
//  @brief      AnimationTimelineBase
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

//==============================================================================
// �� AnimationTimelineBase
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class AnimationTimelineBase
	: public Core::Base::ReferenceObject
{
public:
	AnimationTimelineBase();
	virtual ~AnimationTimelineBase() {}

public:

	/// ���Ԃ̃X�P�[���W���̐ݒ� (�ʏ�̓t���[�����[�g�̒l�����̂܂ܓn���Ă����B�f�t�H���g�l�� 60�BsetTime() �Ŏ��Ԃ̒l�ɏ�Z�����)
    void setTimeTickPerSec( lnFloat tick ) { mTimeTick = tick; }

	/// ���[�v�̈�̐ݒ�
	void setLoopState( LNAnimationLoopMode loop_mode, lnFloat begin_frame_pos, lnFloat end_frame_pos );

	/// �Đ����I�����Ă��邩�𔻒肷�� (���Ԃ��Ō�̃L�[�̃t���[���ʒu�ȍ~�̏ꍇ true�B���[�v�Đ����͏�� false)
    bool isFinished();

public:

	/// �l�̎��
	virtual ValueType getValueType() const = 0;

	/// ���Ԃ̐ݒ� (mTime �����̂܂܁AmCurrentFramePos �����[�v�l���Őݒ肷��)
	virtual void setTime( double time );

	/// ���Ԃ�i�߂� (�i�߂����Ԃ� setTime() ���Ă΂��)
    virtual void advanceTime( lnFloat deltaTime );

	/// ���Ԃ�i�߂� (���ݎ��ԂƑ����l���g�p����B�ŏI�I�Ȍ��ݎ��Ԃ� time + delta_time�ƂȂ�)
    virtual void advanceTime( double time, lnFloat deltaTime );

	/// �I�[�t���[���ʒu�̎擾
	virtual double getLastFramePos() = 0;

	/// XMLElement �֕ۑ�
	virtual XML::Element* serializeXMLElement( XML::Document* doc ) { LN_THROW_NotImpl(0); }

	/// XMLElement ���畜��
    virtual void deserializeXMLElement( XML::Element* element ) { LN_THROW_NotImpl(0); }

protected:

	lnFloat				mTimeTick;
	double				mTime;				///< setTime() �ɂ���Đݒ肳�ꂽ�l (Tick���l��)
	double				mCurrentFramePos;   ///< setTime() �Őݒ肳�ꂽ���Ԃ̃t���[���ʒu (���[�v�͈͓�)
	LNAnimationLoopMode	mLoopMode;
	double				mLoopBeginPos;
    double				mLoopEndPos;
	
};

} // namespace Animation
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================