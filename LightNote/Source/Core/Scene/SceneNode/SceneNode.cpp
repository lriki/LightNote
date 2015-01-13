//==============================================================================
// SceneNode 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <algorithm>
#include "../../Base/STLUtils.h"
#include "../SceneGraph.h"
#include "../SceneShader/SceneShader.h"
#include "../SceneShader/SceneShaderTechnique.h"
#include "../SceneShader/OffscreenScene.h"
#include "Camera.h"
#include "Light.h"
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
// SceneNode
//==============================================================================

	LN_TYPE_INFO_ACCESS_IMPL(SceneNode);
    
 

	//----------------------------------------------------------------------
	// �� �R���X�g���N�^
	//----------------------------------------------------------------------
    SceneNode::SceneNode( SceneGraph* scene_ )
		: FileIO::AsyncLoadObject	( scene_->getFileManager() )
		, mSceneGraph			( scene_ )
        , mDrawCoordSpace		( LN_DRAWCOORD_UNKNOWN )
        , mSubsetCount			( 0 )
        , mParentNode			( NULL )
        , mContext				( NULL )
        , mScale				( 1, 1, 1 )
        , mBillboardType		( LN_BILLBOARD_NOT )
        , mShader				( NULL )
		, mRotOrder				( Math::RotationOrder_XYZ )
        , mPriority				( 0 )
        , mZDistance			( 0.0f )
        , mPriorityParameter    ( NULL )
        , mIsVisible            ( true )
        , mNeedUpdateMatrix     ( false )
        , mIsAutoUpdate         ( true )
        , mEnableDraw           ( true )

        //: mSceneGraph       ( scene_ )
        //, mParentNode       ( NULL )
        //, mZSortFunc        ( LN_ZSORTF_IMM_Z )
        //, mZDistance        ( 0.0f )
        //
        //
        //, mSubsetRenderParams   ( NULL )
        //
        //, mShader           ( NULL )
        //
        //, mUseMatrix        ( true )
        //
        //
    {
        LN_SAFE_ADDREF( mSceneGraph );
        
    }


	//----------------------------------------------------------------------
	// �� �f�X�g���N�^
	//----------------------------------------------------------------------
    SceneNode::~SceneNode()
    {
		if ( mContext ) {
			mContext->releaseAllResource();
			SAFE_DELETE( mContext );
		}
		
#if 0
		SceneNodeRefArray::ObjectArray& ary = mChildNodeRefArray.getObjectArray();
		/* ReferenceObjectArray �̗v�f�� clear() ���ĂԂ� release() �����B
		   ����܂ł͒P�ɍ폜���X�g�ɓ�����邾���Ȃ̂ŁA
		   �����ŉ��߂č폜���X�g�����K�v�͂Ȃ��B
		SceneNodeArray deleteList;
		deleteList.reserve( ary.size() );
		ln_foreach( SceneNode* n, ary )
		{
			deleteList.push_back( n );
		}
		*/
		ln_foreach( SceneNode* n, ary )
		{
			if ( n->mParentNode ) n->mParentNode->removeChild( n );
		}

		mChildNodeRefArray.clear();
#endif
        if ( mParentNode )
        {
            mParentNode->removeChild( this );
        }

        if ( mSceneGraph )
        {
            mSceneGraph->onDeleteNode( this );
        }

        LN_SAFE_RELEASE( mShader );
        LN_SAFE_RELEASE( mSceneGraph );
    }

	//----------------------------------------------------------------------
	// �� ������
	//----------------------------------------------------------------------
    void SceneNode::initialize( lnU32 subsetCount, LNDrawCoordSpace cs_, SceneNodeContext* context_ )
    {
        mDrawCoordSpace = cs_;

        mSceneGraph->addNode( this );

        if ( context_ )
        {
            mContext = context_;
            //this->mSubsetCount = subsetCount;//context_->getSubsetCount();
        }
        else
        {
            //mSubsetCount = subsetCount;
            mContext = LN_NEW SceneNodeContext();
            mContext->initialize( this );
        }

		this->mNodeRenderParam.initialize( this, mSubsetCount );
		this->mNodeRenderParam.RenderState = this->mSceneGraph->getNodeDefaultRenderState();
		this->setSubsetCount( subsetCount );
       
        // �f�t�H���g�̃V�F�[�_���Z�b�g
        // �w�肳�ꂽ���W��Ԃɒǉ�
        if ( getDrawCoordSpace() == LN_DRAWCOORD_3D )
        {
            mShader = mSceneGraph->getDefaultShader( LN_DEFSHADER_BASIC_3D );
            LN_SAFE_ADDREF( mShader );

            mSceneGraph->getDefaultNode( LN_DEFAULT_SCENENODE_3D )->addChild( this );
        }
        else if ( getDrawCoordSpace() == LN_DRAWCOORD_2D )
        {
            mShader = mSceneGraph->getDefaultShader( LN_DEFSHADER_BASIC_2D );
            LN_SAFE_ADDREF( mShader );

            mSceneGraph->getDefaultNode( LN_DEFAULT_SCENENODE_2D )->addChild( this );
        }
        else// if ( getDrawCoordSpace() == LN_DRAWCOORD_UNKNOWN )
        {
            // ���܂̂Ƃ���Ȃɂ����Ȃ�
        }
    }

	//----------------------------------------------------------------------
	// �� �m�[�h���̐ݒ�
	//----------------------------------------------------------------------
    LNRESULT SceneNode::setName( const lnChar* name_ )
    {
        mSceneGraph->onSetNodeName( this, mName.c_str(), name_ );
        mName = name_;
		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	// �� �q�m�[�h��ǉ�����
	//----------------------------------------------------------------------
    void SceneNode::addChild( SceneNode* node )
    {
		if (node)
        {
			if (node->mParentNode)
            {
				node->mParentNode->removeChild(node);
            }

			mChildNodeList.push_back(node);
			//mChildNodeRefArray.registerObject( node_ );
			node->mParentNode = this;
			LN_SAFE_ADDREF(node->mParentNode);
        }
    }

	//----------------------------------------------------------------------
	// �� �q�m�[�h���O��
	//----------------------------------------------------------------------
    LNRESULT SceneNode::removeChild( SceneNode* node_ )
    {
        Base::STLUtils::remove( mChildNodeList, node_ );
		//mChildNodeRefArray.unregisterObject( node_ );
		//node_->mParentNode = NULL;
        LN_SAFE_RELEASE( node_->mParentNode );
		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	// �� �K�p����V�F�[�_�̐ݒ�
	//----------------------------------------------------------------------
#if 0
    void SceneNode::setShader( SceneShader* shader )
    {
        if ( shader )
        {
            // ���L�s�ōŏ��ɃZ�b�g���ꂽ���̂ł͂Ȃ��ꍇ�A���̒������O����������
            //shader_->onSettingShader( this );
        }

		LN_REFOBJ_SET( mShader, shader );
       
        // �V�F�[�_����K�v�ȓK�p���C�g�����擾���āA�z����������Ă���
        if ( mShader )
        {
            mApplyLights.resize( mShader->getRequiredLightNum() );

			// TODO:��
			if ( mShader->hasScriptExternal() )//isPostEffect() )
			{
				this->mSceneGraph->getDefaultNode( LN_DEFAULT_SCENENODE_3D )->addPostEffectShader( mShader, 0 );
			}
        }
    }
#endif
	//----------------------------------------------------------------------
	// �� �s����K�w�I�ɍX�V����
	//----------------------------------------------------------------------
    void SceneNode::updateTransformHierarchy( const LMatrix& parent_marix_, const NodeRenderParam* parent_params_ )
    {
        // ���[���h�s��̍X�V���K�v�ȏꍇ�͍Čv�Z
        if ( mNeedUpdateMatrix )
        {
            mMatrix.identity();
            mMatrix.translation( -mCenter.x, -mCenter.y, -mCenter.z );
            mMatrix.scaling( mScale );
            mMatrix.rotation( mAngle, mRotOrder );
            mMatrix.translation( mPosition );
            mNeedUpdateMatrix = false;   
        }

        mCombinedWorldMatrix = mMatrix * parent_marix_;

        // �T�u�Z�b�g�P�ʂ̃p�����[�^�������ōX�V
        mNodeRenderParam.updateSubsetRenderParam( parent_params_ );


		//SceneNodeArray::iterator itr = mChildNodeRefArray.getObjectArray().begin();
		//SceneNodeArray::iterator end = mChildNodeRefArray.getObjectArray().end();
		SceneNodeArray::iterator itr = mChildNodeList.begin();
		SceneNodeArray::iterator end = mChildNodeList.end();
	    for ( ;itr != end; ++itr )
	    {
		    (*itr)->updateTransformHierarchy( mCombinedWorldMatrix, &mNodeRenderParam );
	    }
    }

	//----------------------------------------------------------------------
	// �� �J�����P�ʂ̕`����̍X�V
	//----------------------------------------------------------------------
#if 0
    void SceneNode::updateParamFromCameraAndLight( Camera* camera_ )
    {
        //-----------------------------------------------------
        // Z �\�[�g�̊�l���v�Z

        switch ( camera_->getZSortType() )
        {
            default:
            case LN_ZSORTF_IMM_Z:
                mZDistance = mPosition.z;
                break;
            case LN_ZSORTF_CAMERA_DIST:
                mZDistance = LVector3::squareLength( mPosition - camera_->getPosition() );
                break;
            case LN_ZSORTF_CAMERA_SCREEN:
                mZDistance = LGeometry::PointPolyDistance(
                    mPosition, 
                    camera_->getPosition(),
                    camera_->getDirection() );
                break;
        }

        //-----------------------------------------------------
        // �q�m�[�h�Ăяo�� & Z �\�[�g

        if ( !mContext->mChildNodeList.empty() )
//		if ( !mChildNodeRefArray.getObjectArray().empty() )
        {
			//SceneNodeRefArray::ObjectArray& ary = mChildNodeRefArray.getObjectArray();
            //ln_foreach( SceneNode* node, ary )
			ln_foreach( SceneNode* node, mContext->mChildNodeList )
            {
                node->updateParamFromCameraAndLight( camera_ );
            }

            //std::stable_sort( ary.begin(), ary.end(), _cmpZAndPrioritySort );
			std::stable_sort( mContext->mChildNodeList.begin(), mContext->mChildNodeList.end(), _cmpZAndPrioritySort );
        }

        //-----------------------------------------------------
        // ������J�����O


        mEnableDraw = mIsVisible;
    }
#endif
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void SceneNode::updateContext()
    {
		//mChildNodeRefArray.commit();

        mContext->updateContext( (mParentNode) ? mParentNode->mContext : NULL );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SceneNode::setSubsetCount( int count )
	{
		this->mSubsetCount = count;
        if ( this->mSubsetCount > 0 )
        {
			this->mNodeRenderParam.setSubsetCount( this->mSubsetCount );
        }
	}

	//----------------------------------------------------------------------
	// �� Z �\�[�g�p�̔�r�֐�
	//----------------------------------------------------------------------
    bool SceneNode::_cmpZAndPrioritySort( const SceneNode* l_, const SceneNode* r_ )
    {
        if ( l_->mPriority == r_->mPriority )
        {
            // ����
            return l_->mZDistance < r_->mZDistance;
        }
        // ����
        return l_->mPriority < r_->mPriority;
    }

#if 1
//==============================================================================
// �� SceneNodeContext
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    SceneNodeContext::SceneNodeContext()
        : mSceneNode            ( NULL )
        , mPriority             ( 0 )
        , mBillboardType        ( LN_BILLBOARD_NOT )
        , mZDistance            ( 0 )
        , mAffectCameraContext  ( NULL )
        , mIsVisible            ( true )
		, mEnableDrawing		( true )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    SceneNodeContext::~SceneNodeContext()
    {
        releaseAllResource();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void SceneNodeContext::initialize( SceneNode* node_/*, lnU32 subset_num_*/ )
    {
        mSceneNode = node_;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void SceneNodeContext::updateContext( SceneNodeContext* parent_context_ )
    {
        releaseAllResource();

		mName = mSceneNode->mName;
        mCombinedWorldMatrix = mSceneNode->mCombinedWorldMatrix;
        mPriority = mSceneNode->mPriority;
        mBillboardType = mSceneNode->mBillboardType;
        mIsVisible = mSceneNode->mIsVisible;

        // �q�m�[�h�̃R���e�L�X�g
#if 1
		SceneNode::SceneNodeRefArray::ObjectArray& ary = mSceneNode->mChildNodeList;
        int n = ary.size();
        mChildSceneNodeContextArray.resize( n );
        for ( int i = 0; i < n; ++i )
        {
            mChildSceneNodeContextArray[i] = ary[i]->mContext;
        }
#else
		SceneNode::SceneNodeRefArray::ObjectArray& ary = mSceneNode->mChildNodeRefArray.getObjectArray();
        int n = ary.size();
        mChildSceneNodeContextArray.resize( n );
        for ( int i = 0; i < n; ++i )
        {
            mChildSceneNodeContextArray[i] = ary[i]->mContext;
        }
#endif
		// �q�m�[�h
		mChildNodeList.resize( n );
        for ( int i = 0; i < n; ++i )
        {
            mChildNodeList[i] = ary[i];
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void SceneNodeContext::releaseAllResource()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void SceneNodeContext::preRender( ContextDrawParam& param )
    {
		mAffectCameraContext = param.CameraContext;

        //-----------------------------------------------------
        // Z �\�[�g�p�̊�l

		switch ( param.CameraContext->getZSortType() )
        {
            default:
            case LN_ZSORTF_IMM_Z:
                mZDistance = mCombinedWorldMatrix.getPosition().z;
                break;
            case LN_ZSORTF_CAMERA_DIST:
                mZDistance = LVector3::squareLength( mCombinedWorldMatrix.getPosition() - param.CameraContext->getPosition() );
                break;
            case LN_ZSORTF_CAMERA_SCREEN:
                mZDistance = LGeometry::PointPolyDistance(
                    mCombinedWorldMatrix.getPosition(), 
                    param.CameraContext->getPosition(),
                    param.CameraContext->getDirection() );
                break;
        }

		//std::stable_sort( mChildSceneNodeContextArray.begin(), mChildSceneNodeContextArray.end(), _cmpZAndPrioritySort );

        //-----------------------------------------------------
        // �r���{�[�h�s��v�Z

        // ���ʂ������r���{�[�h�̏ꍇ
        if ( mBillboardType == LN_BILLBOARD_FRONT )
        {
            LMatrix mat;

			// �܂��͌����ςݍs��� Z+ �����Ɍ�����B
			// �����X, Y ���̉�]�������B
			LVector3 front( 0, 0, 1 );
			LVector3 right, up;
			LVector3::cross( &right, mCombinedWorldMatrix.getUp(), front );
			LVector3::cross( &up, front, right );
			mat[0].set( right.x, right.y, right.z, 0 );
			mat[1].set( up.x, up.y, up.z, 0 );
			mat[2].set( front.x, front.y, front.z, 0 );

            // �r���[�s��̋t�s��̂����A��]������������Z
            mat.setMul3x3( param.CameraContext->getViewMatrixInverse() );

            // �ړ���������Z���Ȃ���
            mat.translation( mCombinedWorldMatrix.m30, mCombinedWorldMatrix.m31, mCombinedWorldMatrix.m32 );

            mCombinedWorldMatrix = mat;
        }
        // Y ����]�̂ݍs���r���{�[�h�̏ꍇ
        else if ( mBillboardType == LN_BILLBOARD_ROT_Y )
        {
            const LMatrix& view = param.CameraContext->getViewMatrix();

            LMatrix mat;
            mat.setRotateMatrix( mCombinedWorldMatrix );

            if ( view.m02 > 0.0f )
		    {
                mat.rotationY( -atanf( view.m22 / view.m02 ) + LMath::PI / 2 );
		    }   
		    else if ( view.m02 == 0.0f )
		    {
                mat.identity();
		    }
		    else
		    {
                mat.rotationY( -atanf( view.m22 / view.m02 ) - LMath::PI / 2 );
		    }

            mat.translation( mCombinedWorldMatrix.getPosition() );

            mCombinedWorldMatrix = mat;
        }

        //-----------------------------------------------------
        // ���C�g��

        // ���C�g���K�v�ȏꍇ (mAffectLightContextArray �� updateContext �� resize() �ς�)
        if ( mAffectLightContextArray.size() > 0 )
        {
			// �����̈ʒu����n���āA�Ŋ��̃��C�g�����炤
            param.SceneGraphContext->selectLights( &mAffectLightContextArray, mCombinedWorldMatrix.getPosition() );
        }

		// TODO: ������J�����O�Ƃ��ł����
		this->mEnableDrawing = this->mIsVisible;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SceneNodeContext::render( ContextDrawParam& param )
	{
		if ( !mChildSceneNodeContextArray.empty() )
		{
			// �q��Z�\�[�g
			std::stable_sort( mChildSceneNodeContextArray.begin(), mChildSceneNodeContextArray.end(), _cmpZAndPrioritySort );

			// �q�Ăяo��
			//SceneNodeArray::iterator itr = mChildNodeRefArray.getObjectArray().begin();
			//SceneNodeArray::iterator end = mChildNodeRefArray.getObjectArray().end();
			SceneNodeContextArray::iterator itr = this->mChildSceneNodeContextArray.begin();
			SceneNodeContextArray::iterator end = this->mChildSceneNodeContextArray.end();
			for ( ;itr != end; ++itr )
			{
				(*itr)->render( param );
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool SceneNodeContext::getObjectMatrix( LMatrix* out_, MMEVariableRequest req_, int light_idx_ ) const
    {
        switch ( req_ )
        {
            case MME_VARREQ_MATRIX_World:
            {
                *out_ = mCombinedWorldMatrix;
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewMatrix();
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewProjectionMatrix();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewMatrix();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewProjectionMatrix();
                return true;
            }

            // �t�s��
            case MME_VARREQ_MATRIX_World_I:
            {
                LMatrix::inverse( out_, mCombinedWorldMatrix );
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView_I:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewMatrix();
                out_->inverse();
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_I:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewProjectionMatrix();
                out_->inverse();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView_I:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewMatrix();
                out_->inverse();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewProjectionMatrix();
                out_->inverse();
                return true;
            }

            // �]�u�s��
            case MME_VARREQ_MATRIX_World_T:
            {
                LMatrix::transpose( out_, mCombinedWorldMatrix );
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView_T:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewMatrix();
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_T:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewProjectionMatrix();
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView_T:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewMatrix();
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewProjectionMatrix();
                out_->transpose();
                return true;
            }

            // �t�s��~�]�u�s��
            case MME_VARREQ_MATRIX_World_IT:
            {
                LMatrix::inverse( out_, mCombinedWorldMatrix );
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView_IT:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewMatrix();
                out_->inverse();
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_IT:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewProjectionMatrix();
                out_->inverse();
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView_IT:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewMatrix();
                out_->inverse();
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewProjectionMatrix();
                out_->inverse();
                out_->transpose();
                return true;
            }
        }

        return false;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool SceneNodeContext::_cmpZAndPrioritySort( const SceneNodeContext* l_, const SceneNodeContext* r_ )
    {
        if ( l_->mPriority == r_->mPriority )
        {
            // �~�� (����������)
            return l_->mZDistance > r_->mZDistance;
        }
        // �~�� (����������)
        return l_->mPriority < r_->mPriority;
    }

#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================