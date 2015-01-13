//==============================================================================
// NodeRenderParam 
//------------------------------------------------------------------------------
///**
//  @file       NodeRenderParam.h
//  @brief      NodeRenderParam
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Common.h"
#include "../SceneShader/MMETypes.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{
class SceneShader;
class SceneNode;
class Camera;

/// �D��p�����[�^�܂Ƃ�
struct LNPriorityParameter
{
    SceneShader*    Shader;     ///< NULL �̏ꍇ�AgetShader() �� SceneNode::mShader ���g����
    bool            Hide;       ///< true �̏ꍇ�AisVisible() ����� false�Bfalse �� SceneNode::mVisible �ŉ������܂�

	/// �����l
    LNPriorityParameter()
        : Shader    ( NULL )
        , Hide      ( false )
    {}

    LNPriorityParameter( bool hide_ )
        : Shader    ( NULL )
        , Hide      ( hide_ )
    {}

    static LNPriorityParameter HIDE_PARAM;

	// Hide �́A�I�t�X�N���[��RT ����̉��w���A������ɓ����Ă��邩�ǂ����Őݒ肷��B
};

//==============================================================================
// �� NodeRenderParam �N���X
//------------------------------------------------------------------------------
///**
//  @brief		�V�F�[�_�ɑ���f�[�^���܂Ƃ߂�����            
//*/
//------------------------------------------------------------------------------
/*
    ����܂ł̓I�u�W�F�N�g�P�ʂƃT�u�Z�b�g�P�ʂō\���̂�
    �����Ă����ǁA�ЂƂɂ܂Ƃ߂������킩��₷���Ǝv�����̂ŗp�ӁB
    �܂��A�}���`�X���b�h�Ή��ׂ̈̃R�s�[�f�[�^�����˂�B

    �`��X���b�h�L���̏ꍇ�A�e�����o�̃������̈�� new �ō쐬�����B
    ���̏ꍇ�Aset�` �Őݒ肳�ꂽ�l�͑S�Ă��̗̈�ɃR�s�[����A
    get�` �͂��̃������̃|�C���^��Ԃ��B

    �`��X���b�h�����̏ꍇ�Aget�`�� set�` �Őݒ肳�ꂽ�|�C���^��
    ���̂܂ܕԂ��B


    �� DirectX �̃T���v���Ƃ��͌����ς݃{�[���s��z��ɂ�
    ���[���h�ϊ��ς݂̂��̂�n���Ă�B
    �����A�~���[�̃V�F�[�_�Ƃ��̓I�u�W�F�N�g�̃��[���h�s���
    ���H���ēK����������ȒP�ɂȂ肻�������A�Ȃɂ�� MME �Ή���
    �V�F�[�_�v���O�����ɑ傫�ȕύX�������Ȃ��Ă����������g����
    �悤�ɂȂ�Ǝv���B
    ����Ȃ킯�ŁA�R�R�ł̓I�u�W�F�N�g�̃��[���h�s��ƁA
    ���[�J���ł̃{�[���̌����s��𕪂��Ċi�[���A���ꂼ��V�F�[�_�ɓn���B 

    [12/8/5]
    �Ȃ񂩌����̃}�j���A���ƈႤ���ǁA�X�L�j���O�p�̃V�F�[�_�Ɋւ��Ă�
    ���_�V�F�[�_�Ńe�N�X�`���̃t�F�b�`���ł���݂����Ȃ̂�
    ����𗘗p��������őg��ł݂�B
*/
//==============================================================================
class NodeRenderParam
{
public:
    NodeRenderParam();

    ~NodeRenderParam();

public: // ���J�����o�ϐ�

    LNSubsetRenderParam*    Subset;
    Graphics::LRenderState            RenderState;
    
public:

	/// ������
    LNRESULT initialize( SceneNode* node_, lnU32 subset_num_ );
	
	int getSubsetCount() const { return mSubsetNum; };

	void setSubsetCount( int count );

	/// ���̃p�����[�^�����m�[�h�̎擾
    SceneNode* getSceneNode() { return mSceneNode; }

	/// �S�̂̉����� (OffscreenRT ����̗D��p�����[�^�l���ς݁B�`�掞�� Node �� getShader() �ł͂Ȃ���������g��)
	//SceneShader* getShader() { return ( mPriorityParameter.Shader ) ? mPriorityParameter.Shader : mSceneShader; }

	/// �S�̂̉����� (OffscreenRT ����̗D��p�����[�^�l���ς݁B�`�掞�� Node �� isVisible() �ł͂Ȃ���������g��)
    bool isVisible() const { return ( mPriorityParameter.Hide ) ? false : mIsVisible; }

	/// ���ۂɕ`�掞�Ɏg�����̍X�V (RenderState �������B parent_params_ �� NULL �̏ꍇ�͐e����󂯌p���Ȃ�)
    void updateSubsetRenderParam( const NodeRenderParam* parent_params_ );

	/// ���ۂɕ`�掞�Ɏg�����̎擾
    const LNSubsetRenderParam& getCombinedSubsetRenderParam( lnU32 idx_ ) const { return mCombinedSubsetRenderParams[ idx_ ]; }

	/// ���ۂɕ`�掞�Ɏg�������_�[�X�e�[�g�̎擾
    const Graphics::LRenderState& getCommittedRenderState() const { return mCommittedRenderState; }

	/// �`��Ɏg���s���񓙂̐ݒ�
    void setAffectElements( const LMatrix* world_, const Camera* camera_, const LightArray* lights_, SceneShader* shader_, bool visible_ );

	/// �D��p�����[�^�̐ݒ�
	//void setPriorityParameter( const LNPriorityParameter& param_ ) { mPriorityParameter = param_; }

	/// �V�F�[�_�ϐ��̗v���ɑΉ�����s��(WORLD ���܂܂�Ă������)��Ԃ� (�����Ȃ��̂ɂ͒P�ʍs���Ԃ�)
	//      light_idx_ : ���C�g���֌W����s�񂪗v�����ꂽ�Ƃ��A���̃C���f�b�N�X (-1 �ŏ�Ƀf�t�H���g���C�g)
    const LMatrix* getObjectMatrix( MMEVariableRequest req_, int light_idx_ ) const;

	/// �e�����C�g�̔z����擾����
    const LightArray& getAffectLightArray() const { return *mAffectLightArray; }



private:
    
    SceneNode*              mSceneNode;     ///< ���̃N���X�Ɗ֘A�t�����Ă���m�[�h
    lnU32                   mSubsetNum;
    SceneShader*            mSceneShader;
    LNSubsetRenderParam*    mCombinedSubsetRenderParams;
	Graphics::LRenderState            mCommittedRenderState;
    LNPriorityParameter     mPriorityParameter;

    const LMatrix*          mAffectWorldMatrix;
    const Camera*           mAffectCamera;
    const LightArray*       mAffectLightArray;
    
    mutable LMatrix         mTempMatrix;
    bool                    mIsVisible;

    
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core

} // namespace LNote

//==============================================================================
//
//==============================================================================