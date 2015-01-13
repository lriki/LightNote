//==============================================================================
// ModelIKCore
//------------------------------------------------------------------------------
///**
//  @file       ModelIKCore.h
//  @brief      ModelIKCore
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Common.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

struct ModelIKLink
{
	int			LinkBoneIndex;	///< IK構成ボーン番号
	bool		IsRotateLimit;	///< 回転制限をするか
	LVector3	MinLimit;		///< 下限
	LVector3	MaxLimit;		///< 上限
};

//==============================================================================
// ModelIKCore2
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ModelIKCore2
	: public Base::ReferenceObject
{
public:
	ModelIKCore2()
		: TargetBoneIndex	( 0 )
		, EffectorBoneIndex	( 0 )
		, LoopCount			( 0 )
		, IKRotateLimit		( 0.0f )
	{}

	virtual ~ModelIKCore2() {}

public:
	/* ① - ② - ③    ④
	 *
	 *   ①② : IK影響下ボーン
	 *   ③	: エフェクタ		 (MMD上では「ターゲットボーン」と呼ばれる)
	 *   ④	: IKターゲットボーン (MMD上では「IKボーン」と呼ばれる。かかと等)
	 */
	
	int				TargetBoneIndex;	///< IKターゲットボーン (MMD:IKボーン)
	int				EffectorBoneIndex;	///< エフェクタ (MMD:ターゲットボーン)
	int				LoopCount;			///< 演算回数
	float			IKRotateLimit;		///< IKループ計算時の1回あたりの制限角度 -> ラジアン角 | PMDのIK値とは4倍異なるので注意

	std::vector<ModelIKLink> IKLinks;	///< IK影響ボーンと制限のリスト

	/* PMD の場合の IKRotateLimit は以下の計算結果を格納する
	 *		PI * Fact * (iLink + 1)
	 */
};




//==============================================================================
// ModelIKCore
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ModelIKCore
	: public Base::ReferenceObject
{
public:
	ModelIKCore() {}
	virtual ~ModelIKCore() {}

public:
	
	/* ①→②→③ ― ④
		①②	：IK影響下ボーン(足ボーン。IKBoneIndexArray)
		③		：IKボーン (目標位置。IK先端ボーン/エフェクタ)
		④		：IKターゲットボーン(かかと)
	*/

    ModelCore2*					OwnerModelCore;

    int							TargetBoneIndex;	///< IKボーン (PMDエディタで IK: のボーン)
    int							EffBoneIndex;		///< IKターゲットボーン (PMDエディタで Target: のボーン)

    int							CalcCount;			///< 再帰演算回数
    float						Fact;				///< IK 影響度
    short						SortVal;			///< IK データソート用の基準値

	ModelFrameCoreIndexArray	IKBoneIndexArray;	///< IKを構成するボーン番号の配列 Child
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
