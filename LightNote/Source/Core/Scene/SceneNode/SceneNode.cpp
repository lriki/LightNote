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
	// ● コンストラクタ
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
	// ● デストラクタ
	//----------------------------------------------------------------------
    SceneNode::~SceneNode()
    {
		if ( mContext ) {
			mContext->releaseAllResource();
			SAFE_DELETE( mContext );
		}
		
#if 0
		SceneNodeRefArray::ObjectArray& ary = mChildNodeRefArray.getObjectArray();
		/* ReferenceObjectArray の要素は clear() を呼ぶと release() される。
		   それまでは単に削除リストに入れられるだけなので、
		   ここで改めて削除リストを作る必要はない。
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
	// ● 初期化
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
       
        // デフォルトのシェーダをセット
        // 指定された座標空間に追加
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
            // いまのところなにもしない
        }
    }

	//----------------------------------------------------------------------
	// ● ノード名の設定
	//----------------------------------------------------------------------
    LNRESULT SceneNode::setName( const lnChar* name_ )
    {
        mSceneGraph->onSetNodeName( this, mName.c_str(), name_ );
        mName = name_;
		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	// ● 子ノードを追加する
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
	// ● 子ノードを外す
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
	// ● 適用するシェーダの設定
	//----------------------------------------------------------------------
#if 0
    void SceneNode::setShader( SceneShader* shader )
    {
        if ( shader )
        {
            // 共有不可で最初にセットされたものではない場合、この中から例外が投げられる
            //shader_->onSettingShader( this );
        }

		LN_REFOBJ_SET( mShader, shader );
       
        // シェーダから必要な適用ライト数を取得して、配列を準備しておく
        if ( mShader )
        {
            mApplyLights.resize( mShader->getRequiredLightNum() );

			// TODO:仮
			if ( mShader->hasScriptExternal() )//isPostEffect() )
			{
				this->mSceneGraph->getDefaultNode( LN_DEFAULT_SCENENODE_3D )->addPostEffectShader( mShader, 0 );
			}
        }
    }
#endif
	//----------------------------------------------------------------------
	// ● 行列を階層的に更新する
	//----------------------------------------------------------------------
    void SceneNode::updateTransformHierarchy( const LMatrix& parent_marix_, const NodeRenderParam* parent_params_ )
    {
        // ワールド行列の更新が必要な場合は再計算
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

        // サブセット単位のパラメータもここで更新
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
	// ● カメラ単位の描画情報の更新
	//----------------------------------------------------------------------
#if 0
    void SceneNode::updateParamFromCameraAndLight( Camera* camera_ )
    {
        //-----------------------------------------------------
        // Z ソートの基準値を計算

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
        // 子ノード呼び出し & Z ソート

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
        // 視錐台カリング


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
	// ● Z ソート用の比較関数
	//----------------------------------------------------------------------
    bool SceneNode::_cmpZAndPrioritySort( const SceneNode* l_, const SceneNode* r_ )
    {
        if ( l_->mPriority == r_->mPriority )
        {
            // 昇順
            return l_->mZDistance < r_->mZDistance;
        }
        // 昇順
        return l_->mPriority < r_->mPriority;
    }

#if 1
//==============================================================================
// ■ SceneNodeContext
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

        // 子ノードのコンテキスト
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
		// 子ノード
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
        // Z ソート用の基準値

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
        // ビルボード行列計算

        // 正面を向くビルボードの場合
        if ( mBillboardType == LN_BILLBOARD_FRONT )
        {
            LMatrix mat;

			// まずは結合済み行列を Z+ 方向に向ける。
			// これでX, Y 軸の回転を消す。
			LVector3 front( 0, 0, 1 );
			LVector3 right, up;
			LVector3::cross( &right, mCombinedWorldMatrix.getUp(), front );
			LVector3::cross( &up, front, right );
			mat[0].set( right.x, right.y, right.z, 0 );
			mat[1].set( up.x, up.y, up.z, 0 );
			mat[2].set( front.x, front.y, front.z, 0 );

            // ビュー行列の逆行列のうち、回転部分だけを乗算
            mat.setMul3x3( param.CameraContext->getViewMatrixInverse() );

            // 移動成分を乗算しなおす
            mat.translation( mCombinedWorldMatrix.m30, mCombinedWorldMatrix.m31, mCombinedWorldMatrix.m32 );

            mCombinedWorldMatrix = mat;
        }
        // Y 軸回転のみ行うビルボードの場合
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
        // ライト列挙

        // ライトが必要な場合 (mAffectLightContextArray は updateContext で resize() 済み)
        if ( mAffectLightContextArray.size() > 0 )
        {
			// 自分の位置情報を渡して、最寄りのライトをもらう
            param.SceneGraphContext->selectLights( &mAffectLightContextArray, mCombinedWorldMatrix.getPosition() );
        }

		// TODO: 視錐台カリングとかできれば
		this->mEnableDrawing = this->mIsVisible;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SceneNodeContext::render( ContextDrawParam& param )
	{
		if ( !mChildSceneNodeContextArray.empty() )
		{
			// 子のZソート
			std::stable_sort( mChildSceneNodeContextArray.begin(), mChildSceneNodeContextArray.end(), _cmpZAndPrioritySort );

			// 子呼び出し
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

            // 逆行列
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

            // 転置行列
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

            // 逆行列×転置行列
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
            // 降順 (遠い方から)
            return l_->mZDistance > r_->mZDistance;
        }
        // 降順 (高い方から)
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