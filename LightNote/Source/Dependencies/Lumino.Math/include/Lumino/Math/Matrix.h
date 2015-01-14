﻿
#ifndef LUMINO_MATH_MATRIX_H
#define LUMINO_MATH_MATRIX_H

#include <stdio.h>
#include <string.h>
#include "Common.h"

namespace Lumino
{
class Vector3;
class Vector4;
class Quaternion;
class SQTTransform;
class Plane;

/**
	@brief      4x4 の行列を定義します。
	@details	行列は行優先で、これは DirectX の定義と同じです。

				このクラスは似た機能の static 関数とインスタンス関数があります。
				例えば RotationX(static 関数) と RotateX(インスタンス関数) です。
				前者は新しい行列を作成して返すのに対し、後者は現在の行列を変更します。
				例えば、以下の m1 と m2 は同じ結果になります。

				@code
				Matrix m1 = Matrix::RotationX(0.1) * Matrix::RotationY(0.2) * Matrix::RotationZ(0.3);

				Matrix m2;
				m2.RotateX(0.1);
				m2.RotateY(0.2);
				m2.RotateZ(0.3);
				@endcode

				なお、後者は行列の生成と乗算をまとめて行うように最適化されており、高速に動作します。
*/
class LUMINO_EXPORT Matrix
{
public:

	union
	{
		struct
		{
			float M11, M12, M13, M14;
			float M21, M22, M23, M24;
			float M31, M32, M33, M34;
			float M41, M42, M43, M44;
		};
		float M[4][4];
		float m[16];
	};

public:

	static const Matrix Identity;	///< 単位行列

public:
	
	/**
		@brief	単位行列を設定してインスタンスを初期化します。
	*/
	Matrix();
	
	/**
		@brief	各要素を指定してインスタンスを初期化します。
	*/
	Matrix(	float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44);
	
	/**
		@brief	各列を指定してインスタンスを初期化します。
	*/
	Matrix(const Vector4& row1, const Vector4& row2, const Vector4& row3, const Vector4& row4);

	/**
		@brief	SQTTransform から Matrix へ変換します。
	*/
	Matrix(const SQTTransform& transform);

public:

	/**
		@brief		この行列が単位行列であるかを判定します。
		@return		true の場合、単位行列である
	*/
	bool IsIdentity() const;

	/**
		@brief		この行列の右方向を示すベクトルを取得します。
	*/
	const Vector3& GetRight() const;

	/**
		@brief		この行列の上方向を示すベクトルを取得します。
	*/
	const Vector3& GetUp() const;

	/**
		@brief		この行列の正面方向を示すベクトルを取得します。
	*/
	const Vector3& GetFront() const;
	
	/**
		@brief		この行列の平行移動量を示すベクトルを取得します。
	*/
	const Vector3& GetPosition() const;

	/**
		@brief		この行列を平行移動します。
		@param[in]	x		: X 軸の移動量
		@param[in]	y		: Y 軸の移動量
		@param[in]	z		: Z 軸の移動量
	*/
	void Translate(float x, float y, float z);

	/**
		@brief		この行列を平行移動します。
		@param[in]	vec		: 移動量
	*/
	void Translate(const Vector3& vec);

	/**
		@brief		この行列を X 軸で回転します。
		@param[in]	r		: 回転角度 (ラジアン単位)
	*/
	void RotateX(float r);

	/**
		@brief		この行列を Y 軸で回転します。
		@param[in]	r		: 回転角度 (ラジアン単位)
	*/
	void RotateY(float r);

	/**
		@brief		この行列を Z 軸で回転します。
		@param[in]	r		: 回転角度 (ラジアン単位)
	*/
	void RotateZ(float r);

	/**
		@brief		この行列を任意軸で回転します。
		@param[in]  axis	: 回転軸
		@param[in]  r		: 回転角度 (ラジアン単位)
		@details	axis が単位ベクトルでない場合は正規化してから計算を行います。
	*/
	void RotateAxis(const Vector3& axis, float r);

	/**
		@brief		この行列を指定したクォータニオンで回転します。
		@param[in]  qua		: 処理の基になるクォータニオン
	*/
	void RotateQuaternion(const Quaternion& qua);

	/**
		@brief		この行列を各軸にそってスケーリングします。
		@param[in]	x		: X 軸の拡縮率
		@param[in]	y		: Y 軸の拡縮率
		@param[in]	z		: Z 軸の拡縮率
	*/
	void Scale(float x, float y, float z);

	/**
		@brief		この行列を各軸にそってスケーリングします。
		@param[in]	vec		: 各軸の拡縮率
	*/
	void Scale(const Vector3& vec);

	/**
		@brief		この行列を各軸にそってスケーリングします。
		@param[in]	xyz		: 全ての軸の拡縮率
	*/
	void Scale(float xyz);

	/**
		@brief		この行列を逆行列化します。
	*/
	void Inverse();

	/**
		@brief		この行列を転置します。
	*/
	void Transpose();
	
	/**
		@brief		この行列をスケーリング、回転、移動要素に分解します。
		@param[in]	scale		: スケーリング要素を格納するベクトルのポインタ
		@param[in]	rotation	: 回転要素を格納するクォータニオンのポインタ
		@param[in]	translation	: 平行移動要素を格納するベクトルのポインタ
		@details	各引数は必要が無ければ NULL を指定できます。
	*/
	void Decompose(Vector3* scale, Quaternion* rotation, Vector3* translation) const;

	/**
		@brief		この行列をスケーリング、回転、移動行列に分解します。
		@param[in]	scale		: スケーリング行列を格納する変数のポインタ
		@param[in]	rotation	: 回転行列を格納する変数のポインタ
		@param[in]	translation	: 平行移動行列を格納する変数のポインタ
		@details	各引数は必要が無ければ NULL を指定できます。
	*/
	void DecomposeMatrices(Matrix* scale, Matrix* rotation, Matrix* translation) const;

	/**
		@brief		この回転行列を XYZ 各軸のオイラー角度に変換します。
		@param[in]  order		: 回転順序
		@param[in]  locked		: ジンバルロックの発生有無を格納する変数へのポインタ (ジンバルロックが発生した場合 true)
		@return		XYZ 各軸のオイラー角度 (ラジアン単位) を格納したベクトル
		@details	回転行列は order に指定された順序で乗算されたものでなければ本来の回転角度が取得できないことに注意してください。<br>
					例えば RotationYawPitchRoll() は、回転軸 Z → Y → X の順に回転を行いますが、
					これから元の角度を取り出すには RotationOrder_ZXY を指定します。
	*/
	Vector3 ToEulerAngles(RotationOrder order = RotationOrder_ZXY, bool* locked = NULL);

	/**
		@brief		要素のいずれかが NaN または Inf かを判別します。
	*/
	bool ContainsNaNOrInf() const;

	/**
		@brief		デバッグ用に文字列を標準出力します。
		@param[in]	format	: 書式指定文字列
		@param[in]	stream	: 出力先ストリーム
		@details	format が NULL の場合、書式は "%f, %f, %f, %f\n %f, %f, %f, %f\n %f, %f, %f, %f\n %f, %f, %f, %f\n" を使用します。
	*/
	void Print(const char* format = NULL, FILE* stream = NULL) const;

public:

	/**
		@brief		2つのク行列の積を計算します。
		@param[in]	mat1	: 処理の基になる行列
		@param[in]	mat2	: 処理の基になる行列
		@return		演算結果の行列
	*/
	static Matrix Multiply(const Matrix& mat1, const Matrix& mat2);

	/**
		@brief		平行移動行列を作成します。
		@param[in]	x		: X 軸の移動量
		@param[in]	y		: Y 軸の移動量
		@param[in]	z		: Z 軸の移動量
		@return		演算結果の行列
	*/
	static Matrix Translation(float x, float y, float z);

	/**
		@brief		平行移動行列を作成します。
		@param[in]	vec		: 移動量
		@return		演算結果の行列
	*/
	static Matrix Translation(const Vector3& vec);

	/**
		@brief		X 軸を回転軸にして回転する行列を作成します。
		@param[in]	r		: 回転角度 (ラジアン単位)
		@return		演算結果の行列
	*/
	static Matrix RotationX(float r);

	/**
		@brief		Y 軸を回転軸にして回転する行列を作成します。
		@param[in]	r		: 回転角度 (ラジアン単位)
		@return		演算結果の行列
	*/
	static Matrix RotationY(float r);

	/**
		@brief		Z 軸を回転軸にして回転する行列を作成します。
		@param[in]	r		: 回転角度 (ラジアン単位)
		@return		演算結果の行列
	*/
	static Matrix RotationZ(float r);
	
	/**
		@brief		回転軸と角度を指定して行列を作成します。
		@param[in]  axis	: 回転軸
		@param[in]  r		: 回転角度 (ラジアン単位)
		@return		演算結果の行列
		@details	axis が単位ベクトルでない場合は正規化してから計算を行います。
	*/
	static Matrix RotationAxis(const Vector3& axis, float r);

	/**
		@brief		クォータニオンから回転行列を作成します。
		@param[in]  qua		: 処理の基になるクォータニオン
		@return		演算結果の行列
	*/
	static Matrix RotationQuaternion(const Quaternion& qua);

	/**
		@brief		XYZ 各軸のオイラー角から回転行列を作成します。
		@param[in]  x		: X 軸の回転角度 (ラジアン単位)
		@param[in]  y		: Y 軸の回転角度 (ラジアン単位)
		@param[in]  z		: Z 軸の回転角度 (ラジアン単位)
		@param[in]  order	: 回転順序
		@return		演算結果の行列
	*/
	static Matrix RotationEulerAngles(float x, float y, float z, RotationOrder order = RotationOrder_ZXY);

	/**
		@brief		XYZ 各軸のオイラー角から回転行列を作成します。
		@param[in]  angles	: XYZ 各軸の回転角度 (ラジアン単位)
		@param[in]  order	: 回転順序
		@return		演算結果の行列
	*/
	static Matrix RotationEulerAngles(const Vector3& angles, RotationOrder order = RotationOrder_ZXY);

	/**
		@brief		ヨー、ピッチ、ロールを指定して行列を作成します。
		@param[in]	yaw		: y 軸を中心とするヨー (ラジアン単位)
		@param[in]	pitch	: x 軸を中心とするピッチ (ラジアン単位)
		@param[in]	roll	: z 軸を中心とするロール (ラジアン単位)
		@return		演算結果の行列
		@details	回転順序は Z → X → Y です。引数の並びとは逆順であることに注意してください。
					これは  D3DXMatrixRotationYawPitchRoll の回転順序と同じで、
					RotationEulerAngles に RotationOrder_ZXY を指定して計算した結果とも同じです。
	*/
	static Matrix RotationYawPitchRoll(float yaw, float pitch, float roll);

	/**
		@brief		各軸にそってスケーリングする行列を作成します。
		@param[in]	x		: X 軸の拡縮率
		@param[in]	y		: Y 軸の拡縮率
		@param[in]	z		: Z 軸の拡縮率
		@return		演算結果の行列
	*/
	static Matrix Scaling(float x, float y, float z);

	/**
		@brief		各軸にそってスケーリングする行列を作成します。
		@param[in]	vec		: 各軸の拡縮率
		@return		演算結果の行列
	*/
	static Matrix Scaling(const Vector3& vec);

	/**
		@brief		各軸にそってスケーリングする行列を作成します。
		@param[in]	xyz		: 全ての軸の拡縮率
		@return		演算結果の行列
	*/
	static Matrix Scaling(float xyz);

	/**
		@brief		指定された行列の逆行列を作成します。
		@param[in]  mat		: 処理の基になる行列
		@return		作成された逆行列
	*/
	static Matrix Inverse(const Matrix& mat);

	/**
		@brief		指定された行列の転置行列を作成します。
		@param[in]  mat		: 処理の基になる行列
		@return		作成された転置行列
	*/
	static Matrix Transpose(const Matrix& mat);

	/**
		@brief		平面に対して座標系を反転した行列を作成します。
		@param[in]  mat			: 処理の基になる行列
		@param[in]  plane		: 処理の基になる平面
		@return		演算結果の行列
		@details	plane は正規化してから計算を行います。
	*/
	static Matrix Reflection(const Plane& plane);

	/**
		@brief		左手座標系ビュー行列を作成します。
		@param[in]	position	: 視点の位置
		@param[in]	lookAt		: 注視点
		@param[in]	up			: 上方向
		@return		演算結果の行列
	*/
	static Matrix LookAtLH(const Vector3& position, const Vector3& lookAt, const Vector3& up);

	/**
		@brief		右手座標系ビュー行列を作成します。
		@param[in]	position	: 視点の位置
		@param[in]	lookAt		: 注視点
		@param[in]	up			: 上方向
		@return		演算結果の行列
	*/
	static Matrix LookAtRH(const Vector3& position, const Vector3& lookAt, const Vector3& up);

	/**
		@brief		右手座標系パースペクティブ射影行列を作成します。
		@param[in]  fovY		: Y 方向への視野角 (ラジアン単位)
		@param[in]  aspect		: アスペクト比 (横幅/高さ)
		@param[in]  near		: 近いビュー平面の Z 値
		@param[in]  far			: 遠いビュー平面の Z 値
		@return		演算結果の行列
	*/
	static Matrix PerspectiveFovLH(float fovY, float aspect, float near, float far);

	/**
		@brief		右手座標系パースペクティブ射影行列を作成します。
		@param[in]  fovY		: Y 方向への視野角 (ラジアン単位)
		@param[in]  aspect		: アスペクト比 (横幅/高さ)
		@param[in]  near		: 近いビュー平面の Z 値
		@param[in]  far			: 遠いビュー平面の Z 値
		@return		演算結果の行列
	*/
	static Matrix PerspectiveFovRH(float fovY, float aspect, float near, float far);

	/**
		@brief		右手座標系正射影行列を作成します。
		@param[in]  width		: スクリーンの幅
		@param[in]  height		: スクリーンの高さ
		@param[in]  near		: 一番近いビュー平面の Z 値
		@param[in]  far			: 一番遠いビュー平面の Z 値
		@return		演算結果の行列
	*/
	static Matrix OrthoLH(float width, float height, float near, float far);

	/**
		@brief		右手座標系正射影行列を作成します。
		@param[in]  width		: スクリーンの幅
		@param[in]  height		: スクリーンの高さ
		@param[in]  near		: 一番近いビュー平面の Z 値
		@param[in]  far			: 一番遠いビュー平面の Z 値
		@return		演算結果の行列
	*/
	static Matrix OrthoRH(float width, float height, float near, float far);

	/**
		@brief		アフィン変換行列を作成します。
		@param[in]  scaling			: スケーリング係数
		@param[in]  rotationCenter	: 回転の中心位置
		@param[in]  rotation		: 回転
		@param[in]  translation		: 平行移動
		@return		演算結果の行列
	*/
	static Matrix AffineTransformation(const Vector3& scaling, const Vector3& rotationCenter, const Quaternion& rotation, const Vector3& translation);

public:

	Matrix& operator *= (const Matrix& mat);

	friend Matrix operator * (const Matrix& mat1, const Matrix& mat2);
	friend Matrix operator * (const Matrix& mat1, float v);

	bool operator == (const Matrix& mat) const;
	bool operator != (const Matrix& mat) const;


#ifdef LN_MATH_MATRIX_CAST_OPERATORS
	LN_MATH_MATRIX_CAST_OPERATORS
#endif
};

} // namespace Lumino

#include "Matrix.inl"

#endif // LUMINO_MATH_MATRIX_H
