//==============================================================================
// ModelIK 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "ModelFrame.h"
#include "Core/ModelCore.h"
#include "Core/ModelFrameCore.h"
#include "Core/ModelIKLimitter.h"
#include "Core/ModelIKCore.h"
#include "ModelIK.h"

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
// ModelIK
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelIK2::ModelIK2()
		: mIKCore	( NULL )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelIK2::~ModelIK2()
	{
		LN_SAFE_RELEASE( mIKCore );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ModelIK2::create( ModelIKCore* ikCore )
	{
		LN_REFOBJ_SET( mIKCore, ikCore );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ModelIK2::solve( ModelFrame2* frames )
	{
		solveCCD( frames );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelIK2::solveCCD( ModelFrame2* frames )
	{
		// ※名前すこしまぎらわしいので注意
		ModelFrame2* targetFrame = &frames[mIKCore->TargetBoneIndex];	// IK ボーン
		ModelFrame2* effFrame    = &frames[mIKCore->EffBoneIndex];		// IK ターゲットボーン

		int linkCount = mIKCore->IKBoneIndexArray.size();

		// グローバル行列を仮更新する
		for ( short j = linkCount - 1 ; j >= 0 ; --j )
        {
            frames[mIKCore->IKBoneIndexArray[j]].updateGlobalMatrix( false );
        }
        effFrame->updateGlobalMatrix( false );

        // IKボーン位置の取得(目標位置)
        LVector3 targetPos = targetFrame->getGlobalMatrix().GetPosition();

		for ( lnU16 it = 0; it < mIKCore->CalcCount; ++it )
        {
            for( lnU8 iLink = 0 ; iLink < linkCount ; ++iLink )
		    {
				// 動かすノード
                ModelFrame2* frame = &frames[ mIKCore->IKBoneIndexArray[iLink] ];
                //LVector3 jointPos = frame->getGlobalMatrix().getPosition();

                // エフェクタの位置
                LVector3 effPos = effFrame->getGlobalMatrix().GetPosition();

                // ワールド座標系から注目ノードの局所座標系への変換
				LMatrix invCoord = LMatrix::Inverse(frame->getGlobalMatrix());

                // 各ベクトルの座標変換を行い、検索中のボーンi基準の座標系にする
                // (1) 注目ノード→エフェクタ位置へのベクトル(a)(注目ノード)
				LVector3 localEffPos = LVector3::TransformCoord(effPos, invCoord);
                
                // (2) 基準関節i→目標位置へのベクトル(b)(ボーンi基準座標系)
				LVector3 localTargetPos = LVector3::TransformCoord(targetPos, invCoord);

                // (1) 基準関節→エフェクタ位置への方向ベクトル
                localEffPos.Normalize();
			    // (2) 基準関節→目標位置への方向ベクトル
                localTargetPos.Normalize();

				// 回転角
				float rotationDotProduct = LVector3::Dot( localEffPos, localTargetPos ); 
                float rotationAngle = acosf( rotationDotProduct );
               
                //回転量制限をかける
                if ( rotationAngle > LMath::PI * mIKCore->Fact * (iLink + 1))
                     rotationAngle = LMath::PI * mIKCore->Fact * (iLink + 1);
                if ( rotationAngle < -LMath::PI * mIKCore->Fact * (iLink + 1))
                     rotationAngle = -LMath::PI * mIKCore->Fact * (iLink + 1);

                // 回転軸
				LVector3 rotationAxis = LVector3::Cross(localEffPos, localTargetPos);
                
				if (frame->getFrameCore()->IKLimitter)
					frame->getFrameCore()->IKLimitter->adjustAxisLimits( &rotationAxis );
				rotationAxis.Normalize();
            
				if (!LMath::IsNaN(rotationAngle) && rotationAngle > 1.0e-3f && !rotationAxis.IsNaNOrInf())
			    {
                    // 関節回転量の補正
                    LQuaternion rotQuat(rotationAxis, rotationAngle);
                    
					frame->getLocalTransformPtr()->Rotation = LQuaternion::Multiply(frame->getLocalTransformPtr()->Rotation, rotQuat);
                    if (frame->getFrameCore()->IKLimitter)
						frame->getFrameCore()->IKLimitter->adjustRotation( frame->getLocalTransformPtr() );
					
                    // IK構成ボーンのグローバル行列を再更新する
					for ( short j = linkCount - 1 ; j >= 0 ; --j )
					{
						frames[mIKCore->IKBoneIndexArray[j]].updateGlobalMatrix( false );
					}
					effFrame->updateGlobalMatrix( false );
                }
            }
        }
	}


//==============================================================================
// ■ ModelIKLimitter クラス
//==============================================================================

	//----------------------------------------------------------------------
	// ● 回転角度制限の適用
	//----------------------------------------------------------------------
    bool ModelIKLimitter::adjustRotation( ModelFrame* bone_ )
    {
        IKRotationLimits::iterator itr = mIKRotationLimits.find( bone_->getName().c_str() );
        if ( itr == mIKRotationLimits.end() ) return false;

        LQuaternion& bone_rot = bone_->getLocalTransformPtr()->Rotation;

#if 1

		LVector3 euler = bone_rot.ToEulerAngles();

        bool limit = false;
        IKRotationLimit& lim = itr->second;
        limit |= lim.adjust( &euler.X, 0 );
        limit |= lim.adjust( &euler.Y, 1 );
        limit |= lim.adjust( &euler.Z, 2 );

		bone_rot = LQuaternion::RotationEulerAngles(euler);

        return limit;

#else

        
        //bone_rot.cdump();

        lnFloat xrot, yrot, zrot;
        int factoring_type;

        // まずはXYZで分解 (ジンバルロックするようなら順序を変えて処理していく)
        if ( !LQuaternion::factoringXYZ( &xrot, &yrot, &zrot, bone_rot ) )
        {
            // YZXで分解
            if ( !LQuaternion::factoringYZX( &yrot, &zrot, &xrot, bone_rot ) )
            {
                // ZXYで分解
                LQuaternion::factoringZXY( &zrot, &xrot, &yrot, bone_rot );
                factoring_type = 2;
            }
            else
            {
                factoring_type = 1;
            }
        }
        else
        {
            factoring_type = 0;
        }

        // 制限適用
        bool limit = false;
        IKRotationLimit& lim = itr->second;
        limit |= lim.adjust( &xrot, 0 );
        limit |= lim.adjust( &yrot, 1 );
        limit |= lim.adjust( &zrot, 2 );


        //if ( xrot <= lim.mMinRot[0] )
        //{
        //    printf("min %f %f\n", xrot, lim.mMinRot[0]);
        //}

        //if ( xrot >= lim.mMaxRot[0] )
        //{
        //    printf("max %f %f %f\n", xrot, lim.mMaxRot[0], pre);
        //}


        
        if ( factoring_type == 0 )
        {
            LMatrix mat;
            mat.rotationX( xrot );
            mat.rotationY( yrot );
            mat.rotationZ( zrot );
            LQuaternion::rotationMatrix( &bone_rot, mat );
        }
        else if ( factoring_type == 1 )
        {
            _p(factoring_type);
            LMatrix mat;
            mat.rotationY( yrot );
            mat.rotationZ( zrot );
            mat.rotationX( xrot );
            LQuaternion::rotationMatrix( &bone_rot, mat );
        }
        else
        {
            _p(factoring_type);
            LMatrix mat;
            mat.rotationZ( zrot );
            mat.rotationX( xrot );
            mat.rotationY( yrot );
            LQuaternion::rotationMatrix( &bone_rot, mat );
        }

        return limit;

#endif
    }

//==============================================================================
// ■ ModelIK クラス
//==============================================================================

	//----------------------------------------------------------------------
	// ● コンストラクタ
	//----------------------------------------------------------------------
    ModelIK::ModelIK()
        : mModelCore        ( NULL )
        , mTargetBoneIdx    ( 0 )
        , mEffBoneIdx       ( 0 )
        , mCalcCount        ( 0 )
        , mFact             ( 0 )
        , mSortVal          ( 0 )
        , mLinkNum          ( 0 )
        , mBoneIndexList    ( NULL )
    {
    }

	//----------------------------------------------------------------------
	// ● デストラクタ
	//----------------------------------------------------------------------
    ModelIK::~ModelIK()
    {
        _release();
    }

	//----------------------------------------------------------------------
	// ● 初期化
	//----------------------------------------------------------------------
    void ModelIK::initializePMD( ModelCore* model_core_, const PMD_IK* ik_data_, ModelFrame* frames_ )
    {
        _release();

        mModelCore = model_core_;

        // IKターゲットボーン
	    mTargetBoneIdx = ik_data_->nTargetNo;

	    // IK先端ボーン
	    mEffBoneIdx = ik_data_->nEffNo;

	    mCalcCount  = ik_data_->unCount;
	    mFact       = ik_data_->fFact * 3.1415926f;
	    mSortVal    = ik_data_->punLinkNo[0];

	    // IKリンク配列の作成
	    mLinkNum = ik_data_->cbNumLink;
	    mBoneIndexList = LN_NEW lnU32[ mLinkNum ];
	    for( unsigned char i = 0 ; i < mLinkNum ; ++i )
	    {
            // ボーン番号は降順で格納されている
		    mBoneIndexList[ i ] = ik_data_->punLinkNo[ i ];

            // PMD の仕様で、膝のIKは自動設定することになっている
            ModelFrame* bone = &( frames_[ mBoneIndexList[ i ] ] );
            if ( bone->getName() == _T( "左ひざ" ) || bone->getName() == _T( "右ひざ" ) )
            {
                printf( "MMDX DefaltIKLimitter.cs のように実装したい\n" );
                bone->setIKLimitEnable( true );
            }
	    }
    }

	//----------------------------------------------------------------------
	// ● 適用
	//----------------------------------------------------------------------
    void ModelIK::update( ModelFrame* frames_ )
    {
        ModelFrame* target_bone = &( frames_[ mTargetBoneIdx ] );
        ModelFrame* eff_bone    = &( frames_[ mEffBoneIdx ] );

        /*

        //IK対象のボーンのGlobalを更新(別のIK影響下のボーンからIKチェインが出ている場合があるので)
        LMatrix local;
        for (int i = mLinkNum - 1; i >= 0; --i)
        {//順番に親子関係になっている。(Processorでチェックかけてある
            //GlobalTransformを仮更新
            ModelFrame* parentBone = frames_[mBoneIndexList[i]].getParent();


            LMatrix::createFromSQTTransform( &local, *frames_[mBoneIndexList[i]].getLocalTransformPtr());

            LMatrix tmp = *frames_[mBoneIndexList[i]].getWorldMatrixPtr();
            (*frames_[mBoneIndexList[i]].getWorldMatrixPtr()) = local * parentBone->getWorldMatrix();
        }

        ModelFrame* parentBone = eff_bone->getParent();
        LMatrix::createFromSQTTransform( &local, *eff_bone->getLocalTransformPtr());
        //LMatrix tmp = eff_bone->getWorldMatrixPtr();
            (*eff_bone->getWorldMatrixPtr()) = local * parentBone->getWorldMatrix();

        //effector.LocalTransform.CreateMatrix(out local);
        //Matrix.Multiply(ref local, ref BoneManager[effector.SkeletonHierarchy].GlobalTransform, out effector.GlobalTransform);
*/

       

        //LVector3 org_target_pos = target_bone->getWorldMatrix().getPosition();  // MMDX で WorldMatrix からだった
        LVector3 eff_pos;
        LVector3 joint_pos;
        LVector3 target_pos;
        //LMatrix bone_inv;
        LMatrix inv_coord;

        LVector3 local_eff_pos;
        LVector3 local_target_pos;

        for ( short j = mLinkNum - 1 ; j >= 0 ; --j )
        {
            frames_[ mBoneIndexList[ j ] ].updateWorldMatrixFromLocalAndParent();
        }
        eff_bone->updateWorldMatrixFromLocalAndParent();



        //ターゲット位置の取得
        target_pos = target_bone->getWorldMatrix().GetPosition();

        ///int add_iterations = 0;

        for ( lnU16 it = 0; it < mCalcCount + 10; ++it )
        {
            for( lnU8 node_idx = 0 ; node_idx < mLinkNum ; ++node_idx )
		    {
                lnU32 bone_idx = mBoneIndexList[ node_idx ];
                ModelFrame* node = &frames_[ bone_idx ];

                

                // エフェクタの位置
                eff_pos = eff_bone->getWorldMatrix().GetPosition();

                // 注目ノードの位置の取得
                joint_pos = node->getWorldMatrix().GetPosition();

                // ワールド座標系から注目ノードの局所座標系への変換
				inv_coord =  LMatrix::Inverse(node->getWorldMatrix());

                // 各ベクトルの座標変換を行い、検索中のボーンi基準の座標系にする
                // (1) 注目ノード→エフェクタ位置へのベクトル(a)(注目ノード)
				local_eff_pos = LVector3::TransformCoord(eff_pos, inv_coord);
                // (2) 基準関節i→目標位置へのベクトル(b)(ボーンi基準座標系)
				local_target_pos = LVector3::TransformCoord(target_pos, inv_coord);


                // 十分近ければ終了 (無い方が幾分かマシな気がする…)
                LVector3 diff = local_eff_pos - local_target_pos;
                if ( LVector3::Dot( diff, diff ) < 0.0000001f )	return;


                // (1) 基準関節→エフェクタ位置への方向ベクトル
                local_eff_pos.Normalize();
			    // (2) 基準関節→目標位置への方向ベクトル
                local_target_pos.Normalize();

                // 十分近ければ終了
                //lnFloat dot = ;
                //if ( dot > 0.9999995f ) return;
                //if ( LVector3::dot( local_eff_pos, local_target_pos ) > 1 - 1.0e-5f ) continue;

                // 回転角
			    float rot_angle = acosf( LVector3::Dot( local_eff_pos, local_target_pos ) );

                if ( 0.00000001f >= fabsf( LVector3::Dot( local_eff_pos, local_target_pos ) ) )	continue;
               
                //回転量制限をかける
                if ( rot_angle > LMath::PI * mFact * (node_idx + 1))
                     rot_angle = LMath::PI * mFact * (node_idx + 1);
                if ( rot_angle < -LMath::PI * mFact * (node_idx + 1))
                     rot_angle = -LMath::PI * mFact * (node_idx + 1);
	//            if( rot_angle < -mFact )	    rot_angle = -mFact;
				//else if( mFact < rot_angle )	rot_angle = mFact;

                // 回転軸
				LVector3 rot_axis = LVector3::Cross(local_eff_pos, local_target_pos);
                mModelCore->getIKLimitter()->adjustAxis( node->getName().c_str(), &rot_axis );   // 制限
                rot_axis.Normalize();

                //if( LVector3::dot( rot_axis, rot_axis ) < 0.0000001f )	continue;

            
                //if ( 0.0000001f < fabsf( rot_angle ) )
				if (!LMath::IsNaN(rot_angle) && rot_angle > 1.0e-3f && !rot_axis.IsNaNOrInf())
			    {
                    // 関節回転量の補正
                    LQuaternion rot_quat(rot_axis, rot_angle);

                    //if ( frames_[ bone_idx ].isIKLimit() )	_limitAngle( &rot_quat, rot_quat );
                    //rot_quat.normalize();
                    //LQuaternion::multiply( &node->mLocalTransform.Rotation, node->mLocalTransform.Rotation, rot_quat );
                    
					node->mLocalTransform.Rotation = LQuaternion::Multiply(node->mLocalTransform.Rotation, rot_quat);
                    if ( mModelCore->getIKLimitter()->adjustRotation( node ) )
                    {
                        //add_iterations += 1;
                    }


                    

                    // 関節回転量の補正
                    //Quaternion subRot = MMDXMath.CreateQuaternionFromAxisAngle(rotationAxis, rotationAngle);
                    //Quaternion.Multiply(ref subRot, ref node.LocalTransform.Rotation, out node.LocalTransform.Rotation);
                    //MMDCore.Instance.IKLimitter.Adjust(node);
                

                
                    // 関節回転量の補正
                    /*
				    LQuaternion rot_quat( rot_axis, rot_angle );
                    if ( frames_[ bone_idx ].isIKLimit() )	_limitAngle( &rot_quat, rot_quat );
                    
                    rot_quat.normalize();
                    LQuaternion::multiply( &node->mLocalTransform.Rotation, node->mLocalTransform.Rotation, rot_quat );
                    
                    if ( node->mLocalTransform.Rotation.isZero() )
                    {
                        printf("node->mLocalTransform.Rotation.isZero()\n");
                    }
                    node->mLocalTransform.Rotation.normalize();
                    */

                    // IK 構成ボーンを更新する
                    for ( short j = node_idx ; j >= 0 ; --j ) { 
                    	frames_[ mBoneIndexList[ j ] ].updateWorldMatrixFromLocalAndParent(); }
                    eff_bone->updateWorldMatrixFromLocalAndParent();
                }
            }
        }
        /*
        for ( lnU16 it = 0; it < mCalcCount; ++it )
        {
            for( lnU8 link_idx = 0 ; link_idx < mLinkNum ; ++link_idx )
		    {
                lnU32 bone_idx = mBoneIndexList[ link_idx ];

                // エフェクタの位置
                eff_pos = eff_bone->getWorldMatrix().getPosition();

                // 注目ノードの位置の取得







                // ワールド座標系から注目ノードの局所(ローカル)座標系への変換
                LMatrix::inverse( &bone_inv, frames_[ bone_idx ].getWorldMatrix() );

                // エフェクタ，到達目標のローカル位置
                eff_pos.transform( bone_inv );
                LVector3::transform( &target_pos, org_target_pos, bone_inv );

			    // 十分近ければ終了
                if ( LVector3::dot( eff_pos, target_pos ) < 0.0000001f ) return;

			    // (1) 基準関節→エフェクタ位置への方向ベクトル
                eff_pos.normalize();

			    // (2) 基準関節→目標位置への方向ベクトル
                target_pos.normalize();
			    
			    // ベクトル (1) を (2) に一致させるための最短回転量（Axis-Angle）

			    // 回転角
			    float r = acosf( LVector3::dot( eff_pos, target_pos ) );

                if ( 0.00000001f < fabsf( r ) )
			    {
				    if( r < -mFact )	    r = -mFact;
				    else if( mFact < r )	r =  mFact;

				    // 回転軸
                    LVector3 rot_axis;
                    LVector3::cross( &rot_axis, eff_pos, target_pos );
                    _p(LVector3::dot( rot_axis, rot_axis ));
                    //if ( LVector3::dot( rot_axis, rot_axis ) < 0.0000001f ) continue;

                    rot_axis.normalize();

				    // 関節回転量の補正
				    LQuaternion rot_quat( rot_axis, r );
				    
                    if ( frames_[ bone_idx ].isIKLimit() )	_limitAngle( &rot_quat, rot_quat );

                    rot_quat.normalize();

                    LQuaternion::multiply( &frames_[ bone_idx ].mRotation, frames_[ bone_idx ].mRotation, rot_quat );
				    
                    frames_[ bone_idx ].mRotation.normalize();

                    // IK 構成ボーンを更新する
                    for ( short j = link_idx ; j >= 0 ; --j ) { frames_[ mBoneIndexList[ j ] ].updateMatrixInverse(); }
                    eff_bone->updateMatrixInverse();
			    }
            }
        }
        */
    }

	//----------------------------------------------------------------------
	// ● 解放
	//----------------------------------------------------------------------
    void ModelIK::_release()
    {
        SAFE_DELETE_ARRAY( mBoneIndexList );
        mLinkNum = 0;
    }

	//----------------------------------------------------------------------
	// ● ボーンの回転角度を制限する
	//----------------------------------------------------------------------
    void ModelIK::_limitAngle( LQuaternion* pvec_out_, const LQuaternion& pvec_src_ )
    {
	    // XYZ軸回転の取得
		LVector3 angle = pvec_src_.ToEulerAngles();

	    // 角度制限
	    if ( angle.X < -3.14159f )	angle.X = -3.14159f;
	    if ( -0.002f < angle.X )    angle.X = -0.002f;
	    angle.Y = 0.0f;
	    angle.Z = 0.0f;

	    // XYZ軸回転からクォータニオンへ
		*pvec_out_ = LQuaternion::RotationEulerAngles(angle);

        //LN_PRINT_NOT_IMPL_FUNCTION;
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