//==============================================================================
// Matrix 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Common.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "SQTTransform.h"
#include "Matrix.h"

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
// ■ Matrix クラス
//==============================================================================

    const Matrix Matrix::IDENTITY = Matrix();

	// デフォルトコンストラクタ ( 単位行列の作成 )
	Matrix::Matrix()
	{
		m00 = m11 = m22 = m33 = 1.0f;
		m01 = m02 = m03 = 0.0f;
		m10 = m12 = m13 = 0.0f;
		m20 = m21 = m23 = 0.0f;
		m30 = m31 = m32 = 0.0f;
	}

	// コピーコンストラクタ
	Matrix::Matrix( const Matrix& mat_ )
	{
		memcpy( this, &mat_, sizeof( Matrix ) );
	}

	// コンストラクタ
	Matrix::Matrix(
		lnFloat m00_, lnFloat m01_, lnFloat m02_, lnFloat m03_,
		lnFloat m10_, lnFloat m11_, lnFloat m12_, lnFloat m13_,
		lnFloat m20_, lnFloat m21_, lnFloat m22_, lnFloat m23_,
		lnFloat m30_, lnFloat m31_, lnFloat m32_, lnFloat m33_ )
	{
		m00 = m00_; m01 = m01_; m02 = m02_; m03 = m03_;
		m10 = m10_; m11 = m11_; m12 = m12_; m13 = m13_;
		m20 = m20_; m21 = m21_; m22 = m22_; m23 = m23_;
		m30 = m30_; m31 = m31_; m32 = m32_; m33 = m33_;
	}

	/// コンストラクタ ( クォータニオンから作成する )
	Matrix::Matrix( const Quaternion& q_ )
    {
		Matrix::rotationQuaternion( this, q_ );
        /*
        float qw, qx, qy, qz;
    float x2, y2, z2;
    float xy, yz, zx;
    float wx, wy, wz;

    qw = lpQ.w; qx = lpQ.x; qy = lpQ.y; qz = lpQ.z;

    x2 = 2.0f * qx * qx;
    y2 = 2.0f * qy * qy;
    z2 = 2.0f * qz * qz;

    xy = 2.0f * qx * qy;
    yz = 2.0f * qy * qz;
    zx = 2.0f * qz * qx;
        
    wx = 2.0f * qw * qx;
    wy = 2.0f * qw * qy;
    wz = 2.0f * qw * qz;

    m00 = 1.0f - y2 - z2;
    m01 = xy - wz;
    m02 = zx + wy;
    m03 = 0.0f;

    m10 = xy + wz;
    m11 = 1.0f - z2 - x2;
    m12 = yz - wx;
    m13 = 0.0f;

    m20 = zx - wy;
    m21 = yz + wx;
    m22 = 1.0f - x2 - y2;
    m23 = 0.0f;

    m03 = m13 = m23 = m30 = m31 = m32 = 0.0f;
        m33 = 1.0f;
        */
        
        //m00 = 1.0f - 2.0f * q_.y * q_.y - 2.0f * q_.z * q_.z;
        //m01 = 2.0f * q_.x * q_.y + 2.0f * q_.w * q_.z;
        //m02 = 2.0f * q_.x * q_.z - 2.0f * q_.w * q_.y;

        //m10 = 2.0f * q_.x * q_.y - 2.0f * q_.w * q_.z;
        //m11 = 1.0f - 2.0f * q_.x * q_.x - 2.0f * q_.z * q_.z;
        //m12 = 2.0f * q_.y * q_.z + 2.0f * q_.w * q_.x;

        //m20 = 2.0f * q_.x * q_.z + 2.0f * q_.w * q_.y;
        //m21 = 2.0f * q_.y * q_.z - 2.0f * q_.w * q_.x;
        //m22 = 1.0f - 2.0f * q_.x * q_.x - 2.0f * q_.y * q_.y;

        //m03 = m13 = m23 = m30 = m31 = m32 = 0.0f;
        //m33 = 1.0f;
        
    }

	/// コンストラクタ (SQTTransform から作成する)
	Matrix::Matrix( const SQTTransform& t_ )
    {
        //this->scaling( t_.Scale );
        Matrix::scaling( this, t_.Scale );
        this->rotationQuaternion( t_.Rotation );
        this->translation( t_.Translation );
    }

	Matrix::Matrix( const LVector4& vec1, const LVector4& vec2, const LVector4& vec3, const LVector4& vec4 )
	{
		(*this)[0] = vec1;
		(*this)[1] = vec2;
		(*this)[2] = vec3;
		(*this)[3] = vec4;
	}

	// 右方向の取得
    const Vector3& Matrix::getRight() const { 
    	return (Vector3&)m00; }

	// 上方向の取得
    const Vector3& Matrix::getUp() const { 
    	return (Vector3&)m10; }

	// 正面方向の取得
    const Vector3& Matrix::getFront() const { 
    	return (Vector3&)m20; }

	// 位置の取得
    const Vector3& Matrix::getPosition() const { 
    	return (Vector3&)m30; }

	//----------------------------------------------------------------------
	// ● 単位行列を作成する
	//----------------------------------------------------------------------
    void Matrix::identity()
    {
        m00 = m11 = m22 = m33 = 1.0f;
		m01 = m02 = m03 = 0.0f;
		m10 = m12 = m13 = 0.0f;
		m20 = m21 = m23 = 0.0f;
		m30 = m31 = m32 = 0.0f;
    }

	//----------------------------------------------------------------------
	// ● 単位行列かを判定する
	//----------------------------------------------------------------------
    bool Matrix::isIdentity() const
    {
        static const Matrix m;
        return ( memcmp( this, &m, sizeof( Matrix ) ) == 0 );//( (*this) == m );
    }

	//----------------------------------------------------------------------
	// ● 値を設定する
	//----------------------------------------------------------------------
    void Matrix::set(
		lnFloat m00_, lnFloat m01_, lnFloat m02_, lnFloat m03_,
		lnFloat m10_, lnFloat m11_, lnFloat m12_, lnFloat m13_,
		lnFloat m20_, lnFloat m21_, lnFloat m22_, lnFloat m23_,
		lnFloat m30_, lnFloat m31_, lnFloat m32_, lnFloat m33_ )
    {
		m00 = m00_; m01 = m01_; m02 = m02_; m03 = m03_;
		m10 = m10_; m11 = m11_; m12 = m12_; m13 = m13_;
		m20 = m20_; m21 = m21_; m22 = m22_; m23 = m23_;
		m30 = m30_; m31 = m31_; m32 = m32_; m33 = m33_;
	}

	//----------------------------------------------------------------------
	// ● 指定された行列から回転要素だけを取り出して設定する
	//----------------------------------------------------------------------
    void Matrix::setRotateMatrix( const Matrix& matrix_ )
    {
        m00 = matrix_.m00; m01 = matrix_.m01; m02 = matrix_.m02; 
		m10 = matrix_.m10; m11 = matrix_.m11; m12 = matrix_.m12;
		m20 = matrix_.m20; m21 = matrix_.m21; m22 = matrix_.m22;
        m03 = m13 = m23 = m30 = m31 = m32 = 0.0f;
        m33 = 1.0f;
    }

	//----------------------------------------------------------------------
	// ● 行列を平行移動する
	//----------------------------------------------------------------------
	void Matrix::translation( lnFloat x_, lnFloat y_, lnFloat z_ )
	{
		/* 普通の行列計算
		m00, m01, m02, m03   1, 0, 0, 0
		m10, m11, m12, m13   0, 1, 0, 0
		m20, m21, m22, m23 * 0, 0, 1, 0
		m30, m31, m32, m33   x, y, z, 1
		*/
		/* 計算イメージ
		m00 = m00 * 1 + m01 * 0 + m02 * 0 + m03 * x;
		m01 = m00 * 0 + m01 * 1 + m02 * 0 + m03 * y;
		m02 = m00 * 0 + m01 * 0 + m02 * 1 + m03 * z;
		m03 = m00 * 0 + m01 * 0 + m02 * 0 + m03 * 1;

		m10 = m10 * 1 + m11 * 0 + m12 * 0 + m13 * x;
		m11 = m10 * 0 + m11 * 1 + m12 * 0 + m13 * y;
		m12 = m10 * 0 + m11 * 0 + m12 * 1 + m13 * z;
		m13 = m10 * 0 + m11 * 0 + m12 * 0 + m13 * 1;

		m20 = m20 * 1 + m21 * 0 + m22 * 0 + m23 * x;
		m21 = m20 * 0 + m21 * 1 + m22 * 0 + m23 * y;
		m22 = m20 * 0 + m21 * 0 + m22 * 1 + m23 * z;
		m23 = m20 * 0 + m21 * 0 + m22 * 0 + m23 * 1;

		m30 = m30 * 1 + m31 * 0 + m32 * 0 + m33 * x;
		m31 = m30 * 0 + m31 * 1 + m32 * 0 + m33 * y;
		m32 = m30 * 0 + m31 * 0 + m32 * 1 + m33 * z;
		m33 = m30 * 0 + m31 * 0 + m32 * 0 + m33 * 1;
		*/
		/* 正しく計算できるようにしたもの
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0 * 1 + mx1 * 0 + mx2 * 0 + m03 * x;
		m01 = mx0 * 0 + mx1 * 1 + mx2 * 0 + m03 * y;
		m02 = mx0 * 0 + mx1 * 0 + mx2 * 1 + m03 * z;
		m03 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m03 * 1;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * 1 + mx1 * 0 + mx2 * 0 + m13 * x;
		m11 = mx0 * 0 + mx1 * 1 + mx2 * 0 + m13 * y;
		m12 = mx0 * 0 + mx1 * 0 + mx2 * 1 + m13 * z;
		m13 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m13 * 1;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * 1 + mx1 * 0 + mx2 * 0 + m23 * x;
		m21 = mx0 * 0 + mx1 * 1 + mx2 * 0 + m23 * y;
		m22 = mx0 * 0 + mx1 * 0 + mx2 * 1 + m23 * z;
		m23 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m23 * 1;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * 1 + mx1 * 0 + mx2 * 0 + m33 * x;
		m31 = mx0 * 0 + mx1 * 1 + mx2 * 0 + m33 * y;
		m32 = mx0 * 0 + mx1 * 0 + mx2 * 1 + m33 * z;
		m33 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m33 * 1;
		*/
		/* 単純に * 0 とかの無駄なところを切る
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0 + m03 * x;
		m01 = mx1 + m03 * y;
		m02 = mx2 + m03 * z;
		m03 = m03;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 + m13 * x;
		m11 = mx1 + m13 * y;
		m12 = mx2 + m13 * z;
		m13 = m13;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 + m23 * x;
		m21 = mx1 + m23 * y;
		m22 = mx2 + m23 * z;
		m23 = m23;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 + m33 * x;
		m31 = mx1 + m33 * y;
		m32 = mx2 + m33 * z;
		m33 = m33;
		*/
		/* 自分自身を代入しているところを切る
		m00 = m00 + m03 * x;
		m01 = m01 + m03 * y;
		m02 = m02 + m03 * z;

		m10 = m10 + m13 * x;
		m11 = m11 + m13 * y;
		m12 = m12 + m13 * z;

		m20 = m20 + m23 * x;
		m21 = m21 + m23 * y;
		m22 = m22 + m23 * z;

		m30 = m30 + m33 * x;
		m31 = m31 + m33 * y;
		m32 = m32 + m33 * z;
		*/
		/* += にできるところを直す
		m00 += m03 * x;
		m01 += m03 * y;
		m02 += m03 * z;

		m10 += m13 * x;
		m11 += m13 * y;
		m12 += m13 * z;

		m20 += m23 * x;
		m21 += m23 * y;
		m22 += m23 * z;

		m30 += m33 * x;
		m31 += m33 * y;
		m32 += m33 * z;
		*/
		// m03 m13 m23 = 0  m33 = 1  とした場合
		m30 += x_;
		m31 += y_;
		m32 += z_;
	}

	void Matrix::translation( const Vector3& vec_ )
	{
		m30 += vec_.x;
		m31 += vec_.y;
		m32 += vec_.z;
	}

	//----------------------------------------------------------------------
	// ● X 軸を回転軸にして行列を回転する ( ラジアン単位 )
	//----------------------------------------------------------------------
	void Matrix::rotationX( lnFloat r_ )
	{
		lnFloat c, s;
        Asm::sincos( r_, &s, &c );

		/* 普通の行列計算
		m00, m01, m02, m03   1, 0, 0, 0
		m10, m11, m12, m13   0, c,-s, 0
		m20, m21, m22, m23 * 0, s, c, 0
		m30, m31, m32, m33   0, 0, 0, 1
		*/
		/* 計算イメージ
		m00 = m00 * 1 + m01 * 0 + m02 * 0 + m03 * 0;
		m01 = m00 * 0 + m01 * c + m02 * s + m03 * 0;
		m02 = m00 * 0 + m01 *-s + m02 * c + m03 * 0;
		m03 = m00 * 0 + m01 * 0 + m02 * 0 + m03 * 1;

		m10 = m10 * 1 + m11 * 0 + m12 * 0 + m13 * 0;
		m11 = m10 * 0 + m11 * c + m12 * s + m13 * 0;
		m12 = m10 * 0 + m11 *-s + m12 * c + m13 * 0;
		m13 = m10 * 0 + m11 * 0 + m12 * 0 + m13 * 1;

		m20 = m20 * 1 + m21 * 0 + m22 * 0 + m23 * 0;
		m21 = m20 * 0 + m21 * c + m22 * s + m23 * 0;
		m22 = m20 * 0 + m21 *-s + m22 * c + m23 * 0;
		m23 = m20 * 0 + m21 * 0 + m22 * 0 + m23 * 1;

		m30 = m30 * 1 + m31 * 0 + m32 * 0 + m33 * 0;
		m31 = m30 * 0 + m31 * c + m32 * s + m33 * 0;
		m32 = m30 * 0 + m31 *-s + m32 * c + m33 * 0;
		m33 = m30 * 0 + m31 * 0 + m32 * 0 + m33 * 1;
		*/
		/* 正しく計算できるようにしたもの
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0 * 1 + mx1 * 0 + mx2 * 0 + m03 * 0;
		m01 = mx0 * 0 + mx1 * c + mx2 * s + m03 * 0;
		m02 = mx0 * 0 + mx1 *-s + mx2 * c + m03 * 0;
		m03 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m03 * 1;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * 1 + mx1 * 0 + mx2 * 0 + m13 * 0;
		m11 = mx0 * 0 + mx1 * c + mx2 * s + m13 * 0;
		m12 = mx0 * 0 + mx1 *-s + mx2 * c + m13 * 0;
		m13 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m13 * 1;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * 1 + mx1 * 0 + mx2 * 0 + m23 * 0;
		m21 = mx0 * 0 + mx1 * c + mx2 * s + m23 * 0;
		m22 = mx0 * 0 + mx1 *-s + mx2 * c + m23 * 0;
		m23 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m23 * 1;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * 1 + mx1 * 0 + mx2 * 0 + m33 * 0;
		m31 = mx0 * 0 + mx1 * c + mx2 * s + m33 * 0;
		m32 = mx0 * 0 + mx1 *-s + mx2 * c + m33 * 0;
		m33 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m33 * 1;
		*/
		/* 単純に * 0 とかの無駄なところを切る
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0;
		m01 = mx1 * c + mx2 * s;
		m02 = mx1 *-s + mx2 * c;
		m03 = m03;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0;
		m11 = mx1 * c + mx2 * s;
		m12 = mx1 *-s + mx2 * c;
		m13 = m13;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0;
		m21 = mx1 * c + mx2 * s;
		m22 = mx1 *-s + mx2 * c;
		m23 = m23;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0;
		m31 = mx1 * c + mx2 * s;
		m32 = mx1 *-s + mx2 * c;
		m33 = m33;
		*/
		
		// 自分自身を代入しているところを切る
        lnFloat mx1 = m01;
		m01 = mx1 * c + m02 *-s;
		m02 = mx1 * s + m02 * c;

		mx1 = m11;
		m11 = mx1 * c + m12 *-s;
		m12 = mx1 * s + m12 * c;

		mx1 = m21;
		m21 = mx1 * c + m22 *-s;
		m22 = mx1 * s + m22 * c;

		mx1 = m31;
		m31 = mx1 * c + m32 *-s;
		m32 = mx1 * s + m32 * c;

        /* OpenGL
        lnFloat mx1 = m01;
		m01 = mx1 * c + m02 * s;
		m02 = mx1 *-s + m02 * c;

		mx1 = m11;
		m11 = mx1 * c + m12 * s;
		m12 = mx1 *-s + m12 * c;

		mx1 = m21;
		m21 = mx1 * c + m22 * s;
		m22 = mx1 *-s + m22 * c;

		mx1 = m31;
		m31 = mx1 * c + m32 * s;
		m32 = mx1 *-s + m32 * c;
        */
	}

	//----------------------------------------------------------------------
	// ● Y 軸を回転軸にして行列を回転する ( ラジアン単位 )
	//----------------------------------------------------------------------
	void Matrix::rotationY( lnFloat r_ )
	{
		lnFloat c, s;
        Asm::sincos( r_, &s, &c );

		/* 普通の行列計算
		m00, m01, m02, m03    c, 0, s, 0
		m10, m11, m12, m13    0, 1, 0, 0
		m20, m21, m22, m23 * -s, 0, c, 0
		m30, m31, m32, m33    0, 0, 0, 1
		*/
		/* 計算イメージ
		m00 = m00 * c + m01 * 0 + m02 *-s + m03 * 0;
		m01 = m00 * 0 + m01 * 1 + m02 * 0 + m03 * 0;
		m02 = m00 * s + m01 * 0 + m02 * c + m03 * 0;
		m03 = m00 * 0 + m01 * 0 + m02 * 0 + m03 * 1;

		m10 = m10 * c + m11 * 0 + m12 *-s + m13 * 0;
		m11 = m10 * 0 + m11 * 1 + m12 * 0 + m13 * 0;
		m12 = m10 * s + m11 * 0 + m12 * c + m13 * 0;
		m13 = m10 * 0 + m11 * 0 + m12 * 0 + m13 * 1;

		m20 = m20 * c + m21 * 0 + m22 *-s + m23 * 0;
		m21 = m20 * 0 + m21 * 1 + m22 * 0 + m23 * 0;
		m22 = m20 * s + m21 * 0 + m22 * c + m23 * 0;
		m23 = m20 * 0 + m21 * 0 + m22 * 0 + m23 * 1;

		m30 = m30 * c + m31 * 0 + m32 *-s + m33 * 0;
		m31 = m30 * 0 + m31 * 1 + m32 * 0 + m33 * 0;
		m32 = m30 * s + m31 * 0 + m32 * c + m33 * 0;
		m33 = m30 * 0 + m31 * 0 + m32 * 0 + m33 * 1;
		*/
		/* 正しく計算できるようにしたもの
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0 * c + mx1 * 0 + mx2 *-s + m03 * 0;
		m01 = mx0 * 0 + mx1 * 1 + mx2 * 0 + m03 * 0;
		m02 = mx0 * s + mx1 * 0 + mx2 * c + m03 * 0;
		m03 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m03 * 1;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * c + mx1 * 0 + mx2 *-s + m13 * 0;
		m11 = mx0 * 0 + mx1 * 1 + mx2 * 0 + m13 * 0;
		m12 = mx0 * s + mx1 * 0 + mx2 * c + m13 * 0;
		m13 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m13 * 1;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * c + mx1 * 0 + mx2 *-s + m23 * 0;
		m21 = mx0 * 0 + mx1 * 1 + mx2 * 0 + m23 * 0;
		m22 = mx0 * s + mx1 * 0 + mx2 * c + m23 * 0;
		m23 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m23 * 1;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * c + mx1 * 0 + mx2 *-s + m33 * 0;
		m31 = mx0 * 0 + mx1 * 1 + mx2 * 0 + m33 * 0;
		m32 = mx0 * s + mx1 * 0 + mx2 * c + m33 * 0;
		m33 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m33 * 1;
		*/
		/* 単純に * 0 とかの無駄なところを切る
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0 * c + mx2 *-s;
		m01 = mx1;
		m02 = mx0 * s + mx2 * c;
		m03 = m03;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * c + mx2 *-s;
		m11 = mx1;
		m12 = mx0 * s + mx2 * c;
		m13 = m13;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * c + mx2 *-s;
		m21 = mx1;
		m22 = mx0 * s + mx2 * c;
		m23 = mx3;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * c + mx2 *-s;
		m31 = mx1;
		m32 = mx0 * s + mx2 * c;
		m33 = m33;
		*/

		// 自分自身を代入しているところを切る
		lnFloat mx0 = m00;
		m00 = mx0 * c + m02 * s;
		m02 = mx0 *-s + m02 * c;

		mx0 = m10;
		m10 = mx0 * c + m12 * s;
		m12 = mx0 *-s + m12 * c;

		mx0 = m20;
		m20 = mx0 * c + m22 * s;
		m22 = mx0 *-s + m22 * c;

		mx0 = m30;
		m30 = mx0 * c + m32 * s;
		m32 = mx0 *-s + m32 * c;

        /* OpenGL
		lnFloat mx0 = m00;
		m00 = mx0 * c + m02 *-s;
		m02 = mx0 * s + m02 * c;

		mx0 = m10;
		m10 = mx0 * c + m12 *-s;
		m12 = mx0 * s + m12 * c;

		mx0 = m20;
		m20 = mx0 * c + m22 *-s;
		m22 = mx0 * s + m22 * c;

		mx0 = m30;
		m30 = mx0 * c + m32 *-s;
		m32 = mx0 * s + m32 * c;
        */
	}

	//----------------------------------------------------------------------
	// ● Z 軸を回転軸にして行列を回転する ( ラジアン単位 )
	//----------------------------------------------------------------------
	void Matrix::rotationZ( lnFloat r_ )
	{
		lnFloat c, s;
        Asm::sincos( r_, &s, &c );

		/* 基本の計算イメージ
		m00 = m00 * c + m01 * s + m02 * 0 + m03 * 0;
		m01 = m00 *-s + m01 * c + m02 * 0 + m03 * 0;
		m02 = m00 * 0 + m01 * 0 + m02 * 1 + m03 * 0;
		m03 = m00 * 0 + m01 * 0 + m02 * 0 + m03 * 1;

		m10 = m10 * c + m11 * s + m12 * 0 + m13 * 0;
		m11 = m10 *-s + m11 * c + m12 * 0 + m13 * 0;
		m12 = m10 * 0 + m11 * 0 + m12 * 1 + m13 * 0;
		m13 = m10 * 0 + m11 * 0 + m12 * 0 + m13 * 1;

		m20 = m20 * c + m21 * s + m22 * 0 + m23 * 0;
		m21 = m20 *-s + m21 * c + m22 * 0 + m23 * 0;
		m22 = m20 * 0 + m21 * 0 + m22 * 1 + m23 * 0;
		m23 = m20 * 0 + m21 * 0 + m22 * 0 + m23 * 1;

		m30 = m30 * c + m31 * s + m32 * 0 + m33 * 0;
		m31 = m30 *-s + m31 * c + m32 * 0 + m33 * 0;
		m32 = m30 * 0 + m31 * 0 + m32 * 1 + m33 * 0;
		m33 = m30 * 0 + m31 * 0 + m32 * 0 + m33 * 1;
		*/
		/* 正しく計算できるようにしたもの
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0 * c + mx1 * s + mx2 * 0 + m03 * 0;
		m01 = mx0 *-s + mx1 * c + mx2 * 0 + m03 * 0;
		m02 = mx0 * 0 + mx1 * 0 + mx2 * 1 + m03 * 0;
		m03 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m03 * 1;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * c + mx1 * s + mx2 * 0 + m13 * 0;
		m11 = mx0 *-s + mx1 * c + mx2 * 0 + m13 * 0;
		m12 = mx0 * 0 + mx1 * 0 + mx2 * 1 + m13 * 0;
		m13 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m13 * 1;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * c + mx1 * s + mx2 * 0 + m23 * 0;
		m21 = mx0 *-s + mx1 * c + mx2 * 0 + m23 * 0;
		m22 = mx0 * 0 + mx1 * 0 + mx2 * 1 + m23 * 0;
		m23 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m23 * 1;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * c + mx1 * s + mx2 * 0 + m33 * 0;
		m31 = mx0 *-s + mx1 * c + mx2 * 0 + m33 * 0;
		m32 = mx0 * 0 + mx1 * 0 + mx2 * 1 + m33 * 0;
		m33 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m33 * 1;
		*/
		/* 単純に * 0 とかの無駄なところを切る
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0 * c + mx1 * s;
		m01 = mx0 *-s + mx1 * c;
		m02 = mx2;
		m03 = m03;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * c + mx1 * s;
		m11 = mx0 *-s + mx1 * c;
		m12 = mx2;
		m13 = m13;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * c + mx1 * s;
		m21 = mx0 *-s + mx1 * c;
		m22 = mx2;
		m23 = m23;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * c + mx1 * s;
		m31 = mx0 *-s + mx1 * c;
		m32 = mx2;
		m33 = m33;
		*/
		// 自分自身を代入しているところを切る
        lnFloat mx0 = m00;
		m00 = mx0 * c + m01 *-s;
		m01 = mx0 * s + m01 * c;

		mx0 = m10;
		m10 = mx0 * c + m11 *-s;
		m11 = mx0 * s + m11 * c;

		mx0 = m20;
		m20 = mx0 * c + m21 *-s;
		m21 = mx0 * s + m21 * c;

		mx0 = m30;
		m30 = mx0 * c + m31 *-s;
		m31 = mx0 * s + m31 * c;
        

        // OpenGL
        /* 
		lnFloat mx0 = m00;
		m00 = mx0 * c + m01 * s;
		m01 = mx0 *-s + m01 * c;

		mx0 = m10;
		m10 = mx0 * c + m11 * s;
		m11 = mx0 *-s + m11 * c;

		mx0 = m20;
		m20 = mx0 * c + m21 * s;
		m21 = mx0 *-s + m21 * c;

		mx0 = m30;
		m30 = mx0 * c + m31 * s;
		m31 = mx0 *-s + m31 * c;
        */
	}

	//----------------------------------------------------------------------
	// ● 回転順序を指定して行列を回転する ( ラジアン単位 )
	//----------------------------------------------------------------------
	void Matrix::rotation( lnFloat x_, lnFloat y_, lnFloat z_, RotationOrder order_ )
	{
		switch ( order_ )
		{
			case RotationOrder_XYZ: 
				rotationX( x_ ); rotationY( y_ ); rotationZ( z_ ); break;
			case RotationOrder_XZY: 
				rotationX( x_ ); rotationZ( z_ ); rotationY( y_ ); break;
			case RotationOrder_YXZ: 
				rotationY( y_ ); rotationX( x_ ); rotationZ( z_ ); break;
			case RotationOrder_YZX: 
				rotationY( y_ ); rotationZ( z_ ); rotationX( x_ ); break;
			case RotationOrder_ZXY: 
				rotationZ( z_ ); rotationX( x_ ); rotationY( y_ ); break;
			case RotationOrder_ZYX: 
				rotationZ( z_ ); rotationY( y_ ); rotationX( x_ ); break;
		}
	}

	//----------------------------------------------------------------------
	// ● 行列を回転する ( ラジアン単位 )
	//----------------------------------------------------------------------
	void Matrix::rotation( const Vector3& vec_, RotationOrder order_ )
    {
        switch ( order_ )
		{
			case RotationOrder_XYZ: 
				rotationX( vec_.x ); rotationY( vec_.y ); rotationZ( vec_.z ); break;
			case RotationOrder_XZY: 
				rotationX( vec_.x ); rotationZ( vec_.z ); rotationY( vec_.y ); break;
			case RotationOrder_YXZ: 
				rotationY( vec_.y ); rotationX( vec_.x ); rotationZ( vec_.z ); break;
			case RotationOrder_YZX: 
				rotationY( vec_.y ); rotationZ( vec_.z ); rotationX( vec_.x ); break;
			case RotationOrder_ZXY: 
				rotationZ( vec_.z ); rotationX( vec_.x ); rotationY( vec_.y ); break;
			case RotationOrder_ZYX: 
				rotationZ( vec_.z ); rotationY( vec_.y ); rotationX( vec_.x ); break;
		}
    }

	//----------------------------------------------------------------------
	// ● 任意の軸を回転軸にして行列を回転する ( ラジアン単位 )
	//----------------------------------------------------------------------
	void Matrix::rotationAxis( const Vector3& axis_, lnFloat r_ )
    {
        //Vector3& axis = Vector3( axis_ );
        Vector3 axis = axis_;
        axis.normalize();

        lnFloat s, c;
        Asm::sincos( r_, &s, &c );
		lnFloat mc = 1.0f - c;

		/* 計算イメージ
		_00 = ( axis_.x * axis_.x ) * mc + c;
		_01 = ( axis_.x * axis_.y ) * mc + ( axis_.z * s );
		_02 = ( axis_.x * axis_.z ) * mc - ( axis_.y * s );
		_03 = 0;

		_10 = ( axis_.x * axis_.y ) * mc - ( axis_.z * s );
		_11 = ( axis_.y * axis_.y ) * mc + c;
		_12 = ( axis_.y * axis_.z ) * mc + ( axis_.x * s );
		_13 = 0;

		_20 = ( axis_.x * axis_.z ) * mc + ( axis_.y * s );
		_21 = ( axis_.y * axis_.z ) * mc - ( axis_.x * s );
		_22 = ( axis_.z * axis_.z ) * mc + c;
		_23 = 0;

		_30 = _31 = _32 = 0;
		_33 = 1;

		m00 = m00 * _00 + m01 * _10 + m02 * _20 + m03 * _30;
		m01 = m00 * _01 + m01 * _11 + m02 * _21 + m03 * _31;
		m02 = m00 * _02 + m01 * _12 + m02 * _22 + m03 * _32;
		m03 = m00 * _03 + m01 * _13 + m02 * _23 + m03 * _33;

		m10 = m10 * _00 + m11 * _10 + m12 * _20 + m13 * _30;
		m11 = m10 * _01 + m11 * _11 + m12 * _21 + m13 * _31;
		m12 = m10 * _02 + m11 * _12 + m12 * _22 + m13 * _32;
		m13 = m10 * _03 + m11 * _13 + m12 * _23 + m13 * _33;

		m20 = m20 * _00 + m21 * _10 + m22 * _20 + m23 * _30;
		m21 = m20 * _01 + m21 * _11 + m22 * _21 + m23 * _31;
		m22 = m20 * _02 + m21 * _12 + m22 * _22 + m23 * _32;
		m23 = m20 * _03 + m21 * _13 + m22 * _23 + m23 * _33;

		m30 = m30 * _00 + m31 * _10 + m32 * _20 + m33 * _30;
		m31 = m30 * _01 + m31 * _11 + m32 * _21 + m33 * _31;
		m32 = m30 * _02 + m31 * _12 + m32 * _22 + m33 * _32;
		m33 = m30 * _03 + m31 * _13 + m32 * _23 + m33 * _33;
		*/

		/* 正しく計算できるようにしたもの
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0 * _00 + mx1 * _10 + mx2 * _20 + m03 * _30;
		m01 = mx0 * _01 + mx1 * _11 + mx2 * _21 + m03 * _31;
		m02 = mx0 * _02 + mx1 * _12 + mx2 * _22 + m03 * _32;
		m03 = mx0 * _03 + mx1 * _13 + mx2 * _23 + m03 * _33;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * _00 + mx1 * _10 + mx2 * _20 + m13 * _30;
		m11 = mx0 * _01 + mx1 * _11 + mx2 * _21 + m13 * _31;
		m12 = mx0 * _02 + mx1 * _12 + mx2 * _22 + m13 * _32;
		m13 = mx0 * _03 + mx1 * _13 + mx2 * _23 + m13 * _33;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * _00 + mx1 * _10 + mx2 * _20 + m23 * _30;
		m21 = mx0 * _01 + mx1 * _11 + mx2 * _21 + m23 * _31;
		m22 = mx0 * _02 + mx1 * _12 + mx2 * _22 + m23 * _32;
		m23 = mx0 * _03 + mx1 * _13 + mx2 * _23 + m23 * _33;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * _00 + mx1 * _10 + mx2 * _20 + m33 * _30;
		m31 = mx0 * _01 + mx1 * _11 + mx2 * _21 + m33 * _31;
		m32 = mx0 * _02 + mx1 * _12 + mx2 * _22 + m33 * _32;
		m33 = mx0 * _03 + mx1 * _13 + mx2 * _23 + m33 * _33;
		*/

		/* 0 を乗算してるところと、*1 で自分自身代入しているところを切る
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0 * _00 + mx1 * _10 + mx2 * _20;
		m01 = mx0 * _01 + mx1 * _11 + mx2 * _21;
		m02 = mx0 * _02 + mx1 * _12 + mx2 * _22;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * _00 + mx1 * _10 + mx2 * _20;
		m11 = mx0 * _01 + mx1 * _11 + mx2 * _21;
		m12 = mx0 * _02 + mx1 * _12 + mx2 * _22;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * _00 + mx1 * _10 + mx2 * _20;
		m21 = mx0 * _01 + mx1 * _11 + mx2 * _21;
		m22 = mx0 * _02 + mx1 * _12 + mx2 * _22;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * _00 + mx1 * _10 + mx2 * _20;
		m31 = mx0 * _01 + mx1 * _11 + mx2 * _21;
		m32 = mx0 * _02 + mx1 * _12 + mx2 * _22;
		*/

		lnFloat _00 = ( axis.x * axis.x ) * mc + c;
		lnFloat _01 = ( axis.x * axis.y ) * mc + ( axis.z * s );
		lnFloat _02 = ( axis.x * axis.z ) * mc - ( axis.y * s );

		lnFloat _10 = ( axis.x * axis.y ) * mc - ( axis.z * s );
		lnFloat _11 = ( axis.y * axis.y ) * mc + c;
		lnFloat _12 = ( axis.y * axis.z ) * mc + ( axis.x * s );

		lnFloat _20 = ( axis.x * axis.z ) * mc + ( axis.y * s );
		lnFloat _21 = ( axis.y * axis.z ) * mc - ( axis.x * s );
		lnFloat _22 = ( axis.z * axis.z ) * mc + c;

		lnFloat mx0 = m00;
		lnFloat mx1 = m01;
		m00 = mx0 * _00 + mx1 * _10 + m02 * _20;
		m01 = mx0 * _01 + mx1 * _11 + m02 * _21;
		m02 = mx0 * _02 + mx1 * _12 + m02 * _22;

		mx0 = m10;
		mx1 = m11;
		m10 = mx0 * _00 + mx1 * _10 + m12 * _20;
		m11 = mx0 * _01 + mx1 * _11 + m12 * _21;
		m12 = mx0 * _02 + mx1 * _12 + m12 * _22;

		mx0 = m20;
		mx1 = m21;
		m20 = mx0 * _00 + mx1 * _10 + m22 * _20;
		m21 = mx0 * _01 + mx1 * _11 + m22 * _21;
		m22 = mx0 * _02 + mx1 * _12 + m22 * _22;

		mx0 = m30;
		mx1 = m31;
		m30 = mx0 * _00 + mx1 * _10 + m32 * _20;
		m31 = mx0 * _01 + mx1 * _11 + m32 * _21;
		m32 = mx0 * _02 + mx1 * _12 + m32 * _22;
    }

	//----------------------------------------------------------------------
	// ● 任意の軸を回転軸にして行列を回転する ( ラジアン単位 )
	//----------------------------------------------------------------------
	void Matrix::rotationAxis( const Vector3& axis_, lnFloat r_, const Vector3& def_axis_ )
    {
        rotationAxis( ( axis_.isZero() ) ? def_axis_ : axis_, r_ );
    }
    

	//----------------------------------------------------------------------
	// ●  X 軸を回転軸にして行列を回転する ( インデックス角度 )
	//----------------------------------------------------------------------
	void Matrix::rotationXIdx( int r_ )
    {
        lnFloat s = LMath::sinIdx( r_ );
        lnFloat c = LMath::cosIdx( r_ );

        lnFloat mx1 = m01;
		m01 = mx1 * c + m02 *-s;
		m02 = mx1 * s + m02 * c;

		mx1 = m11;
		m11 = mx1 * c + m12 *-s;
		m12 = mx1 * s + m12 * c;

		mx1 = m21;
		m21 = mx1 * c + m22 *-s;
		m22 = mx1 * s + m22 * c;

		mx1 = m31;
		m31 = mx1 * c + m32 *-s;
		m32 = mx1 * s + m32 * c;
    }

	//----------------------------------------------------------------------
	// ● Y 軸を回転軸にして行列を回転する ( インデックス角度 )
	//----------------------------------------------------------------------
	void Matrix::rotationYIdx( int r_ )
    {
        lnFloat s = LMath::sinIdx( r_ );
        lnFloat c = LMath::cosIdx( r_ );

		lnFloat mx0 = m00;
		m00 = mx0 * c + m02 * s;
		m02 = mx0 *-s + m02 * c;

		mx0 = m10;
		m10 = mx0 * c + m12 * s;
		m12 = mx0 *-s + m12 * c;

		mx0 = m20;
		m20 = mx0 * c + m22 * s;
		m22 = mx0 *-s + m22 * c;

		mx0 = m30;
		m30 = mx0 * c + m32 * s;
		m32 = mx0 *-s + m32 * c;
    }

	//----------------------------------------------------------------------
	// ● Z 軸を回転軸にして行列を回転する ( インデックス角度 )
	//----------------------------------------------------------------------
	void Matrix::rotationZIdx( int r_ )
    {
        lnFloat s = LMath::sinIdx( r_ );
        lnFloat c = LMath::cosIdx( r_ );

        lnFloat mx0 = m00;
		m00 = mx0 * c + m01 *-s;
		m01 = mx0 * s + m01 * c;

		mx0 = m10;
		m10 = mx0 * c + m11 *-s;
		m11 = mx0 * s + m11 * c;

		mx0 = m20;
		m20 = mx0 * c + m21 *-s;
		m21 = mx0 * s + m21 * c;

		mx0 = m30;
		m30 = mx0 * c + m31 *-s;
		m31 = mx0 * s + m31 * c;
    }

	//----------------------------------------------------------------------
	// ● 行列を回転する ( インデックス角度 )
	//----------------------------------------------------------------------
	void Matrix::rotationIdx( int x_, int y_, int z_, RotationOrder order_ )
    {
        switch ( order_ )
		{
			case RotationOrder_XYZ: 
				rotationXIdx( x_ ); rotationYIdx( y_ ); rotationZIdx( z_ ); break;
			case RotationOrder_XZY: 
				rotationXIdx( x_ ); rotationZIdx( z_ ); rotationYIdx( y_ ); break;
			case RotationOrder_YXZ: 
				rotationYIdx( y_ ); rotationXIdx( x_ ); rotationZIdx( z_ ); break;
			case RotationOrder_YZX: 
				rotationYIdx( y_ ); rotationZIdx( z_ ); rotationXIdx( x_ ); break;
			case RotationOrder_ZXY: 
				rotationZIdx( z_ ); rotationXIdx( x_ ); rotationYIdx( y_ ); break;
			case RotationOrder_ZYX: 
				rotationZIdx( z_ ); rotationYIdx( y_ ); rotationXIdx( x_ ); break;
		}
    }

	//----------------------------------------------------------------------
	// ● 任意の軸を回転軸にして行列を回転する ( インデックス角度 )
	//----------------------------------------------------------------------
	void Matrix::rotationAxisIdx( const Vector3& axis_, int r_ )
    {
        Vector3 axis = axis_;
        axis.normalize();

        lnFloat s = LMath::sinIdx( r_ );
        lnFloat c = LMath::cosIdx( r_ );
		lnFloat mc = 1.0f - c;

        lnFloat _00 = ( axis.x * axis.x ) * mc + c;
		lnFloat _01 = ( axis.x * axis.y ) * mc + ( axis.z * s );
		lnFloat _02 = ( axis.x * axis.z ) * mc - ( axis.y * s );

		lnFloat _10 = ( axis.x * axis.y ) * mc - ( axis.z * s );
		lnFloat _11 = ( axis.y * axis.y ) * mc + c;
		lnFloat _12 = ( axis.y * axis.z ) * mc + ( axis.x * s );

		lnFloat _20 = ( axis.x * axis.z ) * mc + ( axis.y * s );
		lnFloat _21 = ( axis.y * axis.z ) * mc - ( axis.x * s );
		lnFloat _22 = ( axis.z * axis.z ) * mc + c;

		lnFloat mx0 = m00;
		lnFloat mx1 = m01;
		m00 = mx0 * _00 + mx1 * _10 + m02 * _20;
		m01 = mx0 * _01 + mx1 * _11 + m02 * _21;
		m02 = mx0 * _02 + mx1 * _12 + m02 * _22;

		mx0 = m10;
		mx1 = m11;
		m10 = mx0 * _00 + mx1 * _10 + m12 * _20;
		m11 = mx0 * _01 + mx1 * _11 + m12 * _21;
		m12 = mx0 * _02 + mx1 * _12 + m12 * _22;

		mx0 = m20;
		mx1 = m21;
		m20 = mx0 * _00 + mx1 * _10 + m22 * _20;
		m21 = mx0 * _01 + mx1 * _11 + m22 * _21;
		m22 = mx0 * _02 + mx1 * _12 + m22 * _22;

		mx0 = m30;
		mx1 = m31;
		m30 = mx0 * _00 + mx1 * _10 + m32 * _20;
		m31 = mx0 * _01 + mx1 * _11 + m32 * _21;
		m32 = mx0 * _02 + mx1 * _12 + m32 * _22;
    }

	//----------------------------------------------------------------------
	// ● 任意の軸を回転軸にして行列を回転する ( インデックス角度 )
	//----------------------------------------------------------------------
	void Matrix::rotationAxisIdx( const Vector3& axis_, int r_, const Vector3& def_axis_ )
    {
        rotationAxisIdx( ( axis_.isZero() ) ? def_axis_ : axis_, r_ );
    }

	//----------------------------------------------------------------------
	// ● クォータニオンを使って行列を回転する
	//----------------------------------------------------------------------
    void Matrix::rotationQuaternion( const Quaternion& qua_ )
    {
        Matrix t;
        Matrix::rotationQuaternion( &t, qua_ );
        (*this) *= t;
    }

	//----------------------------------------------------------------------
	// ● 行列をスケーリングする
	//----------------------------------------------------------------------
	void Matrix::scaling( lnFloat x_, lnFloat y_, lnFloat z_ )
	{
		m00 *= x_;
		m01 *= y_;
		m02 *= z_;
		m10 *= x_;
		m11 *= y_;
		m12 *= z_;
		m20 *= x_;
		m21 *= y_;
		m22 *= z_;
        m30 *= x_;
		m31 *= y_;
		m32 *= z_;
	}
	void Matrix::scaling( const Vector3& vec_ )
	{
		m00 *= vec_.x;
		m01 *= vec_.y;
		m02 *= vec_.z;
		m10 *= vec_.x;
		m11 *= vec_.y;
		m12 *= vec_.z;
		m20 *= vec_.x;
		m21 *= vec_.y;
		m22 *= vec_.z;
        m30 *= vec_.x;
		m31 *= vec_.y;
		m32 *= vec_.z;
	}
	void Matrix::scaling( lnFloat xyz_ )
	{
		m00 *= xyz_;
		m01 *= xyz_;
		m02 *= xyz_;
		m10 *= xyz_;
		m11 *= xyz_;
		m12 *= xyz_;
		m20 *= xyz_;
		m21 *= xyz_;
		m22 *= xyz_;
        m30 *= xyz_;
		m31 *= xyz_;
		m32 *= xyz_;
	}

	//----------------------------------------------------------------------
	// ● 逆行列化する
	//----------------------------------------------------------------------
	void Matrix::inverse()
    {
#if 1
		Matrix tmp = *this;
		inverse( this, tmp );
#else
        // http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/tech23.html

		float m0011 = this->m00 * this->m11;
		float m0012 = this->m00 * this->m12;
		float m0021 = this->m00 * this->m21;
		float m0022 = this->m00 * this->m22;

		float m0110 = this->m01 * this->m10;
		float m0112 = this->m01 * this->m12;
		float m0120 = this->m01 * this->m20;
		float m0122 = this->m01 * this->m22;

		float m0210 = this->m02 * this->m10;
		float m0211 = this->m02 * this->m11;
		float m0220 = this->m02 * this->m20;
		float m0221 = this->m02 * this->m21;

		float m1021 = this->m10 * this->m21;
		float m1022 = this->m10 * this->m22;

		float m1120 = this->m11 * this->m20;
		float m1122 = this->m11 * this->m22;

		float m1220 = this->m12 * this->m20;
		float m1221 = this->m12 * this->m21;


		float m1122_m1221 = m1122 - m1221;
		float m1220_m1022 = m1220 - m1022;
		float m1021_m1120 = m1021 - m1120;

		
		float delta = this->m00*( m1122_m1221 ) + this->m01*( m1220_m1022 ) + this->m02*( m1021_m1120 );
		float rcpDelta = 1.f / delta;


		m00 = ( m1122_m1221 ) * rcpDelta;
		m10 = ( m1220_m1022 ) * rcpDelta;
		m20 = ( m1021_m1120 ) * rcpDelta;
		m01 = ( m0221-m0122 ) * rcpDelta;
		m11 = ( m0022-m0220 ) * rcpDelta;
		m21 = ( m0120-m0021 ) * rcpDelta;
		m02 = ( m0112-m0211 ) * rcpDelta;
		m12 = ( m0210-m0012 ) * rcpDelta;
		m22 = ( m0011-m0110 ) * rcpDelta;

		float t03 = this->m03;
		float t13 = this->m13;
		m03 = -( m00 * t03 + m01 * t13 + m02 * this->m23 );
		m13 = -( m10 * t03 + m11 * t13 + m12 * this->m23 );
		m23 = -( m20 * t03 + m21 * t13 + m22 * this->m23 );
#endif
    }

	//----------------------------------------------------------------------
	// ● 転置行列化する
	//----------------------------------------------------------------------
	void Matrix::transpose()
    {
        lnFloat t;

        t = m01;
        m01 = m10;
        m10 = t;

        t = m02;
        m02 = m20;
        m20 = t;

        t = m03;
        m03 = m30;
        m30 = t;
        
        t = m12;
        m12 = m21;
        m21 = t;

        t = m13;
        m13 = m31;
        m31 = t;
        
        t = m23;
        m23 = m32;
        m32 = t;
    }

	//----------------------------------------------------------------------
	// ● 行列の 3x3 部分だけを乗算して設定する
	//----------------------------------------------------------------------
    void Matrix::setMul3x3( const Matrix& matrix_ )
    {
        lnFloat mx0 = m00;
		lnFloat mx1 = m01;
		lnFloat mx2 = m02;
		m00 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m03 * matrix_.m30;
		m01 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m03 * matrix_.m31;
		m02 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m03 * matrix_.m32;
        mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m13 * matrix_.m30;
		m11 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m13 * matrix_.m31;
		m12 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m13 * matrix_.m32;
		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m23 * matrix_.m30;
		m21 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m23 * matrix_.m31;
		m22 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m23 * matrix_.m32;
        m03 = m13 = m23 = m30 = m31 = m32 = 0.0f;
        m33 = 1.0f;
    }

    void Matrix::setMul4x3( const Matrix& matrix_ )
    {
        lnFloat mx0 = m00;
		lnFloat mx1 = m01;
		lnFloat mx2 = m02;

		m00 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m03 * matrix_.m30;
		m01 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m03 * matrix_.m31;
		m02 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m03 * matrix_.m32;
        mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m13 * matrix_.m30;
		m11 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m13 * matrix_.m31;
		m12 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m13 * matrix_.m32;
		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m23 * matrix_.m30;
		m21 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m23 * matrix_.m31;
		m22 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m23 * matrix_.m32;
        mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m33 * matrix_.m30;
		m31 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m33 * matrix_.m31;
		m32 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m33 * matrix_.m32;

        m03 = m13 = m23 = m30 = m31 = m32 = 0.0f;
        m33 = 1.0f;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Matrix::containsNaN() const
	{
		return 
			LMath::isNaN( m00 ) || LMath::isNaN( m01 ) || LMath::isNaN( m02 ) || LMath::isNaN( m03 ) ||
			LMath::isNaN( m10 ) || LMath::isNaN( m11 ) || LMath::isNaN( m12 ) || LMath::isNaN( m13 ) ||
			LMath::isNaN( m20 ) || LMath::isNaN( m21 ) || LMath::isNaN( m22 ) || LMath::isNaN( m23 ) ||
			LMath::isNaN( m30 ) || LMath::isNaN( m31 ) || LMath::isNaN( m32 ) || LMath::isNaN( m33 );
	}

	//----------------------------------------------------------------------
	// ● operator
	//----------------------------------------------------------------------
    Matrix& Matrix::operator = ( const SQTTransform t_ )
    {
        //this->scaling( t_.Scale );
        Matrix::scaling( this, t_.Scale );
        this->rotationQuaternion( t_.Rotation );
        this->translation( t_.Translation );
        return *this;
    }

	void Matrix::operator *= ( const Matrix& matrix_ )
	{
		lnFloat mx0 = m00;
		lnFloat mx1 = m01;
		lnFloat mx2 = m02;
		m00 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m03 * matrix_.m30;
		m01 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m03 * matrix_.m31;
		m02 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m03 * matrix_.m32;
		m03 = mx0 * matrix_.m03 + mx1 * matrix_.m13 + mx2 * matrix_.m23 + m03 * matrix_.m33;
		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m13 * matrix_.m30;
		m11 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m13 * matrix_.m31;
		m12 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m13 * matrix_.m32;
		m13 = mx0 * matrix_.m03 + mx1 * matrix_.m13 + mx2 * matrix_.m23 + m13 * matrix_.m33;
		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m23 * matrix_.m30;
		m21 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m23 * matrix_.m31;
		m22 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m23 * matrix_.m32;
		m23 = mx0 * matrix_.m03 + mx1 * matrix_.m13 + mx2 * matrix_.m23 + m23 * matrix_.m33;
		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m33 * matrix_.m30;
		m31 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m33 * matrix_.m31;
		m32 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m33 * matrix_.m32;
		m33 = mx0 * matrix_.m03 + mx1 * matrix_.m13 + mx2 * matrix_.m23 + m33 * matrix_.m33;
	}

	Matrix Matrix::operator * ( const Matrix& matrix_ ) const
	{
		Matrix out;
		out.m00 = m00 * matrix_.m00 + m01 * matrix_.m10 + m02 * matrix_.m20 + m03 * matrix_.m30;
		out.m01 = m00 * matrix_.m01 + m01 * matrix_.m11 + m02 * matrix_.m21 + m03 * matrix_.m31;
		out.m02 = m00 * matrix_.m02 + m01 * matrix_.m12 + m02 * matrix_.m22 + m03 * matrix_.m32;
		out.m03 = m00 * matrix_.m03 + m01 * matrix_.m13 + m02 * matrix_.m23 + m03 * matrix_.m33;

		out.m10 = m10 * matrix_.m00 + m11 * matrix_.m10 + m12 * matrix_.m20 + m13 * matrix_.m30;
		out.m11 = m10 * matrix_.m01 + m11 * matrix_.m11 + m12 * matrix_.m21 + m13 * matrix_.m31;
		out.m12 = m10 * matrix_.m02 + m11 * matrix_.m12 + m12 * matrix_.m22 + m13 * matrix_.m32;
		out.m13 = m10 * matrix_.m03 + m11 * matrix_.m13 + m12 * matrix_.m23 + m13 * matrix_.m33;

		out.m20 = m20 * matrix_.m00 + m21 * matrix_.m10 + m22 * matrix_.m20 + m23 * matrix_.m30;
		out.m21 = m20 * matrix_.m01 + m21 * matrix_.m11 + m22 * matrix_.m21 + m23 * matrix_.m31;
		out.m22 = m20 * matrix_.m02 + m21 * matrix_.m12 + m22 * matrix_.m22 + m23 * matrix_.m32;
		out.m23 = m20 * matrix_.m03 + m21 * matrix_.m13 + m22 * matrix_.m23 + m23 * matrix_.m33;

		out.m30 = m30 * matrix_.m00 + m31 * matrix_.m10 + m32 * matrix_.m20 + m33 * matrix_.m30;
		out.m31 = m30 * matrix_.m01 + m31 * matrix_.m11 + m32 * matrix_.m21 + m33 * matrix_.m31;
		out.m32 = m30 * matrix_.m02 + m31 * matrix_.m12 + m32 * matrix_.m22 + m33 * matrix_.m32;
		out.m33 = m30 * matrix_.m03 + m31 * matrix_.m13 + m32 * matrix_.m23 + m33 * matrix_.m33;
		return out;
	}

	//----------------------------------------------------------------------
	// ● コンソールに出力する
	//----------------------------------------------------------------------
	void Matrix::cdump( const lnChar* str_, int indent_ ) const
	{
		if ( str_ )
		{
			printf( "%s\n", str_ );
		}
		else
		{
			printf( "◆ Matrix\n" );
		}
		int i;
		for ( i = 0; i < indent_; ++i ) { 
			printf( " " ); }
		printf( "[ %f, %f, %f, %f ]\n", m00, m01, m02, m03 );
		for ( i = 0; i < indent_; ++i ) { 
			printf( " " ); }
		printf( "[ %f, %f, %f, %f ]\n", m10, m11, m12, m13 );
		for ( i = 0; i < indent_; ++i ) { 
			printf( " " ); }
		printf( "[ %f, %f, %f, %f ]\n", m20, m21, m22, m23 );
		for ( i = 0; i < indent_; ++i ) { 
			printf( " " ); }
		printf( "[ %f, %f, %f, %f ]\n", m30, m31, m32, m33 );
	}


	//--------------------------------------------------------------------------
	// ▼ static

	//----------------------------------------------------------------------
	// ● 単位行列を作成する
	//----------------------------------------------------------------------
	void Matrix::identity( Matrix* mat_ )
	{
        mat_->m00 = mat_->m11 = mat_->m22 = mat_->m33 = 1.0f;
		mat_->m01 = mat_->m02 = mat_->m03 = 0.0f;
		mat_->m10 = mat_->m12 = mat_->m13 = 0.0f;
		mat_->m20 = mat_->m21 = mat_->m23 = 0.0f;
		mat_->m30 = mat_->m31 = mat_->m32 = 0.0f;
	}

	//----------------------------------------------------------------------
	// ● 平行移動行列を作成する
	//----------------------------------------------------------------------
	void Matrix::translation( Matrix* mat_, lnFloat x_, lnFloat y_, lnFloat z_ )
	{
        mat_->set(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			x_, y_, z_, 1.0f );
	}

	//----------------------------------------------------------------------
	// ● 平行移動行列を作成する
	//----------------------------------------------------------------------
	void Matrix::translation( Matrix* mat_, const Vector3& vec_ )
	{
		mat_->set(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			vec_.x, vec_.y, vec_.z, 1.0f );
	}

	//----------------------------------------------------------------------
	// ● X 軸を回転軸にして回転する行列を作成する ( ラジアン角度 )
	//----------------------------------------------------------------------
	void Matrix::rotationX( Matrix* mat_, lnFloat r_ )
	{
		lnFloat c, s;
        Asm::sincos( r_, &s, &c );
		mat_->set(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f,    c,    s, 0.0f,
			0.0f,   -s,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );

        /* OpenGL
        mat_->set(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f,    c,   -s, 0.0f,
			0.0f,    s,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        */
	}

	//----------------------------------------------------------------------
	// ● Y 軸を回転軸にして回転する行列を作成する ( ラジアン角度 )
	//----------------------------------------------------------------------
	void Matrix::rotationY( Matrix* mat_, lnFloat r_ )
	{
		lnFloat c, s;
        Asm::sincos( r_, &s, &c );
        mat_->set(
			   c, 0.0f,   -s, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			   s, 0.0f,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        /* OpenGL
		mat_->set(
			   c, 0.0f,    s, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			  -s, 0.0f,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        */
	}

	//----------------------------------------------------------------------
	// ● Z 軸を回転軸にして回転する行列を作成する ( ラジアン角度 )
	//----------------------------------------------------------------------
	void Matrix::rotationZ( Matrix* mat_, lnFloat r_ )
	{
		lnFloat c, s;
        Asm::sincos( r_, &s, &c );
        mat_->set(
			   c,    s, 0.0f, 0.0f,
			  -s,    c, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        /* OpenGL
		mat_->set(
			   c,   -s, 0.0f, 0.0f,
			   s,    c, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        */
	}



	//----------------------------------------------------------------------
	// ● 回転行列を作成する ( ラジアン角度 )
	//----------------------------------------------------------------------
	void Matrix::rotation( Matrix* mat_, lnFloat x_, lnFloat y_, lnFloat z_, RotationOrder order_ )
	{
        mat_->identity();
		switch ( order_ )
		{
            default:              
            	mat_->rotationX( x_ ); mat_->rotationY( y_ ); mat_->rotationZ( z_ ); break;
			case RotationOrder_XZY: 
				mat_->rotationX( x_ ); mat_->rotationZ( z_ ); mat_->rotationY( y_ ); break;
			case RotationOrder_YXZ: 
				mat_->rotationY( y_ ); mat_->rotationX( x_ ); mat_->rotationZ( z_ ); break;
			case RotationOrder_YZX: 
				mat_->rotationY( y_ ); mat_->rotationZ( z_ ); mat_->rotationX( x_ ); break;
			case RotationOrder_ZXY: 
				mat_->rotationZ( z_ ); mat_->rotationX( x_ ); mat_->rotationY( y_ ); break;
			case RotationOrder_ZYX: 
				mat_->rotationZ( z_ ); mat_->rotationY( y_ ); mat_->rotationX( x_ ); break;
		}
	}

	//----------------------------------------------------------------------
	// ● 任意の軸を回転軸にして回転する行列を作成する ( ラジアン角度 )
	//----------------------------------------------------------------------
	void Matrix::rotationAxis( Matrix* mat_, const Vector3& axis_, lnFloat r_ )
	{
		// http://marupeke296.com/DXG_No58_RotQuaternionTrans.html
		// http://homepage2.nifty.com/skimp-studio/htm/crawl/1_7_transform2.htm

        Vector3 axis = axis_;
        axis.normalize();

		lnFloat s, c;
        Asm::sincos( r_, &s, &c );
		lnFloat mc = 1.0f - c;

        mat_->set(
			( axis.x * axis.x ) * mc + c,			    ( axis.x * axis.y ) * mc + ( axis.z * s ),	( axis.x * axis.z ) * mc - ( axis.y * s ),	0.0f,
			( axis.x * axis.y ) * mc - ( axis.z * s ),	( axis.y * axis.y ) * mc + c,			    ( axis.y * axis.z ) * mc + ( axis.x * s ),	0.0f,
			( axis.x * axis.z ) * mc + ( axis.y * s ),	( axis.y * axis.z ) * mc - ( axis.x * s ),	( axis.z * axis.z ) * mc + c,			    0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
	}

	//----------------------------------------------------------------------
	// ● 任意の軸を回転軸にして回転する行列を作成する ( ラジアン角度 )
	//----------------------------------------------------------------------
	//void Matrix::rotationAxis( Matrix* mat_, const Vector3& axis_, lnFloat r_, const Vector3& def_axis_ )
	//{
 //       Matrix::rotationAxis( mat_, ( axis_.isZero() ) ? def_axis_ : axis_, r_ );
 //   }

	//----------------------------------------------------------------------
	// ● X 軸を回転軸にして回転する行列を作成する ( インデックス角度 )
	//----------------------------------------------------------------------
	void Matrix::rotationXIdx( Matrix* mat_, int r_ )
	{
        lnFloat c = LMath::cosIdx( r_ );
		lnFloat s = LMath::sinIdx( r_ );
		mat_->set(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f,    c,    s, 0.0f,
			0.0f,   -s,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );

        /* OpenGL
        mat_->set(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f,    c,   -s, 0.0f,
			0.0f,    s,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        */
	}

	//----------------------------------------------------------------------
	// ● Y 軸を回転軸にして回転する行列を作成する ( インデックス角度 )
	//----------------------------------------------------------------------
	void Matrix::rotationYIdx( Matrix* mat_, int r_ )
	{
		lnFloat c = LMath::cosIdx( r_ );
		lnFloat s = LMath::sinIdx( r_ );
		mat_->set(
			   c, 0.0f,   -s, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			   s, 0.0f,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        /* OpenGL
		mat_->set(
			   c, 0.0f,    s, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			  -s, 0.0f,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        */
	}

	//----------------------------------------------------------------------
	// ● Z 軸を回転軸にして回転する行列を作成する ( インデックス角度 )
	//----------------------------------------------------------------------
	void Matrix::rotationZIdx( Matrix* mat_, int r_ )
	{
		lnFloat c = LMath::cosIdx( r_ );
		lnFloat s = LMath::sinIdx( r_ );
		mat_->set(
			   c,    s, 0.0f, 0.0f,
			  -s,    c, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        /* OpenGL
		mat_->set(
			   c,   -s, 0.0f, 0.0f,
			   s,    c, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        */
	}

	//----------------------------------------------------------------------
	// ● 回転行列を作成する ( インデックス角度 )
	//----------------------------------------------------------------------
	void Matrix::rotationIdx( Matrix* mat_, int x_, int y_, int z_, RotationOrder order_ )
	{
		mat_->identity();
		switch ( order_ )
		{
            default:              
            	mat_->rotationXIdx( x_ ); mat_->rotationYIdx( y_ ); mat_->rotationZIdx( z_ ); break;
			case RotationOrder_XZY: 
				mat_->rotationXIdx( x_ ); mat_->rotationZIdx( z_ ); mat_->rotationYIdx( y_ ); break;
			case RotationOrder_YXZ: 
				mat_->rotationYIdx( y_ ); mat_->rotationXIdx( x_ ); mat_->rotationZIdx( z_ ); break;
			case RotationOrder_YZX: 
				mat_->rotationYIdx( y_ ); mat_->rotationZIdx( z_ ); mat_->rotationXIdx( x_ ); break;
			case RotationOrder_ZXY: 
				mat_->rotationZIdx( z_ ); mat_->rotationXIdx( x_ ); mat_->rotationYIdx( y_ ); break;
			case RotationOrder_ZYX: 
				mat_->rotationZIdx( z_ ); mat_->rotationYIdx( y_ ); mat_->rotationXIdx( x_ ); break;
		}
	}

	//----------------------------------------------------------------------
	// ● 任意の軸を回転軸にして回転する行列を作成する ( インデックス角度 )
	//----------------------------------------------------------------------
	void Matrix::rotationAxisIdx( Matrix* mat_, const Vector3& axis_, int r_ )
	{
        Vector3 axis = axis_;
        axis.normalize();

		lnFloat s = LMath::sinIdx( r_ );
		lnFloat c = LMath::cosIdx( r_ );
		lnFloat mc = 1.0f - LMath::cosIdx( r_ );
		mat_->set(
			( axis.x * axis.x ) * mc + c,			    ( axis.x * axis.y ) * mc + ( axis.z * s ),	( axis.x * axis.z ) * mc - ( axis.y * s ),	0.0f,
			( axis.x * axis.y ) * mc - ( axis.z * s ),	( axis.y * axis.y ) * mc + c,			    ( axis.y * axis.z ) * mc + ( axis.x * s ),	0.0f,
			( axis.x * axis.z ) * mc + ( axis.y * s ),	( axis.y * axis.z ) * mc - ( axis.x * s ),	( axis.z * axis.z ) * mc + c,			    0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
	}

	//----------------------------------------------------------------------
	// ● 任意の軸を回転軸にして回転する行列を作成する ( インデックス角度 )
	//----------------------------------------------------------------------
	void Matrix::rotationAxisIdx( Matrix* mat_, const Vector3& axis_, int r_, const Vector3& def_axis_ )
    {
        Matrix::rotationAxisIdx( mat_, ( axis_.isZero() ) ? def_axis_ : axis_, r_ );
    }

	//----------------------------------------------------------------------
	// ● クォータニオンから回転行列を作成する
	//----------------------------------------------------------------------
    void Matrix::rotationQuaternion( Matrix* outMat, const Quaternion& q )
    {
		float xx = q.x * q.x;
		float yy = q.y * q.y;
		float zz = q.z * q.z;
		float xy = q.x * q.y;
		float zw = q.z * q.w;
		float zx = q.z * q.x;
		float yw = q.y * q.w;
		float yz = q.y * q.z;
		float xw = q.x * q.w;
		outMat->_11 = 1.0f - (2.0f * (yy + zz));
		outMat->_12 = 2.0f * (xy + zw);
		outMat->_13 = 2.0f * (zx - yw);
		outMat->_14 = 0.0f;
		outMat->_21 = 2.0f * (xy - zw);
		outMat->_22 = 1.0f - (2.0f * (zz + xx));
		outMat->_23 = 2.0f * (yz + xw);
		outMat->_24 = 0.0f;
		outMat->_31 = 2.0f * (zx + yw);
		outMat->_32 = 2.0f * (yz - xw);
		outMat->_33 = 1.0f - (2.0f * (yy + xx));
		outMat->_34 = 0.0f;
		outMat->_41 = 0.0f;
		outMat->_42 = 0.0f;
		outMat->_43 = 0.0f;
		outMat->_44 = 1.0f;

        //out_->m00 = 1.0f - 2.0f * qua_.y * qua_.y - 2.0f * qua_.z * qua_.z;
        //out_->m01 = 2.0f * qua_.x * qua_.y + 2.0f * qua_.w * qua_.z;
        //out_->m02 = 2.0f * qua_.x * qua_.z - 2.0f * qua_.w * qua_.y;

        //out_->m10 = 2.0f * qua_.x * qua_.y - 2.0f * qua_.w * qua_.z;
        //out_->m11 = 1.0f - 2.0f * qua_.x * qua_.x - 2.0f * qua_.z * qua_.z;
        //out_->m12 = 2.0f * qua_.y * qua_.z + 2.0f * qua_.w * qua_.x;

        //out_->m20 = 2.0f * qua_.x * qua_.z + 2.0f * qua_.w * qua_.y;
        //out_->m21 = 2.0f * qua_.y * qua_.z - 2.0f * qua_.w * qua_.x;
        //out_->m22 = 1.0f - 2.0f * qua_.x * qua_.x - 2.0f * qua_.y * qua_.y;

        //out_->m03 = out_->m13 = out_->m23 = out_->m30 = out_->m31 = out_->m32 = 0.0f;
        //out_->m33 = 1.0f;
    }

	//----------------------------------------------------------------------
	// ● スケーリングする行列を作成する
	//----------------------------------------------------------------------
	void Matrix::scaling( Matrix* mat_, lnFloat x_, lnFloat y_, lnFloat z_ )
	{
		mat_->set(
			x_, 0.0f, 0.0f, 0.0f,
			0.0f, y_, 0.0f, 0.0f,
			0.0f, 0.0f, z_, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
	}

	//----------------------------------------------------------------------
	// ● スケーリングする行列を作成する
	//----------------------------------------------------------------------
	void Matrix::scaling( Matrix* mat_, const Vector3& scale_ )
	{
		mat_->set(
			scale_.x, 0.0f, 0.0f, 0.0f,
			0.0f, scale_.y, 0.0f, 0.0f,
			0.0f, 0.0f, scale_.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
	}

	//----------------------------------------------------------------------
	// ● スケーリングする行列を作成する
	//----------------------------------------------------------------------
	void Matrix::scaling( Matrix* mat_, lnFloat xyz_ )
	{
		mat_->set(
			xyz_, 0.0f, 0.0f, 0.0f,
			0.0f, xyz_, 0.0f, 0.0f,
			0.0f, 0.0f, xyz_, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
	}


    

    

	//----------------------------------------------------------------------
	// ● 行列の逆行列を計算する
	//----------------------------------------------------------------------
	void Matrix::inverse( Matrix* outMat, const Matrix& mat )
	{
#if 1	// 速度は D3DXMatrixInverse の 2～3倍 (100000 回で 3000us。コンストラクタとかが足引っ張ってそうな気がする…)
		lnFloat coef00 = mat.m[2][2] * mat.m[3][3] - mat.m[3][2] * mat.m[2][3];
		lnFloat coef02 = mat.m[1][2] * mat.m[3][3] - mat.m[3][2] * mat.m[1][3];
		lnFloat coef03 = mat.m[1][2] * mat.m[2][3] - mat.m[2][2] * mat.m[1][3];

		lnFloat coef04 = mat.m[2][1] * mat.m[3][3] - mat.m[3][1] * mat.m[2][3];
		lnFloat coef06 = mat.m[1][1] * mat.m[3][3] - mat.m[3][1] * mat.m[1][3];
		lnFloat coef07 = mat.m[1][1] * mat.m[2][3] - mat.m[2][1] * mat.m[1][3];

		lnFloat coef08 = mat.m[2][1] * mat.m[3][2] - mat.m[3][1] * mat.m[2][2];
		lnFloat coef10 = mat.m[1][1] * mat.m[3][2] - mat.m[3][1] * mat.m[1][2];
		lnFloat coef11 = mat.m[1][1] * mat.m[2][2] - mat.m[2][1] * mat.m[1][2];

		lnFloat coef12 = mat.m[2][0] * mat.m[3][3] - mat.m[3][0] * mat.m[2][3];
		lnFloat coef14 = mat.m[1][0] * mat.m[3][3] - mat.m[3][0] * mat.m[1][3];
		lnFloat coef15 = mat.m[1][0] * mat.m[2][3] - mat.m[2][0] * mat.m[1][3];

		lnFloat coef16 = mat.m[2][0] * mat.m[3][2] - mat.m[3][0] * mat.m[2][2];
		lnFloat coef18 = mat.m[1][0] * mat.m[3][2] - mat.m[3][0] * mat.m[1][2];
		lnFloat coef19 = mat.m[1][0] * mat.m[2][2] - mat.m[2][0] * mat.m[1][2];

		lnFloat coef20 = mat.m[2][0] * mat.m[3][1] - mat.m[3][0] * mat.m[2][1];
		lnFloat coef22 = mat.m[1][0] * mat.m[3][1] - mat.m[3][0] * mat.m[1][1];
		lnFloat coef23 = mat.m[1][0] * mat.m[2][1] - mat.m[2][0] * mat.m[1][1];

		LVector4 fac0(coef00, coef00, coef02, coef03);
		LVector4 fac1(coef04, coef04, coef06, coef07);
		LVector4 fac2(coef08, coef08, coef10, coef11);
		LVector4 fac3(coef12, coef12, coef14, coef15);
		LVector4 fac4(coef16, coef16, coef18, coef19);
		LVector4 fac5(coef20, coef20, coef22, coef23);

		LVector4 vec0(mat.m[1][0], mat.m[0][0], mat.m[0][0], mat.m[0][0]);
		LVector4 vec1(mat.m[1][1], mat.m[0][1], mat.m[0][1], mat.m[0][1]);
		LVector4 vec2(mat.m[1][2], mat.m[0][2], mat.m[0][2], mat.m[0][2]);
		LVector4 vec3(mat.m[1][3], mat.m[0][3], mat.m[0][3], mat.m[0][3]);

		LVector4 inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
		LVector4 inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
		LVector4 inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
		LVector4 inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

		LVector4 signA(+1, -1, +1, -1);
		LVector4 signB(-1, +1, -1, +1);
		LMatrix inverse(inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB);

		LVector4 row0(inverse.m[0][0], inverse.m[1][0], inverse.m[2][0], inverse.m[3][0]);

		LVector4 dot0(mat[0] * row0);
		lnFloat dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

		if (dot1 == 0.0)
		{
			// 計算できない。D3D に合わせて、単位行列で返す
			outMat->identity();
			return;
		}

		lnFloat oneOverDeterminant = 1.0f / dot1;

		*outMat = inverse * oneOverDeterminant;
#endif
#if 0	// 速度は D3DXMatrixInverse の 10倍
        int i, j;
        lnFloat buf;

        Matrix	tm( mat_ );
	    identity( out_ );

	    // 掃き出し法
	    for ( i = 0 ; i < 4 ; ++i )
	    {
			//if (tm.m[ i ][ i ] != 0)
				buf = 1.0f / tm.m[ i ][ i ];	// 0除算の可能性がある
			//else
			//	buf = 0.0;
		    for ( j = 0 ; j < 4 ; ++j )
		    {
			    tm.m[ i ][ j ] *= buf;
			    out_->m[ i ][ j ] *= buf;
		    }
		    for ( j = 0 ; j < 4 ; ++j )
		    {
			    if ( i != j )
			    {
				    buf = tm.m[ j ][ i ];
				    for( int k = 0 ; k < 4 ; ++k )
				    {
					    tm.m[ j ][ k ] -= tm.m[ i ][ k ] * buf;
					    out_->m[ j ][ k ] -= out_->m[ i ][ k ] * buf;
				    }
			    }
		    }
	    }
#endif
#if 0
		//D3DXMATRIX dxmout;
		D3DXMatrixInverse( (D3DXMATRIX*)out_, NULL, (D3DXMATRIX*)&mat_ );
		//return Matrix( *((Matrix*)&dxmout) );
		//return Matrix();
#endif
#if 0
		// http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/tech23.html

		float m0011 = mat_.m00 * mat_.m11;
		float m0012 = mat_.m00 * mat_.m12;
		float m0021 = mat_.m00 * mat_.m21;
		float m0022 = mat_.m00 * mat_.m22;

		float m0110 = mat_.m01 * mat_.m10;
		float m0112 = mat_.m01 * mat_.m12;
		float m0120 = mat_.m01 * mat_.m20;
		float m0122 = mat_.m01 * mat_.m22;

		float m0210 = mat_.m02 * mat_.m10;
		float m0211 = mat_.m02 * mat_.m11;
		float m0220 = mat_.m02 * mat_.m20;
		float m0221 = mat_.m02 * mat_.m21;

		float m1021 = mat_.m10 * mat_.m21;
		float m1022 = mat_.m10 * mat_.m22;

		float m1120 = mat_.m11 * mat_.m20;
		float m1122 = mat_.m11 * mat_.m22;

		float m1220 = mat_.m12 * mat_.m20;
		float m1221 = mat_.m12 * mat_.m21;


		float m1122_m1221 = m1122 - m1221;
		float m1220_m1022 = m1220 - m1022;
		float m1021_m1120 = m1021 - m1120;

		
		float delta = mat_.m00*( m1122_m1221 ) + mat_.m01*( m1220_m1022 ) + mat_.m02*( m1021_m1120 );
		float rcpDelta = 1.f / delta;

		Matrix mat;

		mat.m00 = ( m1122_m1221 ) * rcpDelta;
		mat.m10 = ( m1220_m1022 ) * rcpDelta;
		mat.m20 = ( m1021_m1120 ) * rcpDelta;
		mat.m01 = ( m0221-m0122 ) * rcpDelta;
		mat.m11 = ( m0022-m0220 ) * rcpDelta;
		mat.m21 = ( m0120-m0021 ) * rcpDelta;
		mat.m02 = ( m0112-m0211 ) * rcpDelta;
		mat.m12 = ( m0210-m0012 ) * rcpDelta;
		mat.m22 = ( m0011-m0110 ) * rcpDelta;

		float t03 = mat_.m03;
		float t13 = mat_.m13;
		mat.m03 = -( mat.m00 * t03 + mat.m01 * t13 + mat.m02 * mat_.m23 );
		mat.m13 = -( mat.m10 * t03 + mat.m11 * t13 + mat.m12 * mat_.m23 );
		mat.m23 = -( mat.m20 * t03 + mat.m21 * t13 + mat.m22 * mat_.m23 );

		return mat;
#endif
	}

	//----------------------------------------------------------------------
	// ● 行列の転置行列を計算する
	//----------------------------------------------------------------------
	void Matrix::transpose( Matrix* out_, const Matrix& mat_ )
    {
        out_->m01 = mat_.m10;
        out_->m02 = mat_.m20;
        out_->m03 = mat_.m30;
        out_->m12 = mat_.m21;
        out_->m13 = mat_.m31;
        out_->m23 = mat_.m32;
        out_->m10 = mat_.m01;
        out_->m20 = mat_.m02;
        out_->m21 = mat_.m12;
        out_->m30 = mat_.m03;
        out_->m31 = mat_.m13;
        out_->m32 = mat_.m23;
        out_->m00 = mat_.m00;
        out_->m11 = mat_.m11;
        out_->m22 = mat_.m22;
        out_->m33 = mat_.m33;

        //lnFloat t;
        //t = m01; m01 = m10; m10 = t;
        //t = m02; m02 = m20; m20 = t;
        //t = m12; m12 = m21; m21 = t;
        //t = m03; m03 = m30; m30 = t;
        //t = m13; m13 = m31; m31 = t;
        //t = m23; m23 = m32; m32 = t;
    }

	//----------------------------------------------------------------------
	// ● 左手座標系ビュー行列を作成する
	//----------------------------------------------------------------------
	void Matrix::viewTransformLH( Matrix* out_, const Vector3& position_, const Vector3& look_at_, const Vector3& up_ )
	{
        Vector3 xaxis, yaxis, zaxis;
		// 注視点からカメラ位置までのベクトルをZ軸とする
		zaxis = look_at_;
		zaxis -= position_;
		zaxis.normalize();
		// Z軸と上方向のベクトルの外積をとるとX軸が分かる
        Vector3::cross( &xaxis, up_, zaxis );
		xaxis.normalize();
		// 2つの軸がわかったので、その2つの外積は残りの軸(Y軸)になる
        Vector3::cross( &yaxis, zaxis, xaxis );

		out_->set(
			xaxis.x, yaxis.x, zaxis.x, 0.0f,
			xaxis.y, yaxis.y, zaxis.y, 0.0f,
			xaxis.z, yaxis.z, zaxis.z, 0.0f,
			-( xaxis.x * position_.x + xaxis.y * position_.y + xaxis.z * position_.z ),
			-( yaxis.x * position_.x + yaxis.y * position_.y + yaxis.z * position_.z ),
			-( zaxis.x * position_.x + zaxis.y * position_.y + zaxis.z * position_.z ),
			1.0f );

		
	}

	//----------------------------------------------------------------------
	// ● 右手座標系ビュー行列を作成する
	//----------------------------------------------------------------------
	void Matrix::viewTransformRH( Matrix* out_, const Vector3& position_, const Vector3& look_at_, const Vector3& up_ )
	{
        Vector3 xaxis, yaxis, zaxis;
		// 注視点からカメラ位置までのベクトルをZ軸とする
		zaxis = position_;
		zaxis -= look_at_;
		zaxis.normalize();
		// Z軸と上方向のベクトルの外積をとるとX軸が分かる
        Vector3::cross( &xaxis, up_, zaxis );
		xaxis.normalize();
		// 2つの軸がわかったので、その2つの外積は残りの軸(Y軸)になる
        Vector3::cross( &yaxis, zaxis, xaxis );

		out_->set(
			xaxis.x, yaxis.x, zaxis.x, 0.0f,
			xaxis.y, yaxis.y, zaxis.y, 0.0f,
			xaxis.z, yaxis.z, zaxis.z, 0.0f,
			-( xaxis.x * position_.x + xaxis.y * position_.y + xaxis.z * position_.z ),
			-( yaxis.x * position_.x + yaxis.y * position_.y + yaxis.z * position_.z ),
			-( zaxis.x * position_.x + zaxis.y * position_.y + zaxis.z * position_.z ),
			1.0f );
        //D3DXMATRIX mat;
        //D3DXMatrixLookAtRH(&mat, 
        //    (const D3DXVECTOR3 *)&position_,
        //    (const D3DXVECTOR3 *)&look_at_,
        //    (const D3DXVECTOR3 *)&up_ );
        //*out_ = *((Matrix*)&mat);
        /*
        
            */
	}

	//----------------------------------------------------------------------
	// ● 左手座標系射影行列の作成
	//----------------------------------------------------------------------
	void Matrix::perspectiveFovLH( Matrix* out_, lnFloat fov_y_, lnFloat width_, lnFloat height_, lnFloat near_, lnFloat far_ )
	{
		lnFloat aspect = width_ / height_;		// アスペクト比
		perspectiveFovLH( out_, fov_y_, aspect, near_, far_ );
	}
	void Matrix::perspectiveFovLH( Matrix* out_, lnFloat fov_y_, lnFloat aspect, lnFloat near_, lnFloat far_ )
	{
		lnFloat h = 1.f / tan( fov_y_ * 0.5f );	// cot(fovY/2)

        out_->set(
			h / aspect,		0.0f,		0.0f,									0.0f,
			0.0f,			h,			0.0f,									0.0f,
			0.0f,			0.0f,		far_ / ( far_ - near_ ),				1.0f,
			0.0f,			0.0f,		( -near_ * far_ ) / ( far_ - near_ ),	0.0f );
	}

	//----------------------------------------------------------------------
	// ● 右手座標系射影行列の作成
	//----------------------------------------------------------------------
	void Matrix::perspectiveFovRH( Matrix* out_, lnFloat fov_y_, lnFloat width_, lnFloat height_, lnFloat near_, lnFloat far_ )
	{
		lnFloat aspect = width_ / height_;		// アスペクト比
        perspectiveFovRH( out_, fov_y_, aspect, near_, far_ );
	}
	void Matrix::perspectiveFovRH( Matrix* out_, lnFloat fov_y_, lnFloat aspect, lnFloat near_, lnFloat far_ )
	{
        lnFloat h = 1.f / tan( fov_y_ * 0.5f );	// cot(fovY/2)
        out_->set(
			h / aspect,		0.0f,		0.0f,									0.0f,
			0.0f,			h,			0.0f,									0.0f,
			0.0f,			0.0f,		far_ / ( near_ - far_ ),				-1.0f,
			0.0f,			0.0f,		( near_ * far_ ) / ( near_ - far_ ),	0.0f );

        /*
		h = cot(fovY/2)
		w = h / Aspect

		w       0       0               0
		0       h       0               0
		0       0       zf/(zf-zn)      1
		0       0       -zn*zf/(zf-zn)  0

		*/

		//Matrix mat;

        //D3DXMATRIX mat;
        //D3DXMatrixPerspectiveFovRH(&mat, fov_y_, width_ / height_, near_, far_);

        //*out_ = *((Matrix*)&mat);
        
        /*
		lnFloat h = 1.f / tan( fov_y_ * 0.5f );	// cot(fovY/2)
		lnFloat aspect = width_ / height_;		// アスペクト比

        out_->set(
			h / aspect,		0.0f,		0.0f,									0.0f,
			0.0f,			h,			0.0f,									0.0f,
			0.0f,			0.0f,		far_ / ( far_ - near_ ),				1.0f,
			0.0f,			0.0f,		( -near_ * far_ ) / ( far_ - near_ ),	0.0f );
            */

        /*
        lnFloat aspect = width_ / height_;		// アスペクト比
		float xymax = near_ * tan(fov_y_ *  0.00872664626);
          float ymin = -xymax;
          float xmin = -xymax;

          float width = xymax - xmin;
          float height = xymax - ymin;

          float depth = far_ - near_;
          float q = -(far_ + near_) / depth;
          float qn = -2 * (far_ * near_) / depth;

          float w = 2 * near_ / width;
          w = w / aspect;
          float h = 2 * near_ / height;

          out_->set(
			w,		0.0f,		0.0f,									0.0f,
			0.0f,			h,			0.0f,									0.0f,
			0.0f,			0.0f,		q,				-1.0f,
			0.0f,			0.0f,		qn,	0.0f );
*/
	}

	//----------------------------------------------------------------------
	// ● 左手座標系正射影行列の作成
	//----------------------------------------------------------------------
	void Matrix::orthoLH( Matrix* out_, lnFloat width_, lnFloat height_, lnFloat near_, lnFloat far_ )
    {
        out_->set(
			2.0f / width_,  0.0f,		    0.0f,					    0.0f,
			0.0f,			2.0f / height_, 0.0f,					    0.0f,
			0.0f,			0.0f,		    1.0f / ( far_ - near_ ),    0.0f,
			0.0f,			0.0f,		    near_ / ( near_ - far_ ),	1.0f );
    }

	//----------------------------------------------------------------------
	// ● 右手座標系正射影行列の作成
	//----------------------------------------------------------------------
	void Matrix::orthoRH( Matrix* out_, lnFloat width_, lnFloat height_, lnFloat near_, lnFloat far_ )
    {
        out_->set(
			2.0f / width_,  0.0f,		    0.0f,					    0.0f,
			0.0f,			2.0f / height_, 0.0f,					    0.0f,
			0.0f,			0.0f,		    1.0f / ( near_ - far_ ),    0.0f,
			0.0f,			0.0f,		    near_ / ( near_ - far_ ),	1.0f );
    }

	//----------------------------------------------------------------------
	// ● ピクセル単位の2D描画に使う射影行列の作成
	//----------------------------------------------------------------------
	void Matrix::perspective2DLH( Matrix* out_, lnFloat width_, lnFloat height_, lnFloat near_, lnFloat far_ )
    {
#if 1
        out_->set(
			2.0f / width_,              0.0f,		                0.0f,					    0.0f,
			0.0f,			            -2.0f / height_,            0.0f,					    0.0f,
			0.0f,			            0.0f,		                1.0f / ( far_ - near_ ),    0.0f,
			-1.0f,						1.0f,						near_ / ( near_ - far_ ),	1.0f );
#else
        // 平行移動成分は、画面半分 + 0.5 ピクセルの調整
        out_->set(
			2.0f / width_,              0.0f,		                0.0f,					    0.0f,
			0.0f,			            -2.0f / height_,            0.0f,					    0.0f,
			0.0f,			            0.0f,		                1.0f / ( far_ - near_ ),    0.0f,
			-1.0f - ( 0.5f / width_ ),  1.0f + ( 0.5f / height_ ),  near_ / ( near_ - far_ ),	1.0f );
#endif
    }

	//----------------------------------------------------------------------
	// ● 行列の 3x3 部分だけを乗算する
	//----------------------------------------------------------------------
  //  void Matrix::mul3x3( Matrix* out_, const Matrix& mat0_, const Matrix& mat1_ )
  //  {
  //      lnFloat mx0 = m00;
		//lnFloat mx1 = m01;
		//lnFloat mx2 = m02;
		//m00 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m03 * matrix_.m30;
		//m01 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m03 * matrix_.m31;
		//m02 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m03 * matrix_.m32;
  //      mx0 = m10;
		//mx1 = m11;
		//mx2 = m12;
		//m10 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m13 * matrix_.m30;
		//m11 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m13 * matrix_.m31;
		//m12 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m13 * matrix_.m32;
		//mx0 = m20;
		//mx1 = m21;
		//mx2 = m22;
		//m20 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m23 * matrix_.m30;
		//m21 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m23 * matrix_.m31;
		//m22 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m23 * matrix_.m32;
  //      m03 = m13 = m23 = m30 = m31 = m32 = 0.0f;
  //      m33 = 1.0f;
  //  }

	//----------------------------------------------------------------------
	// ● SQTTransform から変換する
	//----------------------------------------------------------------------
    void Matrix::createFromSQTTransform( Matrix* out_, const SQTTransform& t_ )
    {
        Matrix::scaling( out_, t_.Scale );
        out_->rotationQuaternion( t_.Rotation );
        out_->translation( t_.Translation );
    }

	//----------------------------------------------------------------------
	// ● SQTTransform との乗算
	//----------------------------------------------------------------------
	//void Matrix::multiply( Matrix* out_, const SQTTransform& t_, const Matrix& mat_ )
	//{
	//    Matrix::createFromSQTTransform( out_, t_ );
	//    (*out_) *= mat_;
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Matrix::toEuler( Vector3* out, const Matrix& mat )
	{
		const float RADIANS = 57.29578f;
		float angle_x, angle_y, angle_z;
   
		angle_y = -asin(mat.m03/*m[2]*/);
		float c = cos(angle_y);
		angle_y *= RADIANS;
  
		if (fabsf(c) > 0.005) {
			float trX =  mat.m22 / c;//   m[10] / c; 
			float trY = -mat.m12 / c;//-m[6]  / c; 
			angle_x  = atan2( trY, trX ) * RADIANS;
			trX =  mat.m00 / c;
			trY = -mat.m01 / c;
			angle_z  = atan2( trY, trX ) * RADIANS;
		} 
		else {
			// ジンバルロック発生
			angle_x  = 0;
			float trX = mat.m11;
			float trY = mat.m10;//m[4];
			angle_z  = atan2( trY, trX ) * RADIANS;
		}

		out->x = angle_x;
		out->y = angle_y;
		out->z = angle_z;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Matrix::decompose( Vector3* scale, Matrix* rot, Vector3* trans, const Matrix& mat )
	{
		if ( trans )
		{
			trans->x = mat.m30;
			trans->y = mat.m31;
			trans->z = mat.m32;
		}

		LVector3 sc(
			sqrtf( mat.m00 * mat.m00 + mat.m01 * mat.m01 + mat.m02 * mat.m02 ),
			sqrtf( mat.m10 * mat.m10 + mat.m11 * mat.m11 + mat.m12 * mat.m12 ),
			sqrtf( mat.m20 * mat.m20 + mat.m21 * mat.m21 + mat.m22 * mat.m22 ) );
		if ( scale )
		{
			*scale = sc;
		}

		if ( rot )
		{
			rot->m00 = (sc.x != 0.0f) ? mat.m00 / sc.x : 0.0f;
			rot->m01 = (sc.x != 0.0f) ? mat.m01 / sc.x : 0.0f;
			rot->m02 = (sc.x != 0.0f) ? mat.m02 / sc.x : 0.0f;
			rot->m03 = 0.0f;
			rot->m10 = (sc.y != 0.0f) ? mat.m10 / sc.y : 0.0f;
			rot->m11 = (sc.y != 0.0f) ? mat.m11 / sc.y : 0.0f;
			rot->m12 = (sc.y != 0.0f) ? mat.m12 / sc.y : 0.0f;
			rot->m13 = 0.0f;
			rot->m20 = (sc.z != 0.0f) ? mat.m20 / sc.z : 0.0f;
			rot->m21 = (sc.z != 0.0f) ? mat.m21 / sc.z : 0.0f;
			rot->m22 = (sc.z != 0.0f) ? mat.m22 / sc.z : 0.0f;
			rot->m23 = 0.0f;
			rot->m30 = 0.0f;
			rot->m31 = 0.0f;
			rot->m32 = 0.0f;
			rot->m33 = 1.0f;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Matrix::transformBasis( Matrix* m )
	{
		static const Matrix scale(
			1.0f, 0, 0, 0,  
			0, 1.0f, 0, 0,  
			0, 0, -1.0f, 0,  
			0, 0, 0, 1.0f);
		*m = scale * (*m) * scale;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Matrix::multiply( Matrix* out, const Matrix& mat1, const Matrix& mat2 )
	{
		out->m00 = mat1.m00 * mat2.m00 + mat1.m01 * mat2.m10 + mat1.m02 * mat2.m20 + mat1.m03 * mat2.m30;
		out->m01 = mat1.m00 * mat2.m01 + mat1.m01 * mat2.m11 + mat1.m02 * mat2.m21 + mat1.m03 * mat2.m31;
		out->m02 = mat1.m00 * mat2.m02 + mat1.m01 * mat2.m12 + mat1.m02 * mat2.m22 + mat1.m03 * mat2.m32;
		out->m03 = mat1.m00 * mat2.m03 + mat1.m01 * mat2.m13 + mat1.m02 * mat2.m23 + mat1.m03 * mat2.m33;

		out->m10 = mat1.m10 * mat2.m00 + mat1.m11 * mat2.m10 + mat1.m12 * mat2.m20 + mat1.m13 * mat2.m30;
		out->m11 = mat1.m10 * mat2.m01 + mat1.m11 * mat2.m11 + mat1.m12 * mat2.m21 + mat1.m13 * mat2.m31;
		out->m12 = mat1.m10 * mat2.m02 + mat1.m11 * mat2.m12 + mat1.m12 * mat2.m22 + mat1.m13 * mat2.m32;
		out->m13 = mat1.m10 * mat2.m03 + mat1.m11 * mat2.m13 + mat1.m12 * mat2.m23 + mat1.m13 * mat2.m33;

		out->m20 = mat1.m20 * mat2.m00 + mat1.m21 * mat2.m10 + mat1.m22 * mat2.m20 + mat1.m23 * mat2.m30;
		out->m21 = mat1.m20 * mat2.m01 + mat1.m21 * mat2.m11 + mat1.m22 * mat2.m21 + mat1.m23 * mat2.m31;
		out->m22 = mat1.m20 * mat2.m02 + mat1.m21 * mat2.m12 + mat1.m22 * mat2.m22 + mat1.m23 * mat2.m32;
		out->m23 = mat1.m20 * mat2.m03 + mat1.m21 * mat2.m13 + mat1.m22 * mat2.m23 + mat1.m23 * mat2.m33;

		out->m30 = mat1.m30 * mat2.m00 + mat1.m31 * mat2.m10 + mat1.m32 * mat2.m20 + mat1.m33 * mat2.m30;
		out->m31 = mat1.m30 * mat2.m01 + mat1.m31 * mat2.m11 + mat1.m32 * mat2.m21 + mat1.m33 * mat2.m31;
		out->m32 = mat1.m30 * mat2.m02 + mat1.m31 * mat2.m12 + mat1.m32 * mat2.m22 + mat1.m33 * mat2.m32;
		out->m33 = mat1.m30 * mat2.m03 + mat1.m31 * mat2.m13 + mat1.m32 * mat2.m23 + mat1.m33 * mat2.m33;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Matrix::rotationYawPitchRoll( Matrix* out, float yaw, float pitch, float roll )
	{
		LQuaternion q;
		LQuaternion::rotationYawPitchRoll( &q, yaw, pitch, roll );
		*out = Matrix( q );
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