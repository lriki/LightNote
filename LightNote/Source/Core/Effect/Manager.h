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

#if 1

#include "../Base/ReferenceObjectArray.h"
#include "../Task/ParallelTask.h"
#include "Common.h"

namespace LNote
{
namespace Core
{
namespace Effect
{
//==============================================================================
// ■ Manager
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
	struct ConfigData
	{
        FileIO::Manager*	FileManager;
        Graphics::Manager*	GraphicsManager;
        Audio::Manager*		AudioManager;
    };

public:
    Manager();
	~Manager();

public:

	/// 初期化
    void initialize( const ConfigData& configData );

	/// 終了処理
    void finalize();

	/// エフェクトの作成
	Effect* createEffect( const lnChar* filePath );

	/// 保持している各描画クラスに行列を設定する (SceneGraph と関連付けられている場合は必要に応じて呼ばれる)
	void setViewProjection( const LMatrix& view, const LMatrix& proj );

	/// フレームレートの変更
	void setFrameRate( lnFloat frameRate ) { mFrameRate = frameRate; }

	/// [描画情報構築タスク] 描画リストを更新する (update() の後で呼ぶこと)
    void updateRenderContents();

	void OnPauseDevice();
	void OnResumeDevice();

public:
	FileIO::Manager*	getFileIOManager() { return mFileManager; }
	Graphics::Manager*	getGraphicsManager() { return mGraphicsManager; }
	Audio::Manager*		getAudioManager() { return mAudioManager; }
	void				addEffect( EffectBase* effect );
	void				internalUpdate();
	lnFloat				getInternalFrameRate() { return mContext.mFrameRate; }

private:
	typedef Base::ReferenceObjectArray<EffectBase*>	EffectBaseArray;

	FileIO::Manager*	mFileManager;
    Graphics::Manager*	mGraphicsManager;
	Audio::Manager*		mAudioManager;

	EffectBaseArray		mEffectBaseArray;
	Task::ParallelTask	mUpdateThread;

	EffekseerEffectEngine*	mEffekseerEffectEngine;
	
	lnFloat	mFrameRate;

	Threading::Mutex	mCacheFlash;

	struct
	{
		lnFloat	mFrameRate;
	} mContext;

};

} // namespace Effect
} // namespace Core
} // namespace LNote

#else

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Base/Cache.h"
#include "../Threading/Thread.h"
#include "../Threading/Barrier.h"
#include "../Graphics/Common/SpriteRenderer.h"
#include "../Graphics/Common/RingRenderer.h"
#include "../Graphics/Manager.h"
#include "../Game/GameTime.h"
#include "Instance/EffectInstance.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Effect
{
typedef Base::CacheObjectManager< EffectCore >     EffectCoreCache;

//==============================================================================
// ■ Manager クラス
//------------------------------------------------------------------------------
///**
//  @brief		エフェクトの管理クラス
//*/
//==============================================================================
class Manager
    : public Base::ReferenceObject
    , public EffectCoreCache
{
public:

	/// 初期化データ
	struct InitData
	{
        Base::LogFile*      LogFile;
        FileIO::Manager*      FileManager;
        Graphics::Manager*  GraphicsManager;
        lnU32               EffectCacheSize;
        lnU32               MaxSpriteNum;
    };

public:

    Manager();
    LN_REFOBJ_METHODS;

protected:

	virtual ~Manager();

public:

	/// 初期化
    void initialize( const InitData& init_data_ );

	/// 終了処理
    void finalize();

	/// 保持している各描画クラスに行列を設定する (SceneGraph と関連付けられている場合は必要に応じて呼ばれる)
    void setViewProjection( const LMatrix& view_, const LMatrix& proj_, bool is_3d_pahse_ );

	/// 空のエフェクトを作成 (ルートノードのみ作成)
	//Effect* createEffect();

	/// 空のエフェクトコアの作成 (ルートノードのみ作成)
	//EffectCore* createEmptyEffectCore();

	/// エフェクトシステムと関連付けられている 3D スプライト描画クラスの取得
    Graphics::SpriteRenderer* getSprite3DRenderer() { return mSprite3DRenderer; }

	/// エフェクトシステムと関連付けられている 2D スプライト描画クラスの取得
    Graphics::SpriteRenderer* getSprite2DRenderer() { return mSprite2DRenderer; }

    Graphics::RingRenderer* getRingRenderer() { return mRingRenderer; }

	/// 時間のスケール係数の設定 (通常はフレームレートの値をそのまま渡しておく。デフォルト値は 60。setTime() で時間の値に乗算される)
    void setTimeTickPerSec( lnFloat tick_ ) { mTimeTick = tick_; }

	/// 時間のスケール係数の取得
    lnFloat getTimeTickPerSec() const { return mTimeTick; }

	/// 時間を進める (通常、秒単位で渡す)
	//void advanceTime( lnTime delta_time_ );

	/// ゲームタイムの設定 (基本的に毎ループする)
    void setGameTime( const Game::GameTime& game_time_ );

	/// [更新タスク] 各エフェクトを更新する
    void update();

	/// [描画情報構築タスク] 描画リストを更新する (update() の後で呼ぶこと)
    void updateDrawContents();


	///// エフェクト描画開始
	//void beginDraw();

	///// エフェクト描画終了
	//void endDraw();

	/// バッチ描画 (3D 空間)
	//void drawBatch3D();

	/// バッチ描画 (2D 空間)
	//void drawBatch2D();

public:

    FileIO::Manager* getFileManager() { return mFileManager; }

    Graphics::Manager* getGraphicsManager() { return mGraphicsManager; }

private:
	friend class EffectInstance;
    void addEffectInstance( EffectInstance* effect_ );
    void removeEffectInstance( EffectInstance* effect_ );

	/// すべての要素の修正済みフラグ設定する
	//void setAllModifiedFlag( bool flag_ );

private:

	/// 各エフェクトを更新する
    void _update();

private:

    typedef std::vector< EffectInstance* >  EffectInstanceArray;
    typedef std::list< EffectInstance* >    EffectInstanceList;

	///// 実行スレッド
	//class UpdateThread
	//    : public Threading::ThreadObject
	//{
	//public:

	//    /// コンストラクタ
	//    UpdateThread( Manager* manager_ ) : mManager( manager_ ) {}

	//    /// スレッド関数
	//    void run();

	//private:

	//    Manager*    mManager;
	//};

private:

    Base::LogFile*              mLogFile;
    FileIO::Manager*              mFileManager;
    Graphics::Manager*          mGraphicsManager;
    Graphics::SpriteRenderer*   mSprite3DRenderer;
    Graphics::SpriteRenderer*   mSprite2DRenderer;
    Graphics::RingRenderer*     mRingRenderer;

    EffectInstanceArray         mEffectInstanceArray;
    EffectInstanceList          mAddedEffectInstanceList;
	EffectInstanceList			mEffectInstanceRemoveOrderList;
    EffectInstanceArray         mDrawingInstanceArray;

	//Threading::Barrier*            mAsyncBarrier;
	//Threading::EventFlag*          mAsyncUpdating;
	//Threading::EventFlag*          mEndRequested;
	//UpdateThread*               mUpdateThread;

	//lnTime          mTime;
    Game::GameTime  mGameTime;
    lnFloat         mTimeTick;

    friend class UpdateThread;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Effect
} // namespace Core
} // namespace LNote
#endif

//==============================================================================
//
//==============================================================================