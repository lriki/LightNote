//==============================================================================
// Manager 
//------------------------------------------------------------------------------
/*
	■剛体姿勢の get/set と非同期処理
		使う側としては…
		・get … 最後の演算結果がほしい
		・set … 次の演算に使用するパラメータをセットしたい

*/
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <algorithm>
#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btManifoldResult.h>

#include <BulletMultiThreaded/SpuGatheringCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btSimulationIslandManager.h>

// Win32
//#include <BulletMultiThreaded/Win32ThreadSupport.h>
//#include <BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h>

//#include <BulletMultiThreaded/btParallelConstraintSolver.h>
//#include <BulletMultiThreaded/SequentialThreadSupport.h>

#include "../Base/STLUtils.h"
#include "../System/Common/PerfStopwatch.h"
#include "../System/Environment.h"
#include "RigidBody/RigidBody.h"
#include "RigidBody/Plane.h"
#include "RigidBody/Shapes.h"
#include "RigidBody/TriangleMesh.h"
#include "CollisionBody/CollisionBody.h"
#include "Joint/JointBase.h"
#include "IDebugRenderer.h"
#include "Manager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Physics
{

//==============================================================================
// ■ DebugDrawer
//==============================================================================
class DebugDrawer
    : public btIDebugDraw
{
public:

    DebugDrawer()
        : mIDebugRenderer   ( NULL )
        , mDebugMode        ( 0 )
    {}
    virtual ~DebugDrawer() {}

public:

    void setDebugRenderer( IDebugRenderer* r ) { mIDebugRenderer = r; }

public:

    virtual void	drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
    {
        mIDebugRenderer->drawLine(
            LVector3( from.getX(), from.getY(), from.getZ() ),
            LVector3( to.getX(), to.getY(), to.getZ() ),
            LColorF( fromColor.getX(), fromColor.getY(), fromColor.getZ() ),
            LColorF( toColor.getX(), toColor.getY(), toColor.getZ() ) );
    }

	virtual void	drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
    {
        drawLine( from, to, color, color );
    }

	virtual void	drawSphere(const btVector3& p, btScalar radius, const btVector3& color)
    {
    }
	
    virtual void	drawBox(const btVector3& boxMin, const btVector3& boxMax, const btVector3& color, btScalar alpha)
    {
    }

	virtual void	drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha)
    {
    }
	
	virtual void	drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
    {
    }

	virtual void	reportErrorWarning(const char* warningString)
    {
    }

	virtual void	draw3dText(const btVector3& location,const char* textString)
    {
    }

    virtual void	setDebugMode(int debugMode) { mDebugMode = debugMode; }

	virtual int		getDebugMode() const { return mDebugMode; }

protected:

    IDebugRenderer* mIDebugRenderer;
    int mDebugMode;
};

static DebugDrawer gDebugDrawer;

//extern ContactAddedCallback gContactAddedCallback;


struct FilterCallback : public btOverlapFilterCallback
{
	// return true when pairs need collision
	virtual bool needBroadphaseCollision(btBroadphaseProxy* proxy0,btBroadphaseProxy* proxy1) const
	{
		btCollisionObject* collisionObject0 = static_cast<btCollisionObject*>( proxy0->m_clientObject );
		btCollisionObject* collisionObject1 = static_cast<btCollisionObject*>( proxy1->m_clientObject );

		// 処理開始
		BodyBase* pBody0 = static_cast<BodyBase*>(collisionObject0->getUserPointer());
		BodyBase* pBody1 = static_cast<BodyBase*>(collisionObject1->getUserPointer());

		BodyType Type0 = pBody0->getBodyType();
		BodyType Type1 = pBody1->getBodyType();

		// 接地処理
		//Kernel::Math::VectorS vNormal( Point.m_normalWorldOnB.getX(), Point.m_normalWorldOnB.getY(), Point.m_normalWorldOnB.getZ() );
		//bool IsGround = false;

		// キャラ同士は無視
		if ( (Type0 == BodyType_CharacterObject) && (Type1 == BodyType_CharacterObject) )
		{
			//cp.m_combinedFriction = 0.0f;
			//cp.m_combinedRestitution = FLT_MIN;
			return false;
		}


		bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
		collides = collides && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);
		//add some additional logic here that modified 'collides'
		return collides;
	}
};

static FilterCallback gFilterCallback;




#ifdef LN_USE_PARALLEL
btThreadSupportInterface* createSolverThreadSupport(int maxNumThreads)
{
//#define SEQUENTIAL
#ifdef SEQUENTIAL
	SequentialThreadSupport::SequentialThreadConstructionInfo tci("solverThreads",SolverThreadFunc,SolverlsMemoryFunc);
	SequentialThreadSupport* threadSupport = new SequentialThreadSupport(tci);
	threadSupport->startSPU();
#else

#ifdef _WIN32
	Win32ThreadSupport::Win32ThreadConstructionInfo threadConstructionInfo("solverThreads",SolverThreadFunc,SolverlsMemoryFunc,maxNumThreads);
	Win32ThreadSupport* threadSupport = new Win32ThreadSupport(threadConstructionInfo);
	threadSupport->startSPU();
#elif defined (USE_PTHREADS)
	PosixThreadSupport::ThreadConstructionInfo solverConstructionInfo("solver", SolverThreadFunc,
																	  SolverlsMemoryFunc, maxNumThreads);
	
	PosixThreadSupport* threadSupport = new PosixThreadSupport(solverConstructionInfo);
	
#else
	SequentialThreadSupport::SequentialThreadConstructionInfo tci("solverThreads",SolverThreadFunc,SolverlsMemoryFunc);
	SequentialThreadSupport* threadSupport = new SequentialThreadSupport(tci);
	threadSupport->startSPU();
#endif
	
#endif

	return threadSupport;
}
#endif

//==============================================================================
// Manager
//==============================================================================

	//----------------------------------------------------------------------
	// ● コンストラクタ
	//----------------------------------------------------------------------
    Manager::Manager()
        : mLogFile                  ( NULL )
        , mBtCollisionConfig        ( NULL )
        , mBtCollisionDispatcher    ( NULL )
        , mBtBroadphase             ( NULL )
        , mBtSolver                 ( NULL )
        , mBtWorld                  ( NULL )
        , mSoftBodyWorldInfo        ( NULL )
#ifdef LN_USE_PARALLEL
		, m_threadSupportCollision	( NULL )
		, m_threadSupportSolver		( NULL )
#endif
		, mLastUpdateTime			( 0 )
    {
    }

	//----------------------------------------------------------------------
	// ● デストラクタ
	//----------------------------------------------------------------------
    Manager::~Manager()
    {
        SAFE_DELETE( mBtCollisionConfig );
        SAFE_DELETE( mBtCollisionDispatcher );
        SAFE_DELETE( mBtBroadphase );
        SAFE_DELETE( mBtSolver );
        SAFE_DELETE( mBtWorld );
        SAFE_DELETE( mSoftBodyWorldInfo );

#ifdef LN_USE_PARALLEL
		if (m_threadSupportSolver)
		{
			delete m_threadSupportSolver;
		}
		if (m_threadSupportCollision)
		{
			delete m_threadSupportCollision;
		}
#endif
    }

	//----------------------------------------------------------------------
	// ● 初期化
	//----------------------------------------------------------------------
    LNRESULT Manager::initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

		int maxNumOutstandingTasks = 4;

#ifdef LN_USE_PARALLEL
		m_threadSupportCollision = new Win32ThreadSupport(Win32ThreadSupport::Win32ThreadConstructionInfo(
								"collision",
								processCollisionTask,
								createCollisionLocalStoreMemory,
								maxNumOutstandingTasks));
#endif

		// カスタムコリジョンコールバック
		//gContactAddedCallback = CustomMaterialCombinerCallback;

        // コリジョンコンフィグ
		btDefaultCollisionConstructionInfo defaultCollisionConstructionInfo;
		//defaultCollisionConstructionInfo.m_defaultMaxPersistentManifoldPoolSize = 32768;
		mBtCollisionConfig = new btDefaultCollisionConfiguration( defaultCollisionConstructionInfo );
		// ソフトボディ使うとき
        //mBtCollisionConfig = LN_NEW btSoftBodyRigidBodyCollisionConfiguration();// 

#ifdef LN_USE_PARALLEL
		mBtCollisionDispatcher = new SpuGatheringCollisionDispatcher(m_threadSupportCollision,maxNumOutstandingTasks,mBtCollisionConfig);
#else
        // コリジョンディスパッチャ
	    mBtCollisionDispatcher = LN_NEW btCollisionDispatcher( mBtCollisionConfig );

#endif


        // ブロードフェーズアルゴリズム
        /*
            btDbvtBroadphase
                AABB木による高速で動的な階層空間を使います。
            btAxisSweep3 bt32BitAxisSweep3
                逐次3Dスイープ&プルーン(incremental 3D sweep and prune)を実装しています。
            btCudaBroadphase
                GPUを使用した高速な一様グリッドを実装しています。
            http://bulletjpn.web.fc2.com/05_BulletCollisionDetection.html
        */
     //   btVector3	btv3WorldAabbMin( -3000.0f, -3000.0f, -3000.0f );
	    //btVector3	btv3WorldAabbMax(  3000.0f,  3000.0f,  3000.0f );
	    //int			iMaxProxies = 32766;//5 * 5 * 5 + 1024;//
	    //mBtBroadphase = new btAxisSweep3( btv3WorldAabbMin, btv3WorldAabbMax, iMaxProxies );
        mBtBroadphase = LN_NEW btDbvtBroadphase();
		
        // ソルバ
#ifdef LN_USE_PARALLEL
		m_threadSupportSolver = createSolverThreadSupport(maxNumOutstandingTasks);
		mBtSolver = new btParallelConstraintSolver(m_threadSupportSolver);
		//this solver requires the contacts to be in a contiguous pool, so avoid dynamic allocation
		mBtCollisionDispatcher->setDispatcherFlags(btCollisionDispatcher::CD_DISABLE_CONTACTPOOL_DYNAMIC_ALLOCATION);
#else
	    mBtSolver = new btSequentialImpulseConstraintSolver();
#endif

        // ワールドの作成
	    //mBtWorld = LN_NEW btSoftRigidDynamicsWorld( mBtCollisionDispatcher, mBtBroadphase, mBtSolver, mBtCollisionConfig, NULL );
        mBtWorld = new btDiscreteDynamicsWorld( mBtCollisionDispatcher, mBtBroadphase, mBtSolver, mBtCollisionConfig );
        //mBtWorld->setInternalTickCallback(pickingPreTickCallback,this,true);
	    //mBtWorld->getDispatchInfo().m_enableSPU = true;

		//btOverlapFilterCallback * filterCallback = new FilterCallback();
		//mBtWorld->getPairCache()->setOverlapFilterCallback( &gFilterCallback );

#if 0	// MMM setting (デフォルトは 10)
		mBtWorld->getSolverInfo().m_numIterations = 4;
#endif
#ifdef LN_USE_PARALLEL
		mBtWorld->getSimulationIslandManager()->setSplitIslands(false);
		mBtWorld->getSolverInfo().m_numIterations = 4;
		mBtWorld->getSolverInfo().m_solverMode = SOLVER_SIMD+SOLVER_USE_WARMSTARTING;//+SOLVER_RANDMIZE_ORDER;

		mBtWorld->getDispatchInfo().m_enableSPU = true;
#endif
        mBtWorld->setDebugDrawer( &gDebugDrawer );

        // 重力
		setGravity( LVector3( 0.0f, -9.8f * 2.5f, 0.0f ) );
        //setGravity( LVector3( 0.0f, /*-9.8f*/-9.81f * 10.0f/* * 2.0f*/, 0.0f ) );

        mSoftBodyWorldInfo = new btSoftBodyWorldInfo();
        mSoftBodyWorldInfo->air_density   = 1.2f;
	    mSoftBodyWorldInfo->water_density = 0;
	    mSoftBodyWorldInfo->water_offset  = 0;
	    mSoftBodyWorldInfo->water_normal  = btVector3( 0.0f, 0.0f, 0.0f );
	    mSoftBodyWorldInfo->m_gravity     = mBtWorld->getGravity();
	    mSoftBodyWorldInfo->m_broadphase  = mBtBroadphase;
	    mSoftBodyWorldInfo->m_dispatcher  = mBtCollisionDispatcher;
	    mSoftBodyWorldInfo->m_sparsesdf.Initialize();
	    mSoftBodyWorldInfo->m_sparsesdf.Reset();

        /*
           const btScalar	s=4;
	const btScalar	h=6;
	const int		r=20;

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0,h-2,0));



	int fixed=0;//4+8;
	btSoftBody*		psb=btSoftBodyHelpers::CreatePatch(*mSoftBodyWorldInfo,btVector3(-s,h,-s),
		btVector3(+s,h,-s),
		btVector3(-s,h,+s),
		btVector3(+s,h,+s),r,r,fixed,true);
	mBtWorld->addSoftBody(psb);
	psb->setTotalMass(0.1);

	psb->m_cfg.piterations = 10;
	psb->m_cfg.citerations = 10;
	psb->m_cfg.diterations = 10;
//	psb->m_cfg.viterations = 10;

*/
        LN_LOG_INIT_END;
		return ResultCode_OK;
    }


	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::finalize()
    {
        LN_LOG_FIN_BEGIN;

		mRigidBodyArrayList.clear();
		mCollisionBodyArray.clear();
		mJointArray.clear();
      
        LN_LOG_FIN_END;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::updateBefore()
	{
		

		

		//-----------------------------------------------------------
		// add～ で追加された新規オブジェクトをワールドに登録

		// RigidBody
		RigidBodyArray::ObjectArray& regArray = mRigidBodyArrayList.getRegistOrderObjectArray();
		ln_foreach( RigidBody* b, regArray )
		{
			mBtWorld->addRigidBody( b->getBtRigidBody(), b->getGroup(), b->getGroupMask() );
		}
		// CollisionBody
		CollisionBodyArray::ObjectArray& collisionArray = mCollisionBodyArray.getRegistOrderObjectArray();
		ln_foreach( CollisionBody* b, collisionArray )
		{
			mBtWorld->addCollisionObject( b->getBtCollisionObject(), b->getGroup(), b->getGroupMask() );
		}
		// Joint
		JointArray::ObjectArray& jointArray = mJointArray.getRegistOrderObjectArray();
		ln_foreach( JointBase* j, jointArray )
		{
			mBtWorld->addConstraint( j->getBtConstraint() );
		}

		mRigidBodyArrayList.commit();
		mCollisionBodyArray.commit();
		mJointArray.commit();

		// 剛体の姿勢を同期
		RigidBodyArray::ObjectArray& objectArray = mRigidBodyArrayList.getObjectArray();
		ln_foreach( RigidBody* b, objectArray )
		{
			b->syncBeforeUpdate();
		}
	}

	//----------------------------------------------------------------------
	// ● ミュレーション更新
	//----------------------------------------------------------------------
    void Manager::update( lnFloat elapsed_time_ )
    {
		// 剛体の姿勢を同期
		RigidBodyArray::ObjectArray& objectArray = mRigidBodyArrayList.getObjectArray();
		ln_foreach( RigidBody* b, objectArray )
		{
			b->syncBeforeUpdate();
			//b->syncUpdating();
		}

		// RigidBody
		//RigidBodyArray::ObjectArray& regArray = mRigidBodyArrayList.getObjectArray();
		//ln_foreach( RigidBody* b, regArray )
		//{
		//	b->preUpdate();
		//}
		//// CollisionBody
		//CollisionBodyArray::ObjectArray& collisionArray = mCollisionBodyArray.getRegistOrderObjectArray();
		//ln_foreach( CollisionBody* b, collisionArray )
		//{
		//	b->preUpdate();
		//}
		//// Joint
		//JointArray::ObjectArray& jointArray = mJointArray.getRegistOrderObjectArray();
		//ln_foreach( JointBase* j, jointArray )
		//{
		//	
		//}

		//printf("▼\n");


		// 
		//http://d.hatena.ne.jp/ousttrue/20100425/1272165711

		// 処理落ちしている場合は演算結果を捨てたい。
		// そのため、上側の FPS タイマーの経過時間とは合わなくなることがある。
		// 可能な限り他の処理の影響を受けないようにするため、物理演算用の時間計算は直前で行う。
		lnU32 elapsedTime = 16;
		lnU32 time = System::Environment::getSystemTime();
		if ( mLastUpdateTime > 0 )
		{
			elapsedTime = time - mLastUpdateTime;
		}
		mLastUpdateTime = time;
#if 1
		// 16ms に間に合っていない時はステップ数を上げないと、
		// ジョイントで結合された剛体が遅れてしまった。(ばねが極端に伸びたように見える)
		mBtWorld->stepSimulation( 
			0.001f * elapsedTime, 
			((elapsedTime + 16) / 16) /*+ 1*/ );	

		//mBtWorld->stepSimulation( elapsed_time_, 120, 0.008333334f );	// 1.0 / 120
		
#else
		static int a = 0;
        if (a <= 1) mBtWorld->stepSimulation( elapsed_time_, 1, 1.0f / 60.0f );
		++a;
#endif
		//printf("▲\n");
		/*
        // 後処理
        RigidBodyList::iterator itr = mRigidBodyList.begin();
        RigidBodyList::iterator end = mRigidBodyList.end();
        for ( ; itr != end; ++itr )
        {
            (*itr)->postUpdate();
        }
		*/
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::updateContents()
	{
		// TODO 他のも
		{
			RigidBodyArray::ObjectArray& objectArray = mRigidBodyArrayList.getObjectArray();
			ln_foreach( RigidBody* b, objectArray )
			{
				b->clearContactList();
			}
		}

		//-----------------------------------------------------------
		// 接触リスト更新
		//		ひとつの Manifold が持つ Contact のうち、最も近いものを選択
		int manifoldCount = mBtCollisionDispatcher->getNumManifolds();
		for ( int i = 0; i < manifoldCount; i++ )
		{
			btPersistentManifold* manifold = mBtCollisionDispatcher->getManifoldByIndexInternal( i );
			const btCollisionObject* pObjA = (const btCollisionObject*)manifold->getBody0();
			const btCollisionObject* pObjB = (const btCollisionObject*)manifold->getBody1();

			ContactInfo info;
			float min = FLT_MAX;
			int ContactCount = manifold->getNumContacts();
			for ( int j = 0; j < ContactCount; j++ )
			{
				btManifoldPoint& Point = manifold->getContactPoint( j );
				float Distance = Point.getDistance();
				if ( Distance < 0.0f )
				{
					if ( Distance < min )
					{
						const btVector3& PointB = Point.m_positionWorldOnB;
						const btVector3& NormalB = Point.m_normalWorldOnB;

						min = Distance;

						info.Position.x = PointB.getX();
						info.Position.y = PointB.getY();
						info.Position.z = PointB.getZ();
						info.Normal.x = NormalB.getX();
						info.Normal.y = NormalB.getY();
						info.Normal.z = NormalB.getZ();
					}
				}
			}

			if ( min < 0.0f )
			{
				BodyBase* bodyA = static_cast<BodyBase*>(pObjA->getUserPointer());
				BodyBase* bodyB = static_cast<BodyBase*>(pObjB->getUserPointer());

				info.TargetBody = bodyB;
				bodyA->addContact( info );
				info.TargetBody = bodyA;
				bodyB->addContact( info );
			}
		}




		// 剛体の姿勢を同期
		RigidBodyArray::ObjectArray& objectArray = mRigidBodyArrayList.getObjectArray();
		ln_foreach( RigidBody* b, objectArray )
		{
			b->syncUpdating();
		}
	}


	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::setGravity( const LVector3& gravity_ )
    {
        mBtWorld->setGravity( btVector3( gravity_.x, gravity_.y, gravity_.z ) );
    }

	//-------------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------------
    TriMesh* Manager::createStaticTriangleMesh( const LNTriangleMeshBodyInfo& info_, lnU16 group_, lnU16 mask_ )
    {
	    return createStaticTriangleMesh( &info_, 1, group_, mask_ );
    }

	//-------------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------------
    TriMesh* Manager::createStaticTriangleMesh( const LNTriangleMeshBodyInfo* info_, lnU16 info_count_, lnU16 group_, lnU16 mask_ )
    {
		return NULL;
#if 0
	    btTriangleIndexVertexArray* bt_tri_mesh_data = new btTriangleIndexVertexArray();

	    for ( lnU32 i = 0; i < info_count_; ++i )
	    {
		    btIndexedMesh bt_mesh;
		    bt_mesh.m_numTriangles        = info_[i].IndexCount / 3;
		    bt_mesh.m_triangleIndexBase   = reinterpret_cast<const lnU8*>(info_[i].IndexTable);
		    bt_mesh.m_triangleIndexStride = info_[i].IndexStride * 3;
		    bt_mesh.m_numVertices         = info_[i].VertexCount;
		    bt_mesh.m_vertexBase          = reinterpret_cast<const lnU8*>(info_[i].VertexTable);
		    bt_mesh.m_vertexStride        = sizeof(LVector3);
            bt_mesh.m_indexType           = (info_[i].IndexStride == 4) ? PHY_INTEGER : PHY_SHORT;
		    if ( (bt_mesh.m_numTriangles >= 1) && (bt_mesh.m_numVertices >= 3) )
		    {
			    bt_tri_mesh_data->addIndexedMesh( bt_mesh, bt_mesh.m_indexType );
		    }
	    }

	    btBvhTriangleMeshShape* bt_tri_mesh_shape =
            new btBvhTriangleMeshShape( bt_tri_mesh_data, true, true );

	    btVector3 local_inertia( 0.0f, 0.0f, 0.0f );

	    // 剛体を作成する
	    btDefaultMotionState* motion_state = LN_NEW btDefaultMotionState();
	    btRigidBody::btRigidBodyConstructionInfo body_info( 0.0f, motion_state, bt_tri_mesh_shape, local_inertia );
	    btRigidBody* bt_rigid_body = new btRigidBody( body_info );
	    bt_rigid_body->setCollisionFlags( bt_rigid_body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK );

        // TriMesh
        RigidBodyInitArgs args;
        args.Shape      = bt_tri_mesh_shape;
        args.RigidBody  = bt_rigid_body;
        args.Group      = group_;
        args.Mask       = mask_;
	    TriMesh* triangle_mesh = LN_NEW TriMesh( this );
        triangle_mesh->initialize( args, bt_tri_mesh_data );
	    bt_rigid_body->setUserPointer( triangle_mesh );

	    return triangle_mesh;
#endif
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//bool Manager::rayTestSingle( const BodyBase* target_body_, const LVector3& from_, const LVector3& to_, LVector3* hit_pos_, LVector3* hit_normal_ )
	//{
	//    mBtWorld->
	//    rayTest
	//    btRayAabb
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool Manager::rayCast( const LVector3& from_, const LVector3& to_, LVector3* hit_pos_, LVector3* hit_normal_, BodyBase** hit_body_, lnU16 group_, lnU16 mask_ )
    {
        btVector3 from( from_.x, from_.y, from_.z );
	    btVector3 to( to_.x, to_.y, to_.z );

	    btCollisionWorld::ClosestRayResultCallback result_callback( from, to );
	    result_callback.m_collisionFilterGroup = group_;
	    result_callback.m_collisionFilterMask  = mask_;

	    mBtWorld->rayTest( from, to, result_callback );

	    if ( result_callback.hasHit() )
	    {
            if ( hit_body_ )
            {
		        *hit_body_ = static_cast<BodyBase*>(result_callback.m_collisionObject->getUserPointer());
            }

            if ( hit_pos_ )
            {
		        hit_pos_->x = result_callback.m_hitPointWorld.getX();
		        hit_pos_->y = result_callback.m_hitPointWorld.getY();
		        hit_pos_->z = result_callback.m_hitPointWorld.getZ();
            }

            if ( hit_normal_ )
            {
		        hit_normal_->x = result_callback.m_hitNormalWorld.getX();
		        hit_normal_->y = result_callback.m_hitNormalWorld.getY();
		        hit_normal_->z = result_callback.m_hitNormalWorld.getZ();
            }
		    return true;
	    }

	    return false;
    }

	//----------------------------------------------------------------------
	// ● デバッグ描画
	//----------------------------------------------------------------------
    LNRESULT Manager::drawDebugShape( IDebugRenderer* renderer )
    {

        gDebugDrawer.setDebugRenderer( renderer );
        mBtWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

        mBtWorld->debugDrawWorld();

        //RigidBodyList::iterator itr = mRigidBodyList.begin();
        //RigidBodyList::iterator end = mRigidBodyList.end();
        //for ( ; itr != end; ++itr )
        //{
        //    LN_CALL_R( (*itr)->drawDebugShape( renderer_ ) );
        //}
		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    btDynamicsWorld* Manager::getDynamicsWorld() const
    { 
        return mBtWorld;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::addRigidBody( RigidBody* body )
	{ 
		mRigidBodyArrayList.registerObject( body ); 
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::addCollisionBody( CollisionBody* body )
	{
		mCollisionBodyArray.registerObject( body ); 
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::addJoint( JointBase* joint )
	{
		mJointArray.registerObject( joint ); 
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//void Manager::removeRigidBody( RigidBody* body )
	//{ 
	//	/*
 //       btRigidBody* bt_rb = body->getBtRigidBody();

	//    btMotionState* pState = bt_rb->getMotionState();
	//    SAFE_DELETE( pState );

	//    btCollisionShape* pShape = bt_rb->getCollisionShape();
	//    SAFE_DELETE( pShape );

	//    mBtWorld->removeCollisionObject( bt_rb );
	//    SAFE_DELETE( bt_rb );
	//	*/
	//}
#if 0
	//-------------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------------
	//bool Manager::CustomMaterialCombinerCallback( btManifoldPoint& Point, const btCollisionObject* pCollObj0, int partId0, int index0, const btCollisionObject* pCollObj1, int partId1, int index1 )
	bool Manager::CustomMaterialCombinerCallback(
		btManifoldPoint& cp,	
		const btCollisionObjectWrapper* colObj0Wrap,
		int partId0,
		int index0,
		const btCollisionObjectWrapper* colObj1Wrap,
		int partId1,
		int index1 )
	{
		// CollisionBody は一切影響しない
		if ( (colObj0Wrap->getCollisionObject()->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE) != 0
		||	 (colObj1Wrap->getCollisionObject()->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE) != 0
		){
			return false;
		}

		// 処理開始
		BodyBase* pBody0 = static_cast<BodyBase*>(colObj0Wrap->getCollisionObject()->getUserPointer());
		BodyBase* pBody1 = static_cast<BodyBase*>(colObj1Wrap->getCollisionObject()->getUserPointer());

		BodyType Type0 = pBody0->getBodyType();
		BodyType Type1 = pBody1->getBodyType();

		// 接地処理
		//Kernel::Math::VectorS vNormal( Point.m_normalWorldOnB.getX(), Point.m_normalWorldOnB.getY(), Point.m_normalWorldOnB.getZ() );
		//bool IsGround = false;

		// キャラ同士は無視
		if ( (Type0 == BodyType_CharacterObject) && (Type1 == BodyType_CharacterObject) )
		{
			cp.m_combinedFriction = 0.0f;
			cp.m_combinedRestitution = FLT_MIN;
			return false;
		}

		// 両方キャラじゃない場合は無視
		if ( (Type0 != BodyType_CharacterObject) && (Type1 != BodyType_CharacterObject) )
		{
			return false;
		}

		// 接地処理
		if ( Type0 == BodyType_CharacterObject )
		{
			// TODO normal とキャラの上方向の dot を取って、角度で接地しているかをチェック
			//pBody0->接地
		}
		else
		if ( Type1 == BodyType_CharacterObject )
		{
			//pBody1->接地
		}

		//Point.m_combinedFriction = IsGround ? 1.0f : 0.0f;
		cp.m_combinedRestitution = FLT_MIN;

		return true;
	}
#endif
} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================