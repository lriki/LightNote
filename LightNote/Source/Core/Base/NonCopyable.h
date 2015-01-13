//==============================================================================
// NonCopyable 
//------------------------------------------------------------------------------
///**
//  @file       NonCopyable.h
//  @brief      NonCopyable
//  @author     Riki
//*/
//==============================================================================

#pragma once

namespace LNote
{
namespace Core
{
namespace Base
{

//==============================================================================
// NonCopyable
//------------------------------------------------------------------------------
///**
//	@brief      コピーを禁止するクラスに private 継承させる
//*/
//==============================================================================
class NonCopyable
{
protected:
    NonCopyable() {}
    virtual ~NonCopyable() {}

private:
    NonCopyable( NonCopyable const& );
    NonCopyable& operator=( NonCopyable const& );
};

} // namespace Base
} // namespace Core
} // namespace LNote
