//==============================================================================
// CollisionBox 
//------------------------------------------------------------------------------
///**
//  @file       CollisionBox.h
//  @brief      CollisionBox
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "CollisionBody.h"

namespace LNote
{
namespace Core
{
namespace Physics
{

//==============================================================================
// Å° CollisionBox
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class CollisionBox
    : public CollisionBody
{
public:
    CollisionBox();
    virtual ~CollisionBox();

public:
    void create( Manager* manager, const LVector3& size, lnU16 group, lnU16 mask );
};

} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================