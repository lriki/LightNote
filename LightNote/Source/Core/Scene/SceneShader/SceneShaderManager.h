//==============================================================================
// SceneShaderManager 
//------------------------------------------------------------------------------
///**
//  @file       SceneShaderManager.h
//  @brief      SceneShaderManager
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <algorithm>
#include "../../Base/Misc.h"
#include "../../Base/Cache.h"
#include "../../Base/ReferenceObjectArray.h"
#include "../../System/Interface.h"
#include "../../FileIO/Manager.h"
#include "../../Graphics/Interface.h"
#include "../../Graphics/DeviceObjects.h"
#include "../../Graphics/Manager.h"
#include "../../Game/GameTime.h"
#include "../Common.h"
#include "BaseClasses.h"
#include "MMETypes.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{
class SceneGraph;
class SceneShader;
class OffscreenScene;
class DrawCommandContainer;
//typedef Base::CacheObjectManager< SceneShader >    SceneShaderCache;

//==============================================================================
// �� SceneShaderManager �N���X
//------------------------------------------------------------------------------
///**
//  @brief      �V�[���p�V�F�[�_�̃N���X
//*/
//==============================================================================
class SceneShaderManager
    : public Base::CacheManager
{
public:

	/// �������f�[�^
    struct ConfigData
    {
        FileIO::Manager*		FileManager;
        Graphics::Manager*  GraphicsManager;
        lnU32               ShaderCacheSize;
        bool                UseSemantics;       ///< true �̏ꍇ�A�V�F�[�_�ϐ��̌����ɃZ�}���e�B�N�X�����g�� (false �̏ꍇ�͖��O)
    };

	

public:
    SceneShaderManager(/* MMESceneBase* scene */);
    virtual ~SceneShaderManager();

public:

	/// ������
    void initialize( const ConfigData& configData );

	/// �I������
    void finalize();

	/// �e�N�X�`���������ɏ̂���r���[�T�C�Y
	void setBaseViewSize( const LVector2& size ) { mBaseViewSize = size; }

	/// �V�F�[�_�̍쐬
	//LNRESULT createSceneShader( SceneShader** shader_, const void* data_, lnU32 size_, const lnChar* name_ );

	/// �V�F�[�_�̍쐬 (�t�@�C�����w��E�t�@�C�������L�[�Ƃ��Ď����o�^)
    SceneShader* createSceneShader( const lnChar* fulePath, lnString* errors );

	/// �V�F�[�_�̍쐬
	SceneShader* createSceneShader( int dataResourceID, const lnSharingKey& key );

	/// ������K�v�ȃV�F�[�_���Ȃ����`�F�b�N����
    void updateShaderList();

	/// �S�V�F�[�_�ɃV�[���P�ʂŐݒ肷��ׂ��f�[�^��ݒ肷��
	void updateAllShaderGlobalParam(double totalTime, float elapsedTime,/*const Game::GameTime& game_time_,*/ MMESceneBase* scene);

	/// �S�F�[�_�ɃJ�����P�ʂŐݒ肷��ׂ��f�[�^��ݒ肷��
    void updateAllShaderCameraParam( const LNCameraSceneParam& param_ );
  
	///// (SceneGraph::addNode() ����Ă΂��)
	//void addSceneObjectToOffscreenScene( MMESceneObjectBase* obj );

	///// (SceneGraph::onDeleteNode() ����Ă΂��)
	//void removeSceneObjectFromOffscreenScene( MMESceneObjectBase* obj );

	/// �S OffscreenScene �̕`��
	void renderAllOffscreenScene( MMESceneBase* scene, ContextDrawParam& param, MMESceneObjectBase* renderRootNode );

public:

	/// ���b�Z�[�W���� (SceneGraph ����Ă΂��)
    bool onEvent( const System::EventArgs& e );

	//MMESceneBase* getScene() { return mScene; }
	const LVector2& getBaseViewSize() const { return mBaseViewSize; }	// �e�N�X�`���������Ɏg�p����

	/// �Z�}���e�B�N�X�̕������ID�̑Ή��\�̎擾
    const MMESemanticsMap& getSemanticsMap() { return mMMESemanticsMap; }

	/// �A�m�e�[�V�����̕������ID�̑Ή��\
    const MMEAnnotationMap& getAnnotationMap() { return mMMEAnnotationMap; }

    LNRESULT createTexture(
        Graphics::Texture** texture_,
        MMEVariableRequest req_,
        //const lnChar* resource_name_,
		FileIO::PathNameW resourcePath,
        lnU32 width_,
        lnU32 height_,
        lnU32 depth_,
        lnU32 miplevels_,
        Graphics::SurfaceFormat format_ );

	/// GraphicsManager �̎擾
    Graphics::Manager* getGraphicsManager() { return mGraphicsManager; }

	/// �O���t�B�b�N�X�f�o�C�X�̎擾
    Graphics::IGraphicsDevice* getGraphicsDevice() { return mGraphicsDevice; }
    FileIO::Manager* getFileManager() const { return mFileManager; }

    

	/// SceneShader::updateNodeParam() �ōs��E�x�N�g���z����Z�b�g����Ƃ��Ɏg���̂Ō��J
    Base::TempBuffer* getTempBuffer() { return &mTempBuffer; }

 
	void addMMETextureObject( MMETextureObject* tex );
	void removeMMETextureObject( MMETextureObject* tex );
	void addOffscreenScene( OffscreenScene* ofs );
	void removeOffscreenScene( OffscreenScene* ofs );
private:

	/// �V�F�[�_�̍쐬
	LNRESULT _createSceneShader(SceneShader** shader_, const void* data_, lnU32 size_, const lnChar* name_, lnString* errors);

	/// �V�F�[�_�̍쐬 (���̓X�g���[������)
	SceneShader* _createSceneShader(FileIO::Stream* stream, const lnChar* key, lnString* errors);

private:

    typedef std::vector< SceneShader* >     SceneShaderList;
	//typedef Base::ReferenceObjectArray< SceneShader* >		SceneShaderArray;
    typedef std::vector< OffscreenScene* >  OffscreenSceneArray;
	//typedef Base::ReferenceObjectArray< OffscreenScene* >		OffscreenSceneArray;

private:

	//MMESceneBase*				mScene;
    FileIO::Manager*				mFileManager;
    Graphics::Manager*          mGraphicsManager;
    Graphics::IGraphicsDevice*  mGraphicsDevice;
	LVector2					mBaseViewSize;

    MMESemanticsMap             mMMESemanticsMap;   ///< �Z�}���e�B�N�X�̕������ID�̑Ή��\ (�Z�}���e�B�N�X���g��Ȃ��ꍇ�͕ϐ���) 
    MMEAnnotationMap            mMMEAnnotationMap;  ///< �A�m�e�[�V�����̕������ID�̑Ή��\

	//SceneShaderArray			mSceneShaderArray;			///< ����� ~SceneShader �� ~OffscreenScene �� ~SceneShader �̂悤�ɍċA�I�ɌĂ΂�邽�߁A���ʂ� vector �ł� Cache ���Ńf�b�h���b�N����
	SceneShaderList				mSceneShaderList;
	MMETextureObjectArray		mMMETextureObjectArray;		///< �����E�폜�Ȃǂŕ`��X���b�h����Q�Ƃ��邱�Ƃ͂Ȃ����߁A���ʂ� vector �Ŗ��Ȃ�
    OffscreenSceneArray          mOffscreenSceneList;

    double                      mPlayStartTime;
    MMESceneParam               mMMESceneParam;     ///< Scene �P�ʂ̃V�F�[�_�ݒ�p�����[�^ (updateAllShaderGlobalParam() �Őݒ�)
   
    Base::TempBuffer            mTempBuffer;        ///< 
	std::stack<int>				mOffscreenSceneIDStack;	///< (0�`MaxOffscreenScenes - 1)
	
	Threading::Mutex			mShaderListMutex;
	Threading::Mutex			mObjectListMutex;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================