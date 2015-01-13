//==============================================================================
// Light 
//------------------------------------------------------------------------------
///**
//  @file       Light.h
//  @brief      Light
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Geometry/Geometry.h"
#include "../SceneGraph.h"
#include "SceneNode.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// �� Light �N���X
//------------------------------------------------------------------------------
///**
//  @brief      ���C�g�������N���X
//*/
//==============================================================================
class Light
    : public SceneNode//Base::ReferenceObject
{
public:
    Light( SceneGraph* scene_ );
    virtual ~Light();

public:

	/// �쐬
	void create();

	/// ��ނ̎擾
    LNLightType getType() const { return mType; }

	/// ��ނ̐ݒ� (��� initlaize() �Ɉړ�����)
    void setType( LNLightType type_ ) { mType = type_; }

	/// ���C�g�̗L���A�����𔻒肷��
    bool isEnable() const { return mEnable; }

	/// ���C�g�̗L���A������ݒ肷��
    void setEnable( bool f_ ) { mEnable = f_; }

	/// �ʒu�̐ݒ�
	//void setPosition( const LVector3& position_ ) { mPosition = position_; mNeedUpdateMatrices = true; }

	/// �ʒu�̎擾
    const LVector3& getPosition() const { return mPosition; }

	/// �����̐ݒ�
    void setDirection( const LVector3& direction_ ) { mDirection = direction_; mNeedUpdateMatrices = true; }

	/// �����̎擾
    const LVector3& getDirection() const { return mDirection; }

	/// �f�B�t���[�Y�J���[�̐ݒ�
    void setDiffuseColor( const LColorF& color_ ) { mDiffuse = color_; }

	/// �f�B�t���[�Y�J���[�̎擾
    const LColorF& getDiffuseColor() const { return mDiffuse; }

	/// �A���r�G���g�J���[�̐ݒ�
    void setAmbientColor( const LColorF& color_ ) { mAmbient = color_; }

	/// �A���r�G���g�J���[�̎擾
    const LColorF& getAmbientColor() const { return mAmbient; }

	/// �X�y�L�����J���[�̐ݒ�
    void setSpecularColor( const LColorF& color_ ) { mSpecular = color_; }

	/// �X�y�L�����J���[�̎擾
    const LColorF& getSpecularColor() const { return mSpecular; }

	/// ���C�e�B���O�D��x�̐ݒ� (�K�p���郉�C�g�̗񋓎��A�������D�悷��l (�傫�������D��))
    void setLightingPriority( lnS32 priority_ ) { mLightingPriority = priority_; }

	/// ���C�e�B���O�D��x�̎擾
    lnU32 getLightingPriority() const { return mLightingPriority; }

	/// �L�������̐ݒ�
    void setRange( lnFloat range_ ) { mRange = range_; }
    
	/// �L�������̎擾 (���C�g�񋓂ŁA�|�C���g�A�X�|�b�g�̂Ƃ��Ɏ�����ɓ��邩�̔���Ŏg��)
    lnFloat getRange() const { return mRange; }


	/// �X�|�b�g���C�g�̓����R�[���p�x�̐ݒ� (���W�A���P��)
    void setSpotInAngle( lnFloat angle_ ) { mSpotInAngle = angle_; }

	/// �X�|�b�g���C�g�̓����R�[���p�x�̎擾 (���W�A���P��)
    lnFloat getSpotInAngle() const { return mSpotInAngle; }

	/// �X�|�b�g���C�g�̊O���R�[���p�x�̐ݒ� (���W�A���P��)
    void setSpotOutAngle( lnFloat angle_ ) { mSpotOutAngle = angle_; }

	/// �X�|�b�g���C�g�̊O���R�[���p�x�̎擾 (���W�A���P��)
    lnFloat getSpotOutAngle() const { return mSpotOutAngle; }

	/// �V���h�E�̈Â��̐ݒ� (0.0�`1.0)
	//void setShadowStrength( lnFloat strength_ ) { mShadowStrength = strength_; }

	/// �V���h�E�̈Â��̎擾
	//lnFloat getShadowStrength() const { return mShadowStrength; }

	/// �V���h�E�o�b�t�@���쐬����
	//LNRESULT createShadowBuffer( lnU32 width_, lnU32 height_ );

	/// �V���h�E�o�b�t�@�ƂȂ郌���_�[�^�[�Q�b�g�̎擾
	//Graphics::ITexture* getShadowBuffer() const { return mShadowBuffer; }

	/// �V���h�E�o�b�t�@�ƂȂ郌���_�[�^�[�Q�b�g�̐ݒ� (�Q�ƃJ�E���g���ω����܂�)
	//void setShadowBuffer( Core::Graphics::ITexture* tareget_ );

    
	/// �r���[�s��̎擾
    const LMatrix& getViewMatrix() const { _updateMatrices(); return mViewMatrix; }

	/// �v���W�F�N�V�����s��̎擾
    const LMatrix& getProjectionMatrix() const { _updateMatrices(); return mProjectionMatrix; }

	/// �r���[�s��ƃv���W�F�N�V�����s��̐ς̎擾
    const LMatrix& getViewProjectionMatrix() const { _updateMatrices(); return mViewProjMatrix; }

    void updateContext();

    LightContext* getLightContext() { return mLightContext; }

public:

	///// ���݂̏�Ԃ���r���[�s����쐬���� out_ �Ɋi�[����
	//void getViewMatrix( LMatrix* out_, const LVector3& up_ = LVector3( 0, 1, 0 ) );

	///// ���݂̏�Ԃ���v���W�F�N�V�����s����쐬���� out_ �Ɋi�[����
	//void getProjectionMatrix( LMatrix* out_, lnFloat fov_y_, lnFloat near_, lnFloat far_ );

	///// �t���[���X�V (SceneBase::update() ����Ă΂��)
	//void update();


public:

	/// �m�[�h�̎�ނ̎擾
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_LIGHT; }


	/// �f�o�b�O�p�̕`��
    virtual void drawDebug( lnU32 flags_ );

protected:

	/// mNeedUpdateMatrices �� true �̏ꍇ�A�s��v�Z���s��
    void _updateMatrices() const;

protected:

	//SceneGraph*             mScene;

    LNLightType             mType;              ///< ���C�g�̎��
	//LVector3                    mPosition;          ///< �ʒu
    LVector3                mDirection;         ///< ����

    LColorF                  mDiffuse;           ///< �f�B�t���[�Y�J���[
    LColorF                  mAmbient;           ///< �A���r�G���g�J���[
    LColorF                  mSpecular;          ///< �X�y�L�����J���[

    lnS32                   mLightingPriority;

    lnFloat                 mRange;             ///< �L������
    lnFloat                 mSpotInAngle;       ///< �����̃R�[���p�x (���W�A���P��)
    lnFloat                 mSpotOutAngle;      ///< �O���̃R�[���p�x (���W�A���P��)

	//lnFloat                 mShadowStrength;    ///< �V���h�E�̈Â� (0.0�`1.0)
	//Graphics::ITexture*     mShadowBuffer;

    mutable LMatrix         mViewMatrix;        ///< �r���[�s��
    mutable LMatrix         mProjectionMatrix;  ///< �v���W�F�N�V�����s��
    mutable LMatrix         mViewProjMatrix;    ///< �r���[�s��ƃv���W�F�N�V�����s��̐�

    mutable LViewFrustum    mViewFrustum;       ///< �V���h�E�o�b�t�@���g���Ƃ��̎�����
    
    LightContext*           mLightContext;

    bool                    mEnable;            ///< ���C�g���L�����ǂ����̃t���O
    mutable bool            mNeedUpdateMatrices;


    /*
    Intensity
	    ���̖��邳�B�X�|�b�g/�|�C���g�ł� 1�A�f�B���N�V���i���ł� 0.5 �f�t�H�B(unity)
    */

    friend class LightContext;
};

//==============================================================================
// �� LightContext
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class LightContext
	: public MMELightBase
{
public:

    static LightContext DUMMY;

public:

    LightContext();

public:

    void initialize( Light* light_ );

    void updateContext();

public:

    bool isEnable() const { return mEnable; }

    LNLightType getType() const { return mType; }

    

    lnS32 getLightingPriority() const { return mLightingPriority; }

    lnFloat getRange() const { return mRange; }

    

	// �\�[�g�p�̈ꎞ�f�[�^
    lnFloat DistanceForSort;

public:
	virtual const LVector3& getPosition() const { return mPosition; }
    virtual const LVector3& getDirection() const { return mDirection; }

protected:
	virtual const LMatrix& getViewMatrix() const { return mViewMatrix; }
    virtual const LMatrix& getProjectionMatrix() const { return mProjectionMatrix; }
    virtual const LMatrix& getViewProjectionMatrix() const { return mViewProjMatrix; }

    virtual const LColorF&   getDiffuseColor() const { return mDiffuse; }
    virtual const LColorF&   getAmbientColor() const { return mAmbient; }
    virtual const LColorF&   getSpecularColor() const { return mSpecular; }

private:

    Light*          mLight;
    LNLightType     mType;
    LVector3        mPosition;
    LVector3        mDirection;
    LColorF          mDiffuse;
    LColorF          mAmbient;
    LColorF          mSpecular;
    lnS32           mLightingPriority;
    lnFloat         mRange;
    lnFloat         mSpotInAngle;
    lnFloat         mSpotOutAngle;
    LMatrix         mViewMatrix;
    LMatrix         mProjectionMatrix;
    LMatrix         mViewProjMatrix;
    bool            mEnable;
};

} // namespace Scene
} // namespace Core
} // namespace LNote
