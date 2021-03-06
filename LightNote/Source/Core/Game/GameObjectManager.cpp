//=============================================================================
//【 GameObjectManager 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "GameObject.h"
#include "GameObjectManager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Game
{

//=============================================================================
// ■ GameObjectManager
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    GameObjectManager::GameObjectManager()
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    GameObjectManager::~GameObjectManager()
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GameObjectManager::initialize()
    {
    }

	//---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
	void GameObjectManager::finalize()
    {
		ln_foreach( GameObject* obj, mGameObjectArray )
        {
            obj->onDestroy();
            obj->release();
        }
        mGameObjectArray.clear();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GameObjectManager::update()
    {
		ln_foreach( GameObject* obj, mGameObjectArray )
        {
			if ( !obj->isDestroyed() )
				obj->onUpdate();
        }

        // 削除チェック
        GameObjectArray::iterator itr = mGameObjectArray.begin();
        for (; itr != mGameObjectArray.end(); )
        {
            if ((*itr)->isDestroyed())
            {
                (*itr)->onDestroy();
                LN_SAFE_RELEASE(*itr);
                itr = mGameObjectArray.erase(itr);
            }
            else
            {
                ++itr;
            }
        }
    }

	//---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GameObjectManager::registGameObject( GameObject* object )
	{
		LN_SAFE_ADDREF( object );
        mGameObjectArray.push_back( object );

		// TODO:親指定の登録時は、親の後ろに挿入するのがベスト
	}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Game
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================