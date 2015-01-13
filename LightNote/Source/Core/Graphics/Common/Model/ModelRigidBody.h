//==============================================================================
// ModelRigidBody 
//------------------------------------------------------------------------------
///**
//  @file       ModelRigidBody.h
//  @brief      ModelRigidBody
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
//#include <btBulletDynamicsCommon.h>
#include "../../../Base/RefString.h"
#include "../../../Physics/RigidBody/RigidBody.h"
#include "Common.h"
#include "PMDTypes.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
class ModelManager;


enum CollisionShapeType
{
    COLSHAPE_SPHERE = 0,    ///< 球
    COLSHAPE_BOX,           ///< 箱
    COLSHAPE_CAPSULE,       ///< カプセル
};

enum RigidBodyType
{
    RIGIDBODYTYPE_CONTROLLED_BY_BONE = 0,   ///< Bone追従
    RIGIDBODYTYPE_PHYSICS,                  ///< 物理演算
    RIGIDBODYTYPE_PHYSICS_ALIGNMENT,        ///< 物理演算(Bone位置合せ)
}; 

//==============================================================================
// ModelRigidBody
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ModelRigidBody2
{
public:
    ModelRigidBody2();
    virtual ~ModelRigidBody2();

public:

	/// 作成
    void create( Model2* ownerModel, ModelRigidBodyCore* bodyCore );

	/// ボーン位置合わせ (RIGIDBODYTYPE_PHYSICS_ALIGNMENT のときのみ有効。ボーンの位置に剛体を移動する)
    void fixPosition( float fElapsedFrame );
		
	/// 
    void updateBeforePhysics( const LMatrix& externalWorldGlobalMatrix );

	/// 
    void updateAfterPhysics( const LMatrix& externalWorldMatrixInv );
		
	
	/// この剛体を強制的にボーン位置に移動させる（姿勢リセット用）
	void moveBodyForced();

public:
	Physics::RigidBody* getRigidBody() { return mRigidBody; }

private:
	Model2*				mOwnerModel;
	ModelRigidBodyCore*	mRigidBodyCore;
    ModelFrame2*		mRelatedFrame;
	Physics::RigidBody*	mRigidBody;

	LMatrix				mOldWorldTransform;	///< ひとつ前の正常な行列 (NaN を含まない)
	LMatrix				mNewWorldTransform;	///< 現在の正常な行列
};





//==============================================================================
// ■ ModelRigidBody クラス
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ModelRigidBody
	//: public Physics::RigidBody
{
public:

    ModelRigidBody();// {}   // 苦肉のデフォルトコンストラクタ。new 直後に配置 new で初期化すること。
	//ModelRigidBody( ModelManager* manager_ );

    virtual ~ModelRigidBody();

public:

	/// 初期化
    void initializePMD( ModelManager* manager_, const PMD_RigidBody *pmd_rigidbody_, ModelFrame* frame_ );


	/// ボーン位置合わせ (RIGIDBODYTYPE_PHYSICS_ALIGNMENT のときのみ有効。ボーンの位置に剛体を移動する)
    void fixPosition( float fElapsedFrame );
		
	/// ボーンの姿勢を剛体の姿勢と一致させる(そのフレームのシミュレーション終了後に呼ぶ)
    void updateBoneTransform();
		
	/// 剛体をボーンの位置へ強制的に移動させる
    void moveToBonePos();


	/// デバッグ用の形状描画
	//virtual LNRESULT drawDebugShape( Physics::IDebugRenderer* renderer_ );

	Physics::RigidBody* getRigidBody() { return mRigidBody; }

private:

	/// 解放
    void _release();

private:

	/// 形状データ
    struct CollisionShapeData
    {
        CollisionShapeType Type;
        union
        {
            struct
            {
                lnFloat Radius;
            } Sphere;

            struct
            {
                lnFloat Width;
                lnFloat Height;
	            lnFloat Depth;
            } Box;

            struct
            {
                lnFloat Radius;
                lnFloat Height;
            } Capsule;
        };
    };

private:

	Physics::RigidBody*	mRigidBody;
	RigidBodyType       mRigidBodyType;
	LMatrix				mBoneOffset;		///< ボーン行列 → 剛体行列変換用
	LMatrix				mInvBoneOffset;		///< 剛体行列 → ボーン行列変換用



    ModelFrame*         mFrame;

    CollisionShapeData  mColShapeData;
    btCollisionShape*   mColShape;

    
	//btRigidBody*        mRigidBody;

    lnU16	            mColGroup;          // 衝突グループ
	lnU16	            mColGroupMask;	    // 衝突グループマスク
    LVector3	        mInitialPosition;	// 位置(ボーン相対)
	LVector3	        mInitialRotation;	// 回転(radian)
    lnFloat             mMass;			    // 質量
	lnFloat			    mLinearDamping;	    // 移動減
	lnFloat			    mAngularDamping;    // 回転減
	lnFloat			    mRestitution;	    // 反発力
	lnFloat			    mFriction;		    // 摩擦力

//    btTransform			mBtBoneOffset;
 //   btTransform			mBtInvBoneOffset;

    bool                mNoCopyToBone;

};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================