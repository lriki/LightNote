//==============================================================================
// BodyBase 
//------------------------------------------------------------------------------
///**
//  @file       BodyBase.h
//  @brief      BodyBase
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Math/LMath.h"
#include "IDebugRenderer.h"
#include "Manager.h"

class btCollisionObject;

namespace LNote
{
namespace Core
{
namespace Physics
{

//==============================================================================
// ■ BodyBase
//------------------------------------------------------------------------------
///**
//  @brief		剛体・ソフトボディの基底クラス
//*/
//==============================================================================
class BodyBase
    : public Base::ReferenceObject
{
public:
    BodyBase();
    virtual ~BodyBase();

public:
	/// 接触点の数
	int getContactCount() const { return mContactList.size(); }

	/// 接触点の取得
	const ContactInfo& getContact( int index ) const { return mContactList[index]; }

	/// ユーザーデータの設定
	void setUserData( void* data ) { mUserData = data; }

	/// ユーザーデータの取得
	void* getUserData() const { return mUserData; }

public:

	/// 初期化
    void initialize( Manager* manager, btCollisionShape* shape, btCollisionObject* obj );

	/// 種類
	virtual BodyType getBodyType() const = 0;

	///// シミュレーション更新直前の処理
	//virtual void preUpdate() {}

	///// シミュレーション更新直後の処理
	//virtual void postUpdate() {}

	/// デバッグ用の形状描画
    virtual void drawDebugShape( IDebugRenderer* renderer ) {}

public:
	void clearContactList() { mContactList.clear(); }
	void addContact( const ContactInfo& contact ) { mContactList.push_back( contact ); }

protected:
    Manager*				    mManager;

private:
	//btCollisionShape*		    mCollisionShape;
	void*				        mUserData;
	ContactInfoArray			mContactList;
};

} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================