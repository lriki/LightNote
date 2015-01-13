//==============================================================================
// ModelRigidBody 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <btBulletDynamicsCommon.h>
#include "../../../Physics/RigidBody/Shapes.h"
#include "Core/ModelCore.h"
#include "Core/ModelFrameCore.h"
#include "Core/ModelRigidBodyCore.h"
#include "ModelManager.h"
#include "ModelFrame.h"
#include "ModelRigidBody.h"

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
// ModelRigidBody
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelRigidBody2::ModelRigidBody2()
        : mRigidBodyCore	( NULL )
		, mRigidBody		( NULL )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelRigidBody2::~ModelRigidBody2()
	{
		LN_SAFE_RELEASE( mRigidBody );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ModelRigidBody2::create( Model2* ownerModel, ModelRigidBodyCore* bodyCore )
	{
		mOwnerModel = ownerModel;
		mRigidBodyCore = bodyCore;

		// 関連フレーム
		mRelatedFrame = ownerModel->getFrame( mRigidBodyCore->RelatedFrameIndex );

		// 初期姿勢
		LMatrix startTransform = mRigidBodyCore->BoneOffset;
		startTransform.translation( mRelatedFrame->getFrameCore()->OrgPosition );

		// 剛体生成
		Physics::RigidBody::ConfigData configData;
		configData.Mass				= mRigidBodyCore->Mass;
		configData.Group			= mRigidBodyCore->Group;
		configData.GroupMask		= mRigidBodyCore->GroupMask;
		configData.Restitution		= mRigidBodyCore->Restitution;
		configData.Friction			= mRigidBodyCore->Friction;
		configData.LinearDamping	= mRigidBodyCore->LinearDamping;
		configData.AngularDamping	= mRigidBodyCore->AngularDamping;
		configData.InitialTransform = &startTransform;
		configData.AdditionalDamping= true;
		configData.KinematicObject	= (mRigidBodyCore->RigidBodyType == RigidBodyType_ControlledByBone);
	
		// シェイプの作成
		Physics::Manager* physicsManager = ownerModel->getModelCore()->Manager->getPhysicsManager();
		switch ( mRigidBodyCore->ColShapeData.Type )
	    {
			case CollisionShapeType_Sphere:		// 球
			{
				Physics::Sphere* shape = LN_NEW Physics::Sphere();
				shape->create( physicsManager, mRigidBodyCore->ColShapeData.Sphere.Radius, configData );
                mRigidBody = shape;
			    break;
			}
		    case CollisionShapeType_Box:		// 箱
			{
				Physics::Box* shape = LN_NEW Physics::Box();
				LVector3 size( 
					mRigidBodyCore->ColShapeData.Box.Width, 
					mRigidBodyCore->ColShapeData.Box.Height, 
					mRigidBodyCore->ColShapeData.Box.Depth );
				size *= 2.0f;
				shape->create( physicsManager, size, configData );
                mRigidBody = shape;
			    break;
			}
		    case CollisionShapeType_Capsule:	// カプセル
			{
				Physics::Capsule* shape = LN_NEW Physics::Capsule();
				shape->create( 
					physicsManager, 
					mRigidBodyCore->ColShapeData.Capsule.Radius, 
					mRigidBodyCore->ColShapeData.Capsule.Height, 
					configData );
                mRigidBody = shape;
			    break;
			}
	    }

		if ( mRigidBodyCore->RigidBodyType == RigidBodyType_ControlledByBone )
		{
			mRigidBody->setWorldMatrix( startTransform );
		}

#if 1	// CCD 設定
		// http://slis.tsukuba.ac.jp/~fujisawa.makoto.fu/lecture/iml/text/3_collision.html
		float threshold = 0;
		switch ( mRigidBodyCore->ColShapeData.Type )
	    {
			case CollisionShapeType_Sphere:		// 球
			{
				threshold = mRigidBodyCore->ColShapeData.Sphere.Radius / 2.0f;
			    break;
			}
		    case CollisionShapeType_Box:		// 箱
			{
				threshold = std::max( mRigidBodyCore->ColShapeData.Box.Width, std::max( mRigidBodyCore->ColShapeData.Box.Height, mRigidBodyCore->ColShapeData.Box.Depth ) );
			    break;
			}
		    case CollisionShapeType_Capsule:	// カプセル
			{
				threshold = std::max( mRigidBodyCore->ColShapeData.Capsule.Radius / 2.0f, mRigidBodyCore->ColShapeData.Capsule.Height );
			    break;
			}
	    }
		mRigidBody->getBtRigidBody()->setCcdMotionThreshold( threshold );
		mRigidBody->getBtRigidBody()->setCcdSweptSphereRadius( threshold * 0.9f );
#endif

		

	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelRigidBody2::fixPosition( float fElapsedFrame )
	{
		// 剛体の位置はボーンの位置で固定
		if ( mRigidBodyCore->RigidBodyType == RIGIDBODYTYPE_PHYSICS_ALIGNMENT )
	    {

			//const LMatrix& m = mRelatedFrame->getGlobalMatrix();
			//const LVector3& bonePos = mRelatedFrame->getGlobalMatrix().getPosition();
			//LVector3 diff = mRigidBodyCore->BoneOffset.getPosition() - bonePos;

			//mRigidBody->setPosition( diff );
			//mRigidBody->setPosition( (mRigidBodyCore->BoneOffset * m).getPosition() );

		}
#if 0

		// http://d.hatena.ne.jp/ousttrue/20100424/1272126923

	    if ( mRigidBodyType == RIGIDBODYTYPE_PHYSICS_ALIGNMENT )
	    {
            const LVector3& bone_pos = mFrame->getWorldMatrix().getPosition();

		    btTransform rb_transform = this->mBtRigidBody->getCenterOfMassTransform();

		    rb_transform.setOrigin( btVector3( 0.0f, 0.0f, 0.0f ) );
		    rb_transform = mBtBoneOffset * rb_transform;

		    rb_transform.setOrigin( rb_transform.getOrigin() + btVector3( bone_pos.x, bone_pos.y, bone_pos.z ) );
	//		rb_transform.setBasis( m_pbtRigidBody->getWorldTransform().getBasis() );

	//		this->mBtRigidBody->setCenterOfMassTransform( bttrRbTransform );

		    float	fRate = 0.2f * fElapsedFrame;
		    if( fRate > 1.0f )	fRate = 1.0f;
		    this->mBtRigidBody->translate(
                (rb_transform.getOrigin() - this->mBtRigidBody->getCenterOfMassTransform().getOrigin()) * fRate );
	    }
#endif
	}

	

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelRigidBody2::updateBeforePhysics( const LMatrix& externalWorldGlobalMatrix )
	{
		// ボーン追従
		if ( mRigidBodyCore->RigidBodyType == RigidBodyType_ControlledByBone )
	    {
			const LMatrix& frameMat = mRelatedFrame->getGlobalMatrix();

			mOldWorldTransform = mNewWorldTransform;
			mNewWorldTransform = mRigidBodyCore->BoneOffset * frameMat * externalWorldGlobalMatrix;
			mRigidBody->setWorldMatrix( mNewWorldTransform );	// static 剛体なので moveForced() じゃなくてOK
			mRigidBody->activate();
		}
		// ボーン追従以外
		else
		{
			const LMatrix& bodyMat = mRigidBody->getWorldMatrix();

			// 正常な姿勢であれば覚えておく
			if ( !bodyMat.containsNaN() )
			{
				mOldWorldTransform = bodyMat;
			}
		}

#if 0
		if ()

			mOldMatrix

		// ボーン追従
		if ( mRigidBodyCore->RigidBodyType == RigidBodyType_ControlledByBone )
	    {
			const LMatrix& m = mRelatedFrame->getGlobalMatrix();
			//mRigidBody->setWorldMatrix( mRigidBodyCore->BoneOffset * m * externalGlobalMatrix );
			mRigidBody->moveToForced( mRigidBodyCore->BoneOffset * m * externalGlobalMatrix );
		}
#endif
	}
		
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ModelRigidBody2::updateAfterPhysics( const LMatrix& externalWorldMatrixInv )
	{
		// 物理ボーンの場合
		if ( mRigidBodyCore->RigidBodyType != RigidBodyType_ControlledByBone )
		{
			mRigidBody->activate();
			if ( mRigidBody->isContactResponse() )
			{
				LMatrix bodyMat = mRigidBody->getWorldMatrix();

				//「静止」の処理を入れるときはこの辺で bodyMat を操作する

				// 姿勢が壊れていればリセット
				if ( bodyMat.containsNaN() ) {
					bodyMat.identity();
				}

				// フレーム姿勢を剛体姿勢に合わせる
				mRelatedFrame->setGlobalMatrix(
					mRigidBodyCore->InvBoneOffset * bodyMat * externalWorldMatrixInv, 0 );
			}
			else
			{
				// 物理ワールド上に生成されているが、演算の対象外 (衝突検出のみ行うモード)
				// 自分で移動しないので、姿勢を設定しておく必要がある
				mRigidBody->setWorldMatrix( mOldWorldTransform );
			}

			// ボーン位置合わせ
			if ( mRigidBodyCore->RigidBodyType == RigidBodyType_PhysicsAlignment )
			{
#if 0
				// ワールド空間上のボーン位置
				LVector3 bonePos = (mRelatedFrame->getGlobalMatrix() * mOwnerModel->getWorldMatrix()).getPosition();
					
				LVector3 newPos = mRigidBodyCore->BoneOffset.getPosition() + bonePos;
				LMatrix newTransform = mRigidBody->getWorldMatrix();
				newTransform._41 = newPos.x;
				newTransform._42 = newPos.y;
				newTransform._43 = newPos.z;
				mRigidBody->moveToForced( newTransform );
#endif
			}
		}

#if 0
		if ( mRigidBodyCore->RigidBodyType != RigidBodyType_ControlledByBone )
	    {
			// mBtInvBoneOffset を乗算することで、剛体の姿勢をボーンのあるべき姿勢に変換する
			//	剛体の姿勢 = 追従ボーン * (回転と移動)
			//  追従ボーン = 剛体の姿勢 * inv(回転と移動)
			// 要するに、剛体Aの位置はボーンAに対して +10 の位置にあるとしたら、ボーンの位置は 剛体Aの位置 -10
			mRelatedFrame->setGlobalMatrix(
				mRigidBodyCore->InvBoneOffset * mRigidBody->getWorldMatrix() * externalWorldMatrixInv, 0 );
		}
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ModelRigidBody2::moveBodyForced()
	{
		const LMatrix& m = mRelatedFrame->getGlobalMatrix();
		mRigidBody->moveToForced( mRigidBodyCore->BoneOffset * m * mOwnerModel->getWorldMatrix() );
	}





/// Kinematic用モーションステート (ボーン -> 剛体)
struct KinematicMotionState
    : public btMotionState
{
	btTransform m_graphicsWorldTrans;
	btTransform	m_BoneOffset;
	btTransform m_startWorldTrans;
	ModelFrame* mFrame;

	KinematicMotionState( const btTransform& startTrans, const btTransform& boneOffset, ModelFrame* frame_ )
        : m_BoneOffset( boneOffset )
        , m_startWorldTrans( startTrans )
        , mFrame( frame_ )
	{
	}

	///synchronizes world transform from user to physics
	virtual void getWorldTransform( btTransform& centerOfMassWorldTrans ) const
	{
		/* stepSimulation() から呼ばれる。非同期にする場合は注意 */
		//printf("getWorldTransform()\n");

		btTransform		bttrBoneTransform;

        LMatrix gl_mat = mFrame->getWorldMatrix();
        
		bttrBoneTransform.setFromOpenGLMatrix( (float *)(&gl_mat) );

		centerOfMassWorldTrans = bttrBoneTransform * m_BoneOffset;
	}

	///synchronizes world transform from physics to user
	///Bullet only calls the update of worldtransform for active objects
	virtual void setWorldTransform( const btTransform& centerOfMassWorldTrans )
	{
		//printf("setWorldTransform()\n");
		m_graphicsWorldTrans = centerOfMassWorldTrans;
	}
};


//==============================================================================
// ■ ModelRigidBody クラス
//==============================================================================

	//----------------------------------------------------------------------
	// ● コンストラクタ
	//----------------------------------------------------------------------
    ModelRigidBody::ModelRigidBody()
        : mRigidBody    ( NULL )
        , mColShape             ( NULL )
    {
    }

	//----------------------------------------------------------------------
	// ● デストラクタ
	//----------------------------------------------------------------------
    ModelRigidBody::~ModelRigidBody()
    {
        _release();
    }


	void createYawPitchRoll( LMatrix* out, lnFloat yaw, lnFloat pitch, lnFloat roll )
	{
		lnFloat xc = cos( pitch );
		lnFloat yc = cos( yaw );
		lnFloat zc = cos( roll );
		lnFloat xs = sin( pitch );
		lnFloat ys = sin( yaw );
		lnFloat zs = sin( roll );
		out->set(
			yc * xc, xc * xs * ys - zs * yc, zc * xs * yc + zs * ys, 0,
			zs * xc, zs * xs * ys + zc * yc, zs * xs * yc - zc * ys, 0,
			-xs,     xc * ys,                xc * yc,                0,
			0, 0, 0, 1 );
	}

	//----------------------------------------------------------------------
	// ● 初期化
	//----------------------------------------------------------------------
    void ModelRigidBody::initializePMD( ModelManager* manager_, const PMD_RigidBody *pmd_rigidbody_, ModelFrame* frame_ )
    {
		mFrame = frame_;

		// 剛体の種類
        switch( pmd_rigidbody_->cbRigidBodyType )
	    {
            default:
            case 0: 
            	mRigidBodyType = RIGIDBODYTYPE_CONTROLLED_BY_BONE; 
				break;
            case 1: 
            	mRigidBodyType = RIGIDBODYTYPE_PHYSICS; 
				break;
            case 2: 
            	mRigidBodyType = RIGIDBODYTYPE_PHYSICS_ALIGNMENT; 
				break;
        }


		//if ( strcmp( pmd_rigidbody_->szName, "下半身" ) ==0) //mFrame->getName() == _T( "下半身" ) )
		//{
		//	printf("body ModelRigidBody:%p %p\n", this, mRigidBody);
		//	//mRigidBody->getWorldMatrix().cdump();
		//}


//		#pragma comment (lib, "d3dx9.lib");
		// 剛体のローカル剛体の姿勢
		LMatrix bias;
		bias.rotationZ( pmd_rigidbody_->vec3Rotation.z );
		bias.rotationX( pmd_rigidbody_->vec3Rotation.x );
		bias.rotationY( pmd_rigidbody_->vec3Rotation.y );
		/*
		D3DXMatrixRotationYawPitchRoll(
			(D3DXMATRIX*)&bias, 
			pmd_rigidbody_->vec3Rotation.y, 
			pmd_rigidbody_->vec3Rotation.x, 
			pmd_rigidbody_->vec3Rotation.z );
			*/
		//{
		//	btMatrix3x3	btmRotationMat;
		//	btmRotationMat.setEulerZYX( 
		//		pmd_rigidbody_->vec3Rotation.x, 
		//		pmd_rigidbody_->vec3Rotation.y, 
		//		pmd_rigidbody_->vec3Rotation.z );
		//	btmRotationMat.getOpenGLSubMatrix( (btScalar*)&bias );
		//	//bias.transpose();
		//}
		bias.translation( 
			pmd_rigidbody_->vec3Position.x,
			pmd_rigidbody_->vec3Position.y,
			pmd_rigidbody_->vec3Position.z  );
		//xyz
		//zxy
		//yzx
		//LN_ROTORDER_XYZ
		//LMatrix startTransform = mFrame->getWorldMatrix();
		mBoneOffset = bias;
		
		mInvBoneOffset = bias;
		mInvBoneOffset.inverse();
		//D3DXMatrixInverse( (D3DXMATRIX*)&mInvBoneOffset, NULL, (D3DXMATRIX*)&bias );

		// 剛体はボーンのトランスフォームのうち、位置のみ依存する
		LMatrix startTransform = bias;// * startTransform;
		startTransform.translation( mFrame->getWorldMatrix().getPosition() );
		//startTransform = bias * startTransform;

		//_p( pmd_rigidbody_->szName );
		//bias.cdump();
		//startTransform.cdump();
		//if ( mFrame->getName() == _T( "下半身" ) ) startTransform.m32 = 10;
		
		//bttrTransform.setFromOpenGLMatrix( (btScalar*)&startTransform );

		Physics::RigidBody::ConfigData configData;
		configData.Mass = pmd_rigidbody_->fMass;
		configData.Group = 0x0001 << pmd_rigidbody_->cbColGroupIndex;
		configData.GroupMask = pmd_rigidbody_->unColGroupMask;
		configData.Restitution = pmd_rigidbody_->fRestitution;
		configData.Friction = pmd_rigidbody_->fFriction;
		configData.LinearDamping = pmd_rigidbody_->fLinearDamping;
		configData.AngularDamping = pmd_rigidbody_->fAngularDamping;
		configData.InitialTransform = &startTransform;
		configData.AdditionalDamping = true;
		configData.KinematicObject = (mRigidBodyType == RIGIDBODYTYPE_CONTROLLED_BY_BONE);
		
		/*
		_p(pmd_rigidbody_->szName);
		printf( "Mass           %f\n", configData.Mass );
		printf( "Group          %x\n", configData.Group );
		printf( "GroupMask      %x\n", configData.GroupMask );
		printf( "Restitution    %f\n", configData.Restitution );
		printf( "Friction       %f\n", configData.Friction );
		printf( "LinearDamping  %f\n", configData.LinearDamping );
		printf( "AngularDamping %f\n", configData.AngularDamping );
		*/

		// シェイプの作成
	    switch ( pmd_rigidbody_->cbShapeType )
	    {
		    case 0 :	// 球
			{
				Physics::Sphere* shape = LN_NEW Physics::Sphere();
				shape->create( manager_->getPhysicsManager(), pmd_rigidbody_->fWidth, configData );
                mRigidBody = shape;
			    break;
			}
		    case 1 :	// 箱
			{
				Physics::Box* shape = LN_NEW Physics::Box();
				LVector3 size( pmd_rigidbody_->fWidth, pmd_rigidbody_->fHeight, pmd_rigidbody_->fDepth );
				size *= 2.0f;
				shape->create( manager_->getPhysicsManager(), size, configData );
                mRigidBody = shape;
			    break;
			}
		    case 2 :	// カプセル
			{
				Physics::Capsule* shape = LN_NEW Physics::Capsule();
				shape->create( manager_->getPhysicsManager(), pmd_rigidbody_->fWidth, pmd_rigidbody_->fHeight, configData );
                mRigidBody = shape;
			    break;
			}
	    }

		if ( mRigidBodyType == RIGIDBODYTYPE_CONTROLLED_BY_BONE )
		{
			mRigidBody->setWorldMatrix( startTransform );
		}

		if ( mFrame->getName() == _T( "センター" ) )    mNoCopyToBone = true;
	    else										    mNoCopyToBone = false;


		

#if 0




        mFrame = frame_;

        // シェイプの作成
	    switch ( pmd_rigidbody_->cbShapeType )
	    {
		    case 0 :	// 球
                mColShapeData.Type = COLSHAPE_SPHERE;
                mColShapeData.Sphere.Radius = pmd_rigidbody_->fWidth;
			    mColShape = new btSphereShape( mColShapeData.Sphere.Radius );
			    break;

		    case 1 :	// 箱
                mColShapeData.Type = COLSHAPE_BOX;
                mColShapeData.Box.Width  = pmd_rigidbody_->fWidth;
                mColShapeData.Box.Height = pmd_rigidbody_->fHeight;
                mColShapeData.Box.Depth  = pmd_rigidbody_->fDepth;
			    mColShape = new btBoxShape( btVector3( mColShapeData.Box.Width, mColShapeData.Box.Height, mColShapeData.Box.Depth ) );
			    break;

		    case 2 :	// カプセル
                mColShapeData.Type = COLSHAPE_CAPSULE;
                mColShapeData.Capsule.Radius = pmd_rigidbody_->fWidth;
                mColShapeData.Capsule.Height = pmd_rigidbody_->fHeight;
			    mColShape = new btCapsuleShape( mColShapeData.Capsule.Radius, mColShapeData.Capsule.Height );
			    break;
	    }

        // 剛体の種類
        switch( pmd_rigidbody_->cbRigidBodyType )
	    {
            default:
            case 0: 
            	mRigidBodyType = RIGIDBODYTYPE_CONTROLLED_BY_BONE; 
				break;
            case 1: 
            	mRigidBodyType = RIGIDBODYTYPE_PHYSICS; 
				break;
            case 2: 
            	mRigidBodyType = RIGIDBODYTYPE_PHYSICS_ALIGNMENT; 
				break;
        }

        mColGroup           = pmd_rigidbody_->cbColGroupIndex;
        mColGroupMask       = pmd_rigidbody_->unColGroupMask;
        mInitialPosition    = pmd_rigidbody_->vec3Position;
        mInitialRotation    = pmd_rigidbody_->vec3Rotation;
        mMass               = pmd_rigidbody_->fMass;
        mLinearDamping      = pmd_rigidbody_->fLinearDamping;
        mAngularDamping     = pmd_rigidbody_->fAngularDamping;
        mRestitution        = pmd_rigidbody_->fRestitution;
        mFriction           = pmd_rigidbody_->fFriction;

		//mInitialPosition.z *= -1.0f;
		/*
		mInitialRotation.x *= -1.0f;
		mInitialRotation.y *= -1.0f;

		
        
		printf( "%s:rot %f, %f, %f\n", pmd_rigidbody_->szName, mInitialRotation.x, mInitialRotation.y, mInitialRotation.z );
		printf( "%s:pos %f, %f, %f\n", pmd_rigidbody_->szName, mInitialPosition.x, mInitialPosition.y, mInitialPosition.z );
		*/

		// PMD の回転情報は左ねじ。軸方向に対して反時計回りで+。
		// 位置情報も同様に左手で格納されている。

        // ボーンオフセット用トランスフォーム作成
	    btMatrix3x3	btmRotationMat;
		btmRotationMat.setEulerYPR( mInitialRotation.y, mInitialRotation.x, mInitialRotation.z );
		//btmRotationMat.setEulerZYX( mInitialRotation.x, mInitialRotation.y, mInitialRotation.z );

	    mBtBoneOffset.setIdentity();
	    mBtBoneOffset.setOrigin( btVector3( mInitialPosition.x, mInitialPosition.y, mInitialPosition.z ) );
	    mBtBoneOffset.setBasis( btmRotationMat );

        mBtInvBoneOffset = mBtBoneOffset.inverse();

        if ( mFrame->getName() == _T( "センター" ) )    mNoCopyToBone = true;
	    else										    mNoCopyToBone = false;





        

	    // 質量と慣性テンソルの設定
	    btScalar	btsMass( 0.0f );
	    btVector3	btv3LocalInertia( 0.0f, 0.0f ,0.0f );

	    // ボーン追従でない場合は質量を設定
	    if ( mRigidBodyType != RIGIDBODYTYPE_CONTROLLED_BY_BONE )	btsMass = mMass;

	    // 慣性テンソルの計算
	    if ( btsMass != 0.0f )	mColShape->calculateLocalInertia( btsMass, btv3LocalInertia );

	    // ボーンの位置取得
	    LVector3		vec3BonePos = mFrame->getWorldMatrix().getPosition();

	    // 剛体の初期トランスフォーム作成
	    btTransform		bttrTransform;

	    bttrTransform.setIdentity();
	    bttrTransform.setOrigin( btVector3( vec3BonePos.x, vec3BonePos.y, vec3BonePos.z ) );
	    bttrTransform = bttrTransform * mBtBoneOffset;

	    // MotionStateの作成
	    btMotionState	*pbtMotionState;

		//printf( "%f %f %f %f\n", 
		//	mBtBoneOffset.getRotation().getX(), 
		//	mBtBoneOffset.getRotation().getY(),
		//	mBtBoneOffset.getRotation().getZ(),
		//	mBtBoneOffset.getRotation().getW());


		
#if 1
		// モデルのローカル空間内での剛体の姿勢
		LMatrix bias;
		//bias.rotation( -mInitialRotation.x, -mInitialRotation.y, mInitialRotation.z, LN_ROTORDER_YXZ );
		//createYawPitchRoll( &bias, -mInitialRotation.y, -mInitialRotation.x, mInitialRotation.z );
		D3DXMatrixRotationYawPitchRoll((D3DXMATRIX*)&bias, mInitialRotation.y, mInitialRotation.x, mInitialRotation.z );

		bias.translation( mInitialPosition );
		//bias.cdump();
		//mFrame->getWorldMatrix().cdump();

		LMatrix startTransform = mFrame->getWorldMatrix();

		//startTransform.m32 *= -1;

		LMatrix scale(1, 0, 0, 0,   0, 1, 0, 0,   0, 0, -1, 0,   0, 0, 0, 1);
		//startTransform = scale * startTransform * scale;

		startTransform = bias * startTransform;
		//startTransform.m33 = 1;
		//startTransform.cdump();

		bttrTransform.setFromOpenGLMatrix( (btScalar*)&startTransform );
		
		printf("◆");
		printf("%f, %f, %f, %f\n",
			bttrTransform.getBasis().getRow(0).x(),
			bttrTransform.getBasis().getRow(0).y(),
			bttrTransform.getBasis().getRow(0).z(),
			bttrTransform.getBasis().getRow(0).w());
		printf("%f, %f, %f, %f\n",
			bttrTransform.getBasis().getRow(1).x(),
			bttrTransform.getBasis().getRow(1).y(),
			bttrTransform.getBasis().getRow(1).z(),
			bttrTransform.getBasis().getRow(1).w());
		printf("%f, %f, %f, %f\n",
			bttrTransform.getBasis().getRow(2).x(),
			bttrTransform.getBasis().getRow(2).y(),
			bttrTransform.getBasis().getRow(2).z(),
			bttrTransform.getBasis().getRow(2).w());
		printf("%f, %f, %f, %f\n",
			bttrTransform.getOrigin().x(),
			bttrTransform.getOrigin().y(),
			bttrTransform.getOrigin().z(),
			bttrTransform.getOrigin().w());
#endif


	    switch( mRigidBodyType )
	    {
		    case RIGIDBODYTYPE_CONTROLLED_BY_BONE: 
		    	pbtMotionState = new Graphics::KinematicMotionState( bttrTransform, mBtBoneOffset, mFrame ); 
				break;
		    case RIGIDBODYTYPE_PHYSICS: 
		    	pbtMotionState = new btDefaultMotionState( bttrTransform ); 
				break;
		    case RIGIDBODYTYPE_PHYSICS_ALIGNMENT: 
		    	pbtMotionState = new btDefaultMotionState( bttrTransform ); 
				break;
		    //case 2 : pbtMotionState = new btKinematicMotionState( bttrTransform, bttrBoneOffset, pBone );	break;
	    }

	    // 剛体のパラメータの設定
	    btRigidBody::btRigidBodyConstructionInfo	btRbInfo( btsMass, pbtMotionState, mColShape, btv3LocalInertia );
	    btRbInfo.m_linearDamping  = mLinearDamping;	    // 移動減
	    btRbInfo.m_angularDamping = mAngularDamping;	// 回転減
	    btRbInfo.m_restitution    = mRestitution;	    // 反発力
	    btRbInfo.m_friction       = mFriction;		    // 摩擦力
	    btRbInfo.m_additionalDamping = true;


	    // 剛体の作成
	    btRigidBody* rigit_body = new btRigidBody( btRbInfo );

	    // Bone 追従の場合、Kinematic設定
	    if ( mRigidBodyType == RIGIDBODYTYPE_CONTROLLED_BY_BONE )
	    {
            // btCollisionObject::CF_KINEMATIC_OBJECT フラグを追加しておくと、
            // btMotionState::getWorldTransform() が毎フレーム呼ばれる
		    rigit_body->setCollisionFlags( rigit_body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT );
            rigit_body->setActivationState( DISABLE_DEACTIVATION );	// btMotionState::setWorldTransform() を呼ばない
	    }
	    rigit_body->setSleepingThresholds( 0.0f, 0.0f );

	    // 剛体をシミュレーションワールドに追加
        Physics::RigidBodyInitArgs args;
        args.Shape      = mColShape;
        args.RigidBody  = rigit_body;
     
		Physics::RigidBody::ConfigData data;
		data.Group = 0x0001 << mColGroup;
		data.GroupMask = mColGroupMask;
        Physics::RigidBody::initialize( manager_->getPhysicsManager(), args, data );
    
#endif
	}


	//----------------------------------------------------------------------
	// ● 解放
	//----------------------------------------------------------------------
    void ModelRigidBody::_release()
    {
		LN_SAFE_RELEASE( mRigidBody );
    }

	//----------------------------------------------------------------------
	// ● ボーン位置合わせ
	//----------------------------------------------------------------------
    void ModelRigidBody::fixPosition( float fElapsedFrame )
    {
		// 剛体の位置はボーンの位置で固定
		if ( mRigidBodyType == RIGIDBODYTYPE_PHYSICS_ALIGNMENT )
	    {
			LN_THROW_NotImpl( 0 );
		}
#if 0

		// http://d.hatena.ne.jp/ousttrue/20100424/1272126923

	    if ( mRigidBodyType == RIGIDBODYTYPE_PHYSICS_ALIGNMENT )
	    {
            const LVector3& bone_pos = mFrame->getWorldMatrix().getPosition();

		    btTransform rb_transform = this->mBtRigidBody->getCenterOfMassTransform();

		    rb_transform.setOrigin( btVector3( 0.0f, 0.0f, 0.0f ) );
		    rb_transform = mBtBoneOffset * rb_transform;

		    rb_transform.setOrigin( rb_transform.getOrigin() + btVector3( bone_pos.x, bone_pos.y, bone_pos.z ) );
	//		rb_transform.setBasis( m_pbtRigidBody->getWorldTransform().getBasis() );

	//		this->mBtRigidBody->setCenterOfMassTransform( bttrRbTransform );

		    float	fRate = 0.2f * fElapsedFrame;
		    if( fRate > 1.0f )	fRate = 1.0f;
		    this->mBtRigidBody->translate(
                (rb_transform.getOrigin() - this->mBtRigidBody->getCenterOfMassTransform().getOrigin()) * fRate );
	    }
#endif
    }

	//----------------------------------------------------------------------
	// ● ボーンの姿勢を剛体の姿勢と一致させる(そのフレームのシミュレーション終了後に呼ぶ)
	//----------------------------------------------------------------------
    void ModelRigidBody::updateBoneTransform()
    {
		
		if ( mRigidBodyType == RIGIDBODYTYPE_CONTROLLED_BY_BONE && !mNoCopyToBone )
	    {
			// この処理は物理シミュレーション直前におこなうべき
			LMatrix m = mFrame->getWorldMatrix();//*mFrame->getLocalTransformPtr();
			//m *= mBoneOffset;
			//m.cdump();
			mRigidBody->setWorldMatrix( mBoneOffset * m );
		}
		
		if ( mRigidBodyType != RIGIDBODYTYPE_CONTROLLED_BY_BONE && !mNoCopyToBone )
	    {
			mFrame->setWorldMatrix( mInvBoneOffset * mRigidBody->getWorldMatrix(), 0 );
		}
#if 0
		
		return;

        if ( mRigidBodyType != RIGIDBODYTYPE_CONTROLLED_BY_BONE && !mNoCopyToBone )
	    {
			//mBtRigidBody->getWorldTransform();

			// mBtInvBoneOffset を乗算することで、剛体の姿勢をボーンのあるべき姿勢に変換する
			//	剛体の姿勢 = 追従ボーン * (回転と移動)
			//  追従ボーン = 剛体の姿勢 * inv(回転と移動)
			// 要するに、剛体Aの位置はボーンAに対して +10 の位置にあるとしたら、ボーンの位置は 剛体Aの位置 -10
		    btTransform		bttrBoneTransform = mBtRigidBody->getCenterOfMassTransform() * mBtInvBoneOffset;

		    bttrBoneTransform.getOpenGLMatrix( (float *)mFrame->getWorldMatrixPtr() );
	    }
#endif
    }

	//----------------------------------------------------------------------
	// ● 剛体をボーンの位置へ強制的に移動させる
	//----------------------------------------------------------------------
    void ModelRigidBody::moveToBonePos()
    {
		if ( mRigidBodyType != RIGIDBODYTYPE_CONTROLLED_BY_BONE )
	    {
			LMatrix m = mFrame->getWorldMatrix();
			mRigidBody->moveToForced( mBoneOffset * m );
		}
#if 0
		return;
	    if ( mRigidBodyType != RIGIDBODYTYPE_CONTROLLED_BY_BONE )
	    {
            const btVector3 ZERO( 0.0f, 0.0f, 0.0f );

            
            const LVector3& bone_pos = mFrame->getWorldMatrix().getPosition();
		    btTransform		bt_transform = mBtRigidBody->getCenterOfMassTransform();


			
		    bt_transform.setOrigin( ZERO );
		    bt_transform = mBtBoneOffset * bt_transform;

		    bt_transform.setOrigin( bt_transform.getOrigin() + btVector3( bone_pos.x, bone_pos.y, bone_pos.z ) );
		    bt_transform.setBasis( mBtRigidBody->getWorldTransform().getBasis() );


			
#if 0
			printf( "%f %f %f %f\n", 
				bt_transform.getRotation().getX(), 
				bt_transform.getRotation().getY(),
				bt_transform.getRotation().getZ(),
				bt_transform.getRotation().getW());

			float r = acos( bt_transform.getRotation().getW() ) * 2;
			float s = sin(r);
			LQuaternion q1( LVector3( 
				bt_transform.getRotation().getX() / s,
				bt_transform.getRotation().getY() / s,
				bt_transform.getRotation().getZ() / s ), r);
			bt_transform.setRotation( btQuaternion( q1.x, q1.y, -q1.z, q1.w ) );

			printf( "%f %f %f %f\n", 
				bt_transform.getRotation().getX(), 
				bt_transform.getRotation().getY(),
				bt_transform.getRotation().getZ(),
				bt_transform.getRotation().getW());
#endif

            //LQuaternion q;
            //LQuaternion::rotationMatrix( &q, mFrame->getWorldMatrix() );
            //bt_transform.setRotation( btQuaternion( q.x, q.y, q.z, q.w ) );

		    mBtRigidBody->setCenterOfMassTransform( bt_transform );
            
           
		    mBtRigidBody->setLinearVelocity( ZERO );
		    mBtRigidBody->setAngularVelocity( ZERO );
		    mBtRigidBody->setInterpolationLinearVelocity( ZERO );
		    mBtRigidBody->setInterpolationAngularVelocity( ZERO );
		    mBtRigidBody->setInterpolationWorldTransform( mBtRigidBody->getCenterOfMassTransform() );
		    mBtRigidBody->clearForces();


            mBtRigidBody->activate(true);
	    }
#endif
    }

	//----------------------------------------------------------------------
	// ● デバッグ用の形状描画
	//----------------------------------------------------------------------
#if 0
    LNRESULT ModelRigidBody::drawDebugShape( Physics::IDebugRenderer* renderer_ )
    {
		/*
        switch ( mColShapeData.Type )
	    {
            case COLSHAPE_SPHERE:
            {
                renderer_->setMatrix( this->mWorldMatrix );
                return renderer_->drawSphere( LVector3::ZERO, mColShapeData.Sphere.Radius, LColorF( 0, 0, 1, 0.5f ) );
            }
            case COLSHAPE_BOX:
            {
                LVector3 bbmin( -mColShapeData.Box.Width * 0.5f, -mColShapeData.Box.Height * 0.5f, -mColShapeData.Box.Depth * 0.5f );
                LVector3 bbmax(  mColShapeData.Box.Width * 0.5f,  mColShapeData.Box.Height * 0.5f,  mColShapeData.Box.Depth * 0.5f );
                renderer_->setMatrix( this->mWorldMatrix );
                return renderer_->drawBox( bbmin, bbmax, LColorF( 0, 0, 1, 0.5f ) );
            }
            case COLSHAPE_CAPSULE:
            {
                renderer_->setMatrix( this->mWorldMatrix );
                return renderer_->drawCapsule( mColShapeData.Capsule.Radius, mColShapeData.Capsule.Height, LColorF( 0, 0, 1, 0.5f ) );
            }
        }
		*/
        return LN_OK;
    }
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================