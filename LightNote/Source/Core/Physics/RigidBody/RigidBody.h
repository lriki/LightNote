//==============================================================================
// RigidBody 
//------------------------------------------------------------------------------
///**
//  @file       RigidBody.h
//  @brief      RigidBody
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../Math/Quaternion.h"
#include "../BodyBase.h"

namespace LNote
{
namespace Core
{
namespace Physics
{

struct RigidBodyInitArgs
{
    btCollisionShape*   Shape;      ///< (BodyBase  削除時に delete される)
    btRigidBody*        RigidBody;  ///< (RigidBody 削除時に delete される	PMD 側を修正したら削除予定)
	//lnU16               Group;
	//lnU16               Mask;
	//float				Mass;
	//bool				AdditionalDamping;
	//bool				KinematicObject;	///< Kinematicオブジェクトとする (MotionState の getWorldTransform() が呼ばれるようになる)

	RigidBodyInitArgs()
	{
		//AdditionalDamping = false;
		RigidBody = NULL;
		//KinematicObject = false;
	}
};

//==============================================================================
// ■ RigidBody
//------------------------------------------------------------------------------
///**
//  @brief		剛体のクラス
//*/
//==============================================================================
class RigidBody
    : public BodyBase
{
public:
	struct ConfigData
	{
		lnFloat			Mass;				///< 質量
		lnU16			Group;				///< 衝突グループ
		lnU16			GroupMask;			///< 非衝突グループ
	    lnFloat			Restitution;	    ///< 反発力
	    lnFloat			Friction;		    ///< 摩擦力
		lnFloat			LinearDamping;	    ///< 移動減
	    lnFloat			AngularDamping;		///< 回転減
		const LMatrix*	InitialTransform;	///< 初期姿勢 (NULL で Identity)
		bool			AdditionalDamping;	///< 減衰の有効
		bool			KinematicObject;	///< Kinematicオブジェクトとする (質量が 0.0 として扱われ、MotionState の getWorldTransform() が呼ばれるようになる)

		ConfigData()
		{
			Group = 0xffff;
			GroupMask = 0xffff;
			Mass = 0.0;
			Restitution = 0.0;
			Friction = 0.5;
			LinearDamping = 0.0;
			AngularDamping = 0.0;
			InitialTransform = NULL;
			AdditionalDamping = false;
			KinematicObject = false;
		}
	};

public:
    RigidBody();
    virtual ~RigidBody();

public:

	/// 初期化 (剛体を受け取ってワールドに追加する) (現行PMD用にpublic。後で protected にする)
    void initialize( Manager* manager, const RigidBodyInitArgs& args, const ConfigData& configData );

#if 0
	/// 位置の設定
	void setPosition( const LVector3& position );

	/// 回転の設定
	void setRotation( const LQuaternion& rotation );

	/// 回転角度の設定
	void setAngle( const LVector3& euler );

	/// 移動・回転減衰値の設定
    void setDamping( float linDamping, float angDamping );

	/// 反射率の設定
    void setRestitution( float value_ );

	/// 反射率の取得
    float getRestitution() const;

	/// 摩擦係数の設定
    void setFriction( float value_ );

	/// 摩擦係数の取得
    float getFriction() const;

	/// ワールド変換行列の取得
	const LMatrix& getWorldMatrix() const;

	/// (キネマティックな剛体用 setWorldMatrix())
	void setKinematicAlignmentMatrix( const LMatrix& matrix );
#endif
	void activate();

	/// ワールド変換行列の設定
	void setWorldMatrix( const LMatrix& matrix );

	/// ワールド変換行列の取得
	const LMatrix& getWorldMatrix() const;

	/// 指定の姿勢を強制的に設定する (速度が 0 にリセットされる)
	void moveToForced( const LMatrix& matrix );

	/// 物理演算の対象であるか (false の場合、衝突判定のみ対象)
	bool isContactResponse() const { return true; }

public:
	void syncBeforeUpdate();
	void syncUpdating();

public:
	virtual BodyType getBodyType() const { return BodyType_RigidBody; }
	//virtual void preUpdate();
	//virtual void postUpdate();

public:
	btRigidBody*	getBtRigidBody() { return mBtRigidBody; }
	lnU16			getGroup() const { return mGroup; }
	lnU16			getGroupMask() const { return mGroupMask; }

protected:
	struct KinematicMotionState;

    btRigidBody*    mBtRigidBody;
	lnU16			mGroup;
	lnU16			mGroupMask;
    LMatrix         mWorldMatrix;   ///< (postUpdate() で設定される)
	KinematicMotionState*	mKinematicMotionState;
	//LMatrix			mKinematicAlignmentMatrix;

	//LMatrix			mMoveToForcedMatrix;

	bool			mMoveToForced;
	bool			mSetWorldMatrix;
};

} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================