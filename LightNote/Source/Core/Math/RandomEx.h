
#pragma once

namespace LNote
{
namespace Core
{
namespace Math
{
/// �g�����������̃p�����[�^
enum ExRandType
{
	ExRandType_None = 0,		///< �����_���l���g��Ȃ� (��� v0 �̒l�ɂȂ�)
	ExRandType_Range,			///< v0 �𒆉��l�Ƃ��āA�}v1 �͈̔͂Ƃ���
	ExRandType_MinMax,			///< v0 ���ŏ��l�Av1 ���ő�l�Ƃ���
};

/// �g�����������̃p�����[�^
enum ExRandRateType
{
	ExRandRateType_None = 0,	///< ��l�v�Z���g�p���Ȃ� (���ʂ̃����_���l)
	ExRandRateType_Min,			///< rate �l�� 1.0 �̎��͍ŏ��l�ɍ��킹��                                   rate=1.0 �� val=min			rate=0.0 �� val=max
	ExRandRateType_Max,			///< rate �l�� 1.0 �̎��͍ő�l�ɍ��킹��                                   rate=1.0 �� val=max			rate=0.0 �� val=min
	ExRandRateType_Median,		///< rate �l�� 1.0 �̎��͒����l�ɍ��킹��(0.0 �̎��ɂǂ��炩�̓����_��)     rate=1.0 �� val=median	    rate=0.0 �� val=min or max
	ExRandRateType_Extreme,		///< rate �l�� 1.0 �̎��͍ŏ��l���ő�l�ɍ��킹��(�ǂ��炩�̓����_��)       rate=1.0 �� val=min or max  rate=0.0 �� val=median
};

class Random 
	: public Base::ReferenceObject
	, public Lumino::Randomizer
{
public:
	LN_TYPE_INFO_ACCESS_DECL;

	Random() : Lumino::Randomizer() {}
	Random(int seed) : Lumino::Randomizer(seed) {}

public:
	//----------------------------------------------------------------------
	///**
	//	@brief      �����l�����̎擾
	//
	//  @param[in]  v0_        : ���͒l (�l�̈Ӗ��� rand_type_ �ɂ���ĕς��)
	//  @param[in]  v1_        : ���͒l
	//  @param[in]  rand_type_ : v0_ �� v1_ �̈Ӗ���\���l
	//  @param[in]  rate_      : �����Ɏg�����[�g�l (��Ԓl 0.0�`1.0�B�ʏ�A����������_���l�ɂ���)
	//  @param[in]  rate_type_ : rate_ �̈Ӗ���\���l
	//
	//  @par
	//              �܂��ŏ��ɁAv0_�Av1_�Arand_type_ ���烉���_���l�͈̔͂𒲂ׂ܂��B
	//              LN_RAND_MINMAX ���w�肳��Ă���ꍇ�A�ŏ��l�� v0_�A�ő�l�� v1_ ��
	//              ���̂܂܎g�p����܂��B<br>
	//              <br>
	//              ���ɁArate_type_ �ɂ���ď������s���܂��B<br>
	//              <br>
	//              LN_RANDRATE_MIN�ALN_RANDRATE_MAX �̏ꍇ�́A�ŏ��l�ƍő�l��
	//              rate_ �̒l�ŒP���ɐ��`�⊮�����l��Ԃ��܂��B
	//              �����̒l�����ׂē����ꍇ�͏�ɓ����l��Ԃ��܂��B<br>
	//              <br>
	//              LN_RANDRATE_CENTER�ALN_RANDRATE_OUTER �̏ꍇ�́A
	//              �����l�����߂���ɍŏ��l�A�ő�l�̂ǂ�����g���Čv�Z���邩��
	//              �������g���Č��߂܂��B(2��)<br>
	//              ���̌�A���߂��l�ƒ����l�� rate_ �ɂ���Đ��`��Ԃ����l��Ԃ��܂��B
	//
	//  @note
	//              ���܂̂Ƃ���A�G�t�F�N�g��p�[�e�B�N���̃����_���ȓ�����
	//              ����̋K�������������郆�[�e�B���e�B�݂����Ȋ����Ŏg���Ă�B
	//*/
	//----------------------------------------------------------------------
	float getFloatEx(float v0_, float v1_, ExRandType rand_type_, float rate_, ExRandRateType rate_type_)
	{
		if (rand_type_ == ExRandType_None)
		{
			return v0_;
		}
		else
		{
			float min, max;

			if (rand_type_ == ExRandType_Range)
			{
				min = v0_ - v1_;
				max = v0_ + v1_;
			}
			else // if ( rand_type_ == ExRandType_MinMax )
			{
				min = v0_;
				max = v1_;
			}

			switch (rate_type_)
			{

			default:
			{
				return GetFloatRange(min, max);
			}
			// rate = 1.0 �̂Ƃ��ɍŏ��l
			case ExRandRateType_Min:
			{
				return (max - min) * (1.0f - rate_) + min;
			}
			// rate = 1.0 �̂Ƃ��ɍő�l
			case ExRandRateType_Max:
			{
				return (max - min) * rate_ + min;
			}
			// rate = 1.0 �̂Ƃ��ɒ����l
			case ExRandRateType_Median:
			{
				float median = (max + min) * 0.5f;

				if (GetInt(2) & 1)
				{
					return Lumino::Math::Lerp(max, median, rate_); // 0.0 �̂Ƃ��� max
				}
				else
				{
					return Lumino::Math::Lerp(min, median, rate_); // 0.0 �̂Ƃ��� min
				}
			}
			// rate = 1.0 �̂Ƃ��� min �܂��� max
			case ExRandRateType_Extreme:
			{
				float median = (max + min) * 0.5f;

				if (GetInt(2) & 1)
				{
					return Lumino::Math::Lerp(median, max, rate_); // 1.0 �̂Ƃ��� max
				}
				else
				{
					return Lumino::Math::Lerp(median, min, rate_); // 1.0 �̂Ƃ��� max
				}
			}
			}
		}
		return v0_;
	}

};

}
}

typedef Core::Math::Random		LRandom;

}
