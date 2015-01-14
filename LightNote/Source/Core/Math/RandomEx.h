
#pragma once

namespace LNote
{
namespace Core
{
namespace Math
{
/// 拡張乱数生成のパラメータ
enum ExRandType
{
	ExRandType_None = 0,		///< ランダム値を使わない (常に v0 の値になる)
	ExRandType_Range,			///< v0 を中央値として、±v1 の範囲とする
	ExRandType_MinMax,			///< v0 を最小値、v1 を最大値とする
};

/// 拡張乱数生成のパラメータ
enum ExRandRateType
{
	ExRandRateType_None = 0,	///< 基準値計算を使用しない (普通のランダム値)
	ExRandRateType_Min,			///< rate 値が 1.0 の時は最小値に合わせる                                   rate=1.0 → val=min			rate=0.0 → val=max
	ExRandRateType_Max,			///< rate 値が 1.0 の時は最大値に合わせる                                   rate=1.0 → val=max			rate=0.0 → val=min
	ExRandRateType_Median,		///< rate 値が 1.0 の時は中央値に合わせる(0.0 の時にどちらかはランダム)     rate=1.0 → val=median	    rate=0.0 → val=min or max
	ExRandRateType_Extreme,		///< rate 値が 1.0 の時は最小値か最大値に合わせる(どちらかはランダム)       rate=1.0 → val=min or max  rate=0.0 → val=median
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
	//	@brief      実数値乱数の取得
	//
	//  @param[in]  v0_        : 入力値 (値の意味は rand_type_ によって変わる)
	//  @param[in]  v1_        : 入力値
	//  @param[in]  rand_type_ : v0_ と v1_ の意味を表す値
	//  @param[in]  rate_      : 処理に使うレート値 (補間値 0.0～1.0。通常、これをランダム値にする)
	//  @param[in]  rate_type_ : rate_ の意味を表す値
	//
	//  @par
	//              まず最初に、v0_、v1_、rand_type_ からランダム値の範囲を調べます。
	//              LN_RAND_MINMAX が指定されている場合、最小値は v0_、最大値は v1_ が
	//              そのまま使用されます。<br>
	//              <br>
	//              次に、rate_type_ によって処理を行います。<br>
	//              <br>
	//              LN_RANDRATE_MIN、LN_RANDRATE_MAX の場合は、最小値と最大値を
	//              rate_ の値で単純に線形補完した値を返します。
	//              引数の値がすべて同じ場合は常に同じ値を返します。<br>
	//              <br>
	//              LN_RANDRATE_CENTER、LN_RANDRATE_OUTER の場合は、
	//              中央値を求めた後に最小値、最大値のどちらを使って計算するかを
	//              乱数を使って決めます。(2択)<br>
	//              その後、決めた値と中央値を rate_ によって線形補間した値を返します。
	//
	//  @note
	//              いまのところ、エフェクトやパーティクルのランダムな動きに
	//              特定の規則性を持たせるユーティリティみたいな感じで使ってる。
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
			// rate = 1.0 のときに最小値
			case ExRandRateType_Min:
			{
				return (max - min) * (1.0f - rate_) + min;
			}
			// rate = 1.0 のときに最大値
			case ExRandRateType_Max:
			{
				return (max - min) * rate_ + min;
			}
			// rate = 1.0 のときに中央値
			case ExRandRateType_Median:
			{
				float median = (max + min) * 0.5f;

				if (GetInt(2) & 1)
				{
					return Lumino::Math::Lerp(max, median, rate_); // 0.0 のときに max
				}
				else
				{
					return Lumino::Math::Lerp(min, median, rate_); // 0.0 のときに min
				}
			}
			// rate = 1.0 のときに min または max
			case ExRandRateType_Extreme:
			{
				float median = (max + min) * 0.5f;

				if (GetInt(2) & 1)
				{
					return Lumino::Math::Lerp(median, max, rate_); // 1.0 のときに max
				}
				else
				{
					return Lumino::Math::Lerp(median, min, rate_); // 1.0 のときに max
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
