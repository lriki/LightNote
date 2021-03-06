//==============================================================================
// FPSController 
//------------------------------------------------------------------------------
///**
//  @file       FPSController.h
//  @brief      FPSController
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "GameTime.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Game
{

// FPSController クラスで使う実数の型 ( double が必要なときは変更してビルドする )
#define lnFPSFloat float

//==============================================================================
// ■ FPSController クラス
//------------------------------------------------------------------------------
///**
//  @brief      FPS ( Frame Par Second ) の制御モジュール
//
//  @par
//              待機だけを行いたい場合は初期化後、process() を呼び出すだけで
//              コンストラクタに渡されたフレームレートになるように待機します。
//
//  @note
//              自分のノート PC で、なんか動きが妙にぎこちないと思って
//              1フレームの経過時間を調べたら、不定期で1フレーム70msかかる
//              ものがあった。(ほんとに不定期。1秒(60回に0回だったり4回だったり。)
//              他のフレームではほぼ0.016キープ)
//              詳しく調べてみると、winAPI の Sleep がたまにすごく時間かかってた。
//              Sleep( 16 ) で待機してるのに、さっきと同じく 70ms かかってたとか。
//              Sleep() は確実に指定した時間だけ待ってくれる関数じゃない
//              ことはわかってたけど、環境によってここまで変わるとは
//              (デスクトップの方では問題なかった)正直思わなかったので、
//              もし今後似たようなことがあった場合はこの辺を参考にしてみる。
//*/
//==============================================================================
class FPSController
    : public Base::NonCopyable
{
public:

	//----------------------------------------------------------------------
	///**
	//  @brief      コンストラクタ
	//
	//  @par
	//              フレームレート 60 で初期化します。<br>
	//              60 以外に設定したい場合は setFrameRate() を使ってください。
	//*/
	//----------------------------------------------------------------------
	FPSController();

	/// デストラクタ
	~FPSController();

public:

	//----------------------------------------------------------------------
	///**
	//  @brief      GameTime クラスの取得
	//*/
	//----------------------------------------------------------------------
	const GameTime& getGameTime() const { return mGameTime; }

	//----------------------------------------------------------------------
	///**
	//  @brief      現在の FPS 値の取得
	//*/
	//----------------------------------------------------------------------
	lnFPSFloat getFPS() const { return ( mAverageTime > 0 ) ? ( 1.0f / mAverageTime ) : 0; }

	//----------------------------------------------------------------------
	///**
	//  @brief      前回の更新から経過したゲーム時間の取得
	//*/
	//----------------------------------------------------------------------
	lnFPSFloat getElapsedGameTime() const { return mElapsedGameTime; }

	//----------------------------------------------------------------------
	///**
	//  @brief      前回の更新から経過した実時間の取得
	//*/
	//----------------------------------------------------------------------
	lnFPSFloat getElapsedRealTime() const { return mElapsedRealTime; }

	//----------------------------------------------------------------------
	///**
	//  @brief      ゲームの開始以降のゲーム時間の取得
	//*/
	//----------------------------------------------------------------------
	lnFPSFloat getTotalGameTime() const { return 0.001f * mTotalGameTime; }

	//----------------------------------------------------------------------
	///**
	//  @brief      ゲームの開始以降の実時間の取得
	//*/
	//----------------------------------------------------------------------
	lnFPSFloat getTotalRealTime() const { return 0.001f * mTotalRealTime; }

	//----------------------------------------------------------------------
	///**
	//  @brief      最大 FPS 値の取得
	//
	//  @par
	//              処理にどれだけ余裕があるかを示す値を取得します。<br>
	//              この値がフレームレートの値よりも大きいほど、処理に余裕があります。<br><br>
	//              
	//              遅延が全くなかった場合 ( 60フレームの間、全ての処理が 1ミリ秒以内に終了した場合 ) は
	//              0 除算となってしまうため、1 秒にあった遅延を 1 ミリ秒として計算しています。<br><br>
	//              
	//              この関数で値を取得できるようにするには、
	//              getEnableFPSTest() に true を設定してください。
	//*/
	//----------------------------------------------------------------------
	lnFPSFloat getCapacityFPS() const { return mCapacityFPS; }

	//----------------------------------------------------------------------
	///**
	//  @brief      フレームレートの設定
	//
	//  @param[in]  frame_rate_ : フレームレート
	//*/
	//----------------------------------------------------------------------
	void setFrameRate( int frame_rate_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      フレームレートの取得
	//*/
	//----------------------------------------------------------------------
    int getFrameRate() const { return mFrameRate; }

	//----------------------------------------------------------------------
	///**
	//  @brief      FPS テストの有効設定     
	//*/
	//----------------------------------------------------------------------
    void setEnableFPSTest( bool flag_ ) { mEnableFPSTest = flag_; }

	/// process() 時、sleep を行うかの設定
	void setEnableFrameWait( bool enabled ) { mEnableFrameWait = enabled; }

	//----------------------------------------------------------------------
	///**
	//  @brief      フレームレート分のフレームが経過した瞬間を判定する
	//*/
	//----------------------------------------------------------------------
	bool isRoundSec() const { return ( mFrameCount == 0 ); }

	//----------------------------------------------------------------------
	///**
	//  @brief      描画処理等をスキップして速度を確保する必要があるかを判定する
	//
	//  @par
	//              この関数が true を返した場合 ( 最後の process() で待ち時間が 0 以下だった場合 )、
	//              これ以上 FPS をこのクラス内で調整することはできないので、
	//              とにかく安定させるためには他の処理をカットする必要があります。
	//*/
	//----------------------------------------------------------------------
	bool isFrameSkipRequest() const { return ( mTerm <= 0 ); }

	//----------------------------------------------------------------------
	///**
	//  @brief      ゲームループの時間がフレームレートを超えているかを判定する
	//*/
	//----------------------------------------------------------------------
	bool isRunningSlowly() const { return ( mTerm <= mFrameRateRec * 1000 ); }

	//----------------------------------------------------------------------
	///**
	//  @brief      遅延をリセットする
	//
	//  @param[in]  current_time_ : 現在の時間 ( ミリ秒 )
	//
	//  @note
	//              例えばウィンドウが非アクティブのときはゲームの更新処理をしないようにしたとき等の
	//              長い時間の非アクティブ状態からアクティブ状態になった場合、
	//              待ち時間を計算を普通に行うと
	//              極端なフレーム落ちが発生したとして、都合のよくない
	//              時間待ちが発生する。
	//              それを防ぐため、アクティブ状態になったときにこの関数を
	//              呼ぶことで、大きな遅延を回避する。
	//*/
	//----------------------------------------------------------------------
	void refreshSystemDelay( lnU32 current_time_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      更新処理
	//
	//  @par
	//              各値の計算をして、必要な時間だけウェイトをとります。<br>
	//              1 フレームに一度呼び出してください。       
	//*/
	//----------------------------------------------------------------------
	void process();

	/// ウェイトは取らず、測定のみ行う (ツール用)
    void processForMeasure();

private:

	int		    mFrameRate;			///< フレームレート
	lnFPSFloat	mFrameRateRec;		///< フレームレートの逆数
	lnFPSFloat	mFPS;				///< 現在の FPS 値
    
	lnFPSFloat	mCurrentTime;		///< 現在の時間 ( システム起動からの時間 )
	lnFPSFloat	mAverageTime;		///< 1 フレームの平均時間

	int		    mFrameCount;		///< fps 計測用の経過フレーム数 ( 60fpsにする場合は0〜60でループ )
	lnFPSFloat	mTerm;				///< wait() 内で使う待ち時間
	lnFPSFloat	mLastTime;			///< 1 周前の時間を記憶する変数
	lnFPSFloat	mBaseTime;			///< 1 秒にに一度つくられる基準時間
	lnFPSFloat*	mFrameTimes;		///< 各フレームの時間を格納する配列 ( 平均の計算に使う )

    lnFPSFloat  mElapsedGameTime;   ///< ひとつ前のフレームからの経過ゲーム時間
    lnFPSFloat  mLastRealTime;      ///< ひとつ前のフレームの実時間
    lnFPSFloat  mElapsedRealTime;   ///< ひとつ前のフレームからの経過実時間

    lnU32         mStartTime;         ///< コンストラクタで設定したスタート時間
    lnU32         mTotalGameTime;     ///< 経過ゲーム時間の合計
    lnU32         mTotalRealTime;     ///< 経過実時間の合計

    lnFPSFloat  mCapacityFPS;
    lnFPSFloat  mCapaFPSLastTime;
    lnFPSFloat*	mCapaFrameTimes;    ///< 各フレームの時間を格納する配列 ( 平均の計算に使う )
    lnFPSFloat  mCapaAverageTime;

    GameTime    mGameTime;

    bool        mEnableFPSTest;         ///< true の場合、FPS テストを行う
	bool		mEnableFrameWait;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Game
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================