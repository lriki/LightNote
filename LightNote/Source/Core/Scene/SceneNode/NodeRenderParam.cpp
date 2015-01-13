//==============================================================================
// NodeRenderParam 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Common.h"
#include "../SceneShader/SceneShader.h"
#include "Camera.h"
#include "Light.h"
#include "NodeRenderParam.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

    LNPriorityParameter LNPriorityParameter::HIDE_PARAM( true );

//==============================================================================
// �� NodeRenderParam �N���X
//==============================================================================

	//----------------------------------------------------------------------
	// �� �R���X�g���N�^
	//----------------------------------------------------------------------
    NodeRenderParam::NodeRenderParam()
        : Subset        ( NULL )
        , mSceneNode    ( NULL )
        , mSubsetNum    ( 0 )
        , mSceneShader  ( NULL )
        , mCombinedSubsetRenderParams   ( NULL )
        , mAffectWorldMatrix    ( NULL )
        , mAffectCamera         ( NULL )
        , mAffectLightArray     ( NULL )
        , mIsVisible    ( false )
    {
    }

	//----------------------------------------------------------------------
	// �� �f�X�g���N�^
	//----------------------------------------------------------------------
    NodeRenderParam::~NodeRenderParam()
    {
        LN_SAFE_RELEASE( mSceneShader );
        SAFE_DELETE_ARRAY( Subset );
        SAFE_DELETE_ARRAY( mCombinedSubsetRenderParams );
    }

	//----------------------------------------------------------------------
	// �� ������
	//----------------------------------------------------------------------
    LNRESULT NodeRenderParam::initialize( SceneNode* node_, lnU32 subset_num_ )
    {
        mSceneNode = node_;

		setSubsetCount( subset_num_ );
        //if ( subset_num_ > 0 )
        //{
        //    Subset = LN_NEW LNSubsetRenderParam[ subset_num_ ];
        //    mCombinedSubsetRenderParams = LN_NEW LNSubsetRenderParam[ subset_num_ ];
        //    mSubsetNum = subset_num_;
        //}
		return ResultCode_OK;
    }
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void NodeRenderParam::setSubsetCount( int count )
	{
		int oldSubsetCount = mSubsetNum;
		LNSubsetRenderParam* oldParams = Subset;
        SAFE_DELETE_ARRAY( mCombinedSubsetRenderParams );

		mSubsetNum = count;

		if ( mSubsetNum > 0 )
        {
			Subset = LN_NEW LNSubsetRenderParam[mSubsetNum];
			mCombinedSubsetRenderParams = LN_NEW LNSubsetRenderParam[mSubsetNum];

			if ( oldParams )
			{
				for ( int i = 0; i < oldSubsetCount; ++i )
				{
					Subset[i] = oldParams[i];
				}
			}
		}
		SAFE_DELETE_ARRAY( oldParams );
	}

	//----------------------------------------------------------------------
	// �� ���ۂɕ`�掞�Ɏg�����̍X�V
	//----------------------------------------------------------------------
    void NodeRenderParam::updateSubsetRenderParam( const NodeRenderParam* parent_params_ )
    {
        // �܂��͎����������Ă���p�����[�^���ۂ��ƃR�~�b�g
        mCommittedRenderState = RenderState;

        // LMaterial ���ł̓��\�[�X�̎Q�ƃJ�E���g��ω������Ă���̂ŁAmemcpu �� NG
        for ( lnU32 i = 0; i < mSubsetNum; ++i )
        {
            mCombinedSubsetRenderParams[ i ] = Subset[ i ];
        }
        //memcpy( , , sizeof( LNSubsetRenderParam ) * mSubsetNum );

        // �e������ꍇ�� 0 �ԃT�u�Z�b�g�̃R�~�b�g�ςݏ����󂯌p��
        if ( parent_params_ )
        {
            const LNSubsetRenderParam& pp = parent_params_->mCombinedSubsetRenderParams[ 0 ];

            for ( lnU32 i = 0; i < mSubsetNum; ++i )
            {
                LNSubsetRenderParam& pa = mCombinedSubsetRenderParams[ i ];

                ((LVector4&)pa.ColorScale) = ((LVector4&)pa.ColorScale) * ((LVector4&)pp.ColorScale);
                ((LVector4&)pa.BlendColor) = ((LVector4&)pa.BlendColor) + ((LVector4&)pp.BlendColor);
                ((LVector4&)pa.Tone)       = ((LVector4&)pa.Tone)       + ((LVector4&)pp.Tone);
            }
        }
    }

	//----------------------------------------------------------------------
	// �� �`��Ɏg���s���񓙂̐ݒ�
	//----------------------------------------------------------------------
    void NodeRenderParam::setAffectElements( const LMatrix* world_, const Camera* camera_, const LightArray* lights_, SceneShader* shader_, bool visible_ )
    {
        mAffectWorldMatrix = world_;
        mAffectCamera = camera_;
        mAffectLightArray = lights_;
        LN_SAFE_RELEASE( mSceneShader );
        mSceneShader = shader_;
        LN_SAFE_ADDREF( mSceneShader );
        mIsVisible = visible_;
    }

	//----------------------------------------------------------------------
	// �� �V�F�[�_�ϐ��̗v���ɑΉ�����s��(WORLD ���܂܂�Ă������)��Ԃ�
	//----------------------------------------------------------------------
    const LMatrix* NodeRenderParam::getObjectMatrix( MMEVariableRequest req_, int light_idx_ ) const
    {
        switch ( req_ )
        {
            case MME_VARREQ_MATRIX_World:
            {
                return mAffectWorldMatrix;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView:
            {
                mTempMatrix = *mAffectWorldMatrix * mAffectCamera->getViewMatrix();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj:
            {
                mTempMatrix = *mAffectWorldMatrix * mAffectCamera->getViewProjectionMatrix();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView:
            {
                mTempMatrix = *mAffectWorldMatrix * (*mAffectLightArray)[ light_idx_ ]->getViewMatrix();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj:
            {
                mTempMatrix = *mAffectWorldMatrix * (*mAffectLightArray)[ light_idx_ ]->getViewProjectionMatrix();
                return &mTempMatrix;
            }

            // �t�s��
            case MME_VARREQ_MATRIX_World_I:
            {
                LMatrix::inverse( &mTempMatrix, *mAffectWorldMatrix );
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView_I:
            {
                mTempMatrix = *mAffectWorldMatrix * mAffectCamera->getViewMatrix();
                mTempMatrix.inverse();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_I:
            {
                mTempMatrix = *mAffectWorldMatrix * mAffectCamera->getViewProjectionMatrix();
                mTempMatrix.inverse();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView_I:
            {
                mTempMatrix = *mAffectWorldMatrix * (*mAffectLightArray)[ light_idx_ ]->getViewMatrix();
                mTempMatrix.inverse();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I:
            {
                mTempMatrix = *mAffectWorldMatrix * (*mAffectLightArray)[ light_idx_ ]->getViewProjectionMatrix();
                mTempMatrix.inverse();
                return &mTempMatrix;
            }

            // �]�u�s��
            case MME_VARREQ_MATRIX_World_T:
            {
                LMatrix::transpose( &mTempMatrix, *mAffectWorldMatrix );
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView_T:
            {
                mTempMatrix = *mAffectWorldMatrix * mAffectCamera->getViewMatrix();
                mTempMatrix.transpose();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_T:
            {
                mTempMatrix = *mAffectWorldMatrix * mAffectCamera->getViewProjectionMatrix();
                mTempMatrix.transpose();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView_T:
            {
                mTempMatrix = *mAffectWorldMatrix * (*mAffectLightArray)[ light_idx_ ]->getViewMatrix();
                mTempMatrix.transpose();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T:
            {
                mTempMatrix = *mAffectWorldMatrix * (*mAffectLightArray)[ light_idx_ ]->getViewProjectionMatrix();
                mTempMatrix.transpose();
                return &mTempMatrix;
            }

            // �t�s��~�]�u�s��
            case MME_VARREQ_MATRIX_World_IT:
            {
                LMatrix::inverse( &mTempMatrix, *mAffectWorldMatrix );
                mTempMatrix.transpose();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView_IT:
            {
                mTempMatrix = *mAffectWorldMatrix * mAffectCamera->getViewMatrix();
                mTempMatrix.inverse();
                mTempMatrix.transpose();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_IT:
            {
                mTempMatrix = *mAffectWorldMatrix * mAffectCamera->getViewProjectionMatrix();
                mTempMatrix.inverse();
                mTempMatrix.transpose();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView_IT:
            {
                mTempMatrix = *mAffectWorldMatrix * (*mAffectLightArray)[ light_idx_ ]->getViewMatrix();
                mTempMatrix.inverse();
                mTempMatrix.transpose();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT:
            {
                mTempMatrix = *mAffectWorldMatrix * (*mAffectLightArray)[ light_idx_ ]->getViewProjectionMatrix();
                mTempMatrix.inverse();
                mTempMatrix.transpose();
                return &mTempMatrix;
            }
        }

        return NULL;


        /*
        switch ( req_ )
        {
            case MME_VARREQ_MATRIX_World:
                return *m_w;
            case MME_VARREQ_MATRIX_CAMERA_WorldView:
                return m_wv_c;
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj:
                return m_wvp_c;
            case MME_VARREQ_MATRIX_LIGHT_WorldView:
            {
                if ( light_idx_ < 0 ) { return m_wv_l; }
                const Light* l = (*mAffectLightArray)[ light_idx_ ];
                if ( l->isDefaultLight() )
                {
                    return m_wv_l;
                }
                else
                {
                    mTempMatrix = (*m_w) * l->getViewMatrix();
                    return mTempMatrix;
                }
                break;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj:
            {
                if ( light_idx_ < 0 ) { return m_wvp_l; }
                const Light* l = (*mAffectLightArray)[ light_idx_ ];
                if ( l->isDefaultLight() )
                {
                    return m_wvp_l;
                }
                else
                {
                    mTempMatrix = (*m_w) * l->getViewProjectionMatrix();
                    return mTempMatrix;
                }
                break;
            }

            // �t�s��
            case MME_VARREQ_MATRIX_World_I:
                return m_w_i;
            case MME_VARREQ_MATRIX_CAMERA_WorldView_I:
                return m_wv_c_i;
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_I:
                return m_wvp_c_i;
            case MME_VARREQ_MATRIX_LIGHT_WorldView_I:
            {
                if ( light_idx_ < 0 ) { return m_wv_l_i; }
                const Light* l = (*mAffectLightArray)[ light_idx_ ];
                if ( l->isDefaultLight() )
                {
                    return m_wv_l_i;
                }
                else
                {
                    mTempMatrix = (*m_w) * l->getViewMatrix();
                    mTempMatrix.inverse();
                    return mTempMatrix;
                }
                break;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I:
            {
                if ( light_idx_ < 0 ) { return m_wvp_l_i; }
                const Light* l = (*mAffectLightArray)[ light_idx_ ];
                if ( l->isDefaultLight() )
                {
                    return m_wvp_l_i;
                }
                else
                {
                    mTempMatrix = (*m_w) * l->getViewProjectionMatrix();
                    mTempMatrix.inverse();
                    return mTempMatrix;
                }
                break;
            }

            // �]�u�s��
            case MME_VARREQ_MATRIX_World_T:
                return m_w_t;
            case MME_VARREQ_MATRIX_CAMERA_WorldView_T:
                return m_wv_c_t;
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_T:
                return m_wvp_c_t;
            case MME_VARREQ_MATRIX_LIGHT_WorldView_T:
            {
                if ( light_idx_ < 0 ) { return m_wv_l_t; }
                const Light* l = (*mAffectLightArray)[ light_idx_ ];
                if ( l->isDefaultLight() )
                {
                    return m_wv_l_t;
                }
                else
                {
                    mTempMatrix = (*m_w) * l->getViewMatrix();
                    mTempMatrix.transpose();
                    return mTempMatrix;
                }
                break;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T:
            {
                if ( light_idx_ < 0 ) { return m_wvp_l_t; }
                const Light* l = (*mAffectLightArray)[ light_idx_ ];
                if ( l->isDefaultLight() )
                {
                    return m_wvp_l_t;
                }
                else
                {
                    mTempMatrix = (*m_w) * l->getViewProjectionMatrix();
                    mTempMatrix.transpose();
                    return mTempMatrix;
                }
                break;
            }

            // �t�s��~�]�u�s��
            case MME_VARREQ_MATRIX_World_IT:
                return m_w_it;
            case MME_VARREQ_MATRIX_CAMERA_WorldView_IT:
                return m_wv_c_it;
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_IT:
                return m_wvp_c_it;
            case MME_VARREQ_MATRIX_LIGHT_WorldView_IT:
            {
                if ( light_idx_ < 0 ) { return m_wv_l_it; }
                const Light* l = (*mAffectLightArray)[ light_idx_ ];
                if ( l->isDefaultLight() )
                {
                    return m_wv_l_it;
                }
                else
                {
                    mTempMatrix = (*m_w) * l->getViewMatrix();
                    mTempMatrix.inverse();
                    mTempMatrix.transpose();
                    return mTempMatrix;
                }
                break;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT:
            {
                if ( light_idx_ < 0 ) { return m_wvp_l_it; }
                const Light* l = (*mAffectLightArray)[ light_idx_ ];
                if ( l->isDefaultLight() )
                {
                    return m_wvp_l_it;
                }
                else
                {
                    mTempMatrix = (*m_w) * l->getViewProjectionMatrix();
                    mTempMatrix.inverse();
                    mTempMatrix.transpose();
                    return mTempMatrix;
                }
                break;
            }
        }

        return LMatrix::IDENTITY;
        */
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core

} // namespace LNote

//==============================================================================
//
//==============================================================================