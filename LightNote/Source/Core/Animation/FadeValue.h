//==============================================================================
// FadeValue 
//------------------------------------------------------------------------------
///**
//  @file       FadeValue.h
//  @brief      FadeValue
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Animation
{

//==============================================================================
// �� FadeValue �N���X
//------------------------------------------------------------------------------
///**
//  @brief      �t�F�[�h�C���E�A�E�g����l�̃N���X
//*/
//==============================================================================
template < typename TYPE_, typename TIME_TYPE_ = double > class FadeValue
{
public:

	//----------------------------------------------------------------------
	///**
	//  @brief      �R���X�g���N�^
	//*/
	//----------------------------------------------------------------------
	FadeValue()
		: mStartValue( 0 )
		, mTargetValue( 0 )
		, mValue( 0 )
		, mTime( 0.0 )
		, mFadeTime( 0.0 )
	{}

	//----------------------------------------------------------------------
	///**
	//  @brief      �t�F�[�h�J�n
	// 
	//  @param[in]  start_value_  : �J�n���̒l
	//  @param[in]  target_value_ : �I�����̒l
	//  @param[in]  time_         : �ύX�ɂ����鎞�� ( �b )
	// 
	//  @par
	//              time_ �b��� target_value_ �̒l�ɂȂ�܂��B<br>
	//              time_ �� 0 �̏ꍇ�AgetValue() �͏�� target_value_ ��Ԃ��܂��B<br>
	//              <br>
	//              �܂��AsetTime() �Őݒ肳��Ă������Ԃ� 0.0 �Ƀ��Z�b�g����܂��B<br>
	//*/
	//----------------------------------------------------------------------
	void start( TYPE_ start_value_, TYPE_ target_value_, TIME_TYPE_ time_ )
	{
		mStartValue = start_value_;
		mTargetValue = target_value_;
		mFadeTime = time_;
		mTime = 0.0;
        setTime( mTime );
	}

	//----------------------------------------------------------------------
	///**
	//  @brief      �l�̎擾
	// 
	//  @par
	//              �o�ߎ��Ԃ� 0 �ȑO�̏ꍇ�� �J�n���̒l���A
	//              �o�ߎ��Ԃ� �ύX�ɂ����鎞�Ԉȏ� �̏ꍇ�� �I�����̒l ��Ԃ��܂��B
	//*/
	//----------------------------------------------------------------------
	const TYPE_& getValue() const { return mValue; }

	/// �J�n���̒l�̎擾
	TYPE_ getStartValue() const { return mStartValue; }

	/// �I�����̒l�̎擾
	TYPE_ getTargetValue() const { return mTargetValue; }

	/// ���݂̎��Ԃ̎擾
	TIME_TYPE_ getTime() const { return mTime; }


	//----------------------------------------------------------------------
	///**
	//  @brief      ���Ԃ�ݒ肷��
	// 
	//  @param[in]  time_ : �ݒ肷�鎞��
	//*/
	//----------------------------------------------------------------------
	void setTime( TIME_TYPE_ time_ )
	{
		mTime = time_;
		_calcValue();
	}

	//----------------------------------------------------------------------
	///**
	//  @brief      ���Ԃ�i�߂�
	// 
	//  @param[in]  delta_time_ : �i�߂鎞��
	//*/
	//----------------------------------------------------------------------
	void advanceTime( TIME_TYPE_ delta_time_ )
	{
		mTime += delta_time_;
		_calcValue();
	}

	//----------------------------------------------------------------------
	///**
	//  @brief      �I���𔻒肷��
	//*/
	//----------------------------------------------------------------------
	bool isFinished() const
	{
		return ( mTime >= mFadeTime );
	}

	
private:

	/// ���ۂɒl���v�Z����
	void _calcValue()
	{
        if ( mTime >= mFadeTime ) 
		{
			mValue = mTargetValue;
		}
		else if ( mTime <= 0.0 )
		{
			mValue = mStartValue;
		}
		else
		{
			mValue = mStartValue + (TYPE_)( ( mTargetValue - mStartValue ) * ( mTime / mFadeTime ) );	// ���`���
            //mValue = mStartValue + static_cast< TYPE_ >( static_cast< double >( mTargetValue - mStartValue ) * ( mTime / mFadeTime ) );	// ���`���
		    // �x������:�P���� double �ɃL���X�g����ƁAVector4 ���ϊ��ł��Ȃ��Ƃ��x���o��
        }

	}

private:

	TYPE_		mStartValue;
	TYPE_		mTargetValue;
	TYPE_		mValue;

	TIME_TYPE_  mTime;
	TIME_TYPE_  mFadeTime;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Animation
} // namespace Core

} // namespace LNote

//==============================================================================
//
//==============================================================================