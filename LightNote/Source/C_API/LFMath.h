//==============================================================================
// LFMath 
//------------------------------------------------------------------------------
///**
//  @file       LFMath.h
//  @brief      LFMath
//  @defgroup   group_lib_math Math
//  @{
//*/
//==============================================================================

#pragma once

#include "LFCommon.h"
#include "LFTypedef.h"

extern "C" {
	
//==============================================================================
// LNVector2
//------------------------------------------------------------------------------
///**
//  @defgroup   group_vector2 LNVector2
//  @brief      2D ベクトル
//  @{
//*/
//==============================================================================

	//----------------------------------------------------------------------
	///**
	//  @brief      2D ベクトルに値を設定する
	//  @param[in]  vec		: 値を格納する 2D ベクトル
	//  @param[in]  x		: X 値
	//  @param[in]  y		: Y 値
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector2_Set( LNVector2* vec, float x, float y );
	/*DeclOpt
	HSP_Enabled = false;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief      2D ベクトルの長さを計算する
	//  @param[in]	vec			: 処理の基になる 2D ベクトル
	//	@param[out]	outLength	: ベクトルの長さを格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector2_GetLength(const LNVector2* vec, float* outLength);

	//----------------------------------------------------------------------
	///**
	//  @brief      2D ベクトルの長さの2乗を計算する
	//  @param[in]  vec			: 処理の基になる 2D ベクトル
	//	@param[out]	outLength	: ベクトルの長さの2乗を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector2_GetSquareLength(const LNVector2* vec, float* outLength);

	//----------------------------------------------------------------------
	///**
	//  @brief      2D ベクトルを正規化する
	//  @param[in]  vec		: 処理の基になる 2D ベクトル
	//  @param[out] outVec	: 演算結果を格納する 2D ベクトル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector2_Normalize(const LNVector2* vec, LNVector2* outVec);

/** @} */	// defgroup

//==============================================================================
// LNVector3
//------------------------------------------------------------------------------
///**
//  @defgroup   group_vector3 LNVector3
//  @brief      3Dベクトル
//  @{
//*/
//==============================================================================

	//----------------------------------------------------------------------
	///**
	//  @brief      3Dベクトルに値を設定する
	//  @param[out]	vec		: 値を格納するベクトル
	//  @param[in]  x		: X 値
	//  @param[in]  y		: Y 値
	//  @param[in]  z		: Z 値
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector3_Set(LNVector3* vec, float x, float y, float z);
	/*DeclOpt
	HSP_Enabled = false;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief      3Dベクトルに値を設定する
	//  @param[out]	vec		: 値を格納するベクトル
	//  @param[in]  vec2	: 2D ベクトル
	//  @param[in]  z		: Z 値
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector3_SetVec2(LNVector3* vec, const LNVector2* vec2, float z);

	//----------------------------------------------------------------------
	///**
	//  @brief      3Dベクトルの長さを計算する
	//  @param[in]	vec			: 処理の基になるベクトル
	//	@param[out]	outLength	: ベクトルの長さを格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector3_GetLength(const LNVector3* vec, float* outLength);

	//----------------------------------------------------------------------
	///**
	//  @brief      3D ベクトルの長さの2乗を計算する
	//  @param[in]  vec			: 処理の基になる Vector3
	//	@param[out]	outLength	: ベクトルの長さの2乗を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector3_GetSquareLength(const LNVector3* vec, float* outLength);

	//----------------------------------------------------------------------
	///**
	//  @brief      3D ベクトルを正規化する
	//  @param[in]  vec		: 処理の基になる 3D ベクトル
	//  @param[out] outVec	: 演算結果を格納する 3D ベクトル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector3_Normalize(const LNVector3* vec, LNVector3* outVec);

	//----------------------------------------------------------------------
	///**
	//  @brief      2 つの 3D ベクトルの内積を計算する
	//  @param[in]  vec1	: 処理の基になる 3D ベクトル
	//  @param[in]  vec2	: 処理の基になる 3D ベクトル
	//	@param[out]	dot		: ベクトルの内積を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector3_Dot(const LNVector3* vec1, const LNVector3* vec2, float* dot);
	/*DeclOpt
	ForceStaticMethod = true;	// 第1引数が owner 構造体型で、in タイプはインスタンスメソッド化されてしまう。第1引数を float* にして戻り値 void ならいいけど、それは使いやすくない
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief      2 つの 3D ベクトルの外積を計算する
	//  @param[in]  vec1	: 処理の基になる 3D ベクトル
	//  @param[in]  vec2	: 処理の基になる 3D ベクトル
	//  @param[out] outVec	: 演算結果を格納する 3D ベクトル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector3_Cross(const LNVector3* vec1, const LNVector3* vec2, LNVector3* outVec);

	//----------------------------------------------------------------------
	///**
	//  @brief      指定された法線で反射させたベクトルを計算する
	//  @param[in]  vec		: 処理の基になる 3D ベクトル
	//  @param[in]  normal	: 面方向を表す法線 3D ベクトル
	//  @param[out] outVec	: 演算結果を格納する 3D ベクトル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector3_Reflect(const LNVector3* vec, const LNVector3* normal, LNVector3* outVec);

	//----------------------------------------------------------------------
	///**
	//  @brief      指定された法線と方向から滑りベクトルを計算する
	//  @param[in]  vec		: 処理の基になる 3D ベクトル
	//  @param[in]  normal	: 面方向を表す法線 3D ベクトル
	//  @param[out] outVec	: 演算結果を格納する 3D ベクトル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector3_Slide(const LNVector3* vec, const LNVector3* normal, LNVector3* outVec);

	//----------------------------------------------------------------------
	///**
	//  @brief      2 つの 3D ベクトル間を線形補間する
	//  @param[in]  vec1	: 処理の基になる 3D ベクトル ( t = 0.0 のときの値 )
	//  @param[in]  vec2	: 処理の基になる 3D ベクトル ( t = 1.0 のときの値 )
	//  @param[in]  t		: 補間値 ( 0.0 ～ 1.0 )
	//  @param[out] outVec	: 演算結果を格納する 3D ベクトル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector3_Lerp(const LNVector3* vec1, const LNVector3* vec2, float t, LNVector3* outVec);

	//----------------------------------------------------------------------
	///**
	//  @brief      指定された 3D ベクトルを Catmull-Rom 補間する
	//  @param[in]  vec1	: 処理の基になる 3D ベクトル
	//  @param[in]  vec2	: 処理の基になる 3D ベクトル ( t = 0.0 のときの値 )
	//  @param[in]  vec3	: 処理の基になる 3D ベクトル ( t = 1.0 のときの値 )
	//  @param[in]  vec4	: 処理の基になる 3D ベクトル
	//  @param[in]  t		: 補間値 ( 0.0 ～ 1.0 )
	//  @param[out] outVec	: 演算結果を格納する 3D ベクトル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector3_CatmullRom(const LNVector3* vec1, const LNVector3* vec2, const LNVector3* vec3, const LNVector3* vec4, float t, LNVector3* outVec);

	//----------------------------------------------------------------------
	///**
	//  @brief      指定された行列を使用して座標変換する 
	//  @param[in]  vec		: 処理の基になる 3D ベクトル
	//  @param[in]  mat		: 処理の基になる 行列
	//  @param[out] outVec	: 演算結果を格納する 3D ベクトル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//  @details	ベクトルを ( x, y, z, 1 ) として座標変換します。
	//              結果の w は出力されません。<br>
	//              結果を w = 1 に射影する ( x y z を w で除算する ) 場合は
	//              Vector3TransformCoord() を使用してください。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector3_Transform(const LNVector3* vec, const LNMatrix* mat, LNVector3* outVec);

	//----------------------------------------------------------------------
	///**
	//  @brief      指定された行列を使用して座標変換し、結果を w = 1 に射影する
	//  @param[in]  vec		: 処理の基になる 3D ベクトル
	//  @param[in]  mat		: 処理の基になる 行列
	//  @param[out] outVec	: 演算結果を格納する 3D ベクトル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//  @details	ベクトルを ( x, y, z, 1 ) として座標変換し、
	//              結果を w = 1 に射影 ( x y z を w で除算する ) します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector3_TransformCoord(const LNVector3* vec, const LNMatrix* mat, LNVector3* outVec);

	//----------------------------------------------------------------------
	///**
	//  @brief      任意軸周りの回転を行う
	//  @param[in]  vec		: 処理の基になる Vector3
	//  @param[in]  axis	: 回転軸を表す Vector3
	//  @param[in]  r		: 回転角度 (ラジアン)
	//  @param[out] outVec	: 演算結果を格納する Vector3
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector3_RotateAxis(const LNVector3* vec, const LNVector3* axis, float r, LNVector3* outVec);

	//----------------------------------------------------------------------
	///**
	//  @brief      行列の回転成分だけを使って座標変換する
	//  @param[in]  vec		: 処理の基になる Vector3
	//  @param[in]  mat		: 処理の基になる Matrix
	//  @param[out] outVec	: 演算結果を格納する Vector3
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector3_RotateMatrix(const LNVector3* vec, const LNMatrix* mat, LNVector3* outVec);

/** @} */	// defgroup

//==============================================================================
// LNVector4
//------------------------------------------------------------------------------
///**
//  @defgroup   group_vector4 LNVector4
//  @brief      4Dベクトル
//  @{
//*/
//==============================================================================

/** @} */	// defgroup

//==============================================================================
// LNMatrix
//------------------------------------------------------------------------------
///**
//  @defgroup   group_matrix LNMatrix
//  @brief      4x4行列
//  @{
//*/
//==============================================================================

	//----------------------------------------------------------------------
	///**
	//  @brief      単位行列を作成する
	//  @param[out]	mat		: 結果を格納する行列
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_Identity(LNMatrix* mat);
	/*DeclOpt
	HSP_Override = true;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief			行列を平行移動する
	//  @param[in,out]	mat		: 演算結果を格納する行列
	//  @param[in]		x		: X 方向の移動量
	//  @param[in]		y		: Y 方向の移動量
	//  @param[in]		z		: Z 方向の移動量
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//  @details		与えられた引数から平行移動行列を作り、現在の行列に乗算します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_Translate(LNMatrix* mat, float x, float y, float z);
    
	//----------------------------------------------------------------------
	///**
	//  @brief			行列を平行移動する
	//  @param[in,out]	mat		: 演算結果を格納する行列
	//  @param[in]		vec		: 移動量
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//  @details		与えられた引数から平行移動行列を作り、現在の行列に乗算します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_TranslateVec3(LNMatrix* mat, const LNVector3* vec);

	//----------------------------------------------------------------------
	///**
	//  @brief			X 軸を回転軸にして行列を回転する ( ラジアン単位 )
	//  @param[in,out]	mat		: 演算結果を格納する行列
	//  @param[in]		radian	: 回転角度 ( 軸方向に対して反時計回り )
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//  @details		与えられた引数から回転行列を作り、現在の行列に乗算します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_RotateX(LNMatrix* mat, float radian);

	//----------------------------------------------------------------------
	///**
	//  @brief			Y 軸を回転軸にして行列を回転する ( ラジアン単位 )
	//  @param[in,out]	mat		: 演算結果を格納する行列
	//  @param[in]		radian	: 回転角度
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//  @details		与えられた引数から回転行列を作り、現在の行列に乗算します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_RotateY(LNMatrix* mat, float radian);

	//----------------------------------------------------------------------
	///**
	//  @brief			Z 軸を回転軸にして行列を回転する ( ラジアン単位 )
	//  @param[in,out]	mat		: 演算結果を格納する行列
	//  @param[in]		radian	: 回転角度
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//  @details		与えられた引数から回転行列を作り、現在の行列に乗算します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_RotateZ(LNMatrix* mat, float radian);

	//----------------------------------------------------------------------
	///**
	//  @brief			行列を回転する ( ラジアン単位 )
	//  @param[in,out]	mat			: 演算結果を格納する行列
	//  @param[in]		xRad		: X 軸を回転軸とした角度
	//  @param[in]		yRad		: Y 軸を回転軸とした角度
	//  @param[in]		zRad		: Z 軸を回転軸とした角度
	//  @param[in]		rotOrder	: 回転順序の指定
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//  @details		与えられた引数から回転行列を作り、現在の行列に乗算します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_Rotate(LNMatrix* mat, float xRad, float yRad, float zRad, LNRotationOrder rotOrder LNOTE_DEFAULT_ARG(LN_ROTATIONORDER_XYZ));

	//----------------------------------------------------------------------
	///**
	//  @brief			行列を回転する ( ラジアン単位 )
	//  @param[in,out]	mat			: 演算結果を格納する行列
	//  @param[in]		vec			: 各軸の回転角度
	//  @param[in]		rotOrder	: 回転順序の指定
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//  @details		与えられた引数から回転行列を作り、現在の行列に乗算します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_RotateVec3(LNMatrix* mat, const LNVector3* vec, LNRotationOrder rotOrder LNOTE_DEFAULT_ARG(LN_ROTATIONORDER_XYZ));

	//----------------------------------------------------------------------
	///**
	//  @brief			任意の軸を回転軸にして行列を回転する ( ラジアン単位 )
	//  @param[in,out]	mat			: 演算結果を格納する行列
	//  @param[in]		axis		: 回転軸を示す Vector3
	//  @param[in]		radian		: 回転角度
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//  @details		与えられた引数から回転行列を作り、現在の行列に乗算します。<br>
	//					与えられた軸が正規化されていない場合は正規化したものを使って計算します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_RotateAxis(LNMatrix* mat, const LNVector3* axis, float radian);

	//----------------------------------------------------------------------
	///**
	//  @brief			クォータニオンを使って行列を回転する
	//  @param[in,out]	mat		: 演算結果を格納する行列
	//  @param[in]		qua		: 処理の基になる Quaternion
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//  @details		クォータニオンから回転行列を作り、現在の行列に乗算します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_RotateQuaternion(LNMatrix* mat, const LNQuaternion* qua);
	
	//----------------------------------------------------------------------
	///**
	//  @brief			行列をスケーリングする
	//  @param[in,out]	mat		: 演算結果を格納する行列
	//  @param[in]		xyz		: X Y Z 全ての方向に適用する拡大率 (X Y Z を同じ値で拡大)
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//  @details		与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_Scale(LNMatrix* mat, float xyz);

	//----------------------------------------------------------------------
	///**
	//  @brief			行列をスケーリングする
	//  @param[in,out]	mat		: 演算結果を格納する行列
	//  @param[in]		x		: X 方向の拡大率
	//  @param[in]		y		: Y 方向の拡大率
	//  @param[in]		z		: Z 方向の拡大率
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//  @details		与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_ScaleXYZ(LNMatrix* mat, float x, float y, float z);

	//----------------------------------------------------------------------
	///**
	//  @brief			行列をスケーリングする
	//  @param[in,out]	mat		: 演算結果を格納する行列
	//  @param[in]		scale	: 各方向の拡大率
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//  @details		与えられた引数からスケーリング行列を作り、現在の行列に乗算します。<br>
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_ScaleVec3(LNMatrix* mat, const LNVector3* scale);

	//----------------------------------------------------------------------
	///**
	//  @brief      行列を乗算する
	//  @param[in]  mat1	: 処理の基になる行列
	//  @param[in]  mat2	: 処理の基になる行列
	//  @param[out]	matOut	: 結果を格納する行列
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_Multiply(const LNMatrix* mat1, const LNMatrix* mat2, LNMatrix* matOut);

	//----------------------------------------------------------------------
	///**
	//  @brief			逆行列を求める
	//  @param[in]		mat		: 処理の基になる行列	
	//  @param[out]		matOut	: 演算結果を格納する行列
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)		
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_Inverse(const LNMatrix* mat, LNMatrix* matOut);

	//----------------------------------------------------------------------
	///**
	//  @brief			転置行列を求める
	//  @param[in]		mat		: 処理の基になる行列
	//  @param[out]		matOut	: 演算結果を格納する行列
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)	
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_Transpose(const LNMatrix* mat, LNMatrix* matOut);
 
	//----------------------------------------------------------------------
	///**
	//  @brief			左手座標系ビュー行列を作成する
	//  @param[in]		pos		: 視点の位置を示す Vector3
	//  @param[in]		lookAt	: 注視点を示す Vector3
	//  @param[in]		upDir	: 上方向を示す Vector3
	//  @param[out]		matOut	: 演算結果を格納する行列
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_ViewTransformLH(const LNVector3* pos, const LNVector3* lookAt, const LNVector3* upDir, LNMatrix* matOut);

	//----------------------------------------------------------------------
	///**
	//  @brief			右手座標系ビュー行列を作成する
	//  @param[in]		pos		: 視点の位置を示す Vector3
	//  @param[in]		lookAt	: 注視点を示す Vector3
	//  @param[in]		upDir	: 上方向を示す Vector3
	//  @param[out]		matOut	: 演算結果を格納する行列
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_ViewTransformRH(const LNVector3* pos, const LNVector3* lookAt, const LNVector3* upDir, LNMatrix* matOut);

	//----------------------------------------------------------------------
	///**
	//  @brief			左手座標系射影行列の作成
	//  @param[in]		fovY	: Y 方向への視野角 (ラジアン単位)
	//  @param[in]		aspect	: アスペクト比
	//  @param[in]		nearZ	: 近くのビュー平面の Z 値
	//  @param[in]		farZ	: 遠くのビュー平面の Z 値
	//  @param[out]		matOut	: 演算結果を格納する行列
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_PerspectiveFovLH(float fovY, float aspect, float nearZ, float farZ, LNMatrix* matOut);

	//----------------------------------------------------------------------
	///**
	//  @brief			右手座標系射影行列の作成
	//  @param[in]		fovY	: Y 方向への視野角 (ラジアン単位)
	//  @param[in]		aspect	: アスペクト比
	//  @param[in]		nearZ	: 近くのビュー平面の Z 値
	//  @param[in]		farZ	: 遠くのビュー平面の Z 値
	//  @param[out]		matOut	: 演算結果を格納する行列
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_PerspectiveFovRH(float fovY, float aspect, float nearZ, float farZ, LNMatrix* matOut);
	
	//----------------------------------------------------------------------
	///**
	//  @brief			左手座標系正射影行列の作成
	//  @param[in]		width	: ビューの幅
	//  @param[in]		height	: ビューの高さ
	//  @param[in]		nearZ	: 近くのビュー平面の Z 値
	//  @param[in]		farZ	: 遠くのビュー平面の Z 値
	//  @param[out]		matOut	: 演算結果を格納する行列
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_OrthoLH(float width, float height, float nearZ, float farZ, LNMatrix* matOut);
	
	//----------------------------------------------------------------------
	///**
	//  @brief			右手座標系正射影行列の作成
	//  @param[in]		width	: ビューの幅
	//  @param[in]		height	: ビューの高さ
	//  @param[in]		nearZ	: 近くのビュー平面の Z 値
	//  @param[in]		farZ	: 遠くのビュー平面の Z 値
	//  @param[out]		matOut	: 演算結果を格納する行列
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_OrthoRH(float width, float height, float nearZ, float farZ, LNMatrix* matOut);

	//----------------------------------------------------------------------
	///**
	//  @brief      右方向を示す 3D ベクトルの取得
	//  @param[in]  mat		: 処理の基になる行列
	//  @param[out] outVec	: 結果を格納する Vector3 変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_GetRight(const LNMatrix* mat, LNVector3* outVec);

	//----------------------------------------------------------------------
	///**
	//  @brief      上方向を示す 3D ベクトルの取得
	//  @param[in]  mat		: 処理の基になる行列
	//  @param[out] outVec	: 結果を格納する Vector3 変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_GetUp(const LNMatrix* mat, LNVector3* outVec);

	//----------------------------------------------------------------------
	///**
	//  @brief      正面方向を示す 3D ベクトルの取得
	//  @param[in]  mat		: 処理の基になる行列
	//  @param[out] outVec	: 結果を格納する Vector3 変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_GetFront(const LNMatrix* mat, LNVector3* outVec);

	//----------------------------------------------------------------------
	///**
	//  @brief      位置を示す 3D ベクトルの取得
	//  @param[in]  mat		: 処理の基になる行列
	//  @param[out] outVec	: 結果を格納する Vector3 変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_GetPosition(const LNMatrix* mat, LNVector3* outVec);

	//----------------------------------------------------------------------
	///**
	//  @brief			回転行列からオイラー角を計算する
	//  @param[in]		mat		: 処理の基になる行列
	//  @param[out]		outVec	: 各軸の回転角度(ラジアン)を格納する Vector3 変数
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_ToEuler(const LNMatrix* mat, LNVector3* outVec);

	//----------------------------------------------------------------------
	///**
	//  @brief			行列をスケーリング、回転、移動成分に分解する
	//  @param[in]		mat		: 処理の基になる行列
	//  @param[out]		scale	: 各軸のスケーリング量を格納する Vector3 変数
	//  @param[out]		rot		: 各軸の回転角度(ラジアン)を格納する Vector3 変数
	//  @param[out]		trans	: 各軸の平行移動量を格納する Vector3 変数
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_Decompose(const LNMatrix* mat, LNVector3* scale, LNMatrix* rot, LNVector3* trans);

	//----------------------------------------------------------------------
	///**
	//  @brief			右手系⇔左手系の変換
	//  @param[in]		mat		: 処理の元になる行列
	//  @param[out]		matOut	: 演算結果を格納する行列
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_TransformBasis(const LNMatrix* mat, LNMatrix* matOut);

/** @} */	// defgroup

//==============================================================================
// LNQuaternion
//------------------------------------------------------------------------------
///**
//  @defgroup   group_quaternion LNQuaternion
//  @brief      クォータニオン
//  @{
//*/
//==============================================================================

	//----------------------------------------------------------------------
	///**
	//  @brief      クォータニオンに値を設定する
	//  @param[out]	qua		: 値を格納する Quaternion 変数
	//  @param[in]  x		: X 値
	//  @param[in]  y		: Y 値
	//  @param[in]  z		: Z 値
	//  @param[in]  w		: W 値
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNQuaternion_Set(LNQuaternion* qua, float x, float y, float z, float w);
	/*DeclOpt
	HSP_Enabled = false;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief      単位クォータニオンを作成する
	//  @param[out]	qua		: 結果を格納する Quaternion 変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNQuaternion_Identity(LNQuaternion* qua);
	/*DeclOpt
	HSP_Override = true;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief			任意の軸を回転軸にして回転するクォータニオンを作成する
	//  @param[in]		axis	: 回転軸を示す Vector3 変数
	//  @param[in]		r		: 回転角度 (ラジアン)
	//  @param[out]		outQua	: 演算結果を格納する Quaternion 変数
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//  @details		与えられた軸が正規化されていない場合は正規化された軸を使用します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNQuaternion_RotationAxis(const LNVector3* axis, float r, LNQuaternion* outQua);

	//----------------------------------------------------------------------
	///**
	//  @brief			回転行列からクォータニオンを作成する
	//  @param[in]		mat		: 処理の基になる行列
	//  @param[out]		outQua	: 演算結果を格納する Quaternion 変数
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNQuaternion_RotationMatrix(const LNMatrix* mat, LNQuaternion* outQua);

	//----------------------------------------------------------------------
	///**
	//  @brief			ヨー、ピッチ、およびロールを指定してクォータニオンを作成する
	//  @param[in]		yaw		: y 軸を中心とするヨー (ラジアン単位)
	//  @param[in]		pitch	: x 軸を中心とするピッチ (ラジアン単位)
	//  @param[in]		roll	: z 軸を中心とするロール (ラジアン単位)
	//  @param[out]		outQua	: 演算結果を格納する Quaternion 変数
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNQuaternion_RotationYawPitchRoll(float yaw, float pitch, float roll, LNQuaternion* outQua);

	//----------------------------------------------------------------------
	///**
	//  @brief			クォータニオンを正規化する
	//  @param[in]		qua		: 処理の基になるクォータニオン
	//  @param[out]		outQua	: 演算結果を格納する Quaternion 変数
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNQuaternion_Normalize(const LNQuaternion* qua, LNQuaternion* outQua);

	//----------------------------------------------------------------------
	///**
	//  @brief			クォータニオンの共役を計算する
	//  @param[in]		qua		: 処理の基になるクォータニオン
	//  @param[out]		outQua	: 演算結果を格納する Quaternion 変数
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNQuaternion_Conjugate(const LNQuaternion* qua, LNQuaternion* outQua);

	//----------------------------------------------------------------------
	///**
	//  @brief			2 つのクォータニオンの積を計算する
	//  @param[in]		qua1	: 処理の基になるクォータニオン
	//  @param[in]		qua2	: 処理の基になるクォータニオン
	//  @param[out]		outQua	: 演算結果を格納する Quaternion 変数
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNQuaternion_Multiply(const LNQuaternion* qua1, const LNQuaternion* qua2, LNQuaternion* outQua);

	//----------------------------------------------------------------------
	///**
	//  @brief			2 つのクォータニオンを球面線形補間する
	//  @param[in]		qua1	: 処理の基になるクォータニオン
	//  @param[in]		qua2	: 処理の基になるクォータニオン
	//  @param[in]		t		: 補間値
	//  @param[out]		outQua	: 演算結果を格納する Quaternion 変数
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNQuaternion_Slerp(const LNQuaternion* qua1, const LNQuaternion* qua2, float t, LNQuaternion* outQua);

/** @} */	// defgroup

//==============================================================================
// LNRandom
//------------------------------------------------------------------------------
///**
//  @defgroup   group_random LNRandom
//  @brief      長周期乱数を高速で生成する乱数クラス
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNRandom);

	//----------------------------------------------------------------------
	///**
	//	@brief		現在の時間を乱数シードとして、乱数オブジェクトを作成する
	//  @param[out]	random	: 作成された乱数オブジェクトハンドルを格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNRandom_Create(LNHandle* random);

	//----------------------------------------------------------------------
	///**
	//	@brief		乱数シードを指定し、乱数オブジェクトを作成する
	//  @param[out]	random	: 作成された乱数オブジェクトハンドルを格納する変数
	//	@param[in]	seed	: シード値
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNRandom_CreateSeed(LNHandle* random, int seed);

	//----------------------------------------------------------------------
	///**
	//	@brief		乱数オブジェクトに乱数シードを設定する
	//  @param[in]	random	: 乱数オブジェクトハンドル
	//	@param[in]	seed	: シード値
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	random が NULL の場合、グローバル乱数オブジェクトに設定します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNRandom_SetSeed(LNHandle random, int seed);

	//----------------------------------------------------------------------
	///**
	//	@brief		整数値乱数の取得
	//  @param[in]	random		: 乱数オブジェクトハンドル
	//  @param[out]	outValue	: 整数値を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	random が NULL の場合、グローバル乱数オブジェクトから取得します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNRandom_GetInt(LNHandle random, int* outValue);
	/*DeclOpt
	AutoPropertyGenerate = false;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//	@brief		範囲を指定して、整数値乱数を取得する ( minValue <= x < maxValue )
	//  @param[in]	random		: 乱数オブジェクトハンドル
	//  @param[in]	minValue	: 生成される乱数の下限値
	//  @param[in]	maxValue	: 生成される乱数の上限値
	//  @param[out]	outValue	: 整数値を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	random が NULL の場合、グローバル乱数オブジェクトから取得します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNRandom_GetIntRange(LNHandle random, int minValue, int maxValue, int* outValue);

	//----------------------------------------------------------------------
	///**
	//	@brief		範囲を指定して、整数値乱数を取得する (center ± width/2)
	//  @param[in]	random		: 乱数オブジェクトハンドル
	//  @param[in]	center		: 生成される乱数の中央値
	//  @param[in]	width		: 生成される乱数の振れ幅
	//  @param[out]	outValue	: 整数値を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	random が NULL の場合、グローバル乱数オブジェクトから取得します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNRandom_GetIntDeflect(LNHandle random, int center, int width, int* outValue);

	//----------------------------------------------------------------------
	///**
	//	@brief			実数値乱数の取得
	//  @param[in]		random		: 乱数オブジェクトハンドル
	//  @param[out]		outValue	: 実数値を格納する変数
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNRandom_GetFloat(LNHandle random, float* outValue);
	/*DeclOpt
	AutoPropertyGenerate = false;
	HSP_Enabled = false;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//	@brief			実数値乱数の取得 ( minValue <= x < maxValue )
	//  @param[in]		random		: 乱数オブジェクトハンドル
	//  @param[in]		minValue	: 生成される乱数の下限値
	//  @param[in]		maxValue	: 生成される乱数の上限値
	//  @param[out]		outValue	: 実数値を格納する変数
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNRandom_GetFloatRange(LNHandle random, float minValue, float maxValue, float* outValue);
	/*DeclOpt
	HSP_Enabled = false;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//	@brief			実数値乱数の取得 (center ± width/2)
	//  @param[in]		random		: 乱数オブジェクトハンドル
	//  @param[in]		center		: 生成される乱数の中央値
	//  @param[in]		width		: 生成される乱数の振れ幅
	//  @param[out]		outValue	: 実数値を格納する変数
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNRandom_GetFloatDeflect(LNHandle random, float center, float width, float* outValue);
	/*DeclOpt
	HSP_Enabled = false;
	DeclOpt*/

/** @} */	// defgroup

	LNOTEINTERNALAPI LNResult LNVector2_GetLengthD(const LNVector2* vec, double* length);
	LNOTEINTERNALAPI LNResult LNVector2_GetSquareLengthD(const LNVector2* vec, double* length);
	LNOTEINTERNALAPI LNResult LNVector3_GetLengthD(const LNVector3* vec, double* length);
	LNOTEINTERNALAPI LNResult LNVector3_GetSquareLengthD(const LNVector3* vec, double* length);
	LNOTEINTERNALAPI LNResult LNVector3_DotD(const LNVector3* vec1, const LNVector3* vec2, double* dot);
	LNOTEINTERNALAPI LNResult LNVector3_LerpD(const LNVector3* vec1, const LNVector3* vec2, double t, LNVector3* outVec);
	LNOTEINTERNALAPI LNResult LNVector3_CatmullRomD(const LNVector3* vec1, const LNVector3* vec2, const LNVector3* vec3, const LNVector3* vec4, double t, LNVector3* outVec);
	LNOTEINTERNALAPI LNResult LNVector3_RotateAxisD(const LNVector3* vec, const LNVector3* axis, double r, LNVector3* outVec);
	LNOTEINTERNALAPI LNResult LNMatrix_TranslateD(LNMatrix* mat, double x, double y, double z);
	LNOTEINTERNALAPI LNResult LNMatrix_RotateXD(LNMatrix* mat, double radian);
	LNOTEINTERNALAPI LNResult LNMatrix_RotateYD(LNMatrix* mat, double radian);
	LNOTEINTERNALAPI LNResult LNMatrix_RotateZD(LNMatrix* mat, double radian);
	LNOTEINTERNALAPI LNResult LNMatrix_RotateD(LNMatrix* mat, double xRad, double yRad, double zRad, LNRotationOrder rotOrder);
	LNOTEINTERNALAPI LNResult LNMatrix_RotateAxisD(LNMatrix* mat, const LNVector3* axis, double radian);
	LNOTEINTERNALAPI LNResult LNMatrix_ScaleD(LNMatrix* mat, double xyz);
	LNOTEINTERNALAPI LNResult LNMatrix_ScaleXYZD(LNMatrix* mat, double x, double y, double z);
	LNOTEINTERNALAPI LNResult LNMatrix_PerspectiveFovLHD(LNMatrix* mat, double fovY, double aspect, double nearZ, double farZ);
	LNOTEINTERNALAPI LNResult LNMatrix_PerspectiveFovRHD(LNMatrix* mat, double fovY, double aspect, double nearZ, double farZ);
	LNOTEINTERNALAPI LNResult LNMatrix_OrthoLHD(LNMatrix* mat, double width, double height, double nearZ, double farZ);
	LNOTEINTERNALAPI LNResult LNMatrix_OrthoRHD(LNMatrix* mat, double width, double height, double nearZ, double farZ);
	LNOTEINTERNALAPI LNResult LNQuaternion_RotationAxisD(const LNVector3* axis, float r, LNQuaternion* outQua);
	LNOTEINTERNALAPI LNResult LNQuaternion_RotationYawPitchRollD(float yaw, float pitch, float roll, LNQuaternion* outQua);
	LNOTEINTERNALAPI LNResult LNQuaternion_SlerpD(const LNQuaternion* qua1, const LNQuaternion* qua2, float t, LNQuaternion* outQua);

} // extern "C"
	
/** @} */	// defgroup
