﻿//==============================================================================
// LFAudio 
//------------------------------------------------------------------------------
///**
//  @file       LFAudio.h
//  @brief      LFAudio
//	@defgroup   group_lib_audio Audio
//	@{
//*/
//==============================================================================

#pragma once

#include "LFCommon.h"
#include "LFTypedef.h"

extern "C" {

//==============================================================================
// LNAudio
//------------------------------------------------------------------------------
///**
//  @defgroup   group_audio LNAudio
//  @brief      音声管理及びゲームオーディオ機能
//  @{
//*/
//==============================================================================

	//----------------------------------------------------------------------
	///**
	//  @brief      BGM を演奏する
	//  @param[in]  filePath	: ファイル名
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @param[in]  fadeTime	: フェードインにかける時間 (ミリ秒)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
    LNOTEAPI LNResult LNAudio_PlayBGM( const LNChar* filePath, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100), int fadeTime LNOTE_DEFAULT_ARG(0) );
	/*DeclOpt
	HSP_Override = true;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief      メモリ上の音声データから BGM を演奏する
	//  @param[in]  data		: メモリ上の音声ファイルデータ
	//  @param[in]  dataSize	: データサイズ (バイト単位)
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @param[in]  fadeTime	: フェードインにかける時間 (ミリ秒)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
    LNOTEAPI LNResult LNAudio_PlayBGMMem( const void* data, int dataSize, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100), int fadeTime LNOTE_DEFAULT_ARG(0) );

	//----------------------------------------------------------------------
	///**
	//  @brief      BGM の演奏を停止する
	//  @param[in]  fadeTime	: フェードアウトにかける時間 (ミリ秒)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNAudio_StopBGM(int fadeTime LNOTE_DEFAULT_ARG(0));

	//----------------------------------------------------------------------
	///**
	//  @brief      BGS を演奏する
	//  @param[in]  filePath	: ファイル名
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @param[in]  fadeTime	: フェードインにかける時間 (ミリ秒)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
    LNOTEAPI LNResult LNAudio_PlayBGS( const LNChar* filePath, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100), int fadeTime LNOTE_DEFAULT_ARG(0) );
	/*DeclOpt
	HSP_Override = true;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief      メモリ上の音声データから BGS を演奏する
	//  @param[in]  data		: メモリ上の音声ファイルデータ
	//  @param[in]  dataSize	: データサイズ (バイト単位)
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @param[in]  fadeTime	: フェードインにかける時間 (ミリ秒)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNAudio_PlayBGSMem(const void* data, int dataSize, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100), int fadeTime LNOTE_DEFAULT_ARG(0));

	//----------------------------------------------------------------------
	///**
	//  @brief      BGS の演奏を停止する
	//  @param[in]  fadeTime	: フェードアウトにかける時間 (ミリ秒)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNAudio_StopBGS(int fadeTime LNOTE_DEFAULT_ARG(0));

	//----------------------------------------------------------------------
	///**
	//  @brief      ME を演奏する
	//  @param[in]  filePath	: ファイル名
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
    LNOTEAPI LNResult LNAudio_PlayME( const LNChar* filePath, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100) );
	/*DeclOpt
	HSP_Override = true;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief      メモリ上の音声データから ME を演奏する
	//  @param[in]  data		: メモリ上の音声ファイルデータ
	//  @param[in]  dataSize	: データサイズ (バイト単位)
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNAudio_PlayMEMem(const void* data, int dataSize, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100));

	//----------------------------------------------------------------------
	///**
	//  @brief      ME の演奏を停止する
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNAudio_StopME();

	//----------------------------------------------------------------------
	///**
	//  @brief      SE を演奏する
	//  @param[in]  filePath	: ファイル名
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
    LNOTEAPI LNResult LNAudio_PlaySE( const LNChar* filePath, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100) );
	/*DeclOpt
	HSP_Override = true;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief      SE を演奏する (3D サウンド)
	//  @param[in]  filePath	: ファイル名
	//  @param[in]  position	: 3D 空間上の座標
	//  @param[in]  distance	: 減衰距離
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNAudio_PlaySE3D(const LNChar* filePath, const LNVector3* position, float distance, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100));
	/*DeclOpt
	HSP_Override = true;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief      SE を演奏する (3D サウンド)
	//  @param[in]  filePath	: ファイル名
	//  @param[in]  x			: 3D 空間上の X 座標
	//  @param[in]  y			: 3D 空間上の Y 座標
	//  @param[in]  z			: 3D 空間上の Z 座標
	//  @param[in]  distance	: 減衰距離
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
    LNOTEAPI LNResult LNAudio_PlaySE3DXYZ( const LNChar* filePath, float x, float y, float z, float distance, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100) );
	/*DeclOpt
	HSP_Override = true;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief      メモリ上の音声データから SE を演奏する
	//  @param[in]  data		: メモリ上の音声ファイルデータ
	//  @param[in]  dataSize	: データサイズ (バイト単位)
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNAudio_PlaySEMem(const void* data, int dataSize, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100));

	//----------------------------------------------------------------------
	///**
	//  @brief      メモリ上の音声データから SE を演奏する (3D サウンド)
	//  @param[in]  data		: メモリ上の音声ファイルデータ
	//  @param[in]  dataSize	: データサイズ (バイト単位)
	//  @param[in]  position	: 3D 空間上の座標
	//  @param[in]  distance	: 減衰距離
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNAudio_PlaySE3DMem(const void* data, int dataSize, const LNVector3* position, float distance, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100));

	//----------------------------------------------------------------------
	///**
	//  @brief      メモリ上の音声データから SE を演奏する (3D サウンド)
	//  @param[in]  data		: メモリ上の音声ファイルデータ
	//  @param[in]  dataSize	: データサイズ (バイト単位)
	//  @param[in]  x			: 3D 空間上の X 座標
	//  @param[in]  y			: 3D 空間上の Y 座標
	//  @param[in]  z			: 3D 空間上の Z 座標
	//  @param[in]  distance	: 減衰距離
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNAudio_PlaySE3DMemXYZ(const void* data, int dataSize, float x, float y, float z, float distance, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100));

	//----------------------------------------------------------------------
	///**
	//  @brief      すべての SE の演奏を停止する
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNAudio_StopSE();

	//----------------------------------------------------------------------
	///**
	//  @brief      3D 空間の1メートル相当の距離の設定
	//  @param[in]  distance	: 距離
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNAudio_SetMetreUnitDistance(float distance);

	//----------------------------------------------------------------------
	///**
	//  @brief     再生中のBGMの音量を設定する (フェードアウト中は無効)
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  fadeTime	: フェードアウトにかける時間 (ミリ秒)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNAudio_SetBGMVolume(int volume, int fadeTime LNOTE_DEFAULT_ARG(0));

	//----------------------------------------------------------------------
	///**
	//  @brief      再生中のBGSの音量を設定する (フェードアウト中は無効)
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  fadeTime	: フェードアウトにかける時間 (ミリ秒)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNAudio_SetBGSVolume(int volume, int fadeTime LNOTE_DEFAULT_ARG(0));

	//----------------------------------------------------------------------
	///**
	//  @brief      [使用非推奨] BGM、BGS、ME として再生されているサウンドオブジェクトの取得
	//  @param[in]  type	: 取得するサウンドの種類
	//  @return     サウンドハンドル
	//	@details	この関数は、BGM、BGS、ME の音量、ピッチの変更、及び
	//				再生位置の取得のために用意されています。
	//				この関数から取得したサウンドハンドルに対して
	//				Release、Play、Stop、Pause を行わないでください。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNHandle LNAudio_GetInternalGameSound( LNInternalGameSound type );
	/*DeclOpt
	ReturnHandleType = "LNSound";
	DeclOpt*/

/** @} */

//==============================================================================
// LNSoundListener
//------------------------------------------------------------------------------
///**
//  @defgroup   group_soundlistener LNSoundListener
//  @brief      3D音声リスナー
//  @{
//*/
//==============================================================================

	//----------------------------------------------------------------------
	///**
	//  @brief      リスナーの位置の設定
	//  @param[in]  position	: 3D 空間上の座標
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSoundListener_SetPosition(const LNVector3* position);

	//----------------------------------------------------------------------
	///**
	//  @brief      リスナーの位置の設定
	//  @param[in]  x			: 3D 空間上の X 座標
	//  @param[in]  y			: 3D 空間上の Y 座標
	//  @param[in]  z			: 3D 空間上の Z 座標
	//*/
	//----------------------------------------------------------------------
    LNOTEAPI void LNSoundListener_SetPositionXYZ( float x, float y, float z );

	//----------------------------------------------------------------------
	///**
	//  @brief      リスナーの正面方向の設定
	//  @param[in]  direction		: 向き
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSoundListener_SetDirection(const LNVector3* direction);

	//----------------------------------------------------------------------
	///**
	//  @brief      リスナーの正面方向の設定
	//  @param[in]  x			: 向きの X 成分
	//  @param[in]  y			: 向きの Y 成分
	//  @param[in]  z			: 向きの Z 成分
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNSoundListener_SetDirectionXYZ(float x, float y, float z);

	//----------------------------------------------------------------------
	///**
	//  @brief      リスナーの上方向の設定 (正面方向とは直交であること)
	//  @param[in]  direction	: 上方向
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSoundListener_SetUpDirection(const LNVector3* direction);

	//----------------------------------------------------------------------
	///**
	//  @brief      リスナーの上方向の設定 (正面方向とは直交であること)
	//  @param[in]  x			: 向きの X 成分
	//  @param[in]  y			: 向きの Y 成分
	//  @param[in]  z			: 向きの Z 成分
	//*/
	//----------------------------------------------------------------------
    LNOTEAPI void LNSoundListener_SetUpDirectionXYZ( float x, float y, float z );

	//----------------------------------------------------------------------
	///**
	//  @brief      リスナーの速度の設定
	//  @param[in]  velocity	: 速度
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSoundListener_Velocity(const LNVector3* velocity);

	//----------------------------------------------------------------------
	///**
	//  @brief      リスナーの速度の設定
	//  @param[in]  x			: 速度の X 成分
	//  @param[in]  y			: 速度の Y 成分
	//  @param[in]  z			: 速度の Z 成分
	//*/
	//----------------------------------------------------------------------
    LNOTEAPI void LNSoundListener_VelocityXYZ( float x, float y, float z );
    
/** @} */

//==============================================================================
// LNSound
//------------------------------------------------------------------------------
///**
//  @defgroup   group_sound LNSound
//  @brief      音声機能
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNSound);

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルから音声を作成する
	//  @param[out]	sound		: sound
	//  @param[in]	filePath	: ファイル名
	//  @param[in]	enable3d	: true の場合、3D 音源として作成する
	//  @param[in]	playType	: 音声の再生方法
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
    LNOTEAPI LNResult LNSound_Create(LNHandle* sound, const LNChar* filePath, LNBool enable3d LNOTE_DEFAULT_ARG(LN_FALSE), LNSoundPlayingType playType LNOTE_DEFAULT_ARG(LN_SOUNDPLAYINGTYPE_AUTO) );
	
	//----------------------------------------------------------------------
	///**
	//  @brief      メモリ上の音声ファイルデータから音声を作成する
	//  @param[out]	sound		: sound
	//  @param[in]	data		: メモリ上の音声データへのポインタ
	//  @param[in]	dataSize	: データサイズ (バイト単位)
	//  @param[in]	enable3d	: true の場合、3D 音源として作成する
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_CreateMem(LNHandle* sound, const void* data, int dataSize, LNBool enable3d LNOTE_DEFAULT_ARG(LN_FALSE));

	//----------------------------------------------------------------------
	///**
	//  @brief      ボリュームの設定
	//  @param[in]  sound		: サウンドハンドル
	//  @param[in]  volume          : ボリューム (0～100)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_SetVolume(LNHandle sound, int volume);

	//----------------------------------------------------------------------
	///**
	//  @brief      ボリュームの取得
	//  @param[in]	sound		: サウンドハンドル
	//  @param[out]	volume		: ボリュームを格納する変数
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_GetVolume(LNHandle sound, int* volume);

	//----------------------------------------------------------------------
	///**
	//  @brief      ピッチの設定
	//  @param[in]  sound		: サウンドハンドル
	//  @param[in]  pitch		: ピッチ (50～200)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_SetPitch(LNHandle sound, int pitch);

	//----------------------------------------------------------------------
	///**
	//  @brief      ピッチの取得
	//  @param[in]	sound		: サウンドハンドル
	//  @param[out]	pitch		: ピッチを格納する変数
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_GetPitch(LNHandle sound, int* pitch);

	//----------------------------------------------------------------------
	///**
	//  @brief      ループ再生の有効設定
	//  @param[in]  sound		: サウンドハンドル
	//  @param[in]  loopEnable	: LN_TRUE = ループ再生する / LN_FALSE = しない
	//  @param[in]  begin		: ループ領域の先頭位置 (サンプル数単位)
	//  @param[in]  length		: ループ領域長さ (サンプル数単位)
	//  @details	begin と length に 0 を指定すると、全体をループ領域として設定します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_SetLoopState(LNHandle sound, LNBool loopEnable, int begin LNOTE_DEFAULT_ARG(0), int length LNOTE_DEFAULT_ARG(0));

	//----------------------------------------------------------------------
	///**
	//  @brief      ループ再生が有効かを調べる
	//  @param[in]	sound		: サウンドハンドル
	//  @param[out]	enabled		: ループ再生の有無状態を格納する変数
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_IsLoop(LNHandle sound, LNBool* enabled);

	//----------------------------------------------------------------------
	///**
	//  @brief      サウンドの再生状態を取得する
	//  @param[in]  sound		: サウンドハンドル
	//  @param[out]	state		: 状態を格納する変数
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_GetPlayState(LNHandle sound, LNSoundPlayingState* state);

	//----------------------------------------------------------------------
	///**
	//  @brief      音声を再生する
	//  @param[in]  sound		: サウンドハンドル
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_Play(LNHandle sound);

	//----------------------------------------------------------------------
	///**
	//  @brief      再生を停止する
	//  @param[in]  sound		: サウンドハンドル
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_Stop(LNHandle sound);

	//----------------------------------------------------------------------
	///**
	//  @brief      一時停止
	//  @param[in]  sound		: サウンドハンドル
	//  @param[in]  pause		: LN_TRUE = 一時停止 / LN_FALSE = 一時停止解除
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_Pause(LNHandle sound, LNBool pause);

	//----------------------------------------------------------------------
	///**
	//  @brief      音量のフェード
	//  @param[in]  sound			: サウンドハンドル
	//  @param[in]  targetVolume	: 変更先の音量
	//  @param[in]  time			: フェードにかける時間 (ミリ秒)
	//  @param[in]  fadeState		: フェード完了後の動作の指定
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_FadeVolume(LNHandle sound, int targetVolume, int time, LNSoundFadeState fadeState);

	//----------------------------------------------------------------------
	///**
	//  @brief      再生したサンプル数の取得
	//  @param[in]  sound		: サウンドハンドル
	//  @param[out]	samples		: 再生したサンプル数を格納する変数
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_GetUnitsPlayed(LNHandle sound, int* samples);

	//----------------------------------------------------------------------
	///**
	//  @brief      音声データ全体のサンプル数の取得
	//  @param[in]  sound		: サウンドハンドル
	//  @param[out]	samples		: 音声データ全体のサンプル数を格納する変数
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_GetTotalUnits(LNHandle sound, int* samples);

	//----------------------------------------------------------------------
	///**
	//  @brief      サンプリング周波数の取得
	//  @param[in]  sound		: サウンドハンドル
	//  @param[out]	frequency	: サンプリング周波数を格納する変数
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_GetUnitsParSecond(LNHandle sound, int* frequency);

	//----------------------------------------------------------------------
	///**
	//  @brief		3D 音源かを調べる
	//  @param[in]  sound		: サウンドハンドル
	//  @param[out] enabled		: 3D 音源かを示す値を格納する変数
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_Is3DSound(LNHandle sound, LNBool* enabled);

	//----------------------------------------------------------------------
	///**
	//  @brief		3D 音源としての位置を設定する
	//  @param[in]  sound		: サウンドハンドル
	//  @param[in]  position	: 3D 空間上の座標
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_SetEmitterPosition(LNHandle sound, const LNVector3* position);

	//----------------------------------------------------------------------
	///**
	//  @brief		3D 音源としての位置を設定する
	//  @param[in]  sound		: サウンドハンドル
	//  @param[in]  x			: 3D 空間上の X 座標
	//  @param[in]  y			: 3D 空間上の Y 座標
	//  @param[in]  z			: 3D 空間上の Z 座標
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_SetEmitterPositionXYZ(LNHandle sound, float x, float y, float z);

	//----------------------------------------------------------------------
	///**
	//  @brief		3D 音源としての速度を設定する
	//  @param[in]  sound		: サウンドハンドル
	//  @param[in]  velocity	: 速度
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_SetEmitterVelocity(LNHandle sound, const LNVector3* velocity);

	//----------------------------------------------------------------------
	///**
	//  @brief		3D 音源としての速度を設定する
	//  @param[in]  sound		: サウンドハンドル
	//  @param[in]  x			: 速度の X 成分
	//  @param[in]  y			: 速度の Y 成分
	//  @param[in]  z			: 速度の Z 成分
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_SetEmitterVelocityXYZ(LNHandle sound, float x, float y, float z);

	//----------------------------------------------------------------------
	///**
	//  @brief		3D 音源の減衰距離 (聴こえなくなる距離) を設定する
	//  @param[in]  sound		: サウンドハンドル
	//  @param[in]  distance	: 距離
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_SetEmitterDistance(LNHandle sound, float distance);

/** @} */

} // extern "C"

/** @} */	// defgroup
