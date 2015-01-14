//==============================================================================
// Manager 
//------------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include "../Base/ReferenceObjectArray.h"
#include "../Math/LMath.h"
#include "Types.h"

class btDynamicsWorld;
class  btDefaultCollisionConfiguration;
class  btCollisionDispatcher;
struct btDbvtBroadphase;
class  btAxisSweep3;
class  btSequentialImpulseConstraintSolver;
class  btDiscreteDynamicsWorld;
class btSoftRigidDynamicsWorld;
class btCollisionShape;
class btRigidBody;
class btTypedConstraint;
class btTriangleIndexVertexArray;
struct btSoftBodyWorldInfo;

class btCollisionObject;
class btManifoldPoint;
struct btCollisionObjectWrapper;

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
// ■ Manager クラス
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class Manager
    : public Base::ReferenceObject
{
public:

	/// 初期化データ
	struct InitData
	{
        Base::LogFile*      LogFile;
    };

public:

    Manager();

    LN_REFOBJ_METHODS;

public:

	/// 初期化
    LNRESULT initialize( const InitData& init_data_ );

	/// 終了処理
    void finalize();

	/// フレームレートの設定 (デフォルトは 60)
    void setFrameRate( int frame_rate_ );

	/// [メインスレッド]
	void updateBefore();

	/// [物理更新スレッド] シミュレーション更新
    void update( lnFloat elapsed_time_ );

	/// [メインスレッド]同期
	void updateContents();

	/// 接触している物体をチェックしてイベントを発生させる
	//void updateContactPoints();

	/// 重力の設定
    void setGravity( const LVector3& gravity_ );

	/// メッシュの作成
    TriMesh* createStaticTriangleMesh( const LNTriangleMeshBodyInfo& info_, lnU16 group_ = 0xffff, lnU16 mask_ = 0xffff );

	/// メッシュの作成
    TriMesh* createStaticTriangleMesh( const LNTriangleMeshBodyInfo* info_, lnU16 info_count_, lnU16 group_ = 0xffff, lnU16 mask_ = 0xffff );

	/// レイキャスト
	//bool rayTestSingle( const BodyBase* target_body_, const LVector3& from_, const LVector3& to_, LVector3* hit_pos_, LVector3* hit_normal_ );

	/// レイキャスト
    bool rayCast( const LVector3& from_, const LVector3& to_, LVector3* hit_pos_, LVector3* hit_normal_, BodyBase** hit_body_, lnU16 group_ = 0xffff, lnU16 mask_ = 0xffff );

	/// デバッグ描画
    LNRESULT drawDebugShape( IDebugRenderer* renderer_ );

public:

	/// 物理演算ワールドの取得
    btDynamicsWorld* getDynamicsWorld() const;

	void addRigidBody( RigidBody* body );
	void addCollisionBody( CollisionBody* body );
    void addJoint( JointBase* joint );

protected:

    virtual ~Manager();

private:
	//static bool CustomMaterialCombinerCallback( btManifoldPoint& Point, const btCollisionObject* pCollObj0, int partId0, int index0, const btCollisionObject* pCollObj1, int partId1, int index1 );
	static bool CustomMaterialCombinerCallback(btManifoldPoint& cp,	const btCollisionObjectWrapper* colObj0Wrap,int partId0,int index0,const btCollisionObjectWrapper* colObj1Wrap,int partId1,int index1);

private:
	// それぞれワールドへの追加メソッドが違うので分けておく
	typedef Base::ReferenceObjectArray<RigidBody*>		RigidBodyArray;		// ソフトボディとはワールドへの追加方法も違うので分けておく
	typedef Base::ReferenceObjectArray<CollisionBody*>	CollisionBodyArray;
	typedef Base::ReferenceObjectArray<JointBase*>		JointArray;

private:

    Base::LogFile*              mLogFile;

    btDefaultCollisionConfiguration*        mBtCollisionConfig;
	btCollisionDispatcher*                  mBtCollisionDispatcher;
    btDbvtBroadphase*					    mBtBroadphase;
	//btAxisSweep3*                         mBtBroadphase;
	btSequentialImpulseConstraintSolver*    mBtSolver;
	btDiscreteDynamicsWorld*                mBtWorld;
	//btSoftRigidDynamicsWorld*               mBtWorld;

	//btSoftRigidDynamicsWorld*			            mBtWorld;
	//btSoftBodyWorldInfo							mBtSoftBodyWorldInfo;
	//btSoftBodyRigidBodyCollisionConfiguration*	mBtCollisionConfiguration;

    RigidBodyArray			mRigidBodyArrayList;
	CollisionBodyArray		mCollisionBodyArray;
    JointArray				mJointArray;

//#define LN_USE_PARALLEL

#ifdef LN_USE_PARALLEL
	class	btThreadSupportInterface*		m_threadSupportCollision;
	class	btThreadSupportInterface*		m_threadSupportSolver;
#endif

public:
    btSoftBodyWorldInfo*	mSoftBodyWorldInfo;

	lnU32	mLastUpdateTime;
	
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================