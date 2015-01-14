//==============================================================================
// Model 
//==============================================================================

#include "stdafx.h"
#include "Core/ModelCore.h"
#include "ModelManager.h"
#include "ModelFace.h"
#include "ModelIK.h"
#include "ModelRigidBody.h"
#include "ModelConstraint.h"
#include "MotionController.h"
#include "Model.h"


#include "Core/ModelMaterialCore.h"
#include "Core/ModelFrameCore.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// Model
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Model2::Model2()
		: mModelCore			( NULL )
		, mMaterial				( NULL )
		, mRigidResetCount		( 0 )
		, mAnimationController	( NULL )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Model2::~Model2()
	{
		LN_SAFE_RELEASE( mAnimationController );
		LN_SAFE_RELEASE( mMaterial );
		LN_SAFE_RELEASE( mModelCore );
		SAFE_DELETE_ARRAY( mSkinningMatrices );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Model2::create( ModelCore2* modelCore, lnU32 flags )
	{
		LN_REFOBJ_SET( mModelCore, modelCore );
		mIKSolver.create( this );

		// Materials
		if ( flags & ModelCreateFlag_CopyMaterials ) {
			mMaterial = mModelCore->Material.copy();
		}
		else {
			LN_REFOBJ_SET( mMaterial, &mModelCore->Material );
		}

		// Frames
		mFrames.resize( mModelCore->FrameArray.size() );
		for (int i = 0; i < mFrames.size(); i++ )
		{
			// 先にすべて create
			mFrames[i].create( this, mModelCore->FrameArray[i] );
		}
		for (int i = 0; i < mFrames.size(); i++ )
		{
			// 次に親と関連付ける
			int parentIndex = mModelCore->FrameArray[i]->ParentFrameIndex;
			if ( 0 <= parentIndex && parentIndex < mFrames.size() )
				mFrames[parentIndex].addChildFrame( &mFrames[i] );
			else
				mRootFrames.push_back( &mFrames[i] );	// 親がいない

			// IK ボーン
			//if ( mModelCore->FrameArray[i]->IsIK ) {
			//	mIKSolver.addEffectorFrame( &mFrames[i] );
			//}
		}

		// Morphs
		mMorphs.resize( mModelCore->ModelMorphCores.size() );
		for (int i = 0; i < mMorphs.size(); i++ )
		{
			mMorphs[i].create( this, mModelCore->ModelMorphCores[i] );
		}

		// RigidBody
		mRigidBodys.resize( mModelCore->RigidBodys.size() );
		for (int i = 0; i < mRigidBodys.size(); i++ )
		{
			mRigidBodys[i].create( this, mModelCore->RigidBodys[i] );
		}

		// Joints
		mJoints.resize( mModelCore->Constraints.size() );
		for (int i = 0; i < mModelCore->Constraints.size(); i++ )
		{
			mJoints[i].create( this, mModelCore->Constraints[i] );
		}

		// スキニング行列
		mSkinningMatrices = LN_NEW LMatrix[mFrames.size()];

		// アニメーションコントローラ
		mAnimationController = LN_NEW ModelAnimationController2();
		mAnimationController->create( this );

		resetRigidBody();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelFrame2* Model2::findFrame( const wchar_t* name )
    {
		for ( int i = 0; i < mFrames.size(); i++ )
		{
			if ( mFrames[i].getName() == name )
				return &mFrames[i];
		}
		return NULL;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Model2::setWorldMatrix( const LMatrix& matrix )
	{
		mWorldMatrix = matrix;
		mWorldMatrixInv = LMatrix::Inverse(mWorldMatrix);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Model2::preUpdateTransform()
    {

		// ルートフレームから階層更新
		ln_foreach( ModelFrame2* frame, mRootFrames )
		{
			frame->updateGlobalMatrixHierarchy( LMatrix::Identity );
		}

		
		

		// IK 更新
		mIKSolver.updateIKFrames();

		// 付与
		ln_foreach( ModelFrame2& frame, mFrames )
		{
			frame.bestow();
		}

		// ルートフレームから階層更新
		ln_foreach( ModelFrame2* frame, mRootFrames )
		{
			frame->updateGlobalMatrixHierarchy( LMatrix::Identity );
		}




		// 剛体位置をフレーム位置へ
		ln_foreach( ModelRigidBody2& body, mRigidBodys )
		{
			body.updateBeforePhysics( mWorldMatrix );
		}
		// 剛体位置をフレーム位置へ
		ln_foreach( ModelJoint& joint, mJoints )
		{
			//joint.updateBeforePhysics();
		}
		
	
        if ( mRigidResetCount > 0 )
        {
            ln_foreach( ModelRigidBody2& body, mRigidBodys )
			{
				body.moveBodyForced();
			}
            --mRigidResetCount;
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Model2::postUpdateTransform( const LMatrix& externalGlobalMatrix )
	{

		// フレーム位置を剛体位置へ
		ln_foreach( ModelRigidBody2& body, mRigidBodys )
		{
			body.updateAfterPhysics( mWorldMatrixInv );
		}

        

		// スキニング行列の作成
		for ( int i = 0; i < mFrames.size(); ++i )
		{
			mSkinningMatrices[i] =  mFrames[i].getFrameCore()->getSkinningOffsetInv();
			mSkinningMatrices[i] *= mFrames[i].getGlobalMatrix();
			
		}

		
		// 全てのローカルトランスフォームをリセットする
		//		リセットしておかないと、IKで問題が出る。
		//		(IKはその時点のLocalTransformに対して処理を行うため、回転角度がどんどん増えたりする)
		//		なお、一連の更新の最後で行っているのは、アニメーションからの更新を
		//		外部で行っているため。
		ln_foreach( ModelFrame2& frame, mFrames )
		{
			*frame.getLocalTransformPtr() = LSQTTransform::Identity;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Model2::drawSubset( int index )
	{
        IRenderer* r = mModelCore->Manager->getGraphicsDevice()->getRenderer();
        r->setVertexBuffer( mModelCore->VertexBuffer );
        r->setIndexBuffer( mModelCore->IndexBuffer );
        r->drawPrimitiveIndexed(
			PrimitiveType_TriangleList,
			mMaterial->Attributes[index].StartIndex, 
			mMaterial->Attributes[index].PrimitiveNum );
	}


//==============================================================================
// ■ Model クラス
//==============================================================================

	//----------------------------------------------------------------------
	// ● コンストラクタ
	//----------------------------------------------------------------------
    Model::Model( ModelManager* manager_ )
        : mManager          ( manager_ )
        , mGraphicsDevice   ( manager_->getGraphicsDevice() )
        , mModelCore        ( NULL )
        , mAttrNum          ( 0 )
        , mMaterials        ( NULL )
        , mAttributes       ( NULL )
        , mFrameNum         ( 0 )
        , mModelFrames      ( NULL )
        , mRootFrame        ( NULL )
        , mRigidBodyNum     ( 0 )
        , mModelRigidBodys  ( NULL )
        , mConstraintNum    ( 0 )
        , mConstraints      ( NULL )
        , mMotionController ( NULL )
		, mAnimationController	( NULL )
        , mRigidResetCount  ( 0 )
		, mEnablePhysics	( true )
    {
    }

	//----------------------------------------------------------------------
	// ● デストラクタ
	//----------------------------------------------------------------------
    Model::~Model()
    {
		LN_SAFE_RELEASE( mAnimationController );

        for ( lnU32 i = 0; i < mConstraintNum; ++i )
        {
            SAFE_DELETE( mConstraints[ i ] );
        }
        SAFE_DELETE_ARRAY( mConstraints );

        SAFE_DELETE_ARRAY( mMaterials );
        SAFE_DELETE_ARRAY( mAttributes );
        SAFE_DELETE_ARRAY( mModelFrames );

		for ( lnU32 i = 0; i < mRigidBodyNum; ++i )
        {
            SAFE_DELETE( mModelRigidBodys[ i ] );
        }
        SAFE_DELETE_ARRAY( mModelRigidBodys );

		ln_foreach( ModelFace* face, mModelFaceArray )
		{
			face->release();
		}
		

        
        LN_SAFE_RELEASE( mModelCore );

        
 
        SAFE_DELETE( mMotionController );
        
    }

	//----------------------------------------------------------------------
	// ● フレームを検索する
	//----------------------------------------------------------------------
    ModelFrame* Model::findFrame( const lnChar* name_ )
    {
        lnU32 idx = mModelCore->getFrameIndexByName( name_ );
        if ( idx == 0xffffffff ) return NULL;
        return &mModelFrames[ idx ];
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Model::setEnablePhysics( bool flag )
	{
		mEnablePhysics = flag;
		//updateSkinningMatrix();
	}

	//----------------------------------------------------------------------
	// ● すべての剛体を強制的にボーンの位置に移動させる
	//----------------------------------------------------------------------
    void Model::resetRigidBody()
    {
        mRigidResetCount = 3;
    }

	//----------------------------------------------------------------------
	// ● すべてのフレーム行列を階層的に更新する
	//----------------------------------------------------------------------
    void Model::updateFrameMatrix(bool ik_)
    {
        if ( mRootFrame )
        {
            mRootFrame->updateTransform( LMatrix() );
        }

        // 親のないフレームの更新
        const NoParentFrameIndexArray& ary = mNoParentFrameIndexArray;
        if ( !ary.empty() && ik_ )
        {
            NoParentFrameIndexArray::const_iterator itr = ary.begin();
            NoParentFrameIndexArray::const_iterator end = ary.end();
            for ( ; itr != end; ++itr )
            {
                mModelFrames[ (*itr) ].updateTransform( LMatrix() );
            }
        }


        if ( mRigidResetCount > 0 )
        {
            for ( lnU32 i = 0; i < mRigidBodyNum; ++i )
	        {
		        mModelRigidBodys[ i ]->moveToBonePos();
	        }
            --mRigidResetCount;
        }
    }
    
	//----------------------------------------------------------------------
	// ● 
	//----------------------------------------------------------------------
    void Model::postUpdate( lnFloat elapsed_time_ )
    {
        // ボーン位置合わせ (剛体を移動させる)
        //      ボーンのワールド行列が必要。
        //      この関数の前に ModelFrame::mWorldMatrix が確定している必要がある。
        for ( lnU32 i = 0 ; i < mRigidBodyNum ; ++i )
	    {
		    mModelRigidBodys[ i ]->fixPosition( elapsed_time_ ); // もともとは elapsed_frame_
	    }

        // IK 更新
        //      この中では ModelFrame::mRotation を書き換え、
        //      ModelFrame::updateMatrixInverse() を呼んで mWorldMatrix を更新している。
        for ( lnU32 i = 0 ; i < mModelCore->mIKNum ; ++i )
	    {
		    mModelCore->mIKArray[ i ].update( mModelFrames ); // mModelFramesは仮
	    }
    }

	//----------------------------------------------------------------------
	// ● スキニング行列の更新
	//----------------------------------------------------------------------
    void Model::updateSkinningMatrix()
    {
		if ( mEnablePhysics )
		{
			// 物理演算反映 (ボーンの位置を剛体に合わせる)
			for ( lnU32 i = 0; i < mRigidBodyNum; ++i )
  			{
				mModelRigidBodys[ i ]->updateBoneTransform();
			}
		}

        // スキニング用行列の更新
	    for ( lnU32 i = 0; i < mFrameNum; ++i )
	    {
		    mModelFrames[ i ].updateSkinningMatrix();
	    }
    }

	//----------------------------------------------------------------------
	// ● 剛体の位置を現在のボーンの位置にあわせる
	//----------------------------------------------------------------------
	//void Model::resetRigidBodyPos()
	//{
	   // for ( lnU32 i = 0; i < mRigidBodyNum; ++i )
	   // {
		  //  mModelRigidBodys[ i ].moveToBonePos();
	   // }
	//}

	//----------------------------------------------------------------------
	// ● ソフトウェアスキニング
	//----------------------------------------------------------------------
    void Model::skinning()
    {
        if ( mModelFrames )
        {
            mModelCore->skinning( mModelFrames );
        }
    }

	//----------------------------------------------------------------------
	// ● サブセットの描画
	//----------------------------------------------------------------------
    LNRESULT Model::drawSubset( lnU32 idx_ )
    {
        IRenderer* r = mGraphicsDevice->getRenderer();
        r->setVertexBuffer( mModelCore->mVertexBuffer );
        r->setIndexBuffer( mModelCore->mIndexBuffer );
        r->drawPrimitiveIndexed(
			PrimitiveType_TriangleList, mAttributes[idx_].StartIndex, mAttributes[idx_].PrimitiveNum);
		return ResultCode_OK;
    }



//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================