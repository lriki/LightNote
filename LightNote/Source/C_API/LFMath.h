﻿
#pragma once

#include "LFCommon.h"
#include "LFTypedef.h"

extern "C" {

/**
	@brief	
*/
LN_MODULE(Math)

//==============================================================================
/**
	@brief	2次元のベクトルのクラスです。
*/
LN_STRUCT_CLASS(LNVector2)

	/**
		@brief		2D ベクトルに値を設定する
		@param[in]	vec		: 値を格納する 2D ベクトル
		@param[in]	x		: X 値
		@param[in]	y		: Y 値
	*/
	LN_INSTANCE_API
	LNResult LNVector2_Set(LNVector2* vec, float x, float y);

	/**
		@brief		2D ベクトルの長さを計算する
		@param[in]	vec			: 処理の基になる 2D ベクトル
		@param[out]	outLength	: ベクトルの長さを格納する変数
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNVector2_GetLength(const LNVector2* vec, float* outLength);

	/**
		@brief		2D ベクトルの長さの2乗を計算する
		@param[in]	vec			: 処理の基になる 2D ベクトル
		@param[out]	outLength	: ベクトルの長さの2乗を格納する変数
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNVector2_GetSquareLength(const LNVector2* vec, float* outLength);

	/**
		@brief		2D ベクトルを正規化する
		@param[in]	vec		: 処理の基になる 2D ベクトル
		@param[out] outVec	: 演算結果を格納する 2D ベクトル
	*/
	LN_STATIC_API
	LNResult LNVector2_Normalize(const LNVector2* vec, LNVector2* outVec);

	LN_INTERNAL_API LNResult LNVector2_GetLengthD(const LNVector2* vec, double* length);
	LN_INTERNAL_API LNResult LNVector2_GetSquareLengthD(const LNVector2* vec, double* length);
	
LN_CLASS_END

//==============================================================================
/**
	@brief	3 次元ベクトルのクラスです。
*/
LN_STRUCT_CLASS(LNVector3)

	/**
		@brief		3Dベクトルに値を設定する
		@param[out]	vec		: 値を格納するベクトル
		@param[in]	x		: X 値
		@param[in]	y		: Y 値
		@param[in]	z		: Z 値
	*/
	LN_INSTANCE_API
	LN_STRUCT_CONSTRUCTOR
	LNResult LNVector3_Set(LNVector3* vec, float x, float y, float z);

	/**
		@brief		3Dベクトルに値を設定する
		@param[out]	vec		: 値を格納するベクトル
		@param[in]	vec2	: 2D ベクトル
		@param[in]	z		: Z 値
	*/
	LN_INSTANCE_API
	LNResult LNVector3_SetVec2(LNVector3* vec, const LNVector2* vec2, float z);

	/**
		@brief		3Dベクトルの長さを計算する
		@param[in]	vec			: 処理の基になるベクトル
		@param[out]	outLength	: ベクトルの長さを格納する変数
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNVector3_GetLength(const LNVector3* vec, float* outLength);

	/**
		@brief		3D ベクトルの長さの2乗を計算する
		@param[in]	vec			: 処理の基になる Vector3
		@param[out]	outLength	: ベクトルの長さの2乗を格納する変数
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNVector3_GetSquareLength(const LNVector3* vec, float* outLength);

	/**
		@brief		3D ベクトルを正規化する
		@param[in]	vec		: 処理の基になる 3D ベクトル
		@param[out] outVec	: 演算結果を格納する 3D ベクトル
	*/
	LN_STATIC_API
	LNResult LNVector3_Normalize(const LNVector3* vec, LNVector3* outVec);

	/**
		@brief		2 つの 3D ベクトルの内積を計算する
		@param[in]	vec1	: 処理の基になる 3D ベクトル
		@param[in]	vec2	: 処理の基になる 3D ベクトル
		@param[out]	dot		: ベクトルの内積を格納する変数
	*/
	LN_STATIC_API
	LNResult LNVector3_Dot(const LNVector3* vec1, const LNVector3* vec2, float* dot);

	/**
		@brief		2 つの 3D ベクトルの外積を計算する
		@param[in]	vec1	: 処理の基になる 3D ベクトル
		@param[in]	vec2	: 処理の基になる 3D ベクトル
		@param[out] outVec	: 演算結果を格納する 3D ベクトル
	*/
	LN_STATIC_API
	LNResult LNVector3_Cross(const LNVector3* vec1, const LNVector3* vec2, LNVector3* outVec);

	/**
		@brief		指定された法線で反射させたベクトルを計算する
		@param[in]	vec		: 処理の基になる 3D ベクトル
		@param[in]	normal	: 面方向を表す法線 3D ベクトル
		@param[out] outVec	: 演算結果を格納する 3D ベクトル
	*/
	LN_STATIC_API
	LNResult LNVector3_Reflect(const LNVector3* vec, const LNVector3* normal, LNVector3* outVec);

	/**
		@brief		指定された法線と方向から滑りベクトルを計算する
		@param[in]	vec		: 処理の基になる 3D ベクトル
		@param[in]	normal	: 面方向を表す法線 3D ベクトル
		@param[out] outVec	: 演算結果を格納する 3D ベクトル
	*/
	LN_STATIC_API
	LNResult LNVector3_Slide(const LNVector3* vec, const LNVector3* normal, LNVector3* outVec);

	/**
		@brief		2 つの 3D ベクトル間を線形補間する
		@param[in]	vec1	: 処理の基になる 3D ベクトル ( t = 0.0 のときの値 )
		@param[in]	vec2	: 処理の基になる 3D ベクトル ( t = 1.0 のときの値 )
		@param[in]	t		: 補間値 ( 0.0 ～ 1.0 )
		@param[out] outVec	: 演算結果を格納する 3D ベクトル
	*/
	LN_STATIC_API
	LNResult LNVector3_Lerp(const LNVector3* vec1, const LNVector3* vec2, float t, LNVector3* outVec);

	/**
		@brief		指定された 3D ベクトルを Catmull-Rom 補間する
		@param[in]	vec1	: 処理の基になる 3D ベクトル
		@param[in]	vec2	: 処理の基になる 3D ベクトル ( t = 0.0 のときの値 )
		@param[in]	vec3	: 処理の基になる 3D ベクトル ( t = 1.0 のときの値 )
		@param[in]	vec4	: 処理の基になる 3D ベクトル
		@param[in]	t		: 補間値 ( 0.0 ～ 1.0 )
		@param[out] outVec	: 演算結果を格納する 3D ベクトル
	*/
	LN_STATIC_API
	LNResult LNVector3_CatmullRom(const LNVector3* vec1, const LNVector3* vec2, const LNVector3* vec3, const LNVector3* vec4, float t, LNVector3* outVec);

	/**
		@brief		指定された行列を使用して座標変換する 
		@param[in]	vec		: 処理の基になる 3D ベクトル
		@param[in]	mat		: 処理の基になる 行列
		@param[out] outVec	: 演算結果を格納する 3D ベクトル
		@details	ベクトルを ( x, y, z, 1 ) として座標変換します。
		            結果の w は出力されません。<br>
		            結果を w = 1 に射影する ( x y z を w で除算する ) 場合は
		            Vector3TransformCoord() を使用してください。
	*/
	LN_STATIC_API
	LNResult LNVector3_Transform(const LNVector3* vec, const LNMatrix* mat, LNVector4* outVec);

	/**
		@brief		指定された行列を使用して座標変換し、結果を w = 1 に射影する
		@param[in]	vec		: 処理の基になる 3D ベクトル
		@param[in]	mat		: 処理の基になる 行列
		@param[out] outVec	: 演算結果を格納する 3D ベクトル
		@details	ベクトルを ( x, y, z, 1 ) として座標変換し、
		            結果を w = 1 に射影 ( x y z を w で除算する ) します。
	*/
	LN_STATIC_API
	LNResult LNVector3_TransformCoord(const LNVector3* vec, const LNMatrix* mat, LNVector3* outVec);

	LN_INTERNAL_API LNResult LNVector3_GetLengthD(const LNVector3* vec, double* length);
	LN_INTERNAL_API LNResult LNVector3_GetSquareLengthD(const LNVector3* vec, double* length);
	LN_INTERNAL_API LNResult LNVector3_DotD(const LNVector3* vec1, const LNVector3* vec2, double* dot);
	LN_INTERNAL_API LNResult LNVector3_LerpD(const LNVector3* vec1, const LNVector3* vec2, double t, LNVector3* outVec);
	LN_INTERNAL_API LNResult LNVector3_CatmullRomD(const LNVector3* vec1, const LNVector3* vec2, const LNVector3* vec3, const LNVector3* vec4, double t, LNVector3* outVec);
	
LN_CLASS_END

//==============================================================================
/**
	@brief	4 次元ベクトルのクラスです。
*/
LN_STRUCT_CLASS(LNVector4)

	/**
		@brief		3Dベクトルに値を設定する
		@param[out]	vec		: 値を格納するベクトル
		@param[in]	x		: X 値
		@param[in]	y		: Y 値
		@param[in]	z		: Z 値
		@param[in]	w		: W 値
	*/
	LN_INSTANCE_API
	LNResult LNVector4_Set(LNVector4* vec, float x, float y, float z, float w);

LN_CLASS_END

//==============================================================================
/**
	@brief	4x4 行列のクラスです。
*/
LN_STRUCT_CLASS(LNMatrix)

	/**
		@brief		単位行列を作成する
		@param[out]	mat		: 結果を格納する行列
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNMatrix_Identity(LNMatrix* mat);

	/**
		@brief			行列を平行移動する
		@param[in,out]	mat		: 演算結果を格納する行列
		@param[in]		x		: X 方向の移動量
		@param[in]		y		: Y 方向の移動量
		@param[in]		z		: Z 方向の移動量
		@details		与えられた引数から平行移動行列を作り、現在の行列に乗算します。
	*/
	LN_INSTANCE_API
	LNResult LNMatrix_Translate(LNMatrix* mat, float x, float y, float z);

	/**
		@brief			行列を平行移動する
		@param[in,out]	mat		: 演算結果を格納する行列
		@param[in]		vec		: 移動量
		@details		与えられた引数から平行移動行列を作り、現在の行列に乗算します。
	*/
	LN_INSTANCE_API
	LNResult LNMatrix_TranslateVec3(LNMatrix* mat, const LNVector3* vec);

	/**
		@brief			X 軸を回転軸にして行列を回転する ( ラジアン単位 )
		@param[in,out]	mat		: 演算結果を格納する行列
		@param[in]		radian	: 回転角度 ( 軸方向に対して反時計回り )
		@details		与えられた引数から回転行列を作り、現在の行列に乗算します。
	*/
	LN_INSTANCE_API
	LNResult LNMatrix_RotateX(LNMatrix* mat, float radian);

	/**
		@brief			Y 軸を回転軸にして行列を回転する ( ラジアン単位 )
		@param[in,out]	mat		: 演算結果を格納する行列
		@param[in]		radian	: 回転角度
		@details		与えられた引数から回転行列を作り、現在の行列に乗算します。
	*/
	LN_INSTANCE_API
	LNResult LNMatrix_RotateY(LNMatrix* mat, float radian);

	/**
		@brief			Z 軸を回転軸にして行列を回転する ( ラジアン単位 )
		@param[in,out]	mat		: 演算結果を格納する行列
		@param[in]		radian	: 回転角度
		@details		与えられた引数から回転行列を作り、現在の行列に乗算します。
	*/
	LN_INSTANCE_API
	LNResult LNMatrix_RotateZ(LNMatrix* mat, float radian);

	/**
		@brief			行列を回転する ( ラジアン単位 )
		@param[in,out]	mat			: 演算結果を格納する行列
		@param[in]		xRad		: X 軸を回転軸とした角度
		@param[in]		yRad		: Y 軸を回転軸とした角度
		@param[in]		zRad		: Z 軸を回転軸とした角度
		@param[in]		rotOrder	: 回転順序の指定
		@details		与えられた引数から回転行列を作り、現在の行列に乗算します。
	*/
	LN_INSTANCE_API
	LNResult LNMatrix_Rotate(LNMatrix* mat, float xRad, float yRad, float zRad, LNRotationOrder rotOrder LN_DEFAULT_ARG(LN_ROTATIONORDER_XYZ));

	/**
		@brief			行列を回転する ( ラジアン単位 )
		@param[in,out]	mat			: 演算結果を格納する行列
		@param[in]		vec			: 各軸の回転角度
		@param[in]		rotOrder	: 回転順序の指定
		@details		与えられた引数から回転行列を作り、現在の行列に乗算します。
	*/
	LN_INSTANCE_API
	LNResult LNMatrix_RotateVec3(LNMatrix* mat, const LNVector3* vec, LNRotationOrder rotOrder LN_DEFAULT_ARG(LN_ROTATIONORDER_XYZ));

	/**
		@brief			任意の軸を回転軸にして行列を回転する ( ラジアン単位 )
		@param[in,out]	mat			: 演算結果を格納する行列
		@param[in]		axis		: 回転軸を示す Vector3
		@param[in]		radian		: 回転角度
		@details		与えられた引数から回転行列を作り、現在の行列に乗算します。<br>
						与えられた軸が正規化されていない場合は正規化したものを使って計算します。
	*/
	LN_INSTANCE_API
	LNResult LNMatrix_RotateAxis(LNMatrix* mat, const LNVector3* axis, float radian);

	/**
		@brief			クォータニオンを使って行列を回転する
		@param[in,out]	mat		: 演算結果を格納する行列
		@param[in]		qua		: 処理の基になる Quaternion
		@details		クォータニオンから回転行列を作り、現在の行列に乗算します。
	*/
	LN_INSTANCE_API
	LNResult LNMatrix_RotateQuaternion(LNMatrix* mat, const LNQuaternion* qua);

	/**
		@brief			行列をスケーリングする
		@param[in,out]	mat		: 演算結果を格納する行列
		@param[in]		xyz		: X Y Z 全ての方向に適用する拡大率 (X Y Z を同じ値で拡大)
		@details		与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
	*/
	LN_INSTANCE_API
	LNResult LNMatrix_Scale(LNMatrix* mat, float xyz);

	/**
		@brief			行列をスケーリングする
		@param[in,out]	mat		: 演算結果を格納する行列
		@param[in]		x		: X 方向の拡大率
		@param[in]		y		: Y 方向の拡大率
		@param[in]		z		: Z 方向の拡大率
		@details		与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
	*/
	LN_INSTANCE_API
	LNResult LNMatrix_ScaleXYZ(LNMatrix* mat, float x, float y, float z);

	/**
		@brief			行列をスケーリングする
		@param[in,out]	mat		: 演算結果を格納する行列
		@param[in]		scale	: 各方向の拡大率
		@details		与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
	*/
	LN_INSTANCE_API
	LNResult LNMatrix_ScaleVec3(LNMatrix* mat, const LNVector3* scale);

	/**
		@brief		行列を乗算する
		@param[in]	mat1	: 処理の基になる行列
		@param[in]	mat2	: 処理の基になる行列
		@param[out]	matOut	: 結果を格納する行列
	*/
	LN_STATIC_API
	LNResult LNMatrix_Multiply(const LNMatrix* mat1, const LNMatrix* mat2, LNMatrix* matOut);

	/**
		@brief			逆行列を求める
		@param[in]		mat		: 処理の基になる行列	
		@param[out]		matOut	: 演算結果を格納する行列
	*/
	LN_STATIC_API
	LNResult LNMatrix_Inverse(const LNMatrix* mat, LNMatrix* matOut);

	/**
		@brief			転置行列を求める
		@param[in]		mat		: 処理の基になる行列
		@param[out]		matOut	: 演算結果を格納する行列
	*/
	LN_STATIC_API
	LNResult LNMatrix_Transpose(const LNMatrix* mat, LNMatrix* matOut);

	/**
		@brief			左手座標系ビュー行列を作成する
		@param[in]		pos		: 視点の位置を示す Vector3
		@param[in]		lookAt	: 注視点を示す Vector3
		@param[in]		upDir	: 上方向を示す Vector3
		@param[out]		matOut	: 演算結果を格納する行列
	*/
	LN_STATIC_API
	LNResult LNMatrix_ViewTransformLH(const LNVector3* pos, const LNVector3* lookAt, const LNVector3* upDir, LNMatrix* matOut);

	/**
		@brief			右手座標系ビュー行列を作成する
		@param[in]		pos		: 視点の位置を示す Vector3
		@param[in]		lookAt	: 注視点を示す Vector3
		@param[in]		upDir	: 上方向を示す Vector3
		@param[out]		matOut	: 演算結果を格納する行列
	*/
	LN_STATIC_API
	LNResult LNMatrix_ViewTransformRH(const LNVector3* pos, const LNVector3* lookAt, const LNVector3* upDir, LNMatrix* matOut);

	/**
		@brief			左手座標系射影行列の作成
		@param[in]		fovY	: Y 方向への視野角 (ラジアン単位)
		@param[in]		aspect	: アスペクト比
		@param[in]		nearZ	: 近くのビュー平面の Z 値
		@param[in]		farZ	: 遠くのビュー平面の Z 値
		@param[out]		matOut	: 演算結果を格納する行列
	*/
	LN_STATIC_API
	LNResult LNMatrix_PerspectiveFovLH(float fovY, float aspect, float nearZ, float farZ, LNMatrix* matOut);

	/**
		@brief			右手座標系射影行列の作成
		@param[in]		fovY	: Y 方向への視野角 (ラジアン単位)
		@param[in]		aspect	: アスペクト比
		@param[in]		nearZ	: 近くのビュー平面の Z 値
		@param[in]		farZ	: 遠くのビュー平面の Z 値
		@param[out]		matOut	: 演算結果を格納する行列
	*/
	LN_STATIC_API
	LNResult LNMatrix_PerspectiveFovRH(float fovY, float aspect, float nearZ, float farZ, LNMatrix* matOut);

	/**
		@brief			左手座標系正射影行列の作成
		@param[in]		width	: ビューの幅
		@param[in]		height	: ビューの高さ
		@param[in]		nearZ	: 近くのビュー平面の Z 値
		@param[in]		farZ	: 遠くのビュー平面の Z 値
		@param[out]		matOut	: 演算結果を格納する行列
	*/
	LN_STATIC_API
	LNResult LNMatrix_OrthoLH(float width, float height, float nearZ, float farZ, LNMatrix* matOut);

	/**
		@brief			右手座標系正射影行列の作成
		@param[in]		width	: ビューの幅
		@param[in]		height	: ビューの高さ
		@param[in]		nearZ	: 近くのビュー平面の Z 値
		@param[in]		farZ	: 遠くのビュー平面の Z 値
		@param[out]		matOut	: 演算結果を格納する行列
	*/
	LN_STATIC_API
	LNResult LNMatrix_OrthoRH(float width, float height, float nearZ, float farZ, LNMatrix* matOut);

	/**
		@brief		右方向を示す 3D ベクトルの取得
		@param[in]	mat		: 処理の基になる行列
		@param[out] outVec	: 結果を格納する Vector3 変数
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNMatrix_GetRight(const LNMatrix* mat, LNVector3* outVec);

	/**
		@brief		上方向を示す 3D ベクトルの取得
		@param[in]	mat		: 処理の基になる行列
		@param[out] outVec	: 結果を格納する Vector3 変数
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNMatrix_GetUp(const LNMatrix* mat, LNVector3* outVec);

	/**
		@brief		正面方向を示す 3D ベクトルの取得
		@param[in]	mat		: 処理の基になる行列
		@param[out] outVec	: 結果を格納する Vector3 変数
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNMatrix_GetFront(const LNMatrix* mat, LNVector3* outVec);

	/**
		@brief		位置を示す 3D ベクトルの取得
		@param[in]	mat		: 処理の基になる行列
		@param[out] outVec	: 結果を格納する Vector3 変数
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNMatrix_GetPosition(const LNMatrix* mat, LNVector3* outVec);

	/**
		@brief			回転行列からオイラー角を計算する
		@param[in]		mat		: 処理の基になる行列
		@param[out]		outVec	: 各軸の回転角度(ラジアン)を格納する Vector3 変数
	*/
	LN_INSTANCE_API
	LNResult LNMatrix_GetEulerAngles(const LNMatrix* mat, LNVector3* outVec);

	/**
		@brief			行列をスケーリング、回転、移動成分に分解する
		@param[in]		mat		: 処理の基になる行列
		@param[out]		rot		: 各軸の回転情報を格納する Quaternion 変数
		@param[out]		scale	: 各軸のスケーリング量を格納する Vector3 変数
		@param[out]		trans	: 各軸の平行移動量を格納する Vector3 変数
	*/
	LN_INSTANCE_API
	LNResult LNMatrix_Decompose(const LNMatrix* mat, LNVector3* scale, LNQuaternion* rot, LNVector3* trans);

	LN_INTERNAL_API LNResult LNMatrix_TranslateD(LNMatrix* mat, double x, double y, double z);
	LN_INTERNAL_API LNResult LNMatrix_RotateXD(LNMatrix* mat, double radian);
	LN_INTERNAL_API LNResult LNMatrix_RotateYD(LNMatrix* mat, double radian);
	LN_INTERNAL_API LNResult LNMatrix_RotateZD(LNMatrix* mat, double radian);
	LN_INTERNAL_API LNResult LNMatrix_RotateD(LNMatrix* mat, double xRad, double yRad, double zRad, LNRotationOrder rotOrder);
	LN_INTERNAL_API LNResult LNMatrix_RotateAxisD(LNMatrix* mat, const LNVector3* axis, double radian);
	LN_INTERNAL_API LNResult LNMatrix_ScaleD(LNMatrix* mat, double xyz);
	LN_INTERNAL_API LNResult LNMatrix_ScaleXYZD(LNMatrix* mat, double x, double y, double z);
	LN_INTERNAL_API LNResult LNMatrix_PerspectiveFovLHD(LNMatrix* mat, double fovY, double aspect, double nearZ, double farZ);
	LN_INTERNAL_API LNResult LNMatrix_PerspectiveFovRHD(LNMatrix* mat, double fovY, double aspect, double nearZ, double farZ);
	LN_INTERNAL_API LNResult LNMatrix_OrthoLHD(LNMatrix* mat, double width, double height, double nearZ, double farZ);
	LN_INTERNAL_API LNResult LNMatrix_OrthoRHD(LNMatrix* mat, double width, double height, double nearZ, double farZ);
	
LN_CLASS_END

//==============================================================================
/**
	@brief	クォータニオンのクラスです。
*/
LN_STRUCT_CLASS(LNQuaternion)

	/**
		@brief		クォータニオンに値を設定する
		@param[out]	qua		: 値を格納する Quaternion 変数
		@param[in]	x		: X 値
		@param[in]	y		: Y 値
		@param[in]	z		: Z 値
		@param[in]	w		: W 値
	*/
	LN_INSTANCE_API
	LNResult LNQuaternion_Set(LNQuaternion* qua, float x, float y, float z, float w);

	/**
		@brief		単位クォータニオンを作成する
		@param[out]	qua		: 結果を格納する Quaternion 変数
	*/
	LN_STATIC_API
	LN_PROPERTY
	LNResult LNQuaternion_Identity(LNQuaternion* qua);

	/**
		@brief			任意の軸を回転軸にして回転するクォータニオンを作成する
		@param[in]		axis	: 回転軸を示す Vector3 変数
		@param[in]		r		: 回転角度 (ラジアン)
		@param[out]		outQua	: 演算結果を格納する Quaternion 変数
		@details		与えられた軸が正規化されていない場合は正規化された軸を使用します。
	*/
	LN_STATIC_API
	LNResult LNQuaternion_RotationAxis(const LNVector3* axis, float r, LNQuaternion* outQua);

	/**
		@brief			回転行列からクォータニオンを作成する
		@param[in]		mat		: 処理の基になる行列
		@param[out]		outQua	: 演算結果を格納する Quaternion 変数
	*/
	LN_STATIC_API
	LNResult LNQuaternion_RotationMatrix(const LNMatrix* mat, LNQuaternion* outQua);

	/**
		@brief			ヨー、ピッチ、およびロールを指定してクォータニオンを作成する
		@param[in]		yaw		: y 軸を中心とするヨー (ラジアン単位)
		@param[in]		pitch	: x 軸を中心とするピッチ (ラジアン単位)
		@param[in]		roll	: z 軸を中心とするロール (ラジアン単位)
		@param[out]		outQua	: 演算結果を格納する Quaternion 変数
	*/
	LN_STATIC_API
	LNResult LNQuaternion_RotationYawPitchRoll(float yaw, float pitch, float roll, LNQuaternion* outQua);

	/**
		@brief			クォータニオンを正規化する
		@param[in]		qua		: 処理の基になるクォータニオン
		@param[out]		outQua	: 演算結果を格納する Quaternion 変数
	*/
	LN_STATIC_API
	LNResult LNQuaternion_Normalize(const LNQuaternion* qua, LNQuaternion* outQua);

	/**
		@brief			クォータニオンの共役を計算する
		@param[in]		qua		: 処理の基になるクォータニオン
		@param[out]		outQua	: 演算結果を格納する Quaternion 変数
	*/
	LN_STATIC_API
	LNResult LNQuaternion_Conjugate(const LNQuaternion* qua, LNQuaternion* outQua);

	/**
		@brief			2 つのクォータニオンの積を計算する
		@param[in]		qua1	: 処理の基になるクォータニオン
		@param[in]		qua2	: 処理の基になるクォータニオン
		@param[out]		outQua	: 演算結果を格納する Quaternion 変数
	*/
	LN_STATIC_API
	LNResult LNQuaternion_Multiply(const LNQuaternion* qua1, const LNQuaternion* qua2, LNQuaternion* outQua);

	/**
		@brief			2 つのクォータニオンを球面線形補間する
		@param[in]		qua1	: 処理の基になるクォータニオン
		@param[in]		qua2	: 処理の基になるクォータニオン
		@param[in]		t		: 補間値
		@param[out]		outQua	: 演算結果を格納する Quaternion 変数
	*/
	LN_STATIC_API
	LNResult LNQuaternion_Slerp(const LNQuaternion* qua1, const LNQuaternion* qua2, float t, LNQuaternion* outQua);

	LN_INTERNAL_API LNResult LNQuaternion_RotationAxisD(const LNVector3* axis, float r, LNQuaternion* outQua);
	LN_INTERNAL_API LNResult LNQuaternion_RotationYawPitchRollD(float yaw, float pitch, float roll, LNQuaternion* outQua);
	LN_INTERNAL_API LNResult LNQuaternion_SlerpD(const LNQuaternion* qua1, const LNQuaternion* qua2, float t, LNQuaternion* outQua);
	
LN_CLASS_END

//==============================================================================
/**
	@brief		乱数のクラスです。
	@details	長周期乱数を高速に生成します。
*/
LN_CLASS(LNRandom)

	LNOTE_TYPE_INFO_DECL(LNRandom);

	/**
		@brief		現在の時間を乱数シードとして、乱数オブジェクトを作成する
		@param[out]	random	: 作成された乱数オブジェクトハンドルを格納する変数
	*/
	LN_INSTANCE_API
	LNResult LNRandom_Create(LN_HANDLE(LNRandom)* random);

	/**
		@brief		乱数シードを指定し、乱数オブジェクトを作成する
		@param[out]	random	: 作成された乱数オブジェクトハンドルを格納する変数
		@param[in]	seed	: シード値
	*/
	LN_INSTANCE_API
	LNResult LNRandom_CreateSeed(LN_HANDLE(LNRandom)* random, int seed);

	/**
		@brief		乱数オブジェクトに乱数シードを設定する
		@param[in]	random	: 乱数オブジェクトハンドル
		@param[in]	seed	: シード値
		@details	random が NULL の場合、グローバル乱数オブジェクトに設定します。
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNRandom_SetSeed(LN_HANDLE(LNRandom) random, int seed);

	/**
		@brief		整数値乱数の取得
		@param[in]	random		: 乱数オブジェクトハンドル
		@param[out]	outValue	: 整数値を格納する変数
		@details	random が NULL の場合、グローバル乱数オブジェクトから取得します。
	*/
	LN_INSTANCE_API
	LNResult LNRandom_GetInt(LN_HANDLE(LNRandom) random, int* outValue);

	/**
		@brief		範囲を指定して、整数値乱数を取得する ( minValue <= x < maxValue )
		@param[in]	random		: 乱数オブジェクトハンドル
		@param[in]	minValue	: 生成される乱数の下限値
		@param[in]	maxValue	: 生成される乱数の上限値
		@param[out]	outValue	: 整数値を格納する変数
		@details	random が NULL の場合、グローバル乱数オブジェクトから取得します。
	*/
	LN_INSTANCE_API
	LNResult LNRandom_GetIntRange(LN_HANDLE(LNRandom) random, int minValue, int maxValue, int* outValue);

	/**
		@brief		範囲を指定して、整数値乱数を取得する (center ± width/2)
		@param[in]	random		: 乱数オブジェクトハンドル
		@param[in]	center		: 生成される乱数の中央値
		@param[in]	width		: 生成される乱数の振れ幅
		@param[out]	outValue	: 整数値を格納する変数
		@details	random が NULL の場合、グローバル乱数オブジェクトから取得します。
	*/
	LN_INSTANCE_API
	LNResult LNRandom_GetIntWidth(LN_HANDLE(LNRandom) random, int center, int width, int* outValue);

	/**
		@brief			実数値乱数の取得
		@param[in]		random		: 乱数オブジェクトハンドル
		@param[out]		outValue	: 実数値を格納する変数
	*/
	LN_INSTANCE_API
	LNResult LNRandom_GetFloat(LN_HANDLE(LNRandom) random, float* outValue);

	/**
		@brief			実数値乱数の取得 ( minValue <= x < maxValue )
		@param[in]		random		: 乱数オブジェクトハンドル
		@param[in]		minValue	: 生成される乱数の下限値
		@param[in]		maxValue	: 生成される乱数の上限値
		@param[out]		outValue	: 実数値を格納する変数
	*/
	LN_INSTANCE_API
	LNResult LNRandom_GetFloatRange(LN_HANDLE(LNRandom) random, float minValue, float maxValue, float* outValue);

	/**
		@brief			実数値乱数の取得 (center ± width/2)
		@param[in]		random		: 乱数オブジェクトハンドル
		@param[in]		center		: 生成される乱数の中央値
		@param[in]		width		: 生成される乱数の振れ幅
		@param[out]		outValue	: 実数値を格納する変数
	*/
	LN_INSTANCE_API
	LNResult LNRandom_GetFloatWidth(LN_HANDLE(LNRandom) random, float center, float width, float* outValue);

LN_CLASS_END

LN_MODULE_END

} // extern "C"
