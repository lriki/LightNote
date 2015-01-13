//==============================================================================
// Camera 
//------------------------------------------------------------------------------
///**
//  @file       Camera.h
//  @brief      Camera
//*/
//==============================================================================

#pragma once

#include "../../Graphics/Common/Camera.h"
#include "../../Graphics/Common/ScreenEffect.h"
#include "../Common.h"
#include "SceneNode.h"

#define LNOTE_SCENE_CAMERA_SCREEN_EFFECT	0

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// �� CameraContext
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class CameraContext
{
public:

    CameraContext();
    virtual ~CameraContext();
    
public:

    void initialize( Camera* camera_ );

    virtual void updateContext();

	void render( ContextDrawParam* param );
	
public:
	SceneNode*			getRenderRootNode() { return mRenderRootNode; }
	//Graphics::ITexture*	getRenderTarget() { return mRenderTarget; }

	const LVector2& getViewSize() const { return mViewSize; }
	const LMatrix&	getViewProjectionMatrix() const { return mViewProjMatrix; }
	bool			isProjection2D() const { return mProjection2D; }
	const LMatrix&	getProjectionMatrix() const { return mProjectionMatrix; }
	const LMatrix&	getViewMatrixInverse() const { return mCameraSceneParam.Mat_CameraView_Inverse; }
	lnFloat			getFovY() const { return mFovY; }
	lnFloat			getNearClip() const { return mNearClip; }
	lnFloat			getFarClip() const { return mFarClip; }
	const LMatrix&	getViewMatrix() const { return mViewMatrix; }
	const LNCameraSceneParam& getCameraSceneParam() const { return mCameraSceneParam; }









	const LVector4& getPosition() const { return mPosition; }

    const LVector3& getDirection() const { return mDirection; }

	

    

	// (�r���{�[�h�s��̌v�Z�Ŏg�p�BsetProjectionMatrix() ��łȂ��Ǝg���Ȃ��̂Œ���)
        

    

    LNZSortFunc getZSortType() const { return mZSortFunc; }


	/// SceneGraphContext::OnSetActivePaneContext() ����ݒ肳���
    void setProjectionMatrix( const LMatrix& proj_ );

    

	/// WORLDVIEWPROJECTION �Z�}���e�B�N�X���̂��߂Ɏg��
    

private:

    Camera*             mCamera;
	SceneNode*			mRenderRootNode;
	//Graphics::ITexture*	mRenderTarget;

    lnFloat             mFovY;
    lnFloat             mNearClip;
    lnFloat             mFarClip;
	LVector2			mViewSize;
	LMatrix             mViewMatrix;
    LMatrix             mProjectionMatrix;
	LMatrix             mViewProjMatrix;
	bool                mProjection2D;





    LVector4	        mPosition;
    LVector3            mDirection;

	// fovY ���́APane �ɂ��v���W�F�N�V�����s��쐬�̂��߂ɕK�v
    
    
       ///< �r���[�s��ƃv���W�F�N�V�����s��̐�

    LNZSortFunc         mZSortFunc;
    LNCameraSceneParam  mCameraSceneParam;  ///< �J�����P�ʂŃV�F�[�_�ɑ���f�[�^

#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
    Graphics::ScreenEffect* mScreenEffect;
    LColorF				    mBackgroundColor;
#endif
    bool					mOrthographic;

	
};


//==============================================================================
// �� Camera
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class Camera
    : public Base::ReferenceObject//SceneNode//
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
    Camera( SceneGraph* scene );
    virtual ~Camera();

public:

	// ������
    void initialize( bool projection_2d_ );

	/// �J�����^�C�v�̐ݒ�
	void setCameraType( Graphics::CameraType type ) { mCoreCamera.setCameraType( type ); }

	/// �ʒu�̐ݒ�
    void setPosition( const LVector3& pos ) { mCoreCamera.setPosition( pos ); }

	/// �ʒu�̎擾
	const LVector3& getPosition() const { return mCoreCamera.getPosition(); }

	/// �����_�̐ݒ�
	void setLookAt( const LVector3& at ) { mCoreCamera.setLookAt( at ); }

	/// �����_�̎擾
	const LVector3& getLookAt() const { return mCoreCamera.getPosition(); }

	/// ������x�N�g���̐ݒ�
	void setUpDirection( const LVector3& up ) { mCoreCamera.setUpDirection( up ); }

	/// ������x�N�g���̎擾
	const LVector3& getUpDirection() const { return mCoreCamera.getUpDirection(); }

	/// �����̎擾 (���K���ς�)
    const LVector3& getDirection() const { return mCoreCamera.getDirection(); }

	void setAngle( const LVector3& angle ) { mCoreCamera.setAngle( angle ); }

	void setMatrix( const LMatrix& matrix ) { mCoreCamera.setMatrix( matrix ); }

	/// Y ��������p�̐ݒ�
	void setFovY( lnFloat fov_y ) { mCoreCamera.setFovY( fov_y ); }

	/// Y ��������p�̎擾
	lnFloat getFovY() const { return mCoreCamera.getFovY(); }

	/// �ł��߂��r���[�v���[���ʒu�̐ݒ�
	void setNearClip( lnFloat near_clip ) { mCoreCamera.setNearClip( near_clip ); }

	/// �ł��߂��r���[�v���[���ʒu�̎擾
	lnFloat getNearClip() const { return mCoreCamera.getNearClip(); }

	/// �ł������r���[�v���[���ʒu�̐ݒ�
	void setFarClip( lnFloat far_clip ) { mCoreCamera.setFarClip( far_clip ); }

	/// �ł������r���[�v���[���ʒu�̎擾
	lnFloat getFarClip() const { return mCoreCamera.getFarClip(); }

	/// �r���[�s��̎擾
    const LMatrix& getViewMatrix() const { return mCoreCamera.getViewMatrix(); }

	/// �v���W�F�N�V�����s��̎擾
	const LMatrix& getProjectionMatrix() const { return mCoreCamera.getProjectionMatrix(); }

	/// �r���[�s��ƃv���W�F�N�V�����s��̐ς̎擾
	const LMatrix& getViewProjectionMatrix() const { return mCoreCamera.getViewProjectionMatrix(); }

	/// �r���[�s��̋t�s��̎擾
	const LMatrix& getViewMatrixInverse() const { return mCoreCamera.getViewMatrixInverse(); }

	/// ������̎擾
    const LViewFrustum& getViewFrustum() const { return mCoreCamera.getViewFrustum(); }

#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
public:

	/// �X�N���[��������ʂ̎g�p�ݒ�
    void setScreenEffectMode( LNScreenEffectMode mode );

	/// �F���̐ݒ�
    void setTone( const LTone& tone, double duration ) { mScreenEffect->setTone( tone, duration ); }

	/// �g�����W�V�����̏����Ƃ��Č��݂̉�ʂ��Œ肷��
    void freeze() { mScreenEffect->freeze(); }

	/// �g�����W�V�������s
    void transition( double duration, Graphics::ITexture* mask, int vague = 40 ) { mScreenEffect->transition( duration, mask, vague ); }

	//----------------------------------------------------------------------
	///**
	//  @brief      �u���[���s
	//
	//  @param[in]  duration_     : �K�p���鎞�� (�b)
	//  @param[in]  power_        : �u���[�̋��� (�u���[�C���[�W��`�悷�鎞�̃A���t�@�l�B0.0�`1.0 �Ŏw��)
	//  @param[in]  scale_        : �u���[�̑傫�� (����)
	//  @param[in]  center_       : �u���[�̒��S���W
	//  @param[in]  center_is_3d_ : center_ �̍��W��񂪎������W��� (true �̏ꍇ�A3D ��ԏ�̓_)
	//  @par
	//              duration_ �� 0 ���傫���ꍇ�A�l���̎��Ԃ�������<br>
	//              ���X�� power_ �� 0 �ɖ߂��܂��B
	//              <br>
	//              duration_ �� 0 �̏ꍇ�͋����������ŕω������܂���B<br>
	//              ���ɖ߂����߂ɂ́A������x���̊֐����Ă�� power_ �� 0 �ɂ���K�v������܂��B<br>
	//              <br>
	//              <br>
	//              scale_ �͌��̃X�N���[���C���[�W�ɑ΂��āA1�t���[��������ǂ̒��x�g�傷�邩���w�肵�܂��B<br>
	//              1.0 �ŃX�N���[���Ɠ��{ (�ω��Ȃ�)�A2.0 �ŃX�N���[����2�{�̑傫���ƂȂ�܂��B<br>
	//              scale_ �� 1.0 �̏ꍇ�A�u���[�͕��ˏ�Ɋg�U���܂���B<br>
	//              �ʏ�� 1.0�`1.1 �͈̔͂Ŏw�肷��̂�����ł��B<br>
	//              �܂��A1.0 ��菬�����l���w�肷��Ɠ��������̃u���[�ɂȂ�܂��B<br>
	//              <br>
	//              <br>
	//              center_is_3d_ �́Acenter_ �� 2D ��ԏ�̍��W���A3D ��ԏ�̍��W������ʂ��邽�߂Ɏw�肵�܂��B<br>
	//              <br>
	//              false ���w�肳��Ă���ꍇ�Acenter_ �̒l�����̂܂܃u���[�̒��S���W�ɂȂ�܂��B<br>
	//              ���S���W�̓��C���[�̍�������_(0, 0)�Ƃ��āA�s�N�Z���P�ʂŎw�肵�܂��B<br>
	//              <br>
	//              true ���w�肳��Ă���ꍇ�Acenter_ ��3D��ԏ�̓_�Ƃ��āA���̃��C���[�ɃZ�b�g����Ă���J�������猩��
	//              �X�N���[�����W��̓_�𒆐S�Ƃ��܂��B<br>
	//              <br>    
	//*/
	//----------------------------------------------------------------------
    void blur(
        double duration,
        lnFloat power,
        lnFloat scale,
        const LVector3& center,
        bool center_is_3d );

	/// �u���[�K�p�����𔻒肷��
    bool isRunningBlurEffect() const { return mScreenEffect->isBlurEffect(); }

	/// �G�t�F�N�g�K�p�����𔻒肷��
    bool isRunningScreenEffect() const { return mScreenEffect->isEffect(); }

	/// �t���[���X�V
	void updateFrame( const Game::GameTime& gameTime );

#endif

public:

	/// �`��c���[�̃��[�g�m�[�h�̐ݒ�
	void setRenderRootNode( SceneNode* node ) { mRenderRootNode = node; }

	/// �`��c���[�̃��[�g�m�[�h�̎擾
	SceneNode* getRenderRootNode() const { return mRenderRootNode; }

	/// �`���X�N���[���ƂȂ郌���_�����O�^�[�Q�b�g�̐ݒ�
	//void setRenderTarget( Graphics::ITexture* texture );

	/// �`���r���[�T�C�Y
	const LVector2& getViewSize() const;

	/// 2D �J�����Ƃ��č쐬����Ă��邩
	bool isProjection2D() const { return mCoreCamera.isProjection2D(); }



	/// �J��������N���X�̐ݒ� (�Q�ƃJ�E���g�̓C���N�������g���Ȃ����A�f�X�g���N�g���� release())
    void setCameraOperator( Graphics::CameraOperator* camera_operator ) { mCoreCamera.setCameraOperator( camera_operator ); }
    
	/// Z �\�[�g���@�̐ݒ�
    void setZSortType( LNZSortFunc type_ ) { mZSortFunc = type_; }

	/// Z �\�[�g���@�̎擾
    LNZSortFunc getZSortType() const { return mZSortFunc; }

	/// �J�������猩�� 3D ���W�� 2D ���W�ɕϊ�����
    void convertWorldPosToScreenPos( LVector3* out, const LVector3& world_pos ) const;


    LNDrawCoordSpace getCoordSpace() const { return ( isProjection2D() ) ? LN_DRAWCOORD_2D : LN_DRAWCOORD_3D; }


	
	void onUpdateViewSize( const LVector2& viewSize ) { mCoreCamera.setViewSize( viewSize ); }
	/// 
    void updateContext();

    CameraContext* getCameraContext() { return mContext; }

public:
	//void setParentPane( Pane* pane ) { mParentPane = pane; }
	//Pane* getParentPane() { return mParentPane; }
	void setParentLayer(DrawingLayer* pane) { mParentLayer = pane; }
	DrawingLayer* getParentLayer() { return mParentLayer; }

	/// �J�����P�ʂŃV�F�[�_�ɑ���f�[�^�̎擾 (�s�񉉎Z�Ȃǂ��낢�����Ă�̂� 1 �t���[��������ĂԂ͔̂����邱��)
    LNCameraSceneParam* getCameraSceneParam();

	/// (called from SceneGraph::onEvent)
    bool onEvent( const System::EventArgs& e );

#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
private:
	void _createScreenEffectResource();
	void _disposeScreenEffectResource();
#endif

protected:

    SceneGraph*             mScene;
	//Pane*					mParentPane;
	DrawingLayer*			mParentLayer;
    Graphics::Camera        mCoreCamera;
	//LVector2				mViewSize;
    lnU32					mClearFlag;         ///< LNClearFlag �̑g�ݍ��킹
    bool                    mOrthographic;      ///< ���ߊ��V�~�����[�g�t���O

    LNZSortFunc             mZSortFunc;
    LNCameraSceneParam      mCameraSceneParam;  ///< �J�����P�ʂŃV�F�[�_�ɑ���f�[�^
    CameraContext*          mContext;

    mutable bool            mMemberChanged;
     
	//bool                    mProjection2D;

#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
	LNScreenEffectMode		mScreenEffectMode;
	Graphics::ScreenEffect* mScreenEffect;
    LColorF				    mBackgroundColor;
#endif

	SceneNode*				mRenderRootNode;
	//Graphics::ITexture*		mRenderTarget;

    friend class CameraContext;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================