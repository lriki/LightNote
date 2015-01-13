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

	/// �J�v�Z���`��ō쐬
    void createCapsule( Manager* manager, lnFloat radius, lnFloat height );

	/// ���x���w�肵�Ĉړ�
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
