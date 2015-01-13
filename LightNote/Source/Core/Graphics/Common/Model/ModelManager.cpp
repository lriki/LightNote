//==============================================================================
// ModelManager 
//------------------------------------------------------------------------------
/*
	[2014/3/14]
	MMDX から読み取れる更新順序
	・アニメーションをボーンへ適用	MMDModel.BoneUpdate()
	・ボーンのグローバル行列更新	MMDModel.BoneUpdate()
	・<ボーン位置合わせ>			MMDXでは対応していない	http://www10.atwiki.jp/mmdphysics/pages/14.html
	・IK更新						MMDModel.BoneUpdate()
	・物理更新(相互反映)			MMDModel.SkinUpdate()	PhysicsManager.Update() -> motionState.Flush()
	・物理更新						stepSimulation()
	・物理更新(※フレーム落ち対策)	PhysicsManager.DropFrame() -> motionState.FlushFD()
	・表情更新						MMDModel.SkinUpdate()	MMDFaceManager.Update()
	・表情頂点更新					MMDModel.SkinUpdate()
	・スキン行列更新				MMDModel.SkinUpdate()
	・頂点更新						MMDModel.SkinUpdate()
	・描画

	※フレーム落ち対策…
		現在は常に有効。最後に Flush() された時のボーン行列を覚えておいて、
		ボーンを剛体位置に移動させる処理で使用する。
		Flash() が割と重い処理なので、それをスキップするための処置だと思われる。

	現行LNoteは ボーン→剛体 と 剛体→ボーン を別のタイミングに分けていたけど、その必要は無さそう。
	むしろ　IK更新 → 剛体をボーン位置へ  の方が自然になるはず。

	そして LNote 的にまとめなおすと、
	・アニメーションをボーンへ適用
	・ボーンのグローバル行列更新
	・剛体更新① (ボーンを剛体[物理]と[物理+位置合わせ]へ移動)
	・ボーン位置合わせ											※ここまでで、ボーン行列の更新はIK以外完了しておくこと
	★ユーザー任意のボーン更新
	・IK更新													※ボーン行列に対する最終更新。これ以降はボーン行列は変わらない
	・物理更新② (剛体[ボーン追従]をボーン位置へ移動)			
	・表情スキニング
	・opt頂点スキニング
	◆描画情報更新
	◆描画


	[2014/3/21]
	これまで全てのボーンはひとつの配列で管理していて、参照はインデックスで行っていた。
	(ParentIndex や ChildIndexList 等)

	コピーは高速だが、追加削除(しないと思うけど)等に柔軟に対応できないし、
	階層更新するときは for で回せるからシンプルではあるけど
	配列の先頭から順に、ルートボーンから下っていく並びが必須となる。

	今後はアニメーションXファイルや他のモデルファイルのサポートするだろうし、
	変に高速化とか気を使うよりも汎用的な形にした方が良いと思う。

	というわけで、ポインタ管理に変更。

	[2014/3/21]
	・・・が、やっぱり挿入・削除の対応がつらい。
	例えばIKリンクの真ん中のボーンを消したときは前後はどうするの？とか、
	考えなければならないことがどんどん出てきそう。
	それに、実際ゲーム内でボーン等を挿入・削除することがあるのかって言うと疑問。
	できればライブラリとしてはいい感じだけど、普通使わない。

	インデックス管理ならコピーも高速だし、処理もシンプル。
	ということでこのまま・・・。

	[2014/11/11] MMDX
	MMDMotionState.Flash()
		ボーンの種類に応じて、同期を行っている。ボーン→剛体も、剛体→ボーンも、この関数の中。
		剛体の姿勢は m_graphicsWorldTrans。
		これは virtual メソッド setWorldTransform getWorldTransform により Bullet に渡る。

	MMDCore.Update()
		PhysicsThreadManager.Update()	メインスレッド。マルチスレッド有効の時は、物理スレッドの更新完了を待ってから Sync を呼んでいる。
										もし遅れていた場合はDropFrameを呼ぶ。
			PhysicsThreadManager.Sync()
				PhysicsThreadManager.Instanse.Synchronize event
					PhysicsManager.Update()
						MMDMotionState.Flash()

	物理スレッドは PhysicsThreadManager.threadFunc()。
	無限ループ。
	描画と並列に行われる。
	つまり、
	・アニメ更新			・物理更新
	・剛体とボーン同期
	・スキニング行列生成	・物理更新
	・描画					・物理更新
	なので、物理ボーン(髪とか)の状態が1フレーム遅れる。
	キャラが高速で直線移動すると、なんか服だけちょっとずれで見えたりする。
	正解動作は以下のようになる。
	・アニメ更新
	・ボーン追従の剛体位置をボーンに合わせる
	・物理更新
	・物理ボーンの位置を剛体に合わせる
	・スキニング行列生成
	・描画
	非同期処理が難しい。

	または、物理同期時に剛体追従ボーンと剛体のグローバル空間でのずれ(オフセット)を覚えておいて、
	物理ボーンのスキニング行列を作るときに、このオフセットで補正する。
	これができれば描画と非同期で更新できる。


	MikuMikuFlex は…
		BulletManager.StepSimulation() が呼ばれているのはメインスレッド。
		しかもこれ、BulletManager は PMXPhysicsTransformManager 単位。
		つまり、PMXモデル1つに付き1つの BulletWorld が用意されている。

	ちなみにゲームロジックとしての物理は非同期更新でいいの？
	RayCastができないので論外。
*/
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../../Resource/LNResource.h"
#include "../../../FileIO/InFile.h"
#include "../../../Graphics/Common/GraphicsUtil.h"
#include "Core/ModelCore.h"
#include "Motion.h"
#include "MMDModelLoader.h"
#include "PMXLoader.h"
#include "VMDLoader.h"
#include "ModelManager.h"

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
// ■ ModelManager
//==============================================================================

	//----------------------------------------------------------------------
	// ● コンストラクタ
	//----------------------------------------------------------------------
    ModelManager::ModelManager()
        : mLogFile          ( NULL )
		, mGraphicsManager	( NULL )
        , mGraphicsDevice   ( NULL )
        , mFileManager      ( NULL )
        , mPhysicsManager   ( NULL )
    {
    }

	//----------------------------------------------------------------------
	// ● デストラクタ
	//----------------------------------------------------------------------
    ModelManager::~ModelManager()
    {
    }

	//----------------------------------------------------------------------
	// ● 初期化
	//----------------------------------------------------------------------
    LNRESULT ModelManager::initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

		mGraphicsManager = init_data_.GraphicsManager;
        mGraphicsDevice = init_data_.GraphicsDevice;
        mFileManager    = init_data_.FileManager;
        mPhysicsManager = init_data_.PhysicsManager;

        //ModelCoreCache::initialize( init_data_.ModelCoreCacheSize );
		mModelCoreCache.initialize( init_data_.ModelCoreCacheSize, 0 );

		// MMDトゥーンテクスチャ
		mMMDDefaultToonTexture[0] = Util::createTexture( mGraphicsManager, Resource::ID_Image_toon01 );
		mMMDDefaultToonTexture[1] = Util::createTexture( mGraphicsManager, Resource::ID_Image_toon02 );
		mMMDDefaultToonTexture[2] = Util::createTexture( mGraphicsManager, Resource::ID_Image_toon03 );
		mMMDDefaultToonTexture[3] = Util::createTexture( mGraphicsManager, Resource::ID_Image_toon04 );
		mMMDDefaultToonTexture[4] = Util::createTexture( mGraphicsManager, Resource::ID_Image_toon05 );
		mMMDDefaultToonTexture[5] = Util::createTexture( mGraphicsManager, Resource::ID_Image_toon06 );
		mMMDDefaultToonTexture[6] = Util::createTexture( mGraphicsManager, Resource::ID_Image_toon07 );
		mMMDDefaultToonTexture[7] = Util::createTexture( mGraphicsManager, Resource::ID_Image_toon08 );
		mMMDDefaultToonTexture[8] = Util::createTexture( mGraphicsManager, Resource::ID_Image_toon09 );
		mMMDDefaultToonTexture[9] = Util::createTexture( mGraphicsManager, Resource::ID_Image_toon10 );

        LN_LOG_INIT_END;
		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	// ● 終了処理
	//----------------------------------------------------------------------
    void ModelManager::finalize()
    {
        LN_LOG_FIN_BEGIN;

		for ( int i = 0; i < 10; ++i )
		{
			LN_SAFE_RELEASE( mMMDDefaultToonTexture[i] );
		}

        //ModelCoreCache::finalize();
		mModelCoreCache.finalize();

        LN_LOG_FIN_END;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Model2* ModelManager::createModel2(const FileIO::PathNameW& baseDir, FileIO::Stream* stream, lnU32 flags, lnSharingKey key)
	{
		// キャッシュ検索
		ModelCore2* core = static_cast<ModelCore2*>( mModelCoreCache.findCacheAddRef( key ) );
		if ( core == NULL ) 
		{
			// PMD
			MMDModelLoader pmdLoader;
			core = pmdLoader.load(this, stream, baseDir, flags & ModelCreateFlag_SoftwareSkinning);

			// PMX
			if ( core == NULL ) {
				PMXLoader pmxLoader;
				core = pmxLoader.load(this, stream, baseDir, flags & ModelCreateFlag_SoftwareSkinning);
			}

			if (core == NULL) {
				LN_THROW_InvalidFormat(0);
			}
			core->postInitialize();

			// キャッシュ管理に追加
			if ( !key.isEmpty() ) {
				mModelCoreCache.registerCachedObject( key, core );
			}
		}

		// TODO テクスチャ存在しないのを無視

		LRefPtr<Model2> model( LN_NEW Model2() );
		model->create( core, flags );
		model.addRef();
		core->release();
		return model;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Animation::AnimationClip* ModelManager::createMotion2( FileIO::Stream* stream )
	{
		// TODO: モーションのキャッシュ

		VMDLoader2 loader;
		if ( !loader.load( stream ) ) {
			return NULL;
		}

		// ひとつのアニメーションクリップにボーンとフェイスを流し込んでいく
		Animation::AnimationClip* boneAnimClip = LN_NEW Animation::AnimationClip();

		VMDLoader2::BoneAnimationArray& boneAnimArray = loader.getBoneAnimationArray();
		ln_foreach( VMDLoader2::BoneAnimation& anim, boneAnimArray )
		{
			boneAnimClip->addAnimationCurve( anim.TargetBoneName.c_str(), anim.Animation );
		}

		VMDLoader2::FaceAnimationArray& faceAnimArray = loader.getFaceAnimationArray();
		ln_foreach( VMDLoader2::FaceAnimation& anim, faceAnimArray )
		{
			boneAnimClip->addAnimationCurve( anim.TargetFaceName.c_str(), anim.Animation );
		}

		return boneAnimClip;
	}

#if 0
	//----------------------------------------------------------------------
	// ● モデルファイルを読み込む
	//----------------------------------------------------------------------
    LNRESULT ModelManager::createModel( Model** model_, void* data_, lnU32 size_, lnU32 flags_, lnSharingKey key_ )
    {
        LNRESULT lr;
		bool success;

        ModelCore* core;// = LN_NEW ModelCore( this );

		LRefPtr<ModelCore> modelCore;

        // PMD
		modelCore.attach( LN_NEW ModelCore( this ) );
		if ( modelCore->checkPMDData( data_, size_ ) )
		{
			modelCore->loadPMD( data_, size_, flags_, model_ );
			modelCore.addRef();
			return LN_OK;
		}

		// PMX
		core = LN_NEW ModelCore( this );
        success = core->loadPMX( data_, size_, flags_, model_ );
        if ( success ) return LN_OK;
		core->release();

#if defined(LNOTE_DIRECTX)
        // X
		core = LN_NEW ModelCore( this );
        core->loadXFile( data_, size_, flags_, model_ );
		//if ( success ) return LN_OK;
		//core->release();
#endif

        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● モデルファイルを読み込む
	//----------------------------------------------------------------------
    LNRESULT ModelManager::createModel( Model** model_, FileIO::Stream* stream_, lnU32 flags_, lnSharingKey key_ )
    {
        lnU32 size = stream_->getSize();
        lnByte* buf = mTempBuffer.lock( size );

        stream_->read( buf, size );

        LNRESULT lr = createModel( model_, buf, size, flags_, key_ );

        mTempBuffer.unlock();
        return lr;
    }

	//----------------------------------------------------------------------
	// ● モデルファイルを読み込む (ファイル名指定)
	//----------------------------------------------------------------------
    LNRESULT ModelManager::createModel( Model** model_, const lnChar* filename_, lnU32 flags_, lnSharingKey key_ )
    {
        lnChar dir[ LN_MAX_PATH ];
        lnChar filename[ LN_MAX_PATH ];
        FileIO::Path::getDirPathAndFilePath( filename_, dir, filename );

        // ディレクトリ移動
		mFileManager->moveCurrentDirectory( dir );

		LRefPtr<FileIO::InFile> file(
			mFileManager->createInFile( filename ) );

		createModel( model_, file, flags_, key_ );
        
        // ディレクトリ復帰
		mFileManager->returnCurrentDirectory();

        //LN_SAFE_RELEASE( file );
        return LN_OK;
    }
#endif

#if 1
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Animation::AnimationClip* ModelManager::createMotion( const void* data, lnU32 size )
	{
		LN_THROW_NotImpl(0);
		return NULL;
#if 0
		VMDLoader loader;
		loader.load( data, size );

		Animation::AnimationClip* boneAnimClip = LN_NEW Animation::AnimationClip();

		VMDLoader::BoneAnimationArray& boneAnimArray = loader.getBoneAnimationArray();
		ln_foreach( VMDLoader::BoneAnimation& anim, boneAnimArray )
		{
			boneAnimClip->addAnimationCurve( anim.TargetBoneName.c_str(), anim.Animation );
		}

		VMDLoader::FaceAnimationArray& faceAnimArray = loader.getFaceAnimationArray();
		ln_foreach( VMDLoader::FaceAnimation& anim, faceAnimArray )
		{
			boneAnimClip->addAnimationCurve( anim.TargetFaceName.c_str(), anim.Animation );
		}

		return boneAnimClip;
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Animation::AnimationClip* ModelManager::createMotion( FileIO::Stream* stream )
	{
		lnU32 size = stream->getSize();
		Base::TempBufferLock lock( mTempBuffer, size );

        lnByte* buf = lock.getBuffer();
        stream->read( buf, size );

		return createMotion( buf, size );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Animation::AnimationClip* ModelManager::createMotion( const lnChar* filePath )
	{
		LRefPtr<FileIO::InFile> file( mFileManager->createInFile( filePath ) );
		return createMotion( file );
	}

#else
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelManager::createMotion( 
		const void* data, 
		lnU32 size, 
		Animation::AnimationClip** boneMotion, 
		Animation::AnimationClip** faceMotion )
	{
		*boneMotion = NULL;
		*faceMotion = NULL;

		VMDLoader loader;
		loader.load( data, size );

		VMDLoader::BoneAnimationArray& boneAnimArray = loader.getBoneAnimationArray();
		if ( !boneAnimArray.empty() )
		{
			Animation::AnimationClip* boneAnimClip = LN_NEW Animation::AnimationClip();
			ln_foreach( Animation::BezierSQTTransformAnimation* anim, boneAnimArray )
			{

			}
		}

		

	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelManager::createMotion( 
		FileIO::IInStream* stream,
		Animation::AnimationClip** boneMotion, 
		Animation::AnimationClip** faceMotion )
	{
		lnU32 size = stream->getSize();
		Base::TempBufferLock lock( mTempBuffer, size );

        lnByte* buf = lock.getBuffer();
        stream->read( buf, size );

		createMotion( buf, size, boneMotion, faceMotion );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelManager::createMotion( 
		const lnChar* filePath, 
		Animation::AnimationClip** boneMotion, 
		Animation::AnimationClip** faceMotion )
	{
		LRefPtr<FileIO::InFile> file( mFileManager->createInFile( filePath ) );
		createMotion( file, boneMotion, faceMotion );
	}
#endif



	//----------------------------------------------------------------------
	// ● モーションファイルを読み込む
	//----------------------------------------------------------------------
    LNRESULT ModelManager::createMotion( Motion** motion_, void* data_, lnU32 size_, lnSharingKey key_ )
    {
        LNRESULT lr;

        Motion* motion = LN_NEW Motion();
		lr = motion->initializeVMD(data_, size_);
		if (LN_FAILED(lr)) { return lr; }

        *motion_ = motion;
		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	// ● モーションファイルを読み込む
	//----------------------------------------------------------------------
    LNRESULT ModelManager::createMotion( Motion** motion_, FileIO::Stream* stream_, lnSharingKey key_ )
    {
        lnU32 size = stream_->getSize();
        lnByte* buf = mTempBuffer.lock( size );

        stream_->read( buf, size );

        LNRESULT lr = createMotion( motion_, buf, size, key_ );

        mTempBuffer.unlock();
        return lr;
    }

	//----------------------------------------------------------------------
	// ● モーションファイルを読み込む (ファイル名指定)
	//----------------------------------------------------------------------
    LNRESULT ModelManager::createMotion( Motion** motion_, const lnChar* filename_, lnSharingKey key_ )
    {
		LNRESULT lr = ResultCode_OK;
		LRefPtr<FileIO::InFile> file( 
			mFileManager->createInFile( filename_ ) );

        lr = createMotion( motion_, file, key_ );

        //LN_SAFE_RELEASE( file );
        return lr;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
  //  Texture* ModelManager::createTexture( const char* filePath, lnU32 flags )
  //  {
		//// FileNotFound を無視する場合
		//if ( (flags & ModelCreateFlag_IgnoreTextureNotFound) != 0 ) {
		//	if ( !mFileManager->existsFile( filePath ) ) {
		//		return NULL;
		//	}
		//}

		//LRefPtr<FileIO::InFile> file(
		//	mFileManager->createInFile( filePath ) );
		//return mGraphicsDevice->createTexture( file );
  //  }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Texture* ModelManager::createTexture(const FileIO::PathNameW& filePath, lnU32 flags)
	{
		// FileNotFound を無視する場合
		if ((flags & ModelCreateFlag_IgnoreTextureNotFound) != 0) {
			if (!mFileManager->existsFile(filePath)) {
				return NULL;
			}
		}

		LRefPtr<FileIO::InFile> file(mFileManager->createInFile(filePath));
		return mGraphicsDevice->createTexture(file);
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================