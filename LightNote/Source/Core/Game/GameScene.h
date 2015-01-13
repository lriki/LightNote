//=============================================================================
//�y GameScene �z
//-----------------------------------------------------------------------------
///**
//  @file       GameScene.h
//  @brief      GameScene
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Common.h"
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
// �� GameScene
//-----------------------------------------------------------------------------
///**
//  @brief      
//
//  
//*/
//=============================================================================
class GameScene
    : public Base::ReferenceObject
{
public:

    GameScene();
    virtual ~GameScene();

public:

    /// �V�[���J�n
    virtual void onStart() {}

    /// �ĊJ (�q�V�[�����I�����A���̃V�[�����A�N�e�B�u�ɂȂ���)
	virtual void onRestart() {}
    //virtual void onResume() {}

    /// �t���[���X�V
    virtual void onUpdate();

    /// ��~ (�q�V�[�����A�N�e�B�u�ɂȂ���)
    virtual void onPause() {}

    /// �V�[���I��
    virtual void onTerminate() {}

    /// ���C�����[�v
    //virtual void main();

    GameScene* getParentGameScene() { return mParentGameScene; };

	GameObjectManager* getGameObjectManager() { return mGameObjectManager; }

public:

    /// [�����p]
    void initialize();

    /// [�����p]
    void finalize();

    /// [�����p] GameObject ���V�[���ɒǉ�����
    //void addGameObject(GameObject* object_);

protected:

    Manager*            mManager;           ///< �V�[���؂�ւ����� Manager ���璼�ڐݒ�
    GameScene*          mParentGameScene;

	GameObjectManager*	mGameObjectManager;
    //GameObjectArray     mGameObjectArray;

    friend class Manager;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Game
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================