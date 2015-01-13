//==============================================================================
// ModelManager 
//------------------------------------------------------------------------------
///**
//  @file       ModelManager.h
//  @brief      ModelManager
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../../Base/Cache.h"
#include "../../../Base/Misc.h"
#include "../../../FileIO/Interface.h"
#include "../../../FileIO/Manager.h"
#include "../../../Physics/Manager.h"
#include "../../../Animation/AnimationController.h"
#include "../../../Animation/AnimationClip.h"
#include "../../Interface.h"
#include "Model.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
class ModelCore;
class Motion;

//typedef Base::CacheObjectManager< ModelCore >   ModelCoreCache;
//typedef Base::CacheObjectManager< Motion >      MotionCache;

//==============================================================================
// ■ ModelManager クラス
//------------------------------------------------------------------------------
///**
//  @brief		3D モデル管理クラス
//*/
//==============================================================================
class ModelManager
     : public Base::ReferenceObject
     //, public ModelCoreCache
     //, public MotionCache
{
public:

	/// 初期化データ
	struct InitData
	{
        Base::LogFile*      LogFile;
		Manager*			GraphicsManager;
        IGraphicsDevice*    GraphicsDevice;
        FileIO::Manager*      FileManager;
        Physics::Manager*   PhysicsManager;
        lnU32               ModelCoreCacheSize;
	};

public:
    ModelManager();
    LN_REFOBJ_METHODS;

protected:
    virtual ~ModelManager();

public:

	/// 初期化
    LNRESULT initialize( const InitData& init_data_ );

	/// 終了処理
    void finalize();

	/// モデルファイルを読み込む (flags は ModelCreateFlag の組み合わせ)
	Model2* createModel2(const FileIO::PathNameW& baseDir, FileIO::Stream* stream, lnU32 flags, lnSharingKey key );

	/// モーションファイルを読み込む (注意：Bone と Face をひとつのクリップで扱うので、名前が同じ Bone と Face があるとバグる)
	Animation::AnimationClip* createMotion2( FileIO::Stream* stream );



#if 0
	/// モデルファイルを読み込む (flags_ は LNModelFlag の組み合わせ)
    LNRESULT createModel( Model** model_, void* data_, lnU32 size_, lnU32 flags_, lnSharingKey key_ = lnNullKey );

	/// モデルファイルを読み込む
    LNRESULT createModel( Model** model_, FileIO::Stream* stream_, lnU32 flags_, lnSharingKey key_ = lnNullKey );

	/// モデルファイルを読み込む (ファイル名指定)
    LNRESULT createModel( Model** model_, const lnChar* filename_,  lnU32 flags_, lnSharingKey key_ = lnNullKey );
#endif
	
#if 1
	/// (注意：Bone と Face をひとつのクリップで扱うので、名前が同じ Bone と Face があるとバグる)
	Animation::AnimationClip* createMotion( const void* data, lnU32 size );

	Animation::AnimationClip* createMotion( FileIO::Stream* stream );

	Animation::AnimationClip* createMotion( const lnChar* filePath );
#else
	void createMotion( const void* data, lnU32 size, Animation::AnimationClip** boneMotion, Animation::AnimationClip** faceMotion );

	void createMotion( FileIO::IInStream* stream, Animation::AnimationClip** boneMotion, Animation::AnimationClip** faceMotion );

	void createMotion( const lnChar* filePath, Animation::AnimationClip** boneMotion, Animation::AnimationClip** faceMotion );
#endif



	/// モーションファイルを読み込む
    LNRESULT createMotion( Motion** motion_, void* data_, lnU32 size_, lnSharingKey key_ );

	/// モーションファイルを読み込む
    LNRESULT createMotion( Motion** motion_, FileIO::Stream* stream_, lnSharingKey key_ );

	/// モーションファイルを読み込む (ファイル名指定)
    LNRESULT createMotion( Motion** motion_, const lnChar* filename_, lnSharingKey key_ );




	/// IGraphicsDevice の取得
    IGraphicsDevice* getGraphicsDevice() { return mGraphicsDevice; }

public:

	/// 物理管理クラスの取得
    Physics::Manager* getPhysicsManager() { return mPhysicsManager; }

	/// テクスチャ作成 (ModelCore から作成時に呼ばれる。PMD、X ファイルのテクスチャ名は char なので lnChar にはしない)
	//LNRESULT createTexture( ITexture** texture_, const char* filename_ );

	/// テクスチャ作成 (flags は ModelCreateFlag の組み合わせ)
	//Texture* createTexture( const char* filePath, lnU32 flags );

	/// テクスチャ作成
	Texture* createTexture(const FileIO::PathNameW& filePath, lnU32 flags);

	Texture* getMMDDefaultToonTexture( int idx ) { return mMMDDefaultToonTexture[idx]; }

private:

    Base::LogFile*      mLogFile;
	Manager*			mGraphicsManager;
    IGraphicsDevice*    mGraphicsDevice;	//いらなそう
    FileIO::Manager*		mFileManager;
    Physics::Manager*   mPhysicsManager;
    Base::TempBuffer    mTempBuffer;
	Texture*			mMMDDefaultToonTexture[10];

	Base::CacheManager	mModelCoreCache;
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