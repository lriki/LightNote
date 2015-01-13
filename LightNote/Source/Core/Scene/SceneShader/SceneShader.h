//==============================================================================
// SceneShader 
//------------------------------------------------------------------------------
///**
//  @file       SceneShader.h
//  @brief      SceneShader
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Graphics/Interface.h"
#include "../../Graphics/DeviceObjects.h"
#include "../Common.h"
#include "MMETypes.h"
#include "SceneShaderManager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{
class SceneShaderTechnique;
class NodeRenderParam;
class DrawCommandContainer;

//==============================================================================
// SceneShader
//------------------------------------------------------------------------------
///**
//  @brief      �V�[���p�V�F�[�_�̃N���X
//*/
//==============================================================================
class SceneShader
    : public Base::ReferenceObject
	, public Base::ICacheObject
{
	LN_CACHE_OBJECT_DECL;
	LN_TYPE_INFO_ACCESS_DECL;
public:

    struct InitData
    {
        const void*     ShaderData;
        lnU32           ShaderDataSize;
        LRefTString     ShaderName;

    };

public:
    SceneShader( SceneShaderManager* manager );
    virtual ~SceneShader();

public:

	/// ������
	//LNRESULT initialize( const InitData& init_data_ );

	/// �쐬
	void create(const lnChar* filePath, lnString* errors);

	/// �쐬 TODO: ���[�e�B���e�B�֐���
	void create( int dataResourceID );

	/// �ēǂݍ���
    LNRESULT reload();

	/// ���\�[�X�̍č\�z
    LNRESULT refreshResource( const LVector2& view_size_ );

	/// Graphics::IShader �̎擾
    Graphics::IShader* getCoreShader() { return mShader; }

	/// �G���[������̐ݒ� (SceneShaderTechnique �N���X������Ă΂��)
    void setErrorMessage( LRefTString str_ ) { mErrorMessage = str_; }

	/// �G�t�F�N�g�t�@�C���̎g�p�ړI�̎擾
    MMEScriptClass getScriptClass() const { return mScriptClass; }

	/// �G�t�F�N�g�t�@�C���̎��s�^�C�~���O�̎擾
    MMEScriptOrder getScriptOrder() const { return mScriptOrder; }

	/// �|�X�g�G�t�F�N�g���A�|�X�g�G�t�F�N�g�̏ꍇ true
    bool isPostEffect() const { return ( mScriptOrder == MME_SCRORDER_preprocess || mScriptOrder == MME_SCRORDER_postprocess ); }

	/// ScriptExternal �����G�t�F�N�g
    bool hasScriptExternal();



public: // �� �����p

	/// SceneShaderManager �擾
    SceneShaderManager* getSceneShaderManager() { return mManager; }

	const FileIO::PathNameW& getFilePath() const { return mFilePath; }

	/// �ݒ肷��K�v�����郉�C�g�̐��̎擾
    lnU32 getRequiredLightNum() const { return mRequiredLightNum; }

	/// �ϐ����ɑΉ����� MMEShaderVariable ���������� (MMETechnique�Ŏg���B���̃|�C���^�̓V�F�[�_�����邤���͕s�ςȂ̂ŁAMMETechnique �Ŏg���Ȃ� initialize() �Ŏ擾���Ă����Ă�OK)
    MMEShaderVariable* findShaderVariable( const lnChar* name_ );

    

	/// Scene �P�ʂŕK�v�ȃp�����[�^��ݒ肷��
    void updateSceneParam( const MMESceneParam& param_, MMESceneBase* scene );

	/// Camera �P�ʂŕK�v�ȃp�����[�^��ݒ肷��
    void updateCameraParam( const LNCameraSceneParam& param_ );

	/// Node �P�ʂŕK�v�ȃp�����[�^��ݒ肷�� (�m�[�h���Ƃɉe�����郉�C�g�͈قȂ邽�߁A���C�g�����̒�)
    LNRESULT updateNodeParam( const MMESceneObjectBase* param_ );// const NodeRenderParam& param_ );

	/// Subset �P�ʂŕK�v�ȃp�����[�^��ݒ肷��
    void updateSubsetParam( const LNSubsetRenderParam& param_ );

	/// "OffscreenOwner" ��ݒ肵�āA�ΏۂƂȂ� CONTROLOBJECT ���X�V����
    void updateOffscreenOwner( const MMESceneObjectBase* obj );

	/// CONTROLOBJECT ��v�����Ă���ϐ��̒l�����ׂčX�V����
	//void updateControlObjectVariables();

	/// �I�t�X�N���[��RT DefaultEffect �� "self" �Ŏ��ʂ����m�[�h�̎擾 (onSettingShader() �Őݒ�)
    MMESceneObjectBase* getSelfSceneNode() { return mSelfSceneNode; }

	/// ���L�\�ȃV�F�[�_�̏ꍇ true
    bool isShareable() const { return mShareable; }

	/// SceneNode::setShader() �ł̐擪�ŌĂ΂��
	//void onSettingShader( MMESceneObjectBase* node_ );


	/// ���o��
    void dumpInfo( FILE* stream_ = NULL );


	/// �������Ɉ�v����e�N�j�b�N���擾���� (������Ȃ���� NULL��Ԃ�)
	SceneShaderTechniqueArray* findTechnique( MMDPass pass, bool UseTexture, bool UseSphereMap, bool UseToon );

private:

	/// �p�����[�^�Ȃǂ����ۂɐݒ肵�Ă���
    LNRESULT _build();

	/// ���
    void _release();

	/// ���\�[�X���
    void _releaseResource();

	/// �V�F�[�_�ϐ��̃Z�}���e�B�N�X�𒲂ׂđΉ�����v�����ڂ�Ԃ�
    MMEVariableRequest _checkVariableRequest( Graphics::IShaderVariable* var_, MMEShaderVariable* sv_ );

private:

	typedef std::vector<SceneShaderTechniqueArray>          TechniqueGroupArray;
    typedef std::vector< OffscreenScene* >          OffscreenSceneList;

private:

    SceneShaderManager*     mManager;
    Graphics::IShader*      mShader;                ///< �V�F�[�_�v���O�����{��
    //wchar_t					mFileFullPath[LN_MAX_PATH];
	FileIO::PathNameW		mFilePath;

    MMEScriptOutput         mScriptOutput;          ///< STANDARDSGLOBAL �� ScriptOutput (��� "color")
    MMEScriptClass          mScriptClass;           ///< �G�t�F�N�g�t�@�C���̎g�p�ړI (�f�t�H���g�� "object")
    MMEScriptOrder          mScriptOrder;           ///< �G�t�F�N�g�t�@�C���̎��s�^�C�~���O (�f�t�H���g�� "standard")

    MMEShaderVariableArray  mShaderVariableArray;   ///< �V�F�[�_�v���O�������̕ϐ��ꗗ
    MMEShaderVariableMap    mShaderVariableMap;     ///< �ϐ�����MMEShaderVariable�̑Ή��\ (��Ƀe�N�j�b�N�̃X�N���v�g�Ŏg��)
    MMEShaderVariableArray  mControlObjectVarArray; ///< CONTROLOBJECT ��v�����Ă���ϐ����X�g
    SceneShaderTechniqueArray   mTechniqueList;         ///< �e�N�j�b�N�̈ꗗ
	TechniqueGroupArray		mTechniqueGroupArray;
    OffscreenSceneList      mOffscreenSceneList;

    lnU32                   mRequiredLightNum;
    lnU32                   mWorldMatrixCalcMask;   ///< MMEWorldMatrixCalcFlags �̑g�ݍ��킹 (������������g��Ȃ�����)

    MMESceneObjectBase*              mSelfSceneNode;

    LRefTString             mErrorMessage;
    bool                    mIsError;
    bool                    mShareable;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================