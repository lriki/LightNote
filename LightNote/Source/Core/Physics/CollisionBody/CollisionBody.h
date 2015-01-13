//==============================================================================
// CollisionBody 
//------------------------------------------------------------------------------
///**
//  @file       CollisionBody.h
//  @brief      CollisionBody
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../BodyBase.h"

namespace LNote
{
namespace Core
{
namespace Physics
{

//==============================================================================
// ■ CollisionBody
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class CollisionBody
    : public BodyBase
{
public:
    CollisionBody();
    virtual ~CollisionBody();

protected:
    void create( Manager* manager, btCollisionObject* obj, lnU16 group, lnU16 mask );

public:

	///// 位置の設定
	//void setPosition( const LVector3& position );

	///// 回転の設定
	//void setRotation( const LQuaternion& rotation );

	///// 回転角度の設定
	//void setAngle( const LVector3& euler );

	///// 移動・回転減衰値の設定
 //   void setDamping( float linDamping, float angDamping );

 //   /// 反射率の設定
 //   void setRestitution( float value_ );

 //   /// 反射率の取得
 //   float getRestitution() const;

 //   /// 摩擦係数の設定
 //   void setFriction( float value_ );

 //   /// 摩擦係数の取得
 //   float getFriction() const;

	///// ワールド変換行列の取得
	//const LMatrix& getWorldMatrix() const;

	///// (キネマティックな剛体用 setWorldMatrix())
	//void setKinematicAlignmentMatrix( const LMatrix& matrix );

	///// 指定の姿勢を強制的に設定する (速度が 0 にリセットされる)
	//void moveToForced( const LMatrix& matrix );

public:
	virtual BodyType getBodyType() const { return BodyType_CollisionBody; }

public:
	btCollisionObject*	getBtCollisionObject() { return mBtCollisionObject; }
	lnU16				getGroup() const { return mGroup; }
	lnU16				getGroupMask() const { return mGroupMask; }

protected:
    btCollisionObject*	mBtCollisionObject;
	lnU16				mGroup;
	lnU16				mGroupMask;
	//LMatrix				mWorldMatrix;
	bool				mCollisionEnable;
};

} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================