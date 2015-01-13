//==============================================================================
// Interface 
//------------------------------------------------------------------------------
///**
//  @file       Interface.h
//  @brief      Interface
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
class Interface;
typedef void(*InterfaceDeleting)(Interface* obj);

//==============================================================================
// ■ Interface
//------------------------------------------------------------------------------
///**
//  @brief      参照カウントを持つオブジェクトのインターフェース            
//*/
//==============================================================================
class Interface
{
public:

	/// 参照カウントの取得
	virtual lnS32 getRefCount() const = 0;

	/// 参照カウントをひとつ増やす
	virtual lnS32 addRef() = 0;

	/// 参照カウントをひとつ減らす
	virtual lnS32 release() = 0;

	/// SAFE_RELEASE() マクロ対応版 release()
    inline lnS32 Release() { return release(); }

	virtual void setUserData(void* data) = 0;
	virtual void* getUserData() = 0;
	virtual void* getTypeUserData() = 0;

public:

	static InterfaceDeleting mInterfaceDeleting;

protected:

	virtual ~Interface() {}
};

} // namespace Base
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================