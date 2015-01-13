//==============================================================================
// OffscreenScene 
//------------------------------------------------------------------------------
///**
//  @file       OffscreenScene.h
//  @brief      OffscreenScene
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Common.h"
#include "../SceneNode/NodeRenderParam.h"

namespace LNote
{
namespace Core
{
namespace Scene
{
class SceneGraph;
class SceneShader;

//==============================================================================
// �� OffscreenScene
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class OffscreenScene
    : public Base::ReferenceObject
{
public:
	OffscreenScene( SceneShaderManager* manager, SceneShader* shader );
    virtual ~OffscreenScene();

public:
   
	/// ������
    void initialize( 
		const LVector4 &	clearColor,
        lnFloat             clearDepth,
        lnRefString         description,
        lnRefString         defaultEffectStrings,
        Graphics::Texture*	renderTarget );

	/// �����_�����O�^�[�Q�b�g�N���A����F�̐ݒ�
    void setClearColor( const LVector4& color_ );

	/// �����_�����O�^�[�Q�b�g�N���A����F�̎擾
    const LColorF& getClearColor() const { return mClearColor; }

	/// �[�x�^�[�Q�b�g�N���A����Z�l�̐ݒ�
    void setClearDepth( lnFloat depth_ ) { mClearDepth = depth_; }

	/// �[�x�^�[�Q�b�g�N���A����Z�l�̎擾
    lnFloat getClearDepth() const { return mClearDepth; }

	/// �G�t�F�N�g�t�@�C���̐������̐ݒ�
    void setDescription( LRefTString desc_ ) { mDescription = desc_; }

	/// �G�t�F�N�g�t�@�C���̐������̎擾
    LRefTString getDescription() const { return mDescription; }

	/// �G�t�F�N�g�t�@�C���̊��蓖�ĕ��@�̐ݒ� (DefaultEffect �A�m�e�[�V����)
    void setDefaultEffectStrings( LRefTString effects_ ) { mDefaultEffectStrings = effects_; }

	/// �G�t�F�N�g�t�@�C���̊��蓖�ĕ��@�̎擾
	//DefaultEffectStrings getDefaultEffectStrings() { return mDefaultEffectStrings; }
	
	LNPriorityParameter* tryGetPriorityParameter( MMESceneObjectBase* obj );

public:
	void setOffscreenSceneID( int id ) { mOffscreenSceneID = id; }
	int getOffscreenSceneID() const { return mOffscreenSceneID; }

	/// �`��
	void render( MMESceneBase* scene, ContextDrawParam& param, MMESceneObjectBase* renderRootNode );

private:

	/// �G�t�F�N�g�t�@�C���̊��蓖�čX�V
	//      �K�v�ȃV�F�[�_���܂� Manager �ɓo�^����Ă��Ȃ��ꍇ�͐V�����쐬����B
	//      ���ꂾ���Ȃ珉�����̂Ƃ��ɂ���΂����Ǝv�������ǁA
	//      ���̃N���X�� initialzie() �̓V�F�[�_�̏���������Ă΂��B
	//      �Ƃ������Ƃ́AManager::createShader() ���ċA�I�ɌĂ΂�邱�ƂɂȂ��Ă��܂��A
	//      �Ȃ񂩕s���B(���ہAcreateShader() �ł� FileManager �̔r���������s���Ă�)
	//      ����Ȃ킯�ł�����Ɩʓ|���������ǃV�F�[�_�����I�������A
	//      �������㏈�����Ă����`�Ō����E�쐬�����Ă����B
	//
	//      [12/8/12] initialize() �� SceneShader::refreshResource() ����Ăяo���悤�ɂ����̂ŁA
	//      �ʓr������ĂԕK�v�͍��̂Ƃ���Ȃ��B
    void _refreshEffects();

	// �I�t�X�N���[�� RT ���ō쐬�����V�F�[�_���������
    void _releaseEffects();

private:

    enum EntryType
    {
        ENTRY_EFFECT = 0,   ///< �G�t�F�N�g�t�@�C��
        ENTRY_NONE,         ///< �G�t�F�N�g�Ȃ�
        ENTRY_HIDE          ///< ��\��
    };

    struct DefaultEffectEntry
    {
        //EntryType       Type;
        LRefTString     ObjectName;     ///< (���C���h�J�[�h�w��\ * ?)
        LRefTString     EffectName;     ///< (���C���h�J�[�h�w��s��)
        //SceneShader*    Shader;         ///< EffectName �ō쐬�����V�F�[�_
		LNPriorityParameter	PriorityParameter;
    };

	//struct SceneNodeEntry
	//{
	//    MMESceneObjectBase*		Node;				///< �`�悷��m�[�h
	//    LNPriorityParameter		PriorityParameter;  ///< �`��R�}���h�ɓn��
	//};

private:

    typedef std::vector< DefaultEffectEntry >   DefaultEffectEntryList;
	//typedef std::vector< SceneNodeEntry >       SceneNodeEntryList;
private:

	SceneShaderManager*     mManager;
    SceneShader*            mSceneShader;
    LColorF					mClearColor;
    lnFloat                 mClearDepth;
    lnRefString             mDescription;
    lnString				mDefaultEffectStrings;
    Graphics::Texture*		mRTTexture;
	Graphics::Texture*		mDepthTexture;
	int						mOffscreenSceneID;
	MMESceneObjectBase*		mOffscreenOwner;				

	//SceneNodeEntryList      mSceneNodeEntryArray;			///< ���̃I�t�X�N���[��RT�ŕ`�悷��m�[�h�̈ꗗ
	DefaultEffectEntryList  mDefaultEffectEntryList;
    LRefTString             mErrorMessage;

	bool                    mIsThereSelf;					///< string DefaultEffect = �` �� "self=�`" �������ǂ���
    bool                    mNeedRefreshAffectAllNodes;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================