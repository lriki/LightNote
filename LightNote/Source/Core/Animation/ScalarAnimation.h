//==============================================================================
// ScalarAnimation 
//------------------------------------------------------------------------------
///**
//  @file       ScalarAnimation.h
//  @brief      ScalarAnimation
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
// �� ScalarAnimationBase
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class ScalarAnimationBase
	: public AnimationTimelineBase
{
public:
	virtual ~ScalarAnimationBase() {}

public:

	/// �l�̎擾
    virtual lnFloat getValue() const = 0;

public:
	virtual ValueType getValueType() const { return ValueType_Float; }
};

//==============================================================================
// �� ScalarAnimationKey
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
struct ScalarAnimationKey
{
public:

	double      FramePos;       ///< �t���[���ʒu
    lnFloat     Value;          ///< �l
    lnFloat     Velocity;       ///< ���x
    lnFloat     Accel;          ///< �����x
    lnFloat     LeftSlope;      ///< ���� (�I��) ���x
    lnFloat     RightSlope;     ///< �E�� (�J�n) ���x
    lnU8		InterMode;      ///< ���̃L�[�Ƃ̕�ԕ��@ ( LNAnimInterMode �̒l )
    
public:

	/// ������ ( InterMode �� LN_ANIM_INTER_LINEAR�A����ȊO�� 0.0 �ŏ����� )
    ScalarAnimationKey();
};

//==============================================================================
// �� ScalarAnimationSourceKey
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
struct ScalarAnimationSourceKey
{
public:

	double          FramePos;           ///< �t���[���ʒu

    lnFloat         Value;              ///< �l
    lnFloat         ValueRand;          ///< Value �����_����

	lnFloat         Velocity;           ///< ���x
	lnFloat         VelocityRand;       ///< Velocity �����_����

	lnFloat         Accel;              ///< �����x
	lnFloat         AccelRand;          ///< Accel �����_����

	lnFloat         LeftSlope;          ///< ���� (�I��) ���x
	lnFloat         LeftSlopeRand;      ///< LeftSlope �����_����

	lnFloat         RightSlope;         ///< �E�� (�J�n) ���x
	lnFloat         RightSlopeRand;     ///< RightSlope �����_����

    LNAnimInterMode InterMode;          ///< ���̃L�[�Ƃ̕�ԕ��@ ()

public:

	/// ������ ( InterMode �� LN_ANIM_INTER_LINEAR�A����ȊO�� 0.0 �ŏ����� )
    ScalarAnimationSourceKey();
};

//==============================================================================
// �� ScalarAnimation
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class ScalarAnimation
	: public ScalarAnimationBase
{
public:
	static const lnChar* XML_ELEMENT_NAME;
	static const lnChar* XML_ELEMENT_NAME_KEY;

public:
	ScalarAnimation();
	virtual ~ScalarAnimation();

public:

	/// �L�[���ЂƂ��Ȃ����A���Ԃ��ŏ��̃t���[���ʒu���O�̂Ƃ��ɕԂ��l�̐ݒ�
    void setDefaultValue( lnFloat value ) { mDefaultValue = value; }

	/// �L�[�̒ǉ� (�I�[���O�ɒǉ������ꍇ�̓\�[�g���s��)
    void addKeyFrame( double frame_pos, lnFloat value );

	/// �L�[�̒ǉ� (�I�[���O�ɒǉ������ꍇ�̓\�[�g���s��)
    void addKeyFrame( const ScalarAnimationKey& key );

	/// �L�[�̐��̎擾
    int getKeyNum() const { return mKeyFrameList.size(); }

	/// �l�̎擾
    virtual lnFloat getValue() const { return mValue; }

	/// �L�[��S�č폜����
	void clear() { mKeyFrameList.clear(); }

	/// �L�[�ɃA�N�Z�X����
    ScalarAnimationKey& getKeyAt( int index );

	/// �\�[�X�L�[�̒ǉ� (�I�[���O�ɒǉ������ꍇ�̓\�[�g���s��)
    void addSourceKeyFrame( const ScalarAnimationSourceKey& key );

	/// �\�[�X�L�[�̃N���A
    void clearSourceKeys() { mSourceKeyFrameList.clear(); }

	/// �\�[�X�L�[�̎擾
    ScalarAnimationSourceKey& getSourceKeyAt( int index ) { return mSourceKeyFrameList[ index ]; }

	/// �\�[�X�L�[���̎擾
    int getSourceKeyCount() const { return mSourceKeyFrameList.size(); }

	/// �S�Ẵ\�[�X�L�[���L�[�t���[���ɔ��f������ (rand_seed �� 0 �̏ꍇ�̓����_���l��K�p���Ȃ�)
	void applySourceKeys( int rand_seed );

public:
	virtual void setTime( double time );
	virtual double getLastFramePos();
    virtual XML::Element* serializeXMLElement( XML::Document* doc );	// �\�[�X�L�[��ۑ�
    virtual void deserializeXMLElement( XML::Element* element );

private:
	typedef std::vector<ScalarAnimationKey>			KeyFrameList;  
	typedef std::vector<ScalarAnimationSourceKey>	SourceKeyFrameList;  

	KeyFrameList		mKeyFrameList;
	SourceKeyFrameList	mSourceKeyFrameList;
	lnFloat				mDefaultValue;
    lnFloat				mValue;
};

} // namespace Animation
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================