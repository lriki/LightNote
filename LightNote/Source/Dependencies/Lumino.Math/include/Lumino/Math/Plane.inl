
namespace Lumino
{

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool Plane::CheckInside(const Vector3& point) const
{
	return (Plane::DotCoord(*this, point) < 0.0f);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool Plane::CheckInside(const Vector3& point, float radius) const
{
	return (Plane::DotCoord(*this, point) - radius < 0.0f);
}

} // namespace Lumino
