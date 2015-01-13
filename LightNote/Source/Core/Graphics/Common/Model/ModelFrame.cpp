//==============================================================================
// ModelFrame 
//==============================================================================

#include "stdafx.h"
#include "Core/ModelCore.h"
#include "Core/ModelFrameCore.h"
#include "Model.h"
#include "ModelFrame.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ModelFrame2
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ModelFrame2::ModelFrame2()
		: mModelFrameCore	( NULL )
		, mParentFrame		( NULL )
	{
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ModelFrame2::~ModelFrame2()
	{
		LN_SAFE_RELEASE( mModelFrameCore );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelFrame2::create( Model2* ownerModel, ModelFrameCore* frameCore )
	{
		mOwnerModel = ownerModel;
		LN_REFOBJ_SET( mModelFrameCore, frameCore );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	const lnRefStringW& ModelFrame2::getName() const 
	{ 
		return mModelFrameCore->Name; 
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ModelFrame2::setGlobalMatrix( const LMatrix& matrix, int updateDepth )
    {
        mCombinedGlobalMatrix = matrix;

	    if ( updateDepth > 0 )
	    {
		    --updateDepth;

		    // 子フレーム更新
			ln_foreach( ModelFrame2* c, mChildren )
			{
                LMatrix mat = c->mLocalTransform;
                mat.translation( mModelFrameCore->OffsetFromParent );
                mat *= mCombinedGlobalMatrix;
			    c->setGlobalMatrix( mat, updateDepth );
		    }
	    }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelFrame2::addChildFrame( ModelFrame2* frame )
	{
		LN_ERR2_ASSERT( frame->mParentFrame == NULL );

		frame->mParentFrame = this;
		mChildren.push_back( frame );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelFrame2::updateGlobalMatrixHierarchy( const LMatrix& parentMatrix )
	{
		// mLocalTransform はアニメーションが適用されている
		mCombinedGlobalMatrix = mLocalTransform;

		// 親からの平行移動量
		mCombinedGlobalMatrix.translation( mModelFrameCore->OffsetFromParent );

		

		//if ( getFrameCore()->Name == L"左足ＩＫ" )
		//{
		//	wprintf(L"%s ", getFrameCore()->Name);
		//	mLocalTransform.Translation.dump();
		//	mCombinedGlobalMatrix.getPosition().dump();
		//	//getGlobalMatrix().getPosition().dump();
		//	mCombinedGlobalMatrix.translation(1000, 0, 0);
		//}

		// 親行列と結合
		//if ( mParentFrame != NULL )
			mCombinedGlobalMatrix *= parentMatrix;

		// 子フレーム更新
		ln_foreach( ModelFrame2* c, mChildren )
		{
			c->updateGlobalMatrixHierarchy( mCombinedGlobalMatrix );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelFrame2::updateGlobalMatrix( bool hierarchical )
	{
		mCombinedGlobalMatrix = mLocalTransform;
		mCombinedGlobalMatrix.translation( mModelFrameCore->OffsetFromParent );
		if ( mParentFrame != NULL )
			mCombinedGlobalMatrix *= mParentFrame->mCombinedGlobalMatrix;

		// 子フレーム更新
		if ( hierarchical )
		{
			ln_foreach( ModelFrame2* c, mChildren )
			{
				c->updateGlobalMatrix( hierarchical );
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelFrame2::bestow()
	{
		// 移動付与
		if ( mModelFrameCore->IsMoveProvided )
        {
			ModelFrame2* parent = mOwnerModel->getFrame( mModelFrameCore->ProvidedParentBoneIndex );
			LVector3 t;
			LVector3::lerp( 
				&t, 
				LVector3::ZERO, 
				parent->getLocalTransformPtr()->Translation,
				mModelFrameCore->ProvidedRatio );
			getLocalTransformPtr()->Translation += t;
        }

		// 回転付与
        if ( mModelFrameCore->IsRotateProvided )
        {
			ModelFrame2* parent = mOwnerModel->getFrame( mModelFrameCore->ProvidedParentBoneIndex );
			LQuaternion q;
			LQuaternion::slerp( &q, LQuaternion::IDENTITY, parent->getLocalTransformPtr()->Rotation, mModelFrameCore->ProvidedRatio );
            getLocalTransformPtr()->Rotation *= q;
        }
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	const Animation::lnRefKeyString& ModelFrame2::getAnimationTargetName() const 
	{ 
		return mModelFrameCore->Name; 
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelFrame2::onUpdateAnimationValue( Animation::ValueType type, void* value )
	{
		if ( type == Animation::ValueType_SQTTransform )
		{

			mLocalTransform = *((LSQTTransform*)value);
		}
	}

//==============================================================================
// ■ ModelFrame クラス
//==============================================================================

	//----------------------------------------------------------------------
	// ● コンストラクタ
	//----------------------------------------------------------------------
    ModelFrame::ModelFrame()
        : mModelCore        ( NULL )
        , mIndex            ( 0 )
        , mParent           ( NULL )
        , mIKLimitAngle     ( false )
    {
    }

	//----------------------------------------------------------------------
	// ● デストラクタ
	//----------------------------------------------------------------------
    ModelFrame::~ModelFrame()
    {
    }

	//----------------------------------------------------------------------
	// ● ワールド行列を階層的に更新する
	//----------------------------------------------------------------------
    void ModelFrame::updateTransform( const LMatrix& parent_mat_ )
    {
        // ローカルな変換にワールドを適用
	    mWorldMatrix = mLocalTransform;
        mWorldMatrix.translation( mOffset );
        mWorldMatrix *= parent_mat_;

	    // 子供の計算
#if 1
        ModelFrameList::iterator itr = mChildList.begin();
	    ModelFrameList::iterator end = mChildList.end();
	    for ( ; itr != end; ++itr )
        {
            (*itr)->updateTransform( mWorldMatrix );
        }
#else
	    //ModelFrameList::iterator itr = mChildList.begin();
	    //ModelFrameList::iterator end = mChildList.end();
	    //for ( ; itr != end; ++itr )
        int size = mChildList.size();
        if (size)
        {
            ModelFrame** fs = &mChildList[0];
            for (int i = 0; i < size; ++i, ++fs)
	        {
                (*fs)->updateTransform( mWorldMatrix );

		        //(*itr)->updateTransform( mWorldMatrix );
	        }
        }
#endif
    }

	//----------------------------------------------------------------------
	// ● フレームのワールド行列を設定する
	//----------------------------------------------------------------------
    void ModelFrame::setWorldMatrix( const LMatrix& matrix_, lnU32 update_depth_ )
    {
        mWorldMatrix = matrix_;

	    if ( update_depth_ > 0 )
	    {
		    --update_depth_;

		    // 子供の計算
		    ModelFrameList::iterator itr = mChildList.begin();
		    ModelFrameList::iterator end = mChildList.end();
		    for ( ; itr != end; ++itr )
		    {
                LMatrix mat = (*itr)->mLocalTransform;
                mat.translation( mOffset ); // TODO ここでいいのかチェックがいるかも
                mat *= mWorldMatrix;
			    (*itr)->setWorldMatrix( mat, update_depth_ );
		    }
	    }
    }

	//----------------------------------------------------------------------
	// ● スキニング用行列の更新
	//----------------------------------------------------------------------
    void ModelFrame::updateSkinningMatrix()
    {
        mSkinningMatrix =  mInvTransform;
        mSkinningMatrix *= mWorldMatrix;
    }



	//----------------------------------------------------------------------
	// ● 初期化
	//----------------------------------------------------------------------
    void ModelFrame::initializePMD( ModelCore* model_core_, const PMD_Bone* bone_data_, lnU32 index_, ModelFrame* frames_ )
    {
        mModelCore = model_core_;
        mIndex = index_;

        // ボーン名のコピー (ボーン名が 20 で、終端がなかった場合の対処)
        char name[ 21 ];
        strncpy( name, bone_data_->szName, 20 );
	    name[ 20 ] = '\0';
        mName = name;

	    // 位置のコピー
	    mOrgPosition = bone_data_->vec3Position;

        
	    // 親ボーンの設定
	    if ( bone_data_->nParentNo != -1 )
	    {
            mParent = &frames_[ bone_data_->nParentNo ];
            mOffset = mOrgPosition - mParent->mOrgPosition;
	    }
        // 親なし
	    else
	    {		    
		    mOffset = mOrgPosition;
	    }

	    // 子ボーンの設定
	    //if( pPMDBoneData->nChildNo != -1 )
	    //{
		   // m_pChildBone = (cPMDBone *)&(pBoneArray[pPMDBoneData->nChildNo]);
	    //}

        /*
            初期姿勢 = スキニングしなくても同じ姿勢。
            ボーンは最初からオフセットが入ってるけど、
            それをスキニングに適用すると姿勢が崩れてしまう。
            そのため、初期オフセットを打ち消す処理が必要。それがこの行列。

            いままで意識してなかったけど、ID3DXSkinInfo::GetBoneOffsetMatrix() で
            取得できる行列 (SkinnedMeshサンプルの D3DXMESHCONTAINER_DERIVED::pBoneOffsetMatrices) が
            これにあたるものっぽい。
            サンプルでも描画の直前に対象ボーン行列にこの行列を乗算している。
        */
        LMatrix::translation(
            &mInvTransform,
            -mOrgPosition.x,
            -mOrgPosition.y,
            -mOrgPosition.z );	 

	    mIKLimitAngle = false;

	    // 各変数の初期値を設定
	    reset();
    }

	//----------------------------------------------------------------------
	// ● PMD初期化後処理 (子リスト作成、オフセット再計算)
	//----------------------------------------------------------------------
    void ModelFrame::postInitializePMD()
    {
        if ( mParent )
        {
            mOffset = mOrgPosition - mParent->mOrgPosition;
            mParent->mChildList.push_back( this );
        }
    }

	//----------------------------------------------------------------------
	// ● 各変数の初期値を設定
	//----------------------------------------------------------------------
    void ModelFrame::reset()
    {
        mLocalTransform = LSQTTransform::IDENTITY;
        mLocalTransform.Translation = LVector3::ZERO;//mOffset;///mOrgPosition;//
        //mWorldMatrix = mLocalTransform;
        updateWorldMatrixFromLocalAndParent();

    }

	//----------------------------------------------------------------------
	// ● mLocalTransform と mParent->mWorldMatrix から mWorldMatrix を作る
	//----------------------------------------------------------------------
    void ModelFrame::updateWorldMatrixFromLocalAndParent()
    {
        mWorldMatrix = mLocalTransform;
        mWorldMatrix.translation( mOffset );
        if ( mParent ) mWorldMatrix *= mParent->mWorldMatrix;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelFrame::onUpdateAnimationValue( Animation::ValueType type, void* value )
	{
		if ( type == Animation::ValueType_SQTTransform )
		{
			mLocalTransform = *((LSQTTransform*)value);
		}
	}


} // namespace Graphics
} // namespace Core
} // namespace LNote
