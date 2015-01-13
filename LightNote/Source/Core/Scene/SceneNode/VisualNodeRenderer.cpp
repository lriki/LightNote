//=============================================================================
//【 VisualNodeRenderer 】
//-----------------------------------------------------------------------------
///**
//  @file       VisualNodeRenderer.h
//  @brief      VisualNodeRenderer
//  @author     Riki
//*/
//=============================================================================

#include "stdafx.h"
#include "../SceneShader/SceneShader.h"
#include "../SceneShader/SceneShaderTechnique.h"
#include "../SceneShader/OffscreenScene.h"
#include "../SceneGraph.h"
#include "SceneNode.h"
#include "Camera.h"
#include "VisualNodeRenderer.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

////=============================================================================
//// ■ VisualNodeRenderer
////=============================================================================
//
//	//---------------------------------------------------------------------
//	//
//	//---------------------------------------------------------------------
//    VisualNodeRenderer::VisualNodeRenderer()
//		: mOwnerNode		( NULL )
//		, mRendererContext	( NULL )
//		//, mRotOrder			( Math::RotationOrder_XYZ )
//		//, mPriority			( 0 )
//		//, mBillboardType	( LN_BILLBOARD_NOT )
//		//, mIsVisible		( true )
//	{
//	}
//
//	//---------------------------------------------------------------------
//	//
//	//---------------------------------------------------------------------
//    VisualNodeRenderer::~VisualNodeRenderer()
//	{
//		if ( mRendererContext ) {
//			mRendererContext->releaseAllResource();
//			LN_SAFE_RELEASE( mRendererContext );
//		}
//	}
//
//	//---------------------------------------------------------------------
//	//
//	//---------------------------------------------------------------------
//    void VisualNodeRenderer::create( VisualNode* ownerNode, int subsetParamCount, VisualNodeRendererContext* rendererContext )
//	{
//		mOwnerNode = ownerNode;
//
//		mNodeRenderParam.initialize( mOwnerNode, subsetParamCount );
//		mNodeRenderParam.RenderState = mOwnerNode->getSceneGraph()->getNodeDefaultRenderState();
//	
//		mRendererContext = rendererContext;
//		if ( mRendererContext == NULL )
//		{
//			mRendererContext = LN_NEW VisualNodeRendererContext();
//			mRendererContext->create( this );
//		}
//	}
//
//	//---------------------------------------------------------------------
//	//
//	//---------------------------------------------------------------------
// //   void VisualNodeRenderer::setShader( SceneShader* shader )
//	//{
//	//	LN_REFOBJ_SET( mShader, shader );
//	//}
//
////=============================================================================
//// ■ VisualNodeRendererContext
////=============================================================================
//#if 0
//	//---------------------------------------------------------------------
//	//
//	//---------------------------------------------------------------------
//    VisualNodeRendererContext::VisualNodeRendererContext()
//		: mOwnerRenderer	( NULL )
//	{
//	}
//
//	//---------------------------------------------------------------------
//	//
//	//---------------------------------------------------------------------
//    VisualNodeRendererContext::~VisualNodeRendererContext()
//	{
//	}
//
//	//---------------------------------------------------------------------
//	//
//	//---------------------------------------------------------------------
//    void VisualNodeRendererContext::create( VisualNodeRenderer* ownerRenderer )
//	{
//		mOwnerRenderer = ownerRenderer;
//	}
//
//	//---------------------------------------------------------------------
//	//
//	//---------------------------------------------------------------------
//    void VisualNodeRendererContext::releaseAllResource()
//    {
//        LN_SAFE_RELEASE( mShader );
//    }
//
//	//---------------------------------------------------------------------
//	//
//	//---------------------------------------------------------------------
//    void VisualNodeRendererContext::updateContext( VisualNodeRendererContext* parentNodeContext )
//	{
//		releaseAllResource();
//
//		mName = mOwnerRenderer->mOwnerNode->mName;
//
//		// サブセット描画パラメータ配列のリサイズ
//		if ( mSubsetRenderParamArray.size() != mOwnerRenderer->mNodeRenderParam.getSubsetCount() ) {
//			mSubsetRenderParamArray.resize( mOwnerRenderer->mNodeRenderParam.getSubsetCount() );
//		}
//
//		// LMaterial 内ではリソースの参照カウントを変化させているので、memcpy は NG
//		size_t subsetCount = mSubsetRenderParamArray.size();
//		for ( lnU32 i = 0; i < subsetCount; ++i ) {
//			mSubsetRenderParamArray[i] = mOwnerRenderer->mNodeRenderParam.Subset[i];
//		}
//
//        // 親がある場合は 0 番サブセットのコミット済み情報を受け継ぐ
//        if ( parentNodeContext )
//        {
//            const LNSubsetRenderParam& pp = parentNodeContext->mSubsetRenderParamArray[ 0 ];
//
//            for ( lnU32 i = 0; i < subsetCount; ++i )
//            {
//                LNSubsetRenderParam& pa = mSubsetRenderParamArray[ i ];
//
//                ((LVector4&)pa.ColorScale) = ((LVector4&)pa.ColorScale) * ((LVector4&)pp.ColorScale);
//                ((LVector4&)pa.BlendColor) = ((LVector4&)pa.BlendColor) + ((LVector4&)pp.BlendColor);
//                ((LVector4&)pa.Tone)       = ((LVector4&)pa.Tone)       + ((LVector4&)pp.Tone);
//            }
//        }
//
//        mRenderState = mOwnerRenderer->mNodeRenderParam.RenderState;
//
//        //mPriorityParameter = mOwnerRenderer->mPriorityParameter;    // NULL でいいかも…
//        mCombinedWorldMatrix = mOwnerRenderer->mCombinedWorldMatrix;
//        mPriority = mOwnerRenderer->mPriority;
//        mBillboardType = mOwnerRenderer->mBillboardType;
//
//		LN_REFOBJ_SET( mShader, mOwnerRenderer->mShader );
//        mIsVisible = mOwnerRenderer->mIsVisible;
//
//        // 必要なライトの数で配列を resize
//        if ( mShader )
//        {
//			if ( mAffectLightContextArray.size() < mShader->getRequiredLightNum() ) {
//				mAffectLightContextArray.resize( mShader->getRequiredLightNum() );
//			}
//        }
//
//        // 子ノードのコンテキスト
//		//VisualNodeRendererRefArray::ObjectArray& ary = mSceneNode->mChildNodeList;
//  //      int n = ary.size();
//  //      mChildSceneNodeContextArray.resize( n );
//  //      for ( int i = 0; i < n; ++i )
//  //      {
//  //          mChildSceneNodeContextArray[i] = ary[i]->mContext;
//  //      }
//	}
//
//	//---------------------------------------------------------------------
//	//
//	//---------------------------------------------------------------------
//    void VisualNodeRendererContext::preRender( ContextDrawParam& param )
//	{
//		mAffectCameraContext = param.CameraContext;
//
//        //-----------------------------------------------------
//        // Z ソート用の基準値
//
//		switch ( param.CameraContext->getZSortType() )
//        {
//            default:
//            case LN_ZSORTF_IMM_Z:
//                mZDistance = mCombinedWorldMatrix.getPosition().z;
//                break;
//            case LN_ZSORTF_CAMERA_DIST:
//                mZDistance = LVector3::squareLength( mCombinedWorldMatrix.getPosition() - param.CameraContext->getPosition() );
//                break;
//            case LN_ZSORTF_CAMERA_SCREEN:
//                mZDistance = LGeometry::PointPolyDistance(
//                    mCombinedWorldMatrix.getPosition(), 
//                    param.CameraContext->getPosition(),
//                    param.CameraContext->getDirection() );
//                break;
//        }
//
//		//std::stable_sort( mChildSceneNodeContextArray.begin(), mChildSceneNodeContextArray.end(), _cmpZAndPrioritySort );
//
//        //-----------------------------------------------------
//        // ビルボード行列計算
//
//        // 正面を向くビルボードの場合
//        if ( mBillboardType == LN_BILLBOARD_FRONT )
//        {
//            LMatrix mat;
//
//            // 現在のワールド行列から回転部分だけを取り出す
//            mat.setRotateMatrix( mCombinedWorldMatrix );
//
//            // 回転部分だけを乗算
//            mat.setMul3x3( param.CameraContext->getViewMatrixInverse() );
//
//            // 移動成分を乗算しなおす
//            mat.translation( mCombinedWorldMatrix.m30, mCombinedWorldMatrix.m31, mCombinedWorldMatrix.m32 );
//
//            mCombinedWorldMatrix = mat;
//
//            /*
//            // 現在のワールド行列から回転部分だけを取り出す
//            mBillboardMatrix.setRotateMatrix( mCombinedWorldMatrix );
//
//            // 回転部分だけを乗算
//            mBillboardMatrix.setMul3x3( camera_->getViewMatrixInverse() );
//
//            // 移動成分を乗算しなおす
//            mBillboardMatrix.translation( mCombinedWorldMatrix.m30, mCombinedWorldMatrix.m31, mCombinedWorldMatrix.m32 );
//            */
//       
//        }
//        // Y 軸回転のみ行うビルボードの場合
//        else if ( mBillboardType == LN_BILLBOARD_ROT_Y )
//        {
//            const LMatrix& view = param.CameraContext->getViewMatrix();
//
//            LMatrix mat;
//            mat.setRotateMatrix( mCombinedWorldMatrix );
//
//            if ( view.m02 > 0.0f )
//		    {
//                mat.rotationY( -atanf( view.m22 / view.m02 ) + LMath::PI / 2 );
//		    }   
//		    else if ( view.m02 == 0.0f )
//		    {
//                mat.identity();
//		    }
//		    else
//		    {
//                mat.rotationY( -atanf( view.m22 / view.m02 ) - LMath::PI / 2 );
//		    }
//
//            mat.translation( mCombinedWorldMatrix.getPosition() );
//
//            mCombinedWorldMatrix = mat;
//        }
//
//        //-----------------------------------------------------
//        // ライト列挙
//
//        // ライトが必要な場合 (mAffectLightContextArray は updateContext で resize() 済み)
//        if ( mAffectLightContextArray.size() > 0 )
//        {
//			// 自分の位置情報を渡して、最寄りのライトをもらう
//            param.SceneGraphContext->selectLights( &mAffectLightContextArray, mCombinedWorldMatrix.getPosition() );
//        }
//
//		// TODO: 視錐台カリングとかできれば
//		this->mEnableDrawing = this->mIsVisible;
//	}
//
//	//---------------------------------------------------------------------
//	//
//	//---------------------------------------------------------------------
//	bool VisualNodeRendererContext::getObjectMatrix( LMatrix* outMat, MMEVariableRequest req, int lightIndex ) const
//	{
//		switch ( req )
//        {
//            case MME_VARREQ_MATRIX_World:
//            {
//                *outMat = mCombinedWorldMatrix;
//                return true;
//            }
//            case MME_VARREQ_MATRIX_CAMERA_WorldView:
//            {
//                *outMat = mCombinedWorldMatrix * mAffectCameraContext->getViewMatrix();
//                return true;
//            }
//            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj:
//            {
//                *outMat = mCombinedWorldMatrix * mAffectCameraContext->getViewProjectionMatrix();
//                return true;
//            }
//            case MME_VARREQ_MATRIX_LIGHT_WorldView:
//            {
//                *outMat = mCombinedWorldMatrix * mAffectLightContextArray[lightIndex]->getViewMatrix();
//                return true;
//            }
//            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj:
//            {
//                *outMat = mCombinedWorldMatrix * mAffectLightContextArray[lightIndex]->getViewProjectionMatrix();
//                return true;
//            }
//
//            // 逆行列
//            case MME_VARREQ_MATRIX_World_I:
//            {
//                LMatrix::inverse( outMat, mCombinedWorldMatrix );
//                return true;
//            }
//            case MME_VARREQ_MATRIX_CAMERA_WorldView_I:
//            {
//                *outMat = mCombinedWorldMatrix * mAffectCameraContext->getViewMatrix();
//                outMat->inverse();
//                return true;
//            }
//            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_I:
//            {
//                *outMat = mCombinedWorldMatrix * mAffectCameraContext->getViewProjectionMatrix();
//                outMat->inverse();
//                return true;
//            }
//            case MME_VARREQ_MATRIX_LIGHT_WorldView_I:
//            {
//                *outMat = mCombinedWorldMatrix * mAffectLightContextArray[lightIndex]->getViewMatrix();
//                outMat->inverse();
//                return true;
//            }
//            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I:
//            {
//                *outMat = mCombinedWorldMatrix * mAffectLightContextArray[lightIndex]->getViewProjectionMatrix();
//                outMat->inverse();
//                return true;
//            }
//
//            // 転置行列
//            case MME_VARREQ_MATRIX_World_T:
//            {
//                LMatrix::transpose( outMat, mCombinedWorldMatrix );
//                return true;
//            }
//            case MME_VARREQ_MATRIX_CAMERA_WorldView_T:
//            {
//                *outMat = mCombinedWorldMatrix * mAffectCameraContext->getViewMatrix();
//                outMat->transpose();
//                return true;
//            }
//            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_T:
//            {
//                *outMat = mCombinedWorldMatrix * mAffectCameraContext->getViewProjectionMatrix();
//                outMat->transpose();
//                return true;
//            }
//            case MME_VARREQ_MATRIX_LIGHT_WorldView_T:
//            {
//                *outMat = mCombinedWorldMatrix * mAffectLightContextArray[lightIndex]->getViewMatrix();
//                outMat->transpose();
//                return true;
//            }
//            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T:
//            {
//                *outMat = mCombinedWorldMatrix * mAffectLightContextArray[lightIndex]->getViewProjectionMatrix();
//                outMat->transpose();
//                return true;
//            }
//
//            // 逆行列×転置行列
//            case MME_VARREQ_MATRIX_World_IT:
//            {
//                LMatrix::inverse( outMat, mCombinedWorldMatrix );
//                outMat->transpose();
//                return true;
//            }
//            case MME_VARREQ_MATRIX_CAMERA_WorldView_IT:
//            {
//                *outMat = mCombinedWorldMatrix * mAffectCameraContext->getViewMatrix();
//                outMat->inverse();
//                outMat->transpose();
//                return true;
//            }
//            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_IT:
//            {
//                *outMat = mCombinedWorldMatrix * mAffectCameraContext->getViewProjectionMatrix();
//                outMat->inverse();
//                outMat->transpose();
//                return true;
//            }
//            case MME_VARREQ_MATRIX_LIGHT_WorldView_IT:
//            {
//                *outMat = mCombinedWorldMatrix * mAffectLightContextArray[lightIndex]->getViewMatrix();
//                outMat->inverse();
//                outMat->transpose();
//                return true;
//            }
//            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT:
//            {
//                *outMat = mCombinedWorldMatrix * mAffectLightContextArray[lightIndex]->getViewProjectionMatrix();
//                outMat->inverse();
//                outMat->transpose();
//                return true;
//            }
//        }
//
//        return false;
//	}
//
//	//---------------------------------------------------------------------
//	//
//	//---------------------------------------------------------------------
//	void VisualNodeRendererContext::render( ContextDrawParam& param )
//	{
//		if ( this->mEnableDrawing )
//        {
//			// レンダーステートの設定
//			param.Renderer->setRenderState( this->mRenderState );
//
//			// シェーダ選択
//			SceneShader* shader = getShader();
//
//			// 優先パラメータチェック
//			bool drawing = true;
//			if ( param.OffscreenScene )
//			{
//				LNPriorityParameter* pp = param.OffscreenScene->tryGetPriorityParameter( this );
//				if ( pp )
//				{
//					shader = pp->Shader;
//					drawing = !pp->Hide;
//				}
//			}
//			//if ( mPriorityParameter )
//			//{
//			//	shader = mPriorityParameter->Shader;
//			//}
//
//			if ( drawing )
//			{
//
//				if ( shader )
//				{ 
//					// 優先シェーダ等のため、必要なライト数に不足がある場合は再列挙
//					if ( mAffectLightContextArray.size() < shader->getRequiredLightNum() )
//					{
//						mAffectLightContextArray.resize( shader->getRequiredLightNum() );
//						param.SceneGraphContext->selectLights( &mAffectLightContextArray, mCombinedWorldMatrix.getPosition() );
//					}
//
//					
//					if ( !shader->hasScriptExternal() )
//					{
//						SceneShaderTechniqueArray* techs = shader->findTechnique( MMD_PASS_object, true, false, false );
//						if ( techs )
//						{
//							shader->updateNodeParam( this );
//							this->onSetNodeParameter( shader );
//
//							int size = techs->size();
//							for ( int i = 0; i < size; ++i )
//							{
//								DrawingCommandList* list = (*techs)[i]->getDrawingCommandList();
//								list->setOwnerVisualNodeContext( this );
//								list->execute( param );
//							}
//						}
//					}
//				}
//				// シェーダが無い場合はそのまま全部 drawSubset()
//				else
//				{
//					lnU32 subsetCount = mSubsetRenderParamArray.size();
//					for ( lnU32 i = 0 ; i < subsetCount; ++i ) {
//						drawSubset( i );
//					}
//				}
//			}
//		}
//#if 0
//		if ( !mChildContextArray.empty() )
//		{
//			struct cmp {
//				static bool cmpZAndPrioritySort( const VisualNodeRendererContext* l_, const VisualNodeRendererContext* r_ )
//				{
//					if ( l_->mPriority == r_->mPriority )
//					{
//						// 降順 (遠い方から)
//						return l_->mZDistance > r_->mZDistance;
//					}
//					// 降順 (高い方から)
//					return l_->mPriority < r_->mPriority;
//				}
//			};
//
//			// 子のZソート
//			std::stable_sort( mChildContextArray.begin(), mChildContextArray.end(), cmp::cmpZAndPrioritySort );
//
//			// 子呼び出し
//			//SceneNodeArray::iterator itr = mChildNodeRefArray.getObjectArray().begin();
//			//SceneNodeArray::iterator end = mChildNodeRefArray.getObjectArray().end();
//			VisualNodeRendererContextArray::iterator itr = this->mChildContextArray.begin();
//			VisualNodeRendererContextArray::iterator end = this->mChildContextArray.end();
//			for ( ;itr != end; ++itr )
//			{
//				(*itr)->render( param );
//			}
//		}
//#endif
//	}
//
//	//---------------------------------------------------------------------
//	//
//	//---------------------------------------------------------------------
//	int VisualNodeRendererContext::getSubsetCount() 
//	{ 
//		return mSubsetRenderParamArray.size();
//	}
//
//#endif
} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================