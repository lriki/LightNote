//==============================================================================
// ModelPMXIKSolver 
//==============================================================================

#pragma once

#include "stdafx.h"
#include "Core/ModelFrameCore.h"
#include "Model.h"
#include "ModelFrame.h"
#include "ModelPMXIKSolver.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ModelPMXIKSolver
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelPMXIKSolver::ModelPMXIKSolver()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ModelPMXIKSolver::~ModelPMXIKSolver()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
 //   void ModelPMXIKSolver::addEffectorFrame( ModelFrame2* frame )
	//{
	//	IKBones.push_back( frame );
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ModelPMXIKSolver::updateIKFrames()
	{
		//ln_foreach( ModelFrame2* frame, IKBones )
		//{
		//	solve( frame );
		//}
		ln_foreach( ModelIKCore2* ik, mOwnerModel->getModelCore()->IKs )
		{
			solve( 
				ik,
				mOwnerModel->getFrame( ik->TargetBoneIndex ),
				mOwnerModel->getFrame( ik->EffectorBoneIndex ) );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelPMXIKSolver::solve( ModelIKCore2* ik, ModelFrame2* ikTargetBone, ModelFrame2* effector )
	{
		/* ○ - ○ - ○ - ◎    ●
		 *      エフェクタ^     ^ ターゲットボーン (frame)
		 */

		//ModelFrame2* effector = mOwnerModel->getFrame( ikTargetBone->getFrameCore()->IKTargetBoneIndex );

		// IK構成ボーンのグローバル行列を再更新する
					//for ( short jk = ik->IKLinks.size() - 1 ; jk >= 0 ; --jk)
					//{
					//	mOwnerModel->getFrame(ik->IKLinks[jk].LinkBoneIndex)->updateGlobalMatrix(false);
					//	//frames[mIKCore->IKBoneIndexArray[j]].updateGlobalMatrix( false );
					//}
					//effector->updateGlobalMatrix( true );


		ModelFrameCore* frameCore = ikTargetBone->getFrameCore();

		for ( int i = 0; i < ik->LoopCount; i++ )
		{
			// ターゲットボーンのグローバル位置
			LVector3 targetPos = ikTargetBone->getGlobalMatrix().GetPosition();

			for( lnU8 iLink = 0 ; iLink < ik->IKLinks.size() ; ++iLink )
		    {
				// 動かすノード
				ModelIKLink& ikLink = ik->IKLinks[iLink];
				ModelFrame2* ikLinkFrame = mOwnerModel->getFrame( ikLink.LinkBoneIndex );

				// エフェクタの位置
                LVector3 effPos = effector->getGlobalMatrix().GetPosition();

				// ワールド座標系から注目ノードの局所座標系への変換
				// (IKリンク基準のローカル座標系へ変換する行列)
				LMatrix invCoord = LMatrix::Inverse(ikLinkFrame->getGlobalMatrix());

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
				if (rotationDotProduct > 1.f) rotationDotProduct = 1.f;
                float rotationAngle = acosf( rotationDotProduct );
               
#if 0
				//回転量制限をかける
				if ( rotationAngle > LMath::PI * ik->IKRotateLimit * (iLink + 1))
                     rotationAngle = LMath::PI * ik->IKRotateLimit * (iLink + 1);
                if ( rotationAngle < -LMath::PI * ik->IKRotateLimit * (iLink + 1))
                     rotationAngle = -LMath::PI * ik->IKRotateLimit * (iLink + 1);
				
#else
                //回転量制限をかける
				if ( rotationAngle > ik->IKRotateLimit)
                     rotationAngle = ik->IKRotateLimit;
                if ( rotationAngle < -ik->IKRotateLimit)
                     rotationAngle = -ik->IKRotateLimit;
#endif

                // 回転軸
				LVector3 rotationAxis = LVector3::Cross(localEffPos, localTargetPos);
                
				//if (frame->getFrameCore()->IKLimitter)
				//	frame->getFrameCore()->IKLimitter->adjustAxisLimits( &rotationAxis );
				rotationAxis.Normalize();


				if (!LMath::IsNaN(rotationAngle) && rotationAngle > 1.0e-3f && !rotationAxis.IsNaNOrInf())
			    {
					// 関節回転量の補正
                    LQuaternion rotQuat( rotationAxis, rotationAngle );
					ikLinkFrame->getLocalTransformPtr()->Rotation = LQuaternion::Multiply(ikLinkFrame->getLocalTransformPtr()->Rotation, rotQuat);
					
					// 回転制限
					limitRotation( ikLink, ikLinkFrame );


					// IK構成ボーンのグローバル行列を再更新する
					for ( short jk = ik->IKLinks.size() - 1 ; jk >= 0 ; --jk)
					{
						mOwnerModel->getFrame(ik->IKLinks[jk].LinkBoneIndex)->updateGlobalMatrix(false);
						//frames[mIKCore->IKBoneIndexArray[j]].updateGlobalMatrix( false );
					}
					effector->updateGlobalMatrix( true );

					// IK構成ボーンのグローバル行列を再更新する (子も)
					//ikLinkFrame->updateGlobalMatrix( true );
				}
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelPMXIKSolver::limitRotation( const ModelIKLink& ikLink, ModelFrame2* ikLinkFrame )
	{
		if ( !ikLink.IsRotateLimit ) return;

		LQuaternion& localRot = ikLinkFrame->getLocalTransformPtr()->Rotation;

		// オイラー角へ変換
		LVector3 r;
		bool locked;
		Lumino::RotationOrder type;
		r = localRot.ToEulerAngles(Lumino::RotationOrder_XYZ, &locked);
		if (!locked) {
			type = Lumino::RotationOrder_XYZ;
		}
		else {
			r = localRot.ToEulerAngles(Lumino::RotationOrder_YZX, &locked);
			if (!locked) {
				type = Lumino::RotationOrder_YZX;
			}
			else {
				r = localRot.ToEulerAngles(Lumino::RotationOrder_ZXY, &locked);
				if (!locked) {
					type = Lumino::RotationOrder_ZXY;
				}
				else {
					// あり得ないはずだが…
					LN_ERR2_ASSERT_S(0);
				}
			}
		}
		/*
		int type = 0;
		float rx, ry, rz;
		if ( LQuaternion::factoringXYZ( &rx, &ry, &rz, localRot ) ) {
			type = 0;
		}
		else {
			if ( LQuaternion::factoringYZX( &ry, &rz, &rx, localRot ) ) {
				type = 1;
			}
			else {
				if ( LQuaternion::factoringZXY( &rz, &rx, &ry, localRot ) ) {
					type = 2;
				}
				else {
					// あり得ないはずだが…
					LN_ERR2_ASSERT_S( 0 );
				}
			}
		}
		*/

		// 角度修正
		//LVector3 euler( rx, ry, rz );
		//euler.normalizeEular();
		r.Clamp( ikLink.MinLimit, ikLink.MaxLimit );
		
		// 戻す
		localRot = LQuaternion::RotationEulerAngles(r, type);
		/*
		LMatrix rotMat;
		switch ( type )
        {
            case 0:
				rotMat.rotation( euler, Math::RotationOrder_XYZ );
				LQuaternion::rotationMatrix( &localRot, rotMat );
                break;
            case 1:
				rotMat.rotation( euler, Math::RotationOrder_YZX );
				LQuaternion::rotationMatrix( &localRot, rotMat );
                break;
            case 2:
				rotMat.rotation( euler, Math::RotationOrder_ZXY );
				LQuaternion::rotationMatrix( &localRot, rotMat );
                break;
        }
		*/
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote
