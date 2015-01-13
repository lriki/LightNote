//==============================================================================
// ModelCore 
//------------------------------------------------------------------------------
///**
//  @file       ModelCore.h
//  @brief      ModelCore
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <map>
#include "../../../../Base/Cache.h"
#include "../../../../Base/RefString.h"
#include "../../../Interface.h"
#include "../../../DeviceObjects.h"

#include "../Common.h"
#include "ModelMaterialCore.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

/// モーフ・ソフトウェアスキニング時に使用するオリジナルの頂点
struct MorphOrgVertex
{
	LVector3	Position;
	LVector2	UV;
	LVector4	AdditionalUV[4];
};

//==============================================================================
// ModelCore2
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ModelCore2
	: public Base::ReferenceObject
	, public Base::ICacheObject
{
	LN_CACHE_OBJECT_DECL;
public:
	ModelCore2( ModelManager* manager, ModelFormat format );
	virtual ~ModelCore2();

public:
	void postInitialize();	///< 読み込み完了後、Manager から呼ばれる
	void sortIKArray();
	const MorphOrgVertex& getOrgVertex( lnU32 index ) const { return mMorphOrgVertices[index]; }

public:

	ModelManager*		Manager;

	lnRefStringW		Name;

    IVertexBuffer*		VertexBuffer;
    IIndexBuffer*		IndexBuffer;

	ModelMaterialCore	Material;


	ModelFrameCoreArray	FrameArray;		///< フレームリスト
	ModelFrameCoreArray	RootFrames;		///< ルートフレーム (PMD は親のいないボーンがいくつかあるので複数管理)

	ModelIKCoreArray	IKArray;		///< [削除予定] IK リスト
	ModelIKCoreArray2	IKs;

	ModelMorphCoreArray		ModelMorphCores;	///< モーフ (表情) リスト
	ModelRigidBodyCoreArray	RigidBodys;	///< 剛体リスト
	ModelConstraintCoreArray	Constraints;	///< コンストレイント(ジョイント)リスト

private:
	ModelFormat					mFormat;
	std::vector<MorphOrgVertex>	mMorphOrgVertices;	///< postInitialize() で生成
};






class Model;
class ModelManager;
class ModelFrame;
class ModelBone;
class ModelFaceCore;
class ModelIK;
class ModelIKLimitter;
class ModelRigidBody;
class ModelConstraint;

typedef std::vector<ModelFaceCore*>	ModelFaceCoreArray;


struct CollisionMesh
{
    lnU32       VertexCount;
    LVector3*   CollisionVertexBuffer;
    lnU32       IndexCount;
    lnU32       IndexStride;            ///< 2 or 4
    void*       CollisionIndexBuffer;
};



//==============================================================================
// ■ ModelCore クラス
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ModelCore
    : public Base::ReferenceObject
{
public:
    ModelCore( ModelManager* manager_ );
    LN_REFOBJ_METHODS;

protected:
    virtual ~ModelCore();

public:

	/// スキンメッシュかを判定
    bool isSkinMesh() const { return mIsSkinMesh; }

	bool checkPMDData( const void* data, lnU32 size );

	/// PMD モデル読み込み (新しく作成して model_ にポインタを返す)
	/// flags_ は LNModelFlag の組み合わせ
    void loadPMD( const void* data_, lnU32 size_, lnU32 flags_, Model** model_ );

	/// PMX モデル読み込み
    bool loadPMX( const void* data, lnU32 size, lnU32 flags, Model** model );

	/// X ファイル読み込み
    void loadXFile( const void* data_top_, lnU32 size_, lnU32 flags_, Model** model_ );

	/// 名前からフレーム番号を取得する (見つからない場合は LN_INVALID_INDEX)
    lnU32 getFrameIndexByName( const lnChar* name_ );

	/// ソフトウェアスキニング
    void skinning( ModelFrame* frames_ );

	/// IK リミッタの取得
    ModelIKLimitter* getIKLimitter() { return mIKLimitter; }

	/// コリジョンメッシュの取得
    const CollisionMesh& getCollisionMesh() { return *mCollisionMesh; }

public:
	IVertexBuffer*	getVertexBuffer() { return mVertexBuffer; }

private:

	/// モデルデータ解放
    void _release();

	/// スフィアマップ用のテクスチャ名かを判定する
    bool _isSphereMapTexName( const char* tex_name_ ) const;

    void _createCollisionBuffers();

private:

    typedef std::map< LRefTString, lnU32 >   FrameIndexMap;
    typedef std::pair< LRefTString, lnU32 >  FrameIndexPair;
    

private:

    ModelManager*           mManager;
    IGraphicsDevice*        mGraphicsDevice;    // 使うの load でだけだからメンバに持たなくていいかも
    LRefTString             mModelName;

	// 共有する
    IVertexBuffer*          mVertexBuffer;
    IIndexBuffer*           mIndexBuffer;
    LVector3*               mOrgVertexPositions;    ///< (ソフトウェアスキニング用)
    LVector3*               mOrgVertexNormals;      ///< (ソフトウェアスキニング用)
    CollisionMesh*          mCollisionMesh;
    

    FrameIndexMap           mFrameIndexMap;     ///< フレーム名に対応するフレーム番号表

	// 共有する
    lnU32                   mIKNum;
    ModelIK*                mIKArray;
    ModelIKLimitter*        mIKLimitter;

	// 共有する
	ModelFaceCoreArray		mModelFaceCoreArray;
	//lnU32                   mFaceNum;
	//ModelFaceCore*              mModelFaces;

    bool                    mIsSkinMesh;

    friend class Model;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================