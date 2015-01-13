//==============================================================================
// EventAnimation 
//------------------------------------------------------------------------------
///**
//  @file       EventAnimation.h
//  @brief      EventAnimation
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

//==============================================================================
// �� EventAnimationKeyBase
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class EventAnimationKeyBase
	: public Core::Base::ReferenceObject
{
public:

	double	FramePos;

public:
	EventAnimationKeyBase();
	virtual ~EventAnimationKeyBase() {}

public:

	/// ���s�֐�
	virtual void execute() = 0;

};

//==============================================================================
// �� EventAnimation
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class EventAnimation
	: public AnimationTimelineBase
{
public:
	EventAnimation();
	virtual ~EventAnimation();

public:

	/// �L�[�t���[���̒ǉ� (�I�[���O�ɒǉ������ꍇ�̓\�[�g���s��)
    void addKeyFrame( EventAnimationKeyBase* key );

	/// ���Ԃ�ݒ肷�� (�O�񂩂�̎��ԓ��ɃC�x���g������ꍇ�̓R�[���o�b�N����)
	void setTimeWithEvent( double time );

	/// �L�[���̎擾
	int getKeyCount() const { return mEventAnimationKeyArray.size(); }

	/// �L�[�̎擾
	EventAnimationKeyBase* getKeyAt( int index ) { return mEventAnimationKeyArray[index]; }

	/// �L�[�̃\�[�g
	void sortKeys();

public:
	virtual ValueType getValueType() const { return ValueType_Event; }
	virtual void setTime( double time );			// �C�x���g�̓R�[���o�b�N����Ȃ�
    virtual void advanceTime( lnFloat delta_time );	// �O�񂩂�̎��ԓ��ɃC�x���g������ꍇ�̓R�[���o�b�N����
	virtual double getLastFramePos();
    virtual XML::Element* serializeXMLElement( XML::Document* doc );
    virtual void deserializeXMLElement( XML::Element* element );

private:

	// begin_frame <= key < end_frame �Ŏ��s
	void executeRange( double begin_frame, double end_frame );

private:
	EventAnimationKeyArray	mEventAnimationKeyArray;
	int						mNextExecuteIndex;
};

} // namespace Animation
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================