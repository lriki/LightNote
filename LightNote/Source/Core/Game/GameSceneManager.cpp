//=============================================================================
//�y GameSceneManager �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Manager.h"
#include "GameScene.h"
#include "GameSceneManager.h"

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
// �� Manager
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    Manager::Manager()
        : mScene            (NULL)
        , mNextScene        (NULL)
        , mChangeRequested  (false)
        , mEndRequested     (false)
        , mFramework        (NULL)
        , mShouldAddedScene (NULL)
        , mActiveSceneID    (0)
		, mEnableRefCountOperate	( false )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Manager::initialize( Core::Manager* framework )
    {
        mFramework = framework;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Manager::finalize()
    {
        _releaseAllScene();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    //void Manager::run(GameScene* first_scene_)
    //{

    //    gotoGameScene(first_scene_);
    //    do
    //    {
    //        update();
    //        mScene->onUpdate();

    //    } while (mFramework->update());

    //    //mScene = first_scene_;
    //    //while (mScene && !mEndRequested)
    //    //{
    //    //    mScene->main();
    //    //}
    //}

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Manager::gotoGameScene(GameScene* next_scene_)
    {
		if ( mEnableRefCountOperate )
		{
			LN_SAFE_ADDREF( next_scene_ );
		}

        next_scene_->mManager = this;

        EventMessage e;
        e.Command  = SMC_CHANGE;
        e.Scene    = next_scene_;
        mMessageQueue.push_back( e );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Manager::callGameScene(GameScene* next_scene_)
    {
		if ( mEnableRefCountOperate )
		{
			LN_SAFE_ADDREF( next_scene_ );
		}

        next_scene_->mManager = this;

        EventMessage e;
        e.Command  = SMC_PUSH;
        e.Scene    = next_scene_;
        mMessageQueue.push_back( e );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Manager::returnGameScene()
    {
        EventMessage e;
        e.Command  = SMC_RETURN;
        e.Scene    = NULL;
        mMessageQueue.push_back( e );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Manager::exit()
    {
        mNextScene = NULL;
		mEndRequested = true;
        mFramework->exit();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
	//bool Manager::isNextScene() const
 //   {
 //       // ���̃V�[��������A����ɂ��ꂪ���݂̃V�[���Ƃ͈Ⴄ�ꍇ�� true ��Ԃ�
	//	return ( mNextScene != NULL  &&  mNextScene != mScene );
 //   }

    //---------------------------------------------------------------------
    // 
    //---------------------------------------------------------------------
	bool Manager::update()
    {
        //if (!mFramework->update()) return false;

        GameScene* current_scene = mScene;

        while ( !mMessageQueue.empty() )
        {
            const EventMessage& ev = mMessageQueue.front();

            switch ( ev.Command )
            {
                /////////////// ���ڑJ��
                case SMC_CHANGE:
                {
                    // ���݂̑S�ẴV�[������� (onTerminate() �Ăяo��)
                    _releaseAllScene();

                    mScene = ev.Scene;
                    if ( mScene )
                    {
                        mScene->mParentGameScene = NULL;
                        mScene->onStart();
                    }
                    break;
                }
                /////////////// �Ăяo��
                case SMC_PUSH:
                {
                    mScene->onPause();

                    // �A�N�e�B�u�ȃV�[���̎q�Ƃ��Ēǉ�����
                    ev.Scene->mParentGameScene = mScene;
                    mScene = ev.Scene;
                    mScene->onStart();
                    break;
                }
                /////////////// �Ăяo�����֖߂�
                case SMC_RETURN:
                {
                    GameScene* old = mScene;
                    old->onTerminate();
                    old->release();

                    mScene = mScene->mParentGameScene;
                    mScene->onRestart();
                    break;
                }
            }

            mMessageQueue.pop_front();
        }

        if ( mScene ) mScene->onUpdate();
            
        return (mScene == current_scene);
    }







    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Manager::setActiveGameScene(GameScene* scene_, const lnChar* name_)
    {
        
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    int Manager::registerActiveSceneKeyName(const lnChar* name_)
    {
        ln_std_tstring key = name_;
        SceneKeyNameMap::iterator itr = mSceneKeyNameMap.find(key);
        if (itr == mSceneKeyNameMap.end())
        {
            int id = mSceneKeyNameMap.size();
            mSceneKeyNameMap.insert(SceneKeyNamePair(key, id));
            return id;
        }
        else
        {
            return itr->second;
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Manager::_releaseAllScene()
    {
        GameScene* scene = mScene;
        GameScene* tmp;
        while ( scene )
        {
            scene->onPause();
            scene->onTerminate();
            scene->finalize();

            tmp = scene;
            scene = scene->mParentGameScene;
            tmp->release();
        }
        mScene = NULL;
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