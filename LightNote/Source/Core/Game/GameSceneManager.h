//=============================================================================
//�y GameSceneManager �z
//-----------------------------------------------------------------------------
///**
//  @file       GameSceneManager.h
//  @brief      GameSceneManager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Common.h"
#include <map>
#include <string>
#include <deque>

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
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
{
public:

    Manager();

public:

    /// ������
    void initialize( Core::Manager* framework );

    /// �I������
    void finalize();


    //---------------------------------------------------------------------
    ///**
    //  @brief      ���s
    //*/
    //---------------------------------------------------------------------
    //void run(GameScene* first_scene_);

    //---------------------------------------------------------------------
    ///**
    //  @brief      �Q�[���V�[���̕ύX
    //  @attention
    //              �n���ꂽ�Q�[���V�[���́A�I������ (onTerminate() ���Ă΂ꂽ����) ��
    //              release() ����܂����AgotoGameScene() �ł� addRef() ����܂���B
    //              ����� gotoGameScene(new TitleScene()) �̂悤��
    //              �V�[���ύX���Ȍ��ɋL�q�ł���悤�ɂ��邽�߂ł��B
    //*/
    //---------------------------------------------------------------------
    void gotoGameScene(GameScene* next_scene_);

    //---------------------------------------------------------------------
    ///**
    //  @brief      �Q�[���V�[���̌Ăяo��
    //  @par
    //              �w�肵���V�[�����q�V�[���Ƃ��ČĂяo���܂��B
    //              ���̃V�[���ɖ߂�ɂ́AreturnGameScene() ���g�p���܂��B
    //  @attention
    //              gotoGameScene() �Ɠ��l�ɁA�Q�ƃJ�E���g�̓C���N�������g���܂���B
    //*/
    //---------------------------------------------------------------------
    void callGameScene(GameScene* next_scene_);

    //---------------------------------------------------------------------
    ///**
    //  @brief      �Ăяo�����̃Q�[���V�[�� (�e�Q�[���V�[��) �֖߂�
    //*/
    //---------------------------------------------------------------------
    void returnGameScene();

    //---------------------------------------------------------------------
    ///**
    //  @brief      �Q�[���I��
    //  @par
    //              �Q�[���I���� Core::Manager �ɓ`���܂��B
    //*/
    //---------------------------------------------------------------------
    void exit();

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���̃Q�[���V�[�������邩(�ύX�̕K�v�����邩)���ׂ�
    //*/
    //---------------------------------------------------------------------
	//bool isNextScene() const;


    //---------------------------------------------------------------------
    ///**
    //  @brief      �t���[���X�V
    //  @retval     true  : ���s��
    //  @retval     false : �V�[����ύX����K�v������A�܂��͏I������
    //*/
    //---------------------------------------------------------------------
	bool update();

    /// ���݂̃V�[���̎擾
    GameScene* getCurrentGameScene() { return mScene; }





    /// (Ruby �p�b��)
    void setActiveGameScene(GameScene* scene_, const lnChar* name_);

    //---------------------------------------------------------------------
    ///**
    //  @brief      �V�[�����ɑ΂��ă��j�[�N��ID�����蓖�Ă�
    //  @note
    //              �A�N�e�B�u�ȃV�[�������ʂ��邽�߂�ID(�P���ȓo�^��)���쐬���A
    //              �o�^����B���ɓo�^���ꂽ���O������ꍇ�͓o�^���Ȃ��B
    //              (Ruby�Ή��̂��߂Ɏb��I�ɗp��)
    //*/
    //---------------------------------------------------------------------
    int registerActiveSceneKeyName(const lnChar* name_);

	void setEnableRefCountOperate( bool flag ) { mEnableRefCountOperate = flag; }

public:

    Core::Manager* getFramework() { return mFramework; }

private:

    /// ���s���̃V�[�����K�w�\�����܂߂� release ���� (gotoGameScene() �̂��߂Ɏg���B�S�ẴV�[����onTerminate() ���Ă΂��B���Ԃ͎q���e)
    void _releaseAllScene();

private:

    enum EventCommand
    {
        SMC_NONE = 0,       ///< ��`�Ȃ�
        SMC_CHANGE,         ///< ���ڑJ��
        SMC_PUSH,           ///< �Ăяo��
        SMC_RETURN,         ///< �Ăяo�����֖߂�
/*
        SMC_CHANGE_LOADING,
        SMC_PUSH_LOADING
        */
    };

    struct EventMessage
    {
        EventCommand    Command;
        GameScene*      Scene;
    };

    typedef std::map<ln_std_tstring, int>           SceneKeyNameMap;
    typedef std::pair<ln_std_tstring, int>          SceneKeyNamePair;

private:

    Core::Manager*		mFramework;
    GameScene*	        mScene;			    ///< ���݂̃Q�[���V�[��
	GameScene*	        mNextScene;		    ///< �ړ���̃Q�[���V�[��
    bool                mChangeRequested;   ///< �V�[���ύX�v��
	bool		        mEndRequested;	    ///< �I���v��

    GameScene*          mShouldAddedScene;  ///< ���̃t���[���̊J�n���Ɏq�Ƃ��Ēǉ�����V�[�� (�����ɕ����ǉ�����ꍇ�͈�Ԏq���w���B�ǉ�������͂����� NULL �ɂ��Ă���)
    std::deque< EventMessage >  mMessageQueue;

    SceneKeyNameMap mSceneKeyNameMap;
    //GameScene*      mActiveScene;
    int             mActiveSceneID;

	bool			mEnableRefCountOperate;
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