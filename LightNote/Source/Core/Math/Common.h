//==============================================================================
// Enum 
//------------------------------------------------------------------------------
///**
//  @file       Enum.h
//  @brief      Enum
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "Asm.h"
#include "Math.h"

namespace LNote
{
namespace Core
{
namespace Math
{

/// 回転順序
enum RotationOrder
{
	RotationOrder_XYZ = 0,		///< X → Y → Z
	RotationOrder_XZY,			///< X → Z → Y (Quaternion → Euler 変換未実装)
	RotationOrder_YXZ,			///< Y → X → Z (Quaternion → Euler 変換未実装)
	RotationOrder_YZX,			///< Y → Z → X
	RotationOrder_ZXY,			///< Z → X → Y (Quaternion → Euler 変換未実装)
	RotationOrder_ZYX,			///< Z → Y → X
};

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

} // namespace Math
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================