
#include <assert.h>
#include "Asm.h"
#include "../../include/Lumino/Math/MathUtils.h"
#include "../../include/Lumino/Math/Vector3.h"
#include "../../include/Lumino/Math/Matrix.h"
#include "../../include/Lumino/Math/Quaternion.h"

namespace Lumino
{

//=============================================================================
// Quaternion
//=============================================================================

const Quaternion Quaternion::Identity = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float Quaternion::GetLength() const
{
	return Asm::sqrt((X * X) + (Y * Y) + (Z * Z) + (W * W));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float Quaternion::GetLengthSquared() const
{
	return (X * X) + (Y * Y) + (Z * Z) + (W * W);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Quaternion::Normalize()
{
	float t = 1.0f / GetLength();
	X *= t;
	Y *= t;
	Z *= t;
	W *= t;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Quaternion::Multiply(const Quaternion& qua)
{
	float lx = X;
	float ly = Y;
	float lz = Z;
	float lw = W;
	X = (qua.X * lw + lx * qua.W + qua.Y * lz) - (qua.Z * ly);
	Y = (qua.Y * lw + ly * qua.W + qua.Z * lx) - (qua.X * lz);
	Z = (qua.Z * lw + lz * qua.W + qua.X * ly) - (qua.Y * lx);
	W = (qua.W * lw) - (qua.X * lx + qua.Y * ly + qua.Z * lz);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Quaternion::RotateX(float angle)
{
	float s, c;
	Asm::sincos(angle * 0.5f, &s, &c);

	float lx = X;
	float ly = Y;
	float lz = Z;
	float lw = W;
	X = (s * lw + lx * c);
	Y = (ly * c) - (s * lz);
	Z = (lz * c + s * ly);
	W = (c * lw) - (s * lx);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Quaternion::RotateY(float angle)
{
	float s, c;
	Asm::sincos(angle * 0.5f, &s, &c);

	float lx = X;
	float ly = Y;
	float lz = Z;
	float lw = W;
	X = (lx * c + s * lz);
	Y = (s * lw + ly * c);
	Z = (lz * c) - (s * lx);
	W = (c * lw) - (s * ly);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Quaternion::RotateZ(float angle)
{
	float s, c;
	Asm::sincos(angle * 0.5f, &s, &c);

	float lx = X;
	float ly = Y;
	float lz = Z;
	float lw = W;
	X = (lx * c) - (s * ly);
	Y = (ly * c + s * lx);
	Z = (s * lw + lz * c);
	W = (c * lw) - (s * lz);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Quaternion::RotateAxis(const Vector3& axis, float r)
{
	// TODO: もうちょい最適化
	Multiply(RotationAxis(axis, r));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Vector3 Quaternion::ToEulerAngles(RotationOrder order, bool* locked)
{
	// クォータニオン→行列
	Matrix rot = Matrix::RotationQuaternion(Quaternion::Normalize(*this));
	return rot.ToEulerAngles(order, locked);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Quaternion::ToAxisAngle(Vector3* axis, float* angle)
{
	Quaternion q = Quaternion::Normalize(*this);
	if (axis)
	{
		axis->X = q.X;
		axis->Y = q.Y;
		axis->Z = q.Z;
		axis->Normalize();
	}
	if (angle)
	{
		*angle = 2.0f * acos(q.W);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Quaternion::Print(const char* format, FILE* stream) const
{
	if (!format) {
		format = "%f, %f, %f, %f\n";
	}
	if (!stream) {
		stream = stdout;
	}
	fprintf(stream, format, X, Y, Z, W);
}

//-------------------------------------------------------------------------
// static
//-------------------------------------------------------------------------
Quaternion Quaternion::Normalize(const Quaternion& qua)
{
	float length = 1.0f / qua.GetLength();
	return Quaternion(
		qua.X * length,
		qua.Y * length,
		qua.Z * length,
		qua.W * length);
}

//-------------------------------------------------------------------------
// static
//-------------------------------------------------------------------------
Quaternion Quaternion::Conjugate(const Quaternion& qua)
{
	return Quaternion(
		-qua.X,
		-qua.Y,
		-qua.Z,
		qua.W);
}

//-------------------------------------------------------------------------
// static
//-------------------------------------------------------------------------
float Quaternion::Dot(const Quaternion& qua1, const Quaternion& qua2)
{
	return (qua1.X * qua2.X) + (qua1.Y * qua2.Y) + (qua1.Z * qua2.Z) + (qua1.W * qua2.W);
}

//-------------------------------------------------------------------------
// static
//-------------------------------------------------------------------------
Quaternion Quaternion::Inverse(const Quaternion& qua)
{
	float t = 1.0f / ((qua.X * qua.X) + (qua.Y * qua.Y) + (qua.Z * qua.Z) + (qua.W * qua.W));
	return Quaternion(
		-qua.X * t,
		-qua.Y * t,
		-qua.Z * t,
		qua.W * t);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Quaternion Quaternion::Multiply(const Quaternion& qua1, const Quaternion& qua2)
{
	return Quaternion(
		(qua2.X * qua1.W + qua1.X * qua2.W + qua2.Y * qua1.Z) - (qua2.Z * qua1.Y),
		(qua2.Y * qua1.W + qua1.Y * qua2.W + qua2.Z * qua1.X) - (qua2.X * qua1.Z),
		(qua2.Z * qua1.W + qua1.Z * qua2.W + qua2.X * qua1.Y) - (qua2.Y * qua1.X),
		(qua2.W * qua1.W) - (qua2.X * qua1.X + qua2.Y * qua1.Y + qua2.Z * qua1.Z));
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Quaternion Quaternion::RotationAxis(const Vector3& axis, float r)
{
	float tx = axis.X;
	float ty = axis.Y;
	float tz = axis.Z;
	float len = tx * tx + ty * ty + tz * tz;

	// 軸が正規化されていなければ正規化
	if (len != 1.0f)
	{
		len = Asm::sqrt(len);
		tx /= len;
		ty /= len;
		tz /= len;
	}

	float s, c;
	Asm::sincos(r * 0.5f, &s, &c);
	return Quaternion(
		s * tx,
		s * ty,
		s * tz,
		c);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Quaternion Quaternion::RotationMatrix(const Matrix& mat)
{
	float t = mat.M11 + mat.M22 + mat.M33;

	if (t > 0.0f)
	{
		float sq = Asm::sqrt(t + 1.0f);

		float w = sq * 0.5f;
		sq = 0.5f / sq;

		return Quaternion(
			(mat.M23 - mat.M32) * sq,
			(mat.M31 - mat.M13) * sq,
			(mat.M12 - mat.M21) * sq,
			w);
	}
	else if ((mat.M11 >= mat.M22) && (mat.M11 >= mat.M33))
	{
		float sq = Asm::sqrt(1.0f + mat.M11 - mat.M22 - mat.M33);
		float half = 0.5f / sq;

		return Quaternion(
			0.5f * sq,
			(mat.M12 + mat.M21) * half,
			(mat.M13 + mat.M31) * half,
			(mat.M23 - mat.M32) * half);
	}
	else if (mat.M22 > mat.M33)
	{
		float sq = Asm::sqrt(1.0f + mat.M22 - mat.M11 - mat.M33);
		float half = 0.5f / sq;

		return Quaternion(
			(mat.M21 + mat.M12) * half,
			0.5f * sq,
			(mat.M32 + mat.M23) * half,
			(mat.M31 - mat.M13) * half);
	}
	else
	{
		float sq = Asm::sqrt(1.0f + mat.M33 - mat.M11 - mat.M22);
		float half = 0.5f / sq;

		return Quaternion(
			(mat.M31 + mat.M13) * half,
			(mat.M32 + mat.M23) * half,
			0.5f * sq,
			(mat.M12 - mat.M21) * half);
	}
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Quaternion Quaternion::RotationYawPitchRoll(float yaw, float pitch, float roll)
{
	float halfRoll = roll * 0.5f;
	float sinRoll, cosRoll;
	Asm::sincos(halfRoll, &sinRoll, &cosRoll);

	float halfPitch = pitch * 0.5f;
	float sinPitch, cosPitch;
	Asm::sincos(halfPitch, &sinPitch, &cosPitch);

	float halfYaw = yaw * 0.5f;
	float sinYaw, cosYaw;
	Asm::sincos(halfYaw, &sinYaw, &cosYaw);

	return Quaternion(
		(cosYaw * sinPitch * cosRoll) + (sinYaw * cosPitch * sinRoll),
		(sinYaw * cosPitch * cosRoll) - (cosYaw * sinPitch * sinRoll),
		(cosYaw * cosPitch * sinRoll) - (sinYaw * sinPitch * cosRoll),
		(cosYaw * cosPitch * cosRoll) + (sinYaw * sinPitch * sinRoll));
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Quaternion Quaternion::RotationEulerAngles(const Vector3& angles, RotationOrder order)
{
	Quaternion q;
	switch (order)
	{
	case RotationOrder_XYZ:
		q.RotateX(angles.X); q.RotateY(angles.Y); q.RotateZ(angles.Z);
		break;
	case RotationOrder_YZX:
		q.RotateY(angles.Y); q.RotateZ(angles.Z); q.RotateX(angles.X);
		break;
	case RotationOrder_ZXY:
		q.RotateZ(angles.Z); q.RotateX(angles.X); q.RotateY(angles.Y);
		break;
	default:
		assert(0);
		break;
	}
	return q;
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Quaternion Quaternion::Slerp(const Quaternion& qua1, const Quaternion& qua2, float t)
{
	float opposite;
	float inverse;
	float dot = (qua1.X * qua2.X) + (qua1.Y * qua2.Y) + (qua1.Z * qua2.Z) + (qua1.W * qua2.W);
	bool flag = false;

	if( dot < 0.0f )
	{
		flag = true;
		dot = -dot;
	}

	if( dot > 0.999999f )
	{
		inverse = 1.0f - t;
		opposite = flag ? -t : t;
	}
	else
	{
		float ac = acos(dot);
		float invSin = (1.0f / sin(ac));

		inverse = (sin((1.0f - t) * ac)) * invSin;
		opposite = flag ? ((-sin(t * ac)) * invSin) : ((sin(t * ac)) * invSin);
	}

	return Quaternion(
		(inverse * qua1.X) + (opposite * qua2.X),
		(inverse * qua1.Y) + (opposite * qua2.Y),
		(inverse * qua1.Z) + (opposite * qua2.Z),
		(inverse * qua1.W) + (opposite * qua2.W));
}

} // namespace Lumino





#if 0


//==============================================================================
// Quaternion 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Common.h"
#include "Matrix.h"
#include "Quaternion.h"

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
//==============================================================================

    const Quaternion   Quaternion::IDENTITY( 0.0f, 0.0f, 0.0f, 1.0f );

	// デフォルトコンストラクタ
	Quaternion::Quaternion()
	{
		x = y = z = 0.0f;
        w = 1.0f;
	}

	// コピーコンストラクタ
	Quaternion::Quaternion( const Quaternion& vec_ )
	{
		x = vec_.x;
		y = vec_.y;
		z = vec_.z;
		w = vec_.w;
	}

	// コンストラクタ
	Quaternion::Quaternion( lnFloat x_, lnFloat y_, lnFloat z_, lnFloat w_ )
	{
		x = x_;
		y = y_;
		z = z_;
		w = w_;
	}

	/// コンストラクタ ( 軸と回転角度から作成 )
	Quaternion::Quaternion( const Vector3& axis_, lnFloat r_ )
	{
		lnFloat r;
		lnFloat tx = axis_.x;
		lnFloat ty = axis_.y;
		lnFloat tz = axis_.z;
		lnFloat s = axis_.x * axis_.x + axis_.y * axis_.y + axis_.z * axis_.z;

		// 軸が正規化されていなければ正規化
		if ( s != 1.0f )
		{
			r = Asm::sqrt( axis_.x * axis_.x + axis_.y * axis_.y + axis_.z * axis_.z );  
			tx /= r;
			ty /= r;
			tz /= r;
		}

		Asm::sincos( r_ * 0.5f, &s, &w );
		x = s * tx;
		y = s * ty;
		z = s * tz;
	}

	




	//----------------------------------------------------------------------
	// ● 球面線形補間を使って、2 つのクォータニオンを補間する
	//----------------------------------------------------------------------
    void Quaternion::slerp( Quaternion* out_, const Quaternion& qua1_, const Quaternion& qua2_, lnFloat t_ )
    {
	    lnFloat qr = qua1_.x * qua2_.x + qua1_.y * qua2_.y + qua1_.z * qua2_.z + qua1_.w * qua2_.w;
	    lnFloat t0 = 1.0f - t_;

	    if ( qr < 0 )
	    {
		    out_->x = qua1_.x * t0 - qua2_.x * t_;
		    out_->y = qua1_.y * t0 - qua2_.y * t_;
		    out_->z = qua1_.z * t0 - qua2_.z * t_;
		    out_->w = qua1_.w * t0 - qua2_.w * t_;
	    }
	    else
	    {
		    out_->x = qua1_.x * t0 + qua2_.x * t_;
		    out_->y = qua1_.y * t0 + qua2_.y * t_;
		    out_->z = qua1_.z * t0 + qua2_.z * t_;
		    out_->w = qua1_.w * t0 + qua2_.w * t_;
	    }
        Quaternion::normalize( out_, *out_ );
    }

	//----------------------------------------------------------------------
	// ● クォータニオンをオイラー角に変換する
	//----------------------------------------------------------------------
    void Quaternion::toEuler( Vector3* out_, const Quaternion& qua1_ )
    {
        // XYZ軸回転の取得
	    // Y回転を求める
	    lnFloat	x2 = qua1_.x + qua1_.x;
	    lnFloat	y2 = qua1_.y + qua1_.y;
	    lnFloat	z2 = qua1_.z + qua1_.z;
	    lnFloat	xz2 = qua1_.x * z2;
	    lnFloat	wy2 = qua1_.w * y2;
	    lnFloat	temp = -(xz2 - wy2);

	    // 誤差対策
	    if( temp >= 1.f ){ 
	temp = 1.f; }
	    else if( temp <= -1.f ){ 
	    	temp = -1.f; }

	    lnFloat	yRadian = asinf(temp);

	    // 他の回転を求める
	    lnFloat	xx2 = qua1_.x * x2;
	    lnFloat	xy2 = qua1_.x * y2;
	    lnFloat	zz2 = qua1_.z * z2;
	    lnFloat	wz2 = qua1_.w * z2;

	    if( yRadian < 3.1415926f * 0.5f )
	    {
		    if( yRadian > -3.1415926f * 0.5f )
		    {
			    float	yz2 = qua1_.y * z2;
			    float	wx2 = qua1_.w * x2;
			    float	yy2 = qua1_.y * y2;
			    out_->x = atan2f( (yz2 + wx2), (1.f - (xx2 + yy2)) );
			    out_->y = yRadian;
			    out_->z = atan2f( (xy2 + wz2), (1.f - (yy2 + zz2)) );
		    }
		    else
		    {
			    out_->x = -atan2f( (xy2 - wz2), (1.f - (xx2 + zz2)) );
			    out_->y = yRadian;
			    out_->z = 0.f;
		    }
	    }
	    else
	    {
		    out_->x = atan2f( (xy2 - wz2), (1.f - (xx2 + zz2)) );
		    out_->y = yRadian;
		    out_->z = 0.f;
	    }
    }
	
	factoringZXY
	 public static bool DecompositeQuaternion(Quaternion input, out float YRot, out float XRot, out float ZRot)
        {
            //クォータニオンの正規化
            Quaternion inputQ = new Quaternion(input.X, input.Y, input.Z, input.W);
            inputQ.Normalize();
            //マトリクスを生成する
            Matrix rot = Matrix.CreateFromQuaternion(inputQ);
            //ヨー(X軸周りの回転)を取得
            if (rot.M32 > 1 - 1.0e-4 || rot.M32 < -1 + 1.0e-4)
            {//ジンバルロック判定
                XRot = (rot.M32 < 0 ? MathHelper.PiOver2 : -MathHelper.PiOver2);
                ZRot = 0; YRot = -(float)Math.Atan2(rot.M21, rot.M11);
                
                return false;
            }
            XRot = -(float)Math.Asin(rot.M32);
            //ピッチを取得
            YRot = -(float)Math.Atan2(-rot.M31, rot.M33);
            //ロールを取得
            ZRot = -(float)Math.Atan2(-rot.M12, rot.M22);
            return true;
        }
	
	
	http://qiita.com/q_tarou/items/46e5045068742dfb2fa6
	
	factoringZXY	← YawRollPitch で作ったやつはこれで復元しないとダメ。↑のDecompositeQuaternionもこれ
	factoringXYZ
	factoringYZX
	ジンバルロックはそれぞれの最初の軸で発生します。
	(上から順に Z X Y)
	発生した場合、その軸の角度を 0 で返します。
	戻り値は false になります。
	
	

	//----------------------------------------------------------------------
	// ● オイラー角からクォータニオンを作成する
	//----------------------------------------------------------------------
    void Quaternion::fromEuler( Quaternion* out_, const Vector3& angle_ )
    {
        lnFloat	rx = angle_.x * 0.5f;
	    lnFloat	ry = angle_.y * 0.5f;
	    lnFloat	rz = angle_.z * 0.5f;
        lnFloat sx, sy, sz, cx, cy, cz;

        Asm::sincos( rx, &sx, &cx );
        Asm::sincos( ry, &sy, &cy );
        Asm::sincos( rz, &sz, &cz );

	    out_->x = sx * cy * cz - cx * sy * sz;
	    out_->y = cx * sy * cz + sx * cy * sz;
	    out_->z = cx * cy * sz - sx * sy * cz;
	    out_->w = cx * cy * cz + sx * sy * sz;
    }
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Quaternion::rotationYawPitchRoll( Quaternion* out, float yaw, float pitch, float roll )
	{
		float halfRoll = roll * 0.5f;
		float sinRoll = sinf( halfRoll );
		float cosRoll = cosf( halfRoll );

		float halfPitch = pitch * 0.5f;
		float sinPitch = sinf( halfPitch );
		float cosPitch = cosf( halfPitch );

		float halfYaw = yaw * 0.5f;
		float sinYaw = sinf( halfYaw );
		float cosYaw = cosf( halfYaw );

		out->x = (cosYaw * sinPitch * cosRoll) + (sinYaw * cosPitch * sinRoll);
		out->y = (sinYaw * cosPitch * cosRoll) - (cosYaw * sinPitch * sinRoll);
		out->z = (cosYaw * cosPitch * sinRoll) - (sinYaw * sinPitch * cosRoll);
		out->w = (cosYaw * cosPitch * cosRoll) + (sinYaw * sinPitch * sinRoll);
	}


	//----------------------------------------------------------------------
	// ● クォータニオンをYaw(Y回転), Pitch(X回転), Roll(Z回転)に分解する
	//----------------------------------------------------------------------
    bool Quaternion::factoringZXY( lnFloat* z_, lnFloat* x_, lnFloat* y_, const Quaternion& qua1_ )
    {
        // クォータニオン→行列
        Quaternion input_q( qua1_ );
        input_q.normalize();
        Matrix rot( input_q );

        // ジンバルロック判定
        if ( rot.m21 > 1.0f - 1.0e-4 || rot.m21 < -1.0f + 1.0e-4 )
        {
            *x_ = ( rot.m21 < 0 ? LMath::PI_OVER2 : -LMath::PI_OVER2 );
            *z_ = 0;
            *y_ = atan2( -rot.m02, rot.m00 );
            return false;
        }

        *x_ = -(float)asin( rot.m21 );
        *z_ = (float)asin( rot.m01 / cos( *x_ ) );

        //漏れ対策
        if ( LMath::isNaN( *z_ ) )
        {
            *x_ = ( rot.m21 < 0 ? LMath::PI_OVER2 : -LMath::PI_OVER2 );
            *z_ = 0;
            *y_ = atan2( -rot.m02, rot.m00 );
            return false;
        }

        if ( rot.m11 < 0 )
        {
            *z_ = LMath::PI - *z_;
        }

        // ピッチを取得
        *y_ = atan2( rot.m20, rot.m22 );

        //*x_ = -*x_;
        //*y_ = -*y_;
        //*z_ = -*z_;

        return true;
    }

	//----------------------------------------------------------------------
	// ● クォータニオンをX,Y,Z回転に分解する
	//----------------------------------------------------------------------
    bool Quaternion::factoringXYZ( lnFloat* x_, lnFloat* y_, lnFloat* z_, const Quaternion& qua1_ )
    {
        // クォータニオン→行列
        Quaternion input_q( qua1_ );
        input_q.normalize();
        Matrix rot( input_q );

        //ジンバルロック判定
        if ( rot.m02 > 1.0f - 1.0e-4 || rot.m02 < -1.0f + 1.0e-4 )
        {
            *x_ = 0;
            *y_ = ( rot.m02 < 0 ? LMath::PI_OVER2 : -LMath::PI_OVER2 );
            *z_ = -atan2( -rot.m10, rot.m11 );
            return false;
        }

        *y_ = -asin( rot.m02 );
        *x_ = asin( rot.m12 / cos( *y_ ) );

        //ジンバルロック判定(漏れ対策)
        if ( LMath::isNaN( *x_ ) )
        {
            *x_ = 0;
            *y_ = ( rot.m02 < 0 ? LMath::PI_OVER2 : -LMath::PI_OVER2 );
            *z_ = -atan2( -rot.m10, rot.m11 );
            return false;
        }

        if ( rot.m22 < 0 )
        {
            *x_ = LMath::PI - *x_;
        }

        *z_ = atan2( rot.m01, rot.m00 );


        //*x_ = -*x_;
        //*y_ = -*y_;
        //*z_ = -*z_;

        return true;
    }

	//----------------------------------------------------------------------
	// ● クォータニオンをY,Z,X回転に分解する
	//----------------------------------------------------------------------
    bool Quaternion::factoringYZX( lnFloat* y_, lnFloat* z_, lnFloat* x_, const Quaternion& qua1_ )
    {
        // クォータニオン→行列
        Quaternion input_q( qua1_ );
        input_q.normalize();
        Matrix rot( input_q );

        //ジンバルロック判定
        if ( rot.m10 > 1 - 1.0e-4 || rot.m10 < -1 + 1.0e-4 )
        {
            *y_ = 0;
            *z_ = ( rot.m10 < 0 ? LMath::PI_OVER2 : -LMath::PI_OVER2 );
            *x_ = -atan2( -rot.m21, rot.m22 );
            return false;
        }

        *z_ = -asin(rot.m10);
        
        *y_ = asin( rot.m20 / cos( *z_ ) );

        //ジンバルロック判定(漏れ対策)
        if ( LMath::isNaN( *y_ ) )
        {
            *y_ = 0;
            *z_ = ( rot.m10 < 0 ? LMath::PI_OVER2 : -LMath::PI_OVER2 );
            *x_ = -atan2( -rot.m21, rot.m22 );
            return false;
        }

        if ( rot.m00 < 0 )
        {
            *y_ = LMath::PI - *y_;
        }
        
        *x_ = atan2( rot.m12, rot.m11 );

        //*x_ = -*x_;
        //*y_ = -*y_;
        //*z_ = -*z_;

        return true;
    }
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool Quaternion::factoringEuler( Vector3* out, const Quaternion& qua1, RotationOrder order )
	{
		switch ( order )
		{
			case RotationOrder_XYZ: 
				return factoringXYZ( &out->x, &out->y, &out->z, qua1 ); break;
			//case LN_ROTORDER_XZY: 
			//	 break;
			//case LN_ROTORDER_YXZ: 
			//	 break;
			case RotationOrder_YZX: 
				return factoringYZX( &out->y, &out->z, &out->x, qua1 ); break;
			case RotationOrder_ZXY: 
				return factoringZXY( &out->z, &out->x, &out->y, qua1 ); break;
			//case LN_ROTORDER_ZYX: 
			//	break;
			default:
				LN_ERR2_ASSERT( 0 && "invalid rot order." );
				break;
		}
		return false;
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
