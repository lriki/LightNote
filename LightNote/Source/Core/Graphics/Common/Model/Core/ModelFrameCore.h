//==============================================================================
// ModelFrameCore 
//------------------------------------------------------------------------------
///**
//  @file       ModelFrameCore.h
//  @brief      ModelFrameCore
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <vector>
#include "../Common.h"
#include "ModelIKCore.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

enum BoneConnectType
{
    BoneConnectType_PositionOffset = 0,
	BoneConnectType_Bone,
};

enum LocalProvideType
{
    LocalProvideType_UserTransformValue = 0,
	LocalProvideType_ParentLocalTransformValue,
};

//==============================================================================
// ModelFrameCore
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ModelFrameCore
    : public Base::ReferenceObject
{
public:
	ModelFrameCore();
    virtual ~ModelFrameCore();

public:
	void initialize( ModelCore2* owner, int thisIndex, int parentIndex, const lnRefString& name, const LVector3& orgPos );
	void postInitialize();
	void addChild( ModelFrameCore* child )
	{
		// ModelCore2::postInitialize() から呼ばれる。親はもう ParentFrameIndex でわかっている。
		mChildFrames.push_back( child );
	}
	void makeDepthLevel( int level )
	{
		mLevel = level;
		ln_foreach( ModelFrameCore* child, mChildFrames )
		{
			child->makeDepthLevel( level + 1 );
		}
	}

	const LMatrix& getSkinningOffsetInv() const { return mSkinningOffsetInv; }
	int getDepthLevel() const { return mLevel; }

private:
	std::vector<ModelFrameCore*>	mChildFrames;
	int								mLevel;

public:
	ModelCore2* OwnerModelCore;

	//ModelFrameCore*		ParentFrame;
	//ModelFrameCoreArray	ChildFrameArray;	///< (postCreate() で設定される)
	int							FrameIndex;		
	int							ParentFrameIndex;		///< -1 は無し
	ModelFrameCoreIndexArray	ChildFrameIndexArray;

	lnRefStringW			Name;

	LVector3            OrgPosition;		///< モデル原点からの位置
    LVector3		    OffsetFromParent;	///< (postCreate() で設定される) 親ボーンのからの相対位置 (親OrgPosition - OrgPosition)
    LMatrix			    InvTransform;		///< (postCreate() で設定される) 初期値のボーンを原点に移動させるような行列 (移動成分のみ。OrgPositionを使った逆行列)


	ModelIKLimitter2*	IKLimitter;

	// TOOD: IKボーン及びそのIKを親としている系列の変形階層を 0→1
	// 回転影響下以下の変形階層を 0→2 (SortVal の考え方な気がする)
	int					TransformLevel;

	// TODO: ボーンフラグ
	//lnU16				BoneFlags;
	BoneConnectType		BoneConnect;

	bool CanRotate;
	bool CanMove;
	bool IsVisible;
	bool CanOperate;

	bool IsIK;
    LocalProvideType LocalProvide;

	bool IsRotateProvided;
	bool IsMoveProvided;

	bool IsFixAxis;
	bool IsLocalAxis;

	bool TransformAfterPhysics;
	bool ParentTransform;

	LVector3	PositionOffset;		///< [接続先:0 の場合] 座標オフセット, ボーン位置からの相対分
	int			ConnectedBoneIndex;	///< [接続先:1 の場合] 接続先ボーンのボーンIndex

	int			ProvidedParentBoneIndex;	///< [回転付与:1 または 移動付与:1 の場合] 付与親ボーンのボーンIndex
	float		ProvidedRatio;				///< [回転付与:1 または 移動付与:1 の場合] 付与率

	LVector3	AxisDirectionVector;		///< [軸固定:1 の場合] 軸の方向ベクトル

	LVector3	DimentionXDirectionVector;	///< [ローカル軸:1 の場合] X軸の方向ベクトル
	LVector3	DimentionZDirectionVector;	///< [ローカル軸:1 の場合] Z軸の方向ベクトル

	int			KeyValue;					///< [外部親変形:1 の場合] Key値


	//int IKTargetBoneIndex;	///< [IK:1 の場合] IKターゲットボーンのボーンIndex (プログラム内では「エフェクタ」MMD内では「ターゲットボーン」)
	//int IKLoopCount;		///< [IK:1 の場合] IKループ回数
	//float IKRotateLimit;	///< [IK:1 の場合] IKループ計算時の1回あたりの制限角度 -> ラジアン角 | PMDのIK値とは4倍異なるので注意

	//
	//std::vector<ModelIKLink>	IKLinks;

private:
	LMatrix		mSkinningOffsetInv;

};

} // namespace Graphics
} // namespace Core
} // namespace LNote
