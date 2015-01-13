//==============================================================================
// LFScene 
//------------------------------------------------------------------------------
///**
//  @file       LFScene.h
//  @brief      LFScene
//  @defgroup   group_lib_scene SceneGraph
//  @{
//*/
//==============================================================================

#pragma once

#include "LFCommon.h"
#include "LFTypedef.h"

extern "C" {

//==============================================================================
// LNSceneGraph
//------------------------------------------------------------------------------
///**
//  @defgroup   group_scene_graph LNSceneGraph
//  @brief      シーングラフ
//	@details	シーングラフは2D、または3Dの描画空間内に配置されるオブジェクトを
//				階層構造にまとめたものです。
//  @{
//*/
//==============================================================================

/** @} */

//==============================================================================
// LNSceneNode
//------------------------------------------------------------------------------
///**
//  @defgroup   group_visual_node LNSceneNode
//  @brief      シーングラフノードのベースクラス	
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNSceneNode);

	//----------------------------------------------------------------------
	///**
	//  @brief      シーングラフの 2D 空間ルートノードの取得
	//  @param[out]	sceneNode	: シーングラフノードハンドルを格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_GetDefault2DRootNode(LNHandle* sceneNode);

	//----------------------------------------------------------------------
	///**
	//  @brief      シーングラフの 3D 空間ルートノードの取得
	//  @param[out]	sceneNode	: シーングラフノードハンドルを格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_GetDefault3DRootNode(LNHandle* sceneNode);

	//----------------------------------------------------------------------
	///**
	//  @brief      ノード名の取得
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//  @param[out]	outName		: ノードの名前を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_GetName(LNHandle sceneNode, const LNChar** outName);

	//----------------------------------------------------------------------
	///**
	//  @brief      ノード名の設定
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//	@param[in]	name		: 新しい名前
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_SetName(LNHandle sceneNode, const LNChar* name);

	//----------------------------------------------------------------------
	///**
	//  @brief      座標の設定
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//  @param[in]	vec			: 座標
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_SetPosition(LNHandle sceneNode, const LNVector3* vec);

	//----------------------------------------------------------------------
	///**
	//  @brief      座標の設定
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//  @param[in]	x			: X 座標
	//  @param[in]	y			: Y 座標
	//  @param[in]	z			: Z 座標
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_SetPositionXYZ(LNHandle sceneNode, float x, float y, float z);

	//----------------------------------------------------------------------
	///**
	//  @brief      座標の取得
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//  @param[out]	vec			: 座標を格納するベクトル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_GetPosition(LNHandle sceneNode, LNVector3* vec);

	//----------------------------------------------------------------------
	///**
	//  @brief      座標の取得
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//  @param[out]	x			: X 座標を格納する変数
	//  @param[out]	y			: Y 座標を格納する変数
	//  @param[out]	z			: Z 座標を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_GetPositionXYZ(LNHandle sceneNode, float* x, float* y, float* z);

	//----------------------------------------------------------------------
	///**
	//  @brief      回転角度の設定 (ラジアン単位)
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//  @param[in]	vec			: 各軸の回転角度
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_SetAngle(LNHandle sceneNode, const LNVector3* vec);

	//----------------------------------------------------------------------
	///**
	//  @brief      回転角度の設定 (ラジアン単位)
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//  @param[in]	x			: X 軸の回転角度
	//  @param[in]	y			: Y 軸の回転角度
	//  @param[in]	z			: Z 軸の回転角度
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_SetAngleXYZ(LNHandle sceneNode, float x, float y, float z);

	//----------------------------------------------------------------------
	///**
	//  @brief      回転角度の取得 (ラジアン単位)
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//  @param[out]	vec			: 回転角度を格納するベクトル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_GetAngle(LNHandle sceneNode, LNVector3* vec);

	//----------------------------------------------------------------------
	///**
	//  @brief      回転角度の取得 (ラジアン単位)
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//  @param[out]	x			: X 軸の回転角度を格納する変数
	//  @param[out]	y			: Y 軸の回転角度を格納する変数
	//  @param[out]	z			: Z 軸の回転角度を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_GetAngleXYZ(LNHandle sceneNode, float* x, float* y, float* z);

	//----------------------------------------------------------------------
	///**
	//  @brief      拡大率の設定 (ラジアン単位)
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//  @param[in]	vec			: 拡大率
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_SetScale(LNHandle sceneNode, const LNVector3* vec);

	//----------------------------------------------------------------------
	///**
	//  @brief      拡大率の設定 (ラジアン単位)
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//  @param[in]	x			: X 方向の拡大率
	//  @param[in]	y			: Y 方向の拡大率
	//  @param[in]	z			: Z 方向の拡大率
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_SetScaleXYZ(LNHandle sceneNode, float x, float y, float z);

	//----------------------------------------------------------------------
	///**
	//  @brief      拡大率の取得 (ラジアン単位)
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//  @param[out]	vec			: 拡大率を格納するベクトル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_GetScale(LNHandle sceneNode, LNVector3* vec);

	//----------------------------------------------------------------------
	///**
	//  @brief      拡大率の取得 (ラジアン単位)
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//  @param[out]	x			: X 方向の拡大率を格納する変数
	//  @param[out]	y			: Y 方向の拡大率を格納する変数
	//  @param[out]	z			: Z 方向の拡大率を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_GetScaleXYZ(LNHandle sceneNode, float* x, float* y, float* z);

	//----------------------------------------------------------------------
	///**
	//  @brief      移動・回転・拡大の原点の設定
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//  @param[in]	vec			: 各軸のオフセット
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_SetCenter(LNHandle sceneNode, const LNVector3* vec);

	//----------------------------------------------------------------------
	///**
	//  @brief      移動・回転・拡大の原点の設定
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//  @param[in]	x			: X 方向のオフセット
	//  @param[in]	y			: Y 方向のオフセット
	//  @param[in]	z			: Z 方向のオフセット
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_SetCenterXYZ(LNHandle sceneNode, float x, float y, float z);

	//----------------------------------------------------------------------
	///**
	//  @brief      移動・回転・拡大の原点の取得
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//  @param[out]	vec			: 各軸のオフセットを格納するベクトル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_GetCenter(LNHandle sceneNode, LNVector3* vec);

	//----------------------------------------------------------------------
	///**
	//  @brief      移動・回転・拡大の原点の取得
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//  @param[out]	x			: X 方向のオフセットを格納する変数
	//  @param[out]	y			: Y 方向のオフセットを格納する変数
	//  @param[out]	z			: Z 方向のオフセットを格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_GetCenterXYZ(LNHandle sceneNode, float* x, float* y, float* z);

	//----------------------------------------------------------------------
	///**
	//  @brief      座標変換行列の設定
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//	@param[in]	mat			: 座標変換行列
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	単位行列ではない行列が設定されてる場合、
	//				SetPosition()、SetAngle()、SetScale() で設定された値は無効です。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_SetMatrix(LNHandle sceneNode, const LNMatrix* mat);

	//----------------------------------------------------------------------
	///**
	//  @brief      座標変換行列の取得
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//	@param[out]	mat			: 座標変換行列を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_GetMatrix(LNHandle sceneNode, LNMatrix* mat);

	//----------------------------------------------------------------------
	///**
	//  @brief      回転順序の設定
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//	@param[in]	rotOrder	: 回転順序
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_SetRotateOrder(LNHandle sceneNode, LNRotationOrder rotOrder);

	//----------------------------------------------------------------------
	///**
	//  @brief      回転順序の取得
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//	@param[out]	outOrder	: 回転順序を示す値を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_GetRotateOrder(LNHandle sceneNode, LNRotationOrder* outOrder);

	//----------------------------------------------------------------------
	///**
	//  @brief      配置方法の設定
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//	@param[in]	type		: 配置方法
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_SetPlacementType(LNHandle sceneNode, LNPlacementType type);

	//----------------------------------------------------------------------
	///**
	//  @brief      配置方法の取得
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//	@param[out]	outType		: 配置方法を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_GetPlacementType(LNHandle sceneNode, LNPlacementType* outType);

	//----------------------------------------------------------------------
	///**
	//  @brief      子ノードを追加する
	//	@param[in]	sceneNode		: シーングラフノードハンドル
	//	@param[in]	childSceneNode	: 子ノードとして追加するノード
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_AddChild(LNHandle sceneNode, LNHandle childSceneNode);

	//----------------------------------------------------------------------
	///**
	//  @brief      子ノードを取り除く
	//	@param[in]	sceneNode		: シーングラフノードハンドル
	//	@param[in]	childSceneNode	: 取り外す子ノード
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_RemoveChild(LNHandle sceneNode, LNHandle childSceneNode);

	//----------------------------------------------------------------------
	///**
	//  @brief      フレーム更新
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//	@param[in]	elapsedTime	: 経過させる時間
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	エフェクトやアニメーションの状態を elapsedTime 分進めます。
	//				通常、1フレームに1度呼び出します。<br>
	//				自動更新が有効になっている場合はライブラリの
	//				フレーム更新を行った時に自動的に呼ばれるため、
	//				この関数を直接呼び出す必要はありません。<br>
	//				自動更新はデフォルトで有効です。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_Update(LNHandle sceneNode, float elapsedTime);

	//----------------------------------------------------------------------
	///**
	//  @brief      自動更新有無の設定
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//	@param[in]	enabled		: LN_TRUE=有効 / LN_FALSE=無効
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	デフォルト値は LN_TRUE です。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_SetEnableAutoUpdate(LNHandle sceneNode, LNBool enabled);

	//----------------------------------------------------------------------
	///**
	//  @brief      自動更新有無の取得
	//	@param[in]	sceneNode	: シーングラフノードハンドル
	//	@param[out]	outEnabled	: 自動更新の有無を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSceneNode_IsEnableAutoUpdate(LNHandle sceneNode, LNBool* outEnabled);

/** @} */

//==============================================================================
// LNVisualNode
//------------------------------------------------------------------------------
///**
//  @defgroup   group_visual LNVisualNode
//  @brief      描画機能を持つシーンノードのスーパークラス
//	@details	VisualNode は SceneNode のサブクラスです。
//  @extends	LNSceneNode
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNVisualNode);

	//----------------------------------------------------------------------
	///**
	//  @brief      可視状態の設定
	//	@param[in]	visualNode	: ビジュアルノードハンドル
	//	@param[in]	enabled		: LN_TRUE=表示する / LN_FALSE=表示しない
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVisualNode_SetVisible(LNHandle visualNode, LNBool enabled);

	//----------------------------------------------------------------------
	///**
	//  @brief      不透明度の設定
	//	@param[in]	visualNode	: ビジュアルノードハンドル
	//	@param[in]	opacity		: 不透明度 (0.0～1.0)
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVisualNode_SetOpacity(LNHandle visualNode, float opacity);

	//----------------------------------------------------------------------
	///**
	//  @brief      乗算色の設定
	//	@param[in]	visualNode		: ビジュアルノードハンドル
	//	@param[in]	materialIndex	: 設定先のマテリアル番号
	//	@param[in]	color			: 設定する色
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVisualNode_SetColorScale(LNHandle visualNode, int materialIndex, const LNColorF* color);

	//----------------------------------------------------------------------
	///**
	//  @brief      ブレンドカラーの設定
	//	@param[in]	visualNode		: ビジュアルノードハンドル
	//	@param[in]	materialIndex	: 設定先のマテリアル番号
	//	@param[in]	color			: 設定する色
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVisualNode_SetBlendColor(LNHandle visualNode, int materialIndex, const LNColorF* color);

	//----------------------------------------------------------------------
	///**
	//  @brief      色調の設定
	//	@param[in]	visualNode		: ビジュアルノードハンドル
	//	@param[in]	materialIndex	: 設定先のマテリアル番号
	//	@param[in]	tone			: 設定する色調
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVisualNode_SetTone(LNHandle visualNode, int materialIndex, const LNTone* tone);
 
	//----------------------------------------------------------------------
	///**
	//  @brief		UV 変換行列の設定
	//	@param[in]	visualNode		: ビジュアルノードハンドル
	//	@param[in]	materialIndex	: 設定先のマテリアル番号
	//	@param[in]	matrix			: 設定する行列
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVisualNode_SetUVTransform(LNHandle visualNode, int materialIndex, const LNMatrix* matrix);

	//----------------------------------------------------------------------
	///**
	//  @brief		合成方法の設定
	//	@param[in]	visualNode		: ビジュアルノードハンドル
	//	@param[in]	mode			: 合成方法
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVisualNode_SetBlendMode(LNHandle visualNode, LNBlendMode mode);

	//----------------------------------------------------------------------
	///**
	//  @brief		カリング方法の設定
	//	@param[in]	visualNode		: ビジュアルノードハンドル
	//	@param[in]	mode			: カリング方法
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVisualNode_SetCullingMode(LNHandle visualNode, LNCullingMode mode);

	//----------------------------------------------------------------------
	///**
	//  @brief		深度テストの有効設定
	//	@param[in]	visualNode		: ビジュアルノードハンドル
	//	@param[in]	enabled			: 深度テストの有無
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVisualNode_SetEnableDepthTest(LNHandle visualNode, LNBool enabled);

	//----------------------------------------------------------------------
	///**
	//  @brief		深度書き込みの有効設定
	//	@param[in]	visualNode		: ビジュアルノードハンドル
	//	@param[in]	enabled			: 深度書き込みの有無
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVisualNode_SetEnableDepthWrite(LNHandle visualNode, LNBool enabled);


	//----------------------------------------------------------------------
	///**
	//  @brief      シェーダの設定
	//	@param[in]	visualNode	: シーングラフノードハンドル
	//	@param[in]	shader		: シェーダハンドル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVisualNode_SetShader(LNHandle visualNode, LNHandle shader);

/** @} */

//==============================================================================
// LNSprite
//------------------------------------------------------------------------------
///**
//  @defgroup   group_sprite LNSprite
//  @brief      スプライトノード
//	@details	スプライトは VisualNode のサブクラスです。
//  @extends	LNVisualNode
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNSprite);

	//----------------------------------------------------------------------
	///**
	//  @brief      スプライトオブジェクトの作成 (2D空間へ配置する)
	//	@param[out]	sprite		: 作成されたスプライトハンドルを格納する変数
	//	@param[in]	texture		: スプライトが表示するテクスチャのハンドル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSprite_Create(LNHandle* sprite, LNHandle texture);

	//----------------------------------------------------------------------
	///**
	//  @brief      スプライトが表示するテクスチャの設定
	//	@param[in]	sprite	: スプライトハンドル
	//	@param[in]	texture	: テクスチャハンドル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSprite_SetTexture(LNHandle sprite, LNHandle texture);

	//----------------------------------------------------------------------
	///**
	//  @brief      スプライトのサイズの設定
	//	@param[in]	sprite	: スプライトハンドル
	//	@param[in]	width	: 設定する幅
	//	@param[in]	height	: 設定する高さ
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSprite_SetSize(LNHandle sprite, float width, float height);

	//----------------------------------------------------------------------
	///**
	//  @brief      テクスチャの転送矩形の設定
	//	@param[in]	sprite	: スプライトハンドル
	//	@param[in]	rect	: 転送元矩形
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSprite_SetSrcRect(LNHandle sprite, const LNRect* rect);

	//----------------------------------------------------------------------
	///**
	//  @brief      テクスチャの転送矩形の設定
	//	@param[in]	sprite	: スプライトハンドル
	//	@param[in]	x		: 転送元の左上の X 座標
	//	@param[in]	y		: 転送元の左上の Y 座標
	//	@param[in]	w		: 転送元の幅
	//	@param[in]	h		: 転送元の高さ
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSprite_SetSrcRectXYWH(LNHandle sprite, int x, int y, int w, int h);

	//----------------------------------------------------------------------
	///**
	//  @brief      テクスチャの転送矩形の取得
	//	@param[in]	sprite	: スプライトハンドル
	//	@param[out]	rect	: 矩形を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSprite_GetSrcRect(LNHandle sprite, LNRect* rect);

	//----------------------------------------------------------------------
	///**
	//  @brief      テクスチャの転送矩形の取得
	//	@param[in]	sprite	: スプライトハンドル
	//	@param[out]	x		: 転送元の左上の X 座標 を格納する変数
	//	@param[out]	y		: 転送元の左上の Y 座標 を格納する変数
	//	@param[out]	w		: 転送元の幅 を格納する変数
	//	@param[out]	h		: 転送元の高さ を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSprite_GetSrcRectXYWH(LNHandle sprite, int* x, int* y, int* w, int* h);

	//----------------------------------------------------------------------
	///**
	//  @brief      左右反転有無の設定
	//	@param[in]	sprite	: スプライトハンドル
	//	@param[in]	enabled	: LN_TRUE=反転する / LN_FALSE=反転しない
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSprite_SetFlipH(LNHandle sprite, LNBool enabled);

	//----------------------------------------------------------------------
	///**
	//  @brief      左右反転有無の取得
	//	@param[in]	sprite		: スプライトハンドル
	//	@param[out]	outEnabled	: 左右反転の有無を示す値を格納する変数 (LN_TRUE=反転する / LN_FALSE=反転しない)
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSprite_IsFlipH(LNHandle sprite, LNBool* outEnabled);

	//----------------------------------------------------------------------
	///**
	//  @brief      上下反転有無の設定
	//	@param[in]	sprite	: スプライトハンドル
	//	@param[in]	enabled	: LN_TRUE=反転する / LN_FALSE=反転しない
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSprite_SetFlipV(LNHandle sprite, LNBool enabled);

	//----------------------------------------------------------------------
	///**
	//  @brief      上下反転有無の取得
	//	@param[in]	sprite	: スプライトハンドル
	//	@param[out]	outEnabled	: 左右反転の有無を示す値を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSprite_IsFlipV(LNHandle sprite, LNBool* outEnabled);

	//----------------------------------------------------------------------
	///**
	//  @brief      スプライトの正面方向の設定
	//	@param[in]	sprite		: スプライトハンドル
	//	@param[in]	frontAxis	: 正面方向を示す値
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	デフォルトは LN_FRONTAXIS_Z で、
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSprite_SetFrontAxis(LNHandle sprite, LNFrontAxis frontAxis);
	
/** @} */	// group_sprite

//==============================================================================
// LNSpritePath
//------------------------------------------------------------------------------
///**
//  @defgroup   group_spritepath LNSpritePath
//  @brief      スプライトパスノード
//	@details	SpritePath は VisualNode のサブクラスです。
//  @extends	LNVisualNode
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNSpritePath);

	//----------------------------------------------------------------------
	///**
	//  @brief      スプライトパスオブジェクトの作成 (2D 空間に作成されます)
	//	@param[out]	spritePath		: 作成されたスプライトパスハンドルを格納する変数
	//	@param[in]	maxSpriteCount	: このパスが一度の画面更新で描画できるスプライトの数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSpritePath_Create(LNHandle* spritePath, int maxSpriteCount);

	//----------------------------------------------------------------------
	///**
	//  @brief      以降のスプライトの描画で使用するブレンドモードの設定
	//	@param[in]	spritePath	: スプライトパスハンドル
	//	@param[in]	blendMode	: 合成方法
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSpritePath_SetPathBlendMode(LNHandle spritePath, LNBlendMode blendMode);

	//----------------------------------------------------------------------
	///**
	//  @brief      以降のスプライトの描画で使用する座標変換行列の設定
	//	@param[in]	spritePath	: スプライトパスハンドル
	//	@param[in]	matrix		: 座標変換行列
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSpritePath_SetPathTransform(LNHandle spritePath, const LNMatrix* matrix);

	//----------------------------------------------------------------------
	///**
	//  @brief      スプライトの描画
	//	@param[in]	spritePath		: スプライトパスハンドル
	//	@param[in]	position		: 描画先座標
	//	@param[in]	center			: 位置や回転の中心となるオフセット
	//	@param[in]	size			: 描画先スプライトのサイズ
	//	@param[in]	texture			: 描画するテクスチャハンドル
	//	@param[in]	srcRect			: テクスチャからの転送矩形
	//	@param[in]	vertexColors	: 四角形の各頂点の乗算色 (要素数 4 の配列。NULL の場合は考慮しない)
	//	@param[in]	frontAxis		: スプライトの正面方法
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	center は 2D/3D で基準位置が異なります。
	//				2D空間へ描画するときはスプライトの左上、
	//				3D空間へ描画するときはスプライトの中央となります。<br>
	//				front は、2Dの時は通常 LN_FRONTAXIS_RZ を指定します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSpritePath_DrawSprite(LNHandle spritePath, const LNVector3* position, const LNVector3* center, const LNVector2* size, LNHandle texture, const LNRect* srcRect, const LNColorF* vertexColors, LNFrontAxis frontAxis LNOTE_DEFAULT_ARG(LN_FRONTAXIS_RZ));

/** @} */	// group_spritepath

//==============================================================================
// LNModel
//------------------------------------------------------------------------------
///**
//  @defgroup   group_mesh LNModel
//  @brief      モデルノード
//  @extends	LNVisualNode
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNModel);

	//----------------------------------------------------------------------
	///**
	//  @brief		モデルオブジェクトの作成
	//	@param[out]	model			: 作成されたモデルハンドルを格納する変数
	//  @param[in]	filePath		: メッシュファイル名
	//  @param[in]	meshCreateFlags	: 
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNModel_Create(LNHandle* model, const LNChar* filePath, LNMeshCreateFlags meshCreateFlags);
	
/** @} */	// defgroup

//==============================================================================
// LNEffect
//------------------------------------------------------------------------------
///**
//  @defgroup   group_effect LNEffect
//  @brief      エフェクトオブジェクト
//	@details	エフェクトオブジェクトは VisualNode のサブクラスです。
//  @extends	LNVisualNode
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNEffect);

	//----------------------------------------------------------------------
	///**
	//  @brief      エフェクトオブジェクトの作成 (3D空間へ配置する)
	//  @param[out]	effect		: 作成されたエフェクトハンドルを格納する変数
	//  @param[in]	filePath	: エフェクトファイル名 (Effekseer 形式)
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNEffect_Create(LNHandle* effect, const LNChar* filePath);

	//----------------------------------------------------------------------
	///**
	//  @brief      エフェクトの再生を開始する
	//  @param[in]	effect		: エフェクトハンドル
	//  @param[in]	overlap		: LN_TRUE = 再生中のエフェクトを停止しない / LN_FALSE = 再生中のエフェクトを停止する	
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNEffect_Play(LNHandle effect, LNBool overlap);

	//----------------------------------------------------------------------
	///**
	//  @brief      エフェクトを停止する
	//  @param[in]	effect		: エフェクトハンドル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNEffect_Stop(LNHandle effect);

	//----------------------------------------------------------------------
	///**
	//  @brief      エフェクトが再生中かを確認する
	//  @param[in]	effect		: エフェクトハンドル
	//  @param[out]	playing		: 再生中かを示す値を格納する変数 (LN_TRUE=再生中 / LN_FALSE=停止中)
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNEffect_IsPlaying(LNHandle effect, LNBool* playing);
	
/** @} */	// defgroup

//==============================================================================
// LNTilemap
//------------------------------------------------------------------------------
///**
//  @defgroup   group_tilemap LNTilemap
//  @brief      大量のスプライトを格子状に表示するためのクラスです。主に 2D ゲームのマップ表示に使用します。
//  @extends	LNVisualNode
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNTilemap);

	//----------------------------------------------------------------------
	///**
	//  @brief		タイルマップの作成
	//	@param[out]	tilemap		: 作成されたタイルマップハンドルを格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTilemap_Create(LNHandle* tilemap);

	//----------------------------------------------------------------------
	///**
	//  @brief		タイルマップのマップデータを設定する
	//	@param[out]	tilemap		: タイルマップハンドル
	//	@param[in]	intTable	: マップデータとする整数テーブルハンドル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTilemap_SetMapData(LNHandle tilemap, LNHandle intTable);

	//----------------------------------------------------------------------
	///**
	//  @brief		タイルマップのタイルセットとして使用するテクスチャを設定する
	//	@param[out]	tilemap		: タイルマップハンドル
	//	@param[in]	texture		: 設定するテクスチャハンドル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTilemap_SetTilesetTexture(LNHandle tilemap, LNHandle texture);

	//----------------------------------------------------------------------
	///**
	//  @brief		タイルセットテクスチャの横方向のタイル数を設定する
	//	@param[out]	tilemap		: タイルマップハンドル
	//	@param[in]	count		: タイル数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTilemap_SetTilesetHorizontalTileCount(LNHandle tilemap, int count);

	//----------------------------------------------------------------------
	///**
	//  @brief		タイルマップのタイルひとつ分のピクセルサイズを設定する
	//	@param[out]	tilemap		: タイルマップハンドル
	//	@param[in]	size		: ピクセル数 (正方形。縦横同一サイズ)
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTilemap_SetTilePixelSize(LNHandle tilemap, int size);

/** @} */	// defgroup

//==============================================================================
// LNShader
//------------------------------------------------------------------------------
///**
//  @defgroup   group_shader LNShader
//  @brief      シェーダ
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNShader);

	//----------------------------------------------------------------------
	///**
	//  @brief		シェーダの作成
	//  @param[out]	shader		: 作成されたシェーダハンドルを格納する変数
	//  @param[in]	filePath	: シェーダファイル名
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNShader_CreateFromFile(LNHandle* shader, const LNChar* filePath);

	//----------------------------------------------------------------------
	///**
	//  @brief		最後に発生したコンパイルエラーメッセージを取得する
	//  @param[out]	errors		: メッセージ文字列のポインタを格納する変数のアドレス
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNShader_GetLastErrors(const LNChar** errors);


/** @} */	// defgroup

//==============================================================================
// LNViewPane
//------------------------------------------------------------------------------
///**
//  @defgroup   group_view_pane LNViewPane
//  @brief      ビューペイン
//	@details	ビューペインは、シーングラフの最終的な描画結果が書き込まれる
//				レンダリングターゲットを表すオブジェクトです。
//				また、ウィンドウへの描画領域の定義を行います。
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNViewPane);

	//----------------------------------------------------------------------
	///**
	//  @brief      デフォルトで生成されるビューペインの取得
	//	@param[out]	viewPane	: ビューペインハンドルを格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNViewPane_GetDefaultViewPane(LNHandle* viewPane);

	//----------------------------------------------------------------------
	///**
	//  @brief      ViewPane の背景色を設定する
	//	@param[in]	viewPane	: ビューペインハンドル
	//	@param[in]	color		: 背景色に設定する色
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNViewPane_SetBackgroundColor(LNHandle viewPane, const LNColorF* color);

	//----------------------------------------------------------------------
	///**
	//  @brief      ViewPane に設定されている Layer のリストを取得する
	//	@param[in]	viewPane	: ビューペインハンドル
	//	@param[out]	objectList	: オブジェクトリストハンドル(Layer)を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNViewPane_GetLayers(LNHandle viewPane, LNHandle* objectList);
	/*DeclOpt
	ParamTypes["objectList"] = "ObjectList<Layer>";
	DeclOpt*/

/** @} */	// defgroup

//==============================================================================
// LNLayer
//------------------------------------------------------------------------------
///**
//  @defgroup   group_layer LNLayer
//  @brief      レイヤー
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNLayer);

	//----------------------------------------------------------------------
	///**
	//  @brief      【この機能は現在使用できません】
	//	@param[in]	layer		: レイヤーハンドル
	//	@param[in]	priority	: 優先度 (大きいほど手前(後から)に描画されます)
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNLayer_SetPriority(LNHandle layer, int priority);

	//----------------------------------------------------------------------
	///**
	//  @brief      【この機能は現在使用できません】
	//	@param[in]	layer		: レイヤーハンドル
	//	@param[out]	outPriority	: 優先度を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNLayer_GetPriority(LNHandle layer, int* outPriority);
	
/** @} */	// defgroup

//==============================================================================
// LNViewFilter
//------------------------------------------------------------------------------
///**
//  @defgroup   group_view_filter LNViewFilter
//  @brief      ビューフィルタ
//	@details	ビューフィルタはビューペインに描き込まれるイメージに
//				特殊効果を適用するための機能です。
//				いわゆるポストエフェクト処理を行います。
//  @extends	LNLayer
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNViewFilter);

	//----------------------------------------------------------------------
	///**
	//  @brief      フレーム更新
	//	@param[in]	viewFilter	: ビューフィルタハンドル
	//	@param[in]	elapsedTime	: 経過させる時間
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	エフェクトやアニメーションの状態を elapsedTime 分進めます。
	//				通常、1フレームに1度呼び出します。<br>
	//				自動更新が有効になっている場合はライブラリの
	//				フレーム更新を行った時に自動的に呼ばれるため、
	//				この関数を直接呼び出す必要はありません。<br>
	//				自動更新はデフォルトで有効です。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNViewFilter_Update(LNHandle viewFilter, float elapsedTime);

	//----------------------------------------------------------------------
	///**
	//  @brief      自動更新有無の設定
	//	@param[in]	viewFilter	: ビューフィルタハンドル
	//	@param[in]	enabled		: LN_TRUE=有効 / LN_FALSE=無効
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	デフォルト値は LN_TRUE です。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNViewFilter_SetEnableAutoUpdate(LNHandle viewFilter, LNBool enabled);

	//----------------------------------------------------------------------
	///**
	//  @brief      自動更新有無の取得
	//	@param[in]	viewFilter	: ビューフィルタハンドル
	//	@param[out]	outEnabled	: 有無を示す値を格納する変数 (LN_TRUE=有効 / LN_FALSE=無効)
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNViewFilter_IsEnableAutoUpdate(LNHandle viewFilter, LNBool* outEnabled);

/** @} */	// defgroup

//==============================================================================
// LNToneFilter
//------------------------------------------------------------------------------
///**
//  @defgroup   group_tone_filter LNToneFilter
//  @brief      色調フィルタ
//  @extends	LNViewFilter
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNToneFilter);

	//----------------------------------------------------------------------
	///**
	//  @brief      色調を変更するビューフィルタを作成する
	//	@param[out]	toneFilter : 作成された色調ビューフィルタハンドルを格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNToneFilter_Create(LNHandle* toneFilter);

	//----------------------------------------------------------------------
	///**
	//  @brief      色調を設定する
	//	@param[in]	toneFilter		: 色調ビューフィルタハンドル
	//	@param[in]	tone			: 設定する色調
	//	@param[in]	duration		: 変化にかける時間 (デフォルトではフレーム単位。タイムスケール依存)
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNToneFilter_SetTone(LNHandle toneFilter, const LNTone* tone, float duration);

/** @} */	// defgroup

//==============================================================================
// LNTransitionFilter
//------------------------------------------------------------------------------
///**
//  @defgroup   group_transition_filter LNTransitionFilter
//  @brief      トランジションフィルタ
//  @extends	LNViewFilter
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNTransitionFilter);

	//----------------------------------------------------------------------
	///**
	//  @brief      トランジションを行うビューフィルタを作成する
	//	@param[out]	transitionFilter : 作成されたトランジションビューフィルタハンドルを格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTransitionFilter_Create(LNHandle* transitionFilter);

	//----------------------------------------------------------------------
	///**
	//  @brief      トランジションの準備として、現在の画面を固定する
	//	@param[in]	transitionFilter	: トランジションビューフィルタハンドル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	これ以降、LNTransitionFilter_Transition によるトランジション実行まで
	//				フィルタされた画面は更新されなくなります。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTransitionFilter_Freeze(LNHandle transitionFilter);

	//----------------------------------------------------------------------
	///**
	//  @brief      固定された画面から現在の画面へのトランジションを実行する
	//	@param[in]	transitionFilter	: トランジションビューフィルタハンドル
	//	@param[in]	duration			: 変化にかける時間 (デフォルトではフレーム単位。タイムスケール依存)
	//	@param[in]	maskTexture			: トランジションに使用するマスク画像
	//	@param[in]	vague				: マスク画像の境界のあいまいさ
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	maskTexture が NULL の場合は通常のフェードになります。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTransitionFilter_Transition(LNHandle transitionFilter, double duration, LNHandle maskTexture, float vague);

/** @} */	// defgroup

//==============================================================================
// LNBlurFilter
//------------------------------------------------------------------------------
///**
//  @defgroup   group_blur_filter LNBlurFilter
//  @brief      ブラーフィルタ
//  @extends	LNViewFilter
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNBlurFilter);

	//----------------------------------------------------------------------
	///**
	//  @brief      ブラ―(残像)を適用するビューフィルタを作成する
	//	@param[out]	blurFilter : 作成されたブラ―ビューフィルタハンドルを格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNBlurFilter_Create(LNHandle* blurFilter);

	//----------------------------------------------------------------------
	///**
	//  @brief      色調を設定する
	//	@param[in]	blurFilter	: ブラ―ビューフィルタハンドル
	//	@param[in]	duration	: 適用時間 (デフォルトではフレーム単位。タイムスケール依存)
	//	@param[in]	power		: ブラーの強度 (ブラーイメージを描画する時のアルファ値。0.0～1.0 で指定)
	//	@param[in]	scale		: ブラーの大きさ (長さ)
	//	@param[in]	center		: scale を使用して放射ブラーを作るときの中心座標
	//	@param[in]	baseCamera	: center の座標空間の基準となるカメラのハンドル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	duration が 0 より大きい場合、その時間かけて徐々に強度を 0.0 に戻します。
	//				duration が 0 の場合は強度を自動で変化させません。<br>
	//				scale は元のスクリーンイメージに対して、1フレーム当たりどの程度拡大するかを指定します。<br>
	//				1.0 でスクリーンと等倍 (変化なし)、2.0 でスクリーンの2倍の大きさとなります。
	//				通常は 1.0～1.1 の範囲で指定するのが無難です。
	//				1.0 より小さい値を指定すると内側向きのブラーになります。<br>
	//				center は baseCamera が 2Dカメラであれば 2D 空間上の点、3Dカメラであれば3D空間上の点を指定します。
	//				baseCamera が NULL の場合、center は -1.0 ～ 1.0 で指定します。(0.0 が画面中央です)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNBlurFilter_Blur(LNHandle blurFilter, double duration, float power, float scale, const LNVector3* center, LNHandle baseCamera);

/** @} */	// defgroup

//==============================================================================
// LNCamera
//------------------------------------------------------------------------------
///**
//  @defgroup   group_camera LNCamera
//  @brief      3D または 2D シーンのカメラ
//  @extends	LNSceneNode
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNCamera);
	
	//LNOTEINTERNALAPI void LNCamera_SetTypeUserData(void* data);
	//----------------------------------------------------------------------
	///**
	//  @brief      デフォルトで作成される3Dカメラを取得する
	//	@param[out]	camera	: カメラハンドルを格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCamera_GetDefault3DCamera(LNHandle* camera);

	//----------------------------------------------------------------------
	///**
	//  @brief      デフォルトで作成される3Dカメラを取得する
	//	@param[out]	camera	: カメラハンドルを格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCamera_GetDefault2DCamera(LNHandle* camera);

	//----------------------------------------------------------------------
	///**
	//  @brief      カメラ姿勢の設定方法
	//	@param[in]	camera		: カメラハンドル
	//	@param[in]	type		: 姿勢の設定方法
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCamera_SetCameraType(LNHandle camera, LNCameraType type);

	//----------------------------------------------------------------------
	///**
	//  @brief      カメラの位置を設定する
	//	@param[in]	camera		: カメラハンドル
	//	@param[in]	position	: 座標
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@override	LNSceneNode_SetPosition
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCamera_SetPosition(LNHandle camera, const LNVector3* position);

	//----------------------------------------------------------------------
	///**
	//  @brief      カメラの位置を設定する
	//	@param[in]	camera		: カメラハンドル
	//	@param[in]	x			: X 座標
	//	@param[in]	y			: Y 座標
	//	@param[in]	z			: Z 座標
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@override	LNSceneNode_SetPositionXYZ
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCamera_SetPositionXYZ(LNHandle camera, float x, float y, float z);

	//----------------------------------------------------------------------
	///**
	//  @brief      カメラの注視点を設定する
	//	@param[in]	camera		: カメラハンドル
	//	@param[in]	at			: 注視点座標
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCamera_SetLookAt(LNHandle camera, const LNVector3* at);

	//----------------------------------------------------------------------
	///**
	//  @brief      カメラの注視点を設定する
	//	@param[in]	camera		: カメラハンドル
	//	@param[in]	x			: 注視点 X 座標
	//	@param[in]	y			: 注視点 Y 座標
	//	@param[in]	z			: 注視点 Z 座標
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCamera_SetLookAtXYZ(LNHandle camera, float x, float y, float z);

	//----------------------------------------------------------------------
	///**
	//  @brief      カメラの上方向を設定する
	//	@param[in]	camera		: カメラハンドル
	//	@param[in]	up			: 上方向ベクトル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCamera_SetUpDirection(LNHandle camera, const LNVector3* up);

	//----------------------------------------------------------------------
	///**
	//  @brief      カメラの上方向を設定する
	//	@param[in]	camera		: カメラハンドル
	//	@param[in]	x			: 上方向ベクトルの X 成分
	//	@param[in]	y			: 上方向ベクトルの Y 成分
	//	@param[in]	z			: 上方向ベクトルの Z 成分
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCamera_SetUpDirectionXYZ(LNHandle camera, float x, float y, float z);

	//----------------------------------------------------------------------
	///**
	//  @brief      カメラの回転角度を設定する
	//	@param[in]	camera		: カメラハンドル
	//	@param[in]	angle		: 各軸の回転角度 (ラジアン単位)
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@override	LNSceneNode_SetAngle
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCamera_SetAngle(LNHandle camera, const LNVector3* angle);

	//----------------------------------------------------------------------
	///**
	//  @brief      カメラの回転角度を設定する
	//	@param[in]	camera		: カメラハンドル
	//	@param[in]	x			: X 軸の回転角度 (ラジアン単位)
	//	@param[in]	y			: Y 軸の回転角度 (ラジアン単位)
	//	@param[in]	z			: Z 軸の回転角度 (ラジアン単位)
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@override	LNSceneNode_SetAngleXYZ
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCamera_SetAngleXYZ(LNHandle camera, float x, float y, float z);

	//----------------------------------------------------------------------
	///**
	//  @brief      カメラの姿勢行列を設定する
	//	@param[in]	camera		: カメラハンドル
	//	@param[in]	matrix		: 変換行列
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@override	LNSceneNode_SetMatrix
	//	@details	行列を使用し、位置(0,0,0)、正面(0,0,1)、上方向(0,1,0) を
	//				変換して姿勢を算出します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCamera_SetMatrix(LNHandle camera, const LNMatrix* matrix);

	//----------------------------------------------------------------------
	///**
	//  @brief      カメラの視野角を設定する
	//	@param[in]	camera		: カメラハンドル
	//	@param[in]	fov			: 視野角
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCamera_SetFovY(LNHandle camera, float fov);

	//----------------------------------------------------------------------
	///**
	//  @brief      カメラの最手前クリップ距離と最奥クリップ距離を設定する
	//	@param[in]	camera		: カメラハンドル
	//	@param[in]	nearClip	: 手前クリップ距離
	//	@param[in]	farClip		: 奥クリップ距離
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCamera_SetNearFarClip(LNHandle camera, float nearClip, float farClip);

	//----------------------------------------------------------------------
	///**
	//  @brief      カメラのビュー行列を取得する
	//	@param[in]	camera		: カメラハンドル
	//	@param[out]	outMatrix	: ビュー行列を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCamera_GetViewMatrix(LNHandle camera, LNMatrix* outMatrix);

	//----------------------------------------------------------------------
	///**
	//  @brief      カメラのプロジェクション行列を取得する
	//	@param[in]	camera		: カメラハンドル
	//	@param[out]	outMatrix	: プロジェクション行列を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCamera_GetProjectionMatrix(LNHandle camera, LNMatrix* outMatrix);

	//----------------------------------------------------------------------
	///**
	//  @brief      ビュー行列とプロジェクション行列の積を取得する
	//	@param[in]	camera		: カメラハンドル
	//	@param[out]	outMatrix	: ビュー行列とプロジェクション行列の積を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCamera_GetViewProjectionMatrix(LNHandle camera, LNMatrix* outMatrix);

/** @} */	// defgroup

} // extern "C"

/** @} */	// defgroup
