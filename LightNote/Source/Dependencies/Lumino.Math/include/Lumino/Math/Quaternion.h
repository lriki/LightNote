
#ifndef LUMINO_MATH_QUATERNION_H
#define LUMINO_MATH_QUATERNION_H

#include <stdio.h>
#include "Common.h"

namespace Lumino
{
class Vector3;
class Matrix;

/**
	@brief		クォータニオンを定義します。

	@details	このクォータニオンクラスの乗算の順番は左から右です。
				つまり、q1 * q2 は、先に q1、次に q2 の回転を適用する事と同じです。

				この乗算順序は標準的な四元数の定義からは逆行していますが、
				ゲームプログラムをメインとした3Dグラフィックス向けの線形代数ライブラリとしては
				この動作がほぼ一般的なので、これに合わせています。<br>
				(DirectX(D3DX)、XNA、Unity、cocos2dx 等はこの動作)
*/
class LUMINO_EXPORT Quaternion
{
public:

	float	X;		///< X 要素
	float	Y;		///< Y 要素
	float	Z;		///< Z 要素
	float	W;		///< W 要素

public:

	static const Quaternion Identity;	///< 単位クォータニオン (0, 0, 0, 1)

public:

	/**
		@brief		単位クォータニオンを設定してインスタンスを初期化します。
	*/
	Quaternion();

	/**
		@brief		指定した値を使用してインスタンスを初期化します。
	*/
	Quaternion(float x, float y, float z, float w);

public:

	/**
		@brief		各要素に値を設定します。
	*/
	void Set(float x, float y, float z, float w);

	/**
		@brief		このクォータニオンの長さを返します。
	*/
	float GetLength() const;

	/**
		@brief		このクォータニオンの長さの 2 乗を返します。
	*/
	float GetLengthSquared() const;
	
	/**
		@brief		このクォータニオンを正規化します。
	*/
	void Normalize();
	
	/**
		@brief		このクォータニオンが単位クォータニオンであるかを判定します。
		@return		true の場合、単位クォータニオンである
	*/
	bool IsIdentity() const;

	/**
		@brief		このクォータニオンに指定されたクォータニオンを乗算します。
		@param[in]	qua		: 乗算するクォータニオン
	*/
	void Multiply(const Quaternion& qua);

	/**
		@brief		このクォータニオンを X 軸で回転します。
		@param[in]	angle	: 回転角度 (ラジアン単位)
	*/
	void RotateX(float angle);

	/**
		@brief		このクォータニオンを Y 軸で回転します。
		@param[in]	angle	: 回転角度 (ラジアン単位)
	*/
	void RotateY(float angle);

	/**
		@brief		このクォータニオンを Z 軸で回転します。
		@param[in]	angle	: 回転角度 (ラジアン単位)
	*/
	void RotateZ(float angle);
	
	/**
		@brief		このクォータニオンを回転軸と角度を指定して回転します。
		@param[in]  axis	: 回転軸
		@param[in]  r		: 回転角度 (ラジアン単位)
		@details	axis が単位ベクトルでなければ正規化してから計算を行います。
	*/
	void RotateAxis(const Vector3& axis, float r);

	/**
		@brief		このクォータニオンを XYZ 各軸のオイラー角度に変換します。
		@param[in]  order		: 回転順序
		@param[in]  locked		: ジンバルロックの発生有無を格納する変数へのポインタ (ジンバルロックが発生した場合 true)
		@return		XYZ 各軸のオイラー角度 (ラジアン単位) を格納したベクトル
		@details	クォータニオンは order に指定された順序で乗算されたものでなければ本来の回転角度が取得できないことに注意してください。<br>
					例えば RotationYawPitchRoll() は、回転軸 Z → Y → X の順に回転を行いますが、
					これから元の角度を取り出すには RotationOrder_ZXY を指定します。
	*/
	Vector3 ToEulerAngles(RotationOrder order = RotationOrder_ZXY, bool* locked = NULL);

	/**
		@brief		このクォータニオンの回転軸と回転角度を取得します。
		@param[in]	axis		: 回転軸を格納するベクトルのポインタ
		@param[in]	angle		: 回転角度を格納する変数のポインタ
		@details	axis には単位ベクトルを格納します。
	*/
	void ToAxisAngle(Vector3* axis, float* angle);

	/**
		@brief		デバッグ用に文字列を標準出力します。
		@param[in]	format	: 書式指定文字列
		@param[in]	stream	: 出力先ストリーム
		@details	format が NULL の場合、書式は "(%f, %f, %f, %f)\n" を使用します。
	*/
	void Print(const char* format = NULL, FILE* stream = NULL) const;

public:

	/**
		@brief		指定クォータニオンを正規化したクォータニオンを返します。
		@param[in]	qua		: 処理の基になるクォータニオン
		@return		正規化されたクォータニオン
	*/
    static Quaternion Normalize(const Quaternion& qua);
	
	/**
		@brief		指定クォータニオンの共役を返します。
		@param[in]	qua		: 処理の基になるクォータニオン
		@return		共役クォータニオン
	*/
    static Quaternion Conjugate(const Quaternion& qua);
	
	/**
		@brief		2つのクォータニオンの内積を計算します。
		@param[in]	qua1	: 処理の基になるクォータニオン
		@param[in]	qua2	: 処理の基になるクォータニオン
		@return		2つのクォータニオンの内積
	*/
	static float Dot(const Quaternion& qua1, const Quaternion& qua2);
	
	/**
		@brief		クォータニオンを共役し、再正規化します。
		@param[in]	qua		: 処理の基になるクォータニオン
		@return		逆数クォータニオン
	*/
	static Quaternion Inverse(const Quaternion& qua);

	/**
		@brief		2つのクォータニオンの積を計算します。
		@param[in]	qua1	: 処理の基になるクォータニオン
		@param[in]	qua2	: 処理の基になるクォータニオン
		@details	Multiply(qua1, qua2) は、先に qua1、次に qua2 の回転を行うクォータニオンを作成します。
	*/
	static Quaternion Multiply(const Quaternion& qua1, const Quaternion& qua2);
	
	/**
		@brief		回転軸と角度を指定してクォータニオンを作成します。
		@param[in]  axis	: 回転軸
		@param[in]  r		: 回転角度 (ラジアン単位)
		@return		新しく作成されたクォータニオン
		@details	axis が単位ベクトルでなければ正規化してから計算を行います。
	*/
	static Quaternion RotationAxis(const Vector3& axis, float r);
	
	/**
		@brief		回転行列からクォータニオンを作成します。
		@param[in]  mat		: 処理の基になる行列
		@return		新しく作成されたクォータニオン
	*/
	static Quaternion RotationMatrix(const Matrix& mat);
	
	/**
		@brief		ヨー、ピッチ、ロールを指定してクォータニオンを作成します。
		@param[in]	yaw		: y 軸を中心とするヨー (ラジアン単位)
		@param[in]	pitch	: x 軸を中心とするピッチ (ラジアン単位)
		@param[in]	roll	: z 軸を中心とするロール (ラジアン単位)
		@return		新しく作成されたクォータニオン
		@details	回転順序は Z → X → Y です。引数の並びとは逆順であることに注意してください。
					これは  D3DXQuaternionRotationYawPitchRoll の回転順序と同じで、
					RotationEulerAngles に RotationOrder_ZXY を指定して計算した結果とも同じです。
	*/
	static Quaternion RotationYawPitchRoll(float yaw, float pitch, float roll);

	/**
		@brief		XYZ 各軸のオイラー角からクォータニオンを作成します。
		@param[in]  angles	: XYZ 各軸の回転角度 (ラジアン単位)
		@param[in]  order	: 回転順序
		@return		演算結果のクォータニオン
	*/
	static Quaternion RotationEulerAngles(const Vector3& angles, RotationOrder order = RotationOrder_ZXY);

	/**
		@brief		球面線形補間を使用して、2 つのクオータニオン間を補間します。
		@param[in]	qua1	: 処理の基になるクオータニオン (t = 0.0 のときの値)
		@param[in]	qua2	: 処理の基になるクオータニオン (t = 1.0 のときの値)
		@param[in]	t		: 加重係数
		@return		補間結果のクォータニオン
		@details	t は通常、0.0～1.0 を指定します。
	*/
	static Quaternion Slerp(const Quaternion& qua1, const Quaternion& qua2, float t);

public:
	
	Quaternion& operator += (const Quaternion& v);
	Quaternion& operator += (float v);
	Quaternion& operator -= (const Quaternion& v);
	Quaternion& operator -= (float v);
	Quaternion& operator *= (const Quaternion& v);	///< this->Multiply(v); と同じです。
	Quaternion& operator *= (float v);
	Quaternion& operator /= (float v);

	friend Quaternion operator + (const Quaternion& v1,	const Quaternion& v2);
	friend Quaternion operator + (const Quaternion& v1,	float v2);
	friend Quaternion operator + (float v1,				const Quaternion& v2);
	friend Quaternion operator - (const Quaternion& v1,	const Quaternion& v2);
	friend Quaternion operator - (const Quaternion& v1,	float v2);
	friend Quaternion operator - (float v1,				const Quaternion& v2);
	friend Quaternion operator * (const Quaternion& v1, const Quaternion& v2);	///< Quaternion::Multiply(v1, v2); と同じです。
	friend Quaternion operator * (const Quaternion& v1,	float v2);
	friend Quaternion operator * (float v1,				const Quaternion& v2);
	friend Quaternion operator / (const Quaternion& v1,	float v2);

	friend Quaternion operator - (const Quaternion& v1);

	bool operator == (const Quaternion& v) const;
	bool operator != (const Quaternion& v) const;

};

} // namespace Lumino

#include "Quaternion.inl"

#endif // LUMINO_MATH_QUATERNION_H





#if 0

//==============================================================================
// Quaternion 
//------------------------------------------------------------------------------
///**
//  @file       Quaternion.h
//  @brief      Quaternion
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

#include "Vector3.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Math
{

//==============================================================================
// ■ Quaternion クラス
//------------------------------------------------------------------------------
///**
//  @brief      四元数のクラス
//	@note
//				右ねじの回転
//*/
//==============================================================================
class Quaternion
{
public:

	lnFloat x, y, z, w;

public:

    static const Quaternion    IDENTITY;       ///< ( 0, 0, 0, 1 )

public:

	/// デフォルトコンストラクタ (IDENTITY で初期化 )
	Quaternion();

	/// コピーコンストラクタ
	Quaternion( const Quaternion& vec_ );

	/// コンストラクタ
	Quaternion( lnFloat x_, lnFloat y_, lnFloat z_, lnFloat w_ );

	//----------------------------------------------------------------------
	///**
	//	@brief      コンストラクタ ( 軸と回転角度から作成 )
	//
	//  @par
	//              与えられた軸が正規化されていない場合は正規化したものを使って計算します。
	//*/
	//----------------------------------------------------------------------
	Quaternion( const Vector3& axis_, lnFloat r_ );

	//----------------------------------------------------------------------
	///**
	//	@brief      コンストラクタ ( オイラー角度から作成・ラジアン単位 )
	//
	//  @par
	//              回転順序は今のところ X → Y → Z だけです。
	//*/
	//----------------------------------------------------------------------
	Quaternion( const Vector3& angle_ );

public:

	
	//----------------------------------------------------------------------
	///**
	//	@brief		長さが 0 ( すべての要素が 0 ) かを判定する
	//*/
	//----------------------------------------------------------------------
	bool isZero() const { return ( x == 0 && y == 0 && z == 0 && w == 0 ); }

public:

    Quaternion& operator = ( const Quaternion& q_ );

	Quaternion operator + ( const Quaternion& q_ ) const;
	Quaternion operator - ( const Quaternion& q_ ) const;
	Quaternion operator * ( const Quaternion& q_ ) const;
	Quaternion operator * ( const lnFloat a_ ) const;
	Quaternion operator / ( const lnFloat a_ ) const;

	// SlimDX と同じ
	void operator *= ( const Quaternion& q );

/*
	Vector3& operator = ( const lnFloat a_ );
	Vector3& operator = ( const Vector3& vec_ );
	void operator += ( const lnFloat a_ );
	void operator += ( const Vector3& vec_ );
	void operator -= ( const lnFloat a_ );
	void operator -= ( const Vector3& vec_ );
	void operator *= ( const lnFloat a_ );
	void operator *= ( const Vector3& vec_ );
	void operator /= ( const lnFloat a_ );
	void operator /= ( const Vector3& vec_ );
	
	bool operator == ( const Vector3& vec_ ) const;
	bool operator != ( const Vector3& vec_ ) const;
	*/

public:

	
	

	//----------------------------------------------------------------------
	///**
	//  @brief      任意の軸を回転軸にして回転するクォータニオンを作成する ( インデックス角度 )
	//
	//  @param[out] out_      : 演算結果を格納する Quaternion へのポインタ
	//  @param[in]  axis_     : 回転軸
	//  @param[in]  r_        : 回転角度
	//
	//  @par
	//              与えられた軸が正規化されていない場合は正規化したものを使って計算します。
	//*/
	//----------------------------------------------------------------------
	static void rotationAxisIdx( Quaternion* out_, const Vector3& axis_, int r_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      2 つのクォータニオンの積を計算する
	//
	//  @param[out] out_  : 演算結果を格納する Quaternion へのポインタ
	//  @param[in]  qua1_ : 処理の基になる Quaternion
	//  @param[in]  qua2_ : 処理の基になる Quaternion
	//
	//  @attention
	//              Direct3D の D3DXQUATERNION や D3DXQuaternionMultiply() の計算順序とは逆です。<br>
	//              Direct3D は行列の乗算と同じような順番で乗算できるようにするため、
	//              普通とは違う方法をとっているとのこと。<br><br>
	//              
	//              ちなみに Quaternion の operator * の順番はこの関数と同じです。
	//*/
	//----------------------------------------------------------------------
    static void multiply( Quaternion* out_, const Quaternion& q1_, const Quaternion& q2_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      2 つのクォータニオンの積を計算する ( 逆順 )
	//
	//  @param[out] out_  : 演算結果を格納する Quaternion へのポインタ
	//  @param[in]  qua1_ : 処理の基になる Quaternion
	//  @param[in]  qua2_ : 処理の基になる Quaternion
	//
	//  @par
	//              QuaternionMultiply() とは逆の順番で乗算します。<br>
	//              D3DXQuaternionMultiply() と同じ動作。
	//*/
	//----------------------------------------------------------------------
    static void multiplyRO( Quaternion* out_, const Quaternion& q1_, const Quaternion& q2_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      球面線形補間を使って、2 つのクォータニオンを補間する
	//
	//  @param[out] out_  : 演算結果を格納する Quaternion へのポインタ
	//  @param[in]  qua1_ : 処理の基になる Quaternion
	//  @param[in]  qua2_ : 処理の基になる Quaternion
	//  @param[in]  t_    : 補間値
	//*/
	//----------------------------------------------------------------------
    static void slerp( Quaternion* out_, const Quaternion& qua1_, const Quaternion& qua2_, lnFloat t_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      クォータニオンをオイラー角に変換する
	//
	//  @param[out] out_  : 演算結果を格納する Vector3 へのポインタ
	//  @param[in]  qua1_ : 処理の基になる Quaternion
	//*/
	//----------------------------------------------------------------------
    static void toEuler( Vector3* out_, const Quaternion& qua1_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      オイラー角からクォータニオンを作成する
	//
	//  @param[out] out_  : 演算結果を格納する Quaternion へのポインタ
	//  @param[in]  qua1_ : 処理の基になる Vector3 (各軸の角度)
	//*/
	//----------------------------------------------------------------------
    static void fromEuler( Quaternion* out_, const Vector3& angle_ );
	// ↑↓ 回転行列を掛け合わせた順に合わせないとダメ

	//----------------------------------------------------------------------
	///**
	//  @brief      クォータニオンをYaw(Y回転), Pitch(X回転), Roll(Z回転)に分解する
	//
	//  @param[out] z_      : Z軸回転
	//  @param[out] x_      : X軸回転(-PI/2～PI/2)
	//  @param[out] y_      : Y軸回転
	//  @param[in]  qua1_   : 処理の基になる Quaternion
	//
	//  @return
	//              ジンバルロックが発生した時はfalse。ジンバルロックはX軸回転で発生
	//*/
	//----------------------------------------------------------------------
    static bool factoringZXY( lnFloat* z_, lnFloat* x_, lnFloat* y_, const Quaternion& qua1_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      クォータニオンをX,Y,Z回転に分解する
	//
	//  @param[out] x_      : X軸回転
	//  @param[out] y_      : Y軸回転(-PI/2～PI/2)
	//  @param[out] z_      : Z軸回転
	//  @param[in]  qua1_   : 処理の基になる Quaternion
	//
	//  @return
	//              ジンバルロックが発生した時はfalse。ジンバルロックはY軸回転で発生
	//*/
	//----------------------------------------------------------------------
    static bool factoringXYZ( lnFloat* x_, lnFloat* y_, lnFloat* z_, const Quaternion& qua1_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      クォータニオンをY,Z,X回転に分解する
	//
	//  @param[out] y_      : Y軸回転
	//  @param[out] z_      : Z軸回転(-PI/2～PI/2)
	//  @param[out] x_      : X軸回転
	//  @param[in]  qua1_   : 処理の基になる Quaternion
	//
	//  @return
	//              ジンバルロックが発生した時はfalse。ジンバルロックはZ軸回転で発生
	//*/
	//----------------------------------------------------------------------
    static bool factoringYZX( lnFloat* y_, lnFloat* z_, lnFloat* x_, const Quaternion& qua1_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      クォータニオンをY,Z,X回転に分解する
	//
	//  @param[out] out		: 計算結果を格納する Vector3
	//  @param[in]  qua1    : 処理の基になる Quaternion
	//
	//  @return
	//              ジンバルロックが発生した時はfalse。ジンバルロックはZ軸回転で発生
	//*/
	//----------------------------------------------------------------------
    static bool factoringEuler( Vector3* out, const Quaternion& qua1, RotationOrder order = RotationOrder_XYZ );
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

    inline Quaternion& Quaternion::operator = ( const Quaternion& q_ )
    {
        x = q_.x;
		y = q_.y;
		z = q_.z;
        w = q_.w;
		return *this;
    }

    inline Quaternion Quaternion::operator + ( const Quaternion& q_ ) const
	{
		return Quaternion( x + q_.x, y + q_.y, z + q_.z, w + q_.w );
	}

	inline Quaternion Quaternion::operator - ( const Quaternion& q_ ) const
	{
		return Quaternion( x - q_.x, y - q_.y, z - q_.z, w - q_.w );
	}

	inline Quaternion Quaternion::operator * ( const Quaternion& q_ ) const
	{
		//return Quaternion(
		//	y * q_.z - z * q_.y + w * q_.x + x * q_.w,
		//	z * q_.x - x * q_.z + w * q_.y + y * q_.w,
		//	x * q_.y - y * q_.x + w * q_.z + z * q_.w,
		//	w * q_.w - x * q_.x - y * q_.y - z * q_.z );

		/*
		lnFloat tx, ty, tz;
		tx = y * q_.z - z * q_.y;
		ty = -( x * q_.z - z * q_.x );
		tz = x * q_.y - y * q_.x;

		return Quaternion(
			w * q_.x + q_.w * x + tx,
			w * q_.y + q_.w * y + ty,
			w * q_.z + q_.w * z + tz,
			w * q_.w - ( x * q_.x + y * q_.y + z * q_.z ) );

		*/

		return Quaternion(
			w * q_.x + x * q_.w + y * q_.z + ( -z * q_.y ),
			w * q_.y + y * q_.w + z * q_.x + ( -x * q_.z ),
			w * q_.z + z * q_.w + x * q_.y + ( -y * q_.x ),
			w * q_.w + ( -x * q_.x ) + ( -y * q_.y ) + ( -z * q_.z ) );
		
	}

	inline Quaternion Quaternion::operator * ( const lnFloat a_ ) const
	{
		return Quaternion( x * a_, y * a_, z * a_, w * a_ );
	}


	inline Quaternion Quaternion::operator / ( const lnFloat a_ ) const
	{
		lnFloat r = 1.0f / a_;
		return Quaternion( x * a_, y * a_, z * a_, w * a_ );
	}

	
	inline void Quaternion::operator *= ( const Quaternion& q )
	{
		float lx = x;
        float ly = y;
        float lz = z;
        float lw = w;
        float rx = q.x;
        float ry = q.y;
        float rz = q.z;
        float rw = q.w;

        x = (rx * lw + lx * rw + ry * lz) - (rz * ly);
        y = (ry * lw + ly * rw + rz * lx) - (rx * lz);
        z = (rz * lw + lz * rw + rx * ly) - (ry * lx);
        w = (rw * lw) - (rx * lx + ry * ly + rz * lz);
	}

 

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Math
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================

#endif
