﻿//==============================================================================
// MotionController 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "ModelFrame.h"
#include "ModelFace.h"
#include "Model.h"
#include "MotionController.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ModelAnimationController2
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ModelAnimationController2::ModelAnimationController2()
		: mOwnerModel	( NULL )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ModelAnimationController2::~ModelAnimationController2()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelAnimationController2::create( Model2* ownerModel )
	{
		mOwnerModel = ownerModel;

		// フレームをアニメーションターゲットとして登録
		for ( int i = 0; i < mOwnerModel->getFrameCount(); i++ )
		{
			addAnimationApplyTarget( mOwnerModel->getFrame( i ) );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelAnimationController2::advanceTime( lnFloat deltaTime )
	{
		Animation::AnimationController::advanceTime( deltaTime );
	}


//==============================================================================
// ■ ModelAnimationController
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ModelAnimationController::ModelAnimationController( Model* parentModel )
		: mParentModel	( parentModel )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ModelAnimationController::~ModelAnimationController() 
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelAnimationController::advanceTime( lnFloat deltaTime )
	{
		// モーション更新前に表情をリセット
		ModelFaceArray& faceArray = mParentModel->getModelFaceArray();
		if ( !faceArray.empty() )
		{
			IVertexBuffer* vb = mParentModel->getModelCore()->getVertexBuffer();
			Vertex::PMDVertex* v = (Vertex::PMDVertex*)vb->lock();
			faceArray[0]->getFaceCore()->applyFace( v );
			vb->unlock();
		}

		// ボーン、表情を更新
		Animation::AnimationController::advanceTime( deltaTime );
	}



//==============================================================================
// ■ MotionController クラス
//==============================================================================

	//----------------------------------------------------------------------
	// ● コンストラクタ
	//----------------------------------------------------------------------
    MotionController::MotionController( ModelFrame* frames_, lnU32 frame_num_, ModelFace* faces_, lnU32 face_num_ )
        : mModelFrameNum    ( frame_num_ )
        , mModelFrames      ( frames_ )
        , mModelFaceNum     ( face_num_ )
        , mModelFaces       ( faces_ )
    {
        for ( int i = 0; i < LN_MAX_ATTACH_MOTIONS; ++i )
        {
            mAttachedMotionDataArray[ i ].initialize( mModelFrameNum );
        }
    }

	//----------------------------------------------------------------------
	// ● デストラクタ
	//----------------------------------------------------------------------
    MotionController::~MotionController()
    {
        for ( int i = 0; i < LN_MAX_ATTACH_MOTIONS; ++i )
        {
            LN_SAFE_RELEASE( mAttachedMotionDataArray[ i ].AttachedMotion );
        }
    }

	//----------------------------------------------------------------------
	// ● モーションのアタッチ
	//----------------------------------------------------------------------
    void MotionController::attachMotion( lnU32 slot_idx_, Motion* motion_ )
    {
        LN_SAFE_RELEASE( mAttachedMotionDataArray[ slot_idx_ ].AttachedMotion );
        mAttachedMotionDataArray[ slot_idx_ ].AttachedMotion = motion_;
        LN_SAFE_ADDREF( mAttachedMotionDataArray[ slot_idx_ ].AttachedMotion );

        // ボーン名に対応するアニメーションを検索する
        for ( lnU32 i = 0; i < mModelFrameNum; ++i )
        {
            mAttachedMotionDataArray[ slot_idx_ ].BoneAnimations[ i ] =
                motion_->findBoneAnimation( mModelFrames[ i ].getName().c_str() );

            //_p(mModelFrames[ i ].getName().c_str());
            //_p(mAttachedMotionDataArray[ slot_idx_ ].BoneAnimations[ i ]);
        }

        // 表情名に対応するアニメーションを検索する
        for ( lnU32 i = 0; i < mModelFaceNum; ++i )
        {
            mAttachedMotionDataArray[ slot_idx_ ].FaceAnimations[ i ] =
                motion_->findFaceAnimation( mModelFaces[ i ].getName().c_str() );
        }

        // 時間初期化
        mAttachedMotionDataArray[ slot_idx_ ].Time = 0.0;
    }

	//----------------------------------------------------------------------
	// ● 時間を進める、ボーン行列を更新する (秒)
	//----------------------------------------------------------------------
    void MotionController::advanceTime( lnFloat elapsed_time_ )
    {
        // モーションのアタッチされているトラックの時間を進める
        for ( int i = 0; i < LN_MAX_ATTACH_MOTIONS; ++i )
        {
            if ( mAttachedMotionDataArray[ i ].AttachedMotion )
            {
                mAttachedMotionDataArray[ i ].Time += elapsed_time_;
            }
        }


        for ( int i = 0; i < LN_MAX_ATTACH_MOTIONS; ++i )
        {
            if ( mAttachedMotionDataArray[ i ].AttachedMotion )
            {
                for ( lnU32 frame_idx = 0; frame_idx < mModelFrameNum; ++frame_idx )
                {
                    if ( mAttachedMotionDataArray[ i ].BoneAnimations[ frame_idx ] )
                    {
                        mAttachedMotionDataArray[ i ].BoneAnimations[ frame_idx ]->setTime(
                            mAttachedMotionDataArray[ i ].Time );

                        *(mModelFrames[ frame_idx ].getLocalTransformPtr()) =
                            mAttachedMotionDataArray[ i ].BoneAnimations[ frame_idx ]->getTransform();
                    }
                }
            }
        }
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