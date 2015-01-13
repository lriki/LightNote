//==============================================================================
// LRoundTripValue 
//------------------------------------------------------------------------------
///**
//  @file       RoundTripValue.h
//  @brief      RoundTripValue
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
// �� RoundTripValue
//------------------------------------------------------------------------------
///**
//	@brief		��������l�̃N���X
//*/
//==============================================================================
template < typename VALUE_T = lnfloat, typename TIME_T = lnfloat > 
class RoundTripValue
{
public:

	/// �R���X�g���N�^
	//RoundTripValue() {}

	//----------------------------------------------------------------------
	///**
	//  @brief      �A�j���[�V�����J�n
	// 
	//  @param[in]  begin_value_	: �J�n�l ( ���Ԃ� 0.0 �̂Ƃ��̒l )
	//  @param[in]  end_value_		: �I�[�l ( ���Ԃ� cycle_time_ �̂Ƃ��̒l )
	//  @param[in]  cycle_time_		: �ύX�ɂ����鎞�� (�Г�)
	//*/
	//----------------------------------------------------------------------
	void start( VALUE_T begin_value_, VALUE_T end_value_, TIME_T cycle_time_ )
	{
		mBeginValue = begin_value_;
		mEndValue = end_value_;
		mValue = 0;
		mCycleTime = cycle_time_;
		mCurrentTime = 0;
	}

	/// ���Ԃ�i�߂�
	void advanceTime( TIME_T delta_time_ )
	{
		mCurrentTime += delta_time_;
		_calcValue();
	}

	/// �l�̎擾
	VALUE_T getValue() const { return mValue; }

private:

	void _calcValue()
	{
		if ( mCurrentTime > mCycleTime )
		{
			mCurrentTime = -mCycleTime;
		}
		TIME_T t = abs( mCurrentTime ) / mCycleTime;
		mValue = static_cast< VALUE_T >( mBeginValue * ( 1.0 - t ) + mEndValue * t );
	}

private:

	VALUE_T	mBeginValue;
	VALUE_T	mEndValue;
	VALUE_T	mValue;
	TIME_T	mCurrentTime;
	TIME_T	mCycleTime;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Animation
} // namespace Core
} // namespace LNote

//==============================================================================
//								end of file
//==============================================================================