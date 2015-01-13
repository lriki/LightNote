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

	/// 値の設定
	void set( lnFloat x_, lnFloat y_, lnFloat z_, lnFloat w_ );

	//----------------------------------------------------------------------
	///**
	//	@brief      長さの取得
	//*/
	//----------------------------------------------------------------------
	//lnFloat getLength() const;

	//----------------------------------------------------------------------
	///**
	//	@brief		3D ベクトルの長さの2乗を返す
	//*/
	//----------------------------------------------------------------------
	//lnFloat getSquareLength() const;

	//----------------------------------------------------------------------
	///**
	//	@brief		正規化する
	//*/
	//----------------------------------------------------------------------
	void normalize();

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

	/// 標準出力へ出力する
    void cdump( const lnChar* str_ = NULL ) const
    {
        _tprintf( "%s ( %f, %f, %f, %f )",
            ( str_ ) ? str_ : _T( "Quaternion" ),
            x, y, z, w );
    }

public:

	//--------------------------------------------------------------------------
	// ▼ static

	//----------------------------------------------------------------------
	///**
	//  @brief      単位クォータニオンの作成
	//*/
	//----------------------------------------------------------------------
	static void identity(Quaternion* out);

	//----------------------------------------------------------------------
	///**
	//  @brief      正規化したクォータニオンを返す
	//
	//  @param[out] out_ : 演算結果を格納する Quaternion へのポインタ
	//  @param[in]  qua_ : 処理の基になる Quaternion
	//*/
	//----------------------------------------------------------------------
    static void normalize( Quaternion* out_, const Quaternion& qua_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      クォータニオンの共役を返す
	//
	//  @param[out] out_ : 演算結果を格納する Quaternion へのポインタ
	//  @param[in]  qua_ : 処理の基になる Quaternion
	//*/
	//----------------------------------------------------------------------
    static void conjugate( Quaternion* out_, const Quaternion& qua_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      任意の軸を回転軸にして回転するクォータニオンを作成する ( ラジアン角度 )
	//
	//  @param[out] out_      : 演算結果を格納する Quaternion へのポインタ
	//  @param[in]  axis_     : 回転軸
	//  @param[in]  r_        : 回転角度
	//
	//  @par
	//              与えられた軸が正規化されていない場合は正規化したものを使って計算します。
	//*/
	//----------------------------------------------------------------------
	static void rotationAxis( Quaternion* out_, const Vector3& axis_, lnFloat r_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      回転行列からクォータニオンを作成する
	//
	//  @param[out] out_ : 演算結果を格納する Quaternion へのポインタ
	//  @param[in]  mat_ : 処理の基になる Matrix
	//*/
	//----------------------------------------------------------------------
    static void rotationMatrix( Quaternion* out_, const Matrix& mat_ );

	/// クォータニオンを生成する
	static void rotationYawPitchRoll( Quaternion* out, float yaw, float pitch, float roll );


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