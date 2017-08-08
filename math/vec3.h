/////////////////////////////////////////////////////////////////////////////
// 
// File:	Vec3.h
// Desc:	Snap 3d vector class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

///////////////////////////////////////////////////////////////////////////////
// 3d vector
///////////////////////////////////////////////////////////////////////////////
class Vec3
{
public:

	Vec3() {}
	Vec3(float _x, float _y, float _z) {x=_x; y=_y; z=_z;}

	float LenSqr() const { return x*x+y*y+z*z;}
	float Length() const { return std::sqrt(LenSqr());}
	float Norm() { float l = Length();  float il=1.f/l; x*=il; y*=il; z*=il; return l;}
	Vec3 &Negate() {x=-x; y=-y; z=-z; return *this;}

	float GetHeading();
	void RotateX(float theta);
	void RotateY(float theta);
	void RotateZ(float theta);
	void RotateAxis(Vec3 &axis, float theta);

	static Vec3 DirVector(float theta, float phi);
	static Vec3 HeadingVector(float theta);
	static void BuildBasis(Vec3 &input, Vec3 &right, Vec3 &up);
	static Vec3 Lerp(Vec3& start, Vec3& end,float alpha);

	Vec3 operator*=(float f) { x*=f; y*=f; z*=f; return *this; }
	Vec3 operator*(float f) const { return Vec3(f*x,f*y,f*z); }
	Vec3 operator-() const { return Vec3(-x, -y, -z); }
	Vec3 operator+=(const Vec3 v) { x+=v.x; y+=v.y; z+=v.z; return *this; }

	float x,y,z;
};

inline Vec3 operator*(float f, const Vec3& v)
{
	return v.operator*(f);
}

inline Vec3 operator-(const Vec3& a, const Vec3& b)
{
	return Vec3(a.x-b.x, a.y-b.y, a.z-b.z);
}

inline Vec3 operator+(const Vec3& a, const Vec3& b)
{
	return Vec3(a.x+b.x, a.y+b.y, a.z+b.z);
}

///////////////////////////////////////////////////////////////////////////////
// Dot product
///////////////////////////////////////////////////////////////////////////////
inline float Dot(const Vec3 &v1, const Vec3 &v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

///////////////////////////////////////////////////////////////////////////////
// Cross product
///////////////////////////////////////////////////////////////////////////////
inline Vec3 Cross(const Vec3 &v1, const Vec3 &v2)
{
	Vec3 v3;

	v3.x = v1.y*v2.z - v2.y*v1.z;
	v3.y = v1.z*v2.x - v2.z*v1.x;
	v3.z = v1.x*v2.y - v2.x*v1.y;

	return v3;
}

///////////////////////////////////////////////////////////////////////////////
// dot product operator
///////////////////////////////////////////////////////////////////////////////
inline float operator %(const Vec3 v1, const Vec3 v2)
{
	return Dot(v1,v2);
}

///////////////////////////////////////////////////////////////////////////////
// cross product operator
///////////////////////////////////////////////////////////////////////////////
inline Vec3 operator ^(const Vec3 &v1, const Vec3 &v2)
{
	return Cross(v1,v2);
}

///////////////////////////////////////////////////////////////////////////////
// GetHeading
// return an angle in Degrees given a direction vector using only XY components
// result is 0-360 CW from Y axis
///////////////////////////////////////////////////////////////////////////////
inline float Vec3::GetHeading()
{
	float heading = std::atan2f(x,y);
	if (heading < 0)
		heading += (float)M_PI*2.0f;

	return heading;
}

///////////////////////////////////////////////////////////////////////////////
// RotateX
// rotate 'this' vector around X axis by theta degrees
// rotation is in CCW direction when looking down the axis
// (Y axis rotates towards Z axis)
///////////////////////////////////////////////////////////////////////////////
inline void Vec3::RotateX(float theta)
{
	float c = std::cosf(theta);
	float s = std::sinf(theta);
	float newY = c * y - s * z;
	z = s * y + c * z;
	y = newY;
}

///////////////////////////////////////////////////////////////////////////////
// RotateY
// rotate 'this' vector around Y axis by theta degrees
// rotation is in CCW direction when looking down the axis
// (Z axis rotates towards X axis)
///////////////////////////////////////////////////////////////////////////////
inline void Vec3::RotateY(float theta)
{
	float c = std::cosf(theta);
	float s = std::sinf(theta);
	float newX = c * x + s * z;
	z = -s * x + c * z;
	x = newX;
}

///////////////////////////////////////////////////////////////////////////////
// RotateY
// rotate 'this' vector around Y axis by theta degrees
// rotation is in CW direction when looking down the axis
// NOTE: this is different from RotateX and RotateY
// (Y axis rotates towards X axis)
///////////////////////////////////////////////////////////////////////////////
inline void Vec3::RotateZ(float theta)
{
	float c = std::cosf(theta);
	float s = std::sinf(theta);
	float newX = c * x + s * y;
	y = -s * x + c * y;
	x = newX;
}

///////////////////////////////////////////////////////////////////////////////
// RotateAxis
// rotate 'this' vector around given axis by theta degrees
// rotation is in CCW direction when looking down the axis
// (Z axis rotates towards X axis)
///////////////////////////////////////////////////////////////////////////////
inline void Vec3::RotateAxis(Vec3 &axis, float theta)
{
	float c = std::cosf(theta);
	float s = std::sinf(theta);
	Vec3 r = *this;

	*this = c * r + (1-c)*(r % axis)*axis + s * (axis ^ r);
}


///////////////////////////////////////////////////////////////////////////////
// DirVector
// return a direction vector given two angles in Degrees
// computes a unit vector using spherical coordinates
// theta is azimuth (horizontal angle from 12:00 clockwise)
// phi is elevation (vertical angle positive is up)
///////////////////////////////////////////////////////////////////////////////
inline Vec3 Vec3::DirVector(float theta, float phi)
{
	float cosPhi = std::cosf(phi);
	return Vec3( std::sinf(theta)*cosPhi, 
				 std::cosf(theta)*cosPhi, 
				 std::sinf(phi) );
}


///////////////////////////////////////////////////////////////////////////////
// HeadingVector
// return a heading vector in the XY plane from an angle in Degrees
///////////////////////////////////////////////////////////////////////////////
inline Vec3 Vec3::HeadingVector(float theta)
{
	return Vec3( std::sinf(theta), 
				 std::cosf(theta), 
				 0);
}

///////////////////////////////////////////////////////////////////////////////
// Lerp between two vectors
///////////////////////////////////////////////////////////////////////////////
inline Vec3 Vec3::Lerp(Vec3& start, Vec3& end,float alpha)
{
	return (1.0f - alpha) * start + alpha * end;
}

///////////////////////////////////////////////////////////////////////////////
// BuildBasis
// given a direction vector (assumed normal), builds two orthonormal vectors
// to make a 3d basis set of axes, one to the 'right' and one 'up'
///////////////////////////////////////////////////////////////////////////////
inline void Vec3::BuildBasis(Vec3 &forward, Vec3 &right, Vec3 &up)
{
	// take the smallest component, zero it out, and swap the other two, negating the first
	if (std::fabsf(forward.x) <= std::fabsf(forward.y))
	{
		if (std::fabsf(forward.x) <= std::fabsf(forward.z))
			right = Vec3(0, -forward.z, forward.y);
		else if (std::fabsf(forward.z) <= std::fabsf(forward.y))
			right = Vec3(-forward.y, forward.x, 0);
	}
	else if (std::fabsf(forward.y) <= fabsf(forward.z))
	{
		right = Vec3(-forward.z, 0, forward.x);
	}
	else right = Vec3(-forward.y, forward.x, 0);

	right.Norm();

	up = Cross(forward, right);
}
