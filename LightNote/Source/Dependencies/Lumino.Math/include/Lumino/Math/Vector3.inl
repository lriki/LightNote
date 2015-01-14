
namespace Lumino
{

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3::Vector3()
{
	X = Y = Z = 0.0f;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3::Vector3(float x_, float y_, float z_)
{
	X = x_;
	Y = y_;
	Z = z_;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline void Vector3::Set(float x_, float y_, float z_)
{
	X = x_;
	Y = y_;
	Z = z_;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline float Vector3::GetLengthSquared() const
{
	return X * X + Y * Y + Z * Z;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3& Vector3::operator += (const Vector3& v)
{
	X += v.X;
	Y += v.Y;
	Z += v.Z;
	return (*this);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3& Vector3::operator += (float v)
{
	X += v;
	Y += v;
	Z += v;
	return (*this);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3& Vector3::operator -= (const Vector3& v)
{
	X -= v.X;
	Y -= v.Y;
	Z -= v.Z;
	return (*this);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3& Vector3::operator -= (float v)
{
	X -= v;
	Y -= v;
	Z -= v;
	return (*this);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3& Vector3::operator *= (const Vector3& v)
{
	X *= v.X;
	Y *= v.Y;
	Z *= v.Z;
	return (*this);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3& Vector3::operator *= (float v)
{
	X *= v;
	Y *= v;
	Z *= v;
	return (*this);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3& Vector3::operator /= (const Vector3& v)
{
	X /= v.X;
	Y /= v.Y;
	Z /= v.Z;
	return (*this);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3& Vector3::operator /= (float v)
{
	v = 1.0f / v;
	X *= v;
	Y *= v;
	Z *= v;
	return (*this);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3 operator + (const Vector3& v1, const Vector3& v2)
{
	return Vector3(
		v1.X + v2.X,
		v1.Y + v2.Y,
		v1.Z + v2.Z);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3 operator + (const Vector3& v1, float v2)
{
	return Vector3(
		v1.X + v2,
		v1.Y + v2,
		v1.Z + v2);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3 operator + (float v1, const Vector3& v2)
{
	return Vector3(
		v1 + v2.X,
		v1 + v2.Y,
		v1 + v2.Z);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3 operator - (const Vector3& v1, const Vector3& v2)
{
	return Vector3(
		v1.X - v2.X,
		v1.Y - v2.Y,
		v1.Z - v2.Z);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3 operator - (const Vector3& v1, float v2)
{
	return Vector3(
		v1.X - v2,
		v1.Y - v2,
		v1.Z - v2);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3 operator - (float v1, const Vector3& v2)
{
	return Vector3(
		v1 - v2.X,
		v1 - v2.Y,
		v1 - v2.Z);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3 operator * (const Vector3& v1, const Vector3& v2)
{
	return Vector3(
		v1.X * v2.X,
		v1.Y * v2.Y,
		v1.Z * v2.Z);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3 operator * (const Vector3& v1, float v2)
{
	return Vector3(
		v1.X * v2,
		v1.Y * v2,
		v1.Z * v2);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3 operator * (float v1, const Vector3& v2)
{
	return Vector3(
		v1 * v2.X,
		v1 * v2.Y,
		v1 * v2.Z);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3 operator / (const Vector3& v1, const Vector3& v2)
{
	return Vector3(
		v1.X / v2.X,
		v1.Y / v2.Y,
		v1.Z / v2.Z);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3 operator / (const Vector3& v1, float v2)
{
	v2 = 1.0f / v2;
	return Vector3(
		v1.X * v2,
		v1.Y * v2,
		v1.Z * v2);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3 operator / (float v1, const Vector3& v2)
{
	return Vector3(
		v1 / v2.X,
		v1 / v2.Y,
		v1 / v2.Z);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline Vector3 operator - (const Vector3& v1)
{
	return Vector3(
		-v1.X,
		-v1.Y,
		-v1.Z);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool Vector3::operator == (const Vector3& v) const
{
	return (X == v.X && Y == v.Y && Z == v.Z);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool Vector3::operator != (const Vector3& v) const
{
	return (X != v.X || Y != v.Y || Z != v.Z);
}

} // namespace Lumino
