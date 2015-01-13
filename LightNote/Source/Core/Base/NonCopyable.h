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
//	@brief      �R�s�[���֎~����N���X�� private �p��������
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
