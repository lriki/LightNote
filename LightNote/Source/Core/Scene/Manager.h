//==============================================================================
// Manager 
//------------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//*/
//==============================================================================

#pragma once

#include "../Base/STLUtils.h"
#include "../Math/Vector2.h"
#include "../Threading/EventFlag.h"
#include "../System/Interface.h"
#include "../Game/GameTime.h"
#include "../Graphics/Common/ProfilerRenderer.h"
#include "Common.h"
#include "Pane.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// Manager
//------------------------------------------------------------------------------
///**
//  @brief		�V�[���̊Ǘ��N���X
//*/
//==============================================================================
class Manager
    : public Base::ReferenceObject
	, public System::IEventListener
{
public:

	/// �������f�[�^
	struct ConfigData
	{
		System::Manager*	SystemManager;
        FileIO::Manager*	FileManager;
        Graphics::Manager*  GraphicsManager;
        Effect::Manager*    EffectManager;      ///< �f�t�H���g�̃V�[���ɐݒ肳��� EffectManage
        Physics::Manager*   PhysicsManager;     ///< �f�o�b�O�`��p
        LVector2            VirtualViewSize;    ///< �J������2D�v���W�F�N�V�����A3D��2D�̍��W�ϊ��A�V�F�[�_��VIEWPIXELSIZE
        lnU32               ShaderCacheSize;
		float				AutoUpdateTimeScale;
        bool                EnableDrawThread;

		Base::Profiler*		Profiler;
    };

public:
    Manager();
	virtual ~Manager();

public:

	/// ������
    void initialize( const ConfigData& configData );

	/// �I������
    void finalize();

	/// �f�t�H���g�̃V�[�����擾����
	SceneGraph* getDefaultSceneGraph() { return mDefaultScene; }
	Pane* getDefaultPane() { return mDefaultPane; }

	/// �f�t�H���g�� Pane ���擾����
	//Pane* getDefaultPane() { return mDefaultPane; }

	// TODO: ���̕Ӄf�t�H���g�V�[���݂̂̍X�V�����ǁAAutoUpdate �p�ӂ��āA�S�X�V�ł����Ǝv��

	/// �������O�X�V
	void updateBeforePhysics();

	/// �f�t�H���g�V�[���̃t���[���X�V
    void updateDefaultScene( const Game::GameTime& gameTime );
	
	/// ��������X�V
	void updateAfterPhysics();

	/// [�`��O�����^�X�N] ������X�V�A�`��R�}���h�\�z�Ȃ�
    void updateDefaultSceneRenderContent();

	/// [�`��^�X�N] �f�t�H���g�̃V�[����`�悷��
    void renderDefaultScene(Graphics::Texture* renderTarget);

	void executeGC();

	/// ���b�Z�[�W���� (Core::Manager ����Ă΂��)
    virtual bool onEvent( const System::EventArgs& e );

	/// �t���[���X�V�̌㏈���B
	/// ���C���X���b�h����A�`��X�L�b�v�̗L���Ɋ֌W�Ȃ��K�����t���[����΂��B
	void postFrameUpdate();

public:
	Graphics::Manager*	getGraphicsManager() { return mGraphicsManager; }
	void setBaseViewSize( const LVector2& size );
	void addImageFilter( ImageFilter* filter );

private:
	System::Manager*			mSystemManager;
    Graphics::Manager*			mGraphicsManager;
    Effect::Manager*			mEffectManager;     ///< �}���`�R�A�œK���̏����̂��߁B���Ԃ�����Ƃ��A������ TaskManager �Ŏ���������
    SceneGraph*					mDefaultScene;
	Pane*						mDefaultPane;
	DrawingLayer*				mDefault3DLayer;
	DrawingLayer*				mDefault2DLayer;
	float						mAutoUpdateTimeScale;

	ImageFilterArray			mImageFilterArray;	///< �ʃX���b�h�ŃC�e���[�g����邱�Ƃ͂Ȃ��͂��Ȃ̂ŕ��ʂ� vector

	Graphics::ProfilerRenderer	mProfilerRenderer;
};

} // namespace Scene
} // namespace Core
} // namespace LNote
