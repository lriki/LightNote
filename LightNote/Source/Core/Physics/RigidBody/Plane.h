//==============================================================================
// Plane 
//------------------------------------------------------------------------------
///**
//  @file       Plane.h
//  @brief      Plane
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "RigidBody.h"

namespace LNote
{
namespace Core
{
namespace Physics
{

//==============================================================================
// ■ Plane
//------------------------------------------------------------------------------
///**
//  @brief		平面地面のクラス
//*/
//==============================================================================
class Plane
    : public RigidBody
{
public:
    Plane();
    virtual ~Plane();

public:

	/// 初期化
    void initialize( Manager* manager, lnU16 group = 0xffff, lnU16 groupMask = 0xffff );

protected:
    virtual void drawDebugShape( IDebugRenderer* renderer );
};

} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================