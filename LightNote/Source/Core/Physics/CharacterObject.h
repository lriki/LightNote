//==============================================================================
// CharacterObject
//------------------------------------------------------------------------------
///**
//  @file       CharacterObject.h
//  @brief      CharacterObject
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "RigidBody/RigidBody.h"

namespace LNote
{
namespace Core
{
namespace Physics
{

//==============================================================================
// CharacterObject
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class CharacterObject
    : public RigidBody
{
public:
    CharacterObject();
    virtual ~CharacterObject();

public:

	/// カプセル形状で作成
    void createCapsule( Manager* manager, lnFloat radius, lnFloat height );

	/// 速度を指定して移動
	void move( const LVector3& velocity ) { mMoveVelocity = velocity; }

public:
	virtual BodyType getBodyType() const { return BodyType_CharacterObject; }
	virtual void preUpdate();

public:
	LVector3	mMoveVelocity;	// TOOD: context
};

} // namespace Physics
} // namespace Core
} // namespace LNote
