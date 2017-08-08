/////////////////////////////////////////////////////////////////////////////
// 
// File:	Matrix4.h
// Desc:	Snap 4x4 Matrix class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once


#include <memory.h>
#include "vec3.h"
#include <float.h>

///////////////////////////////////////////////////////////////////////////////
// 4x4 matrix class
///////////////////////////////////////////////////////////////////////////////
class Matrix4
{
public:
	Matrix4() {Identity();}
	Matrix4(Matrix4& m) {memcpy(this,&m,sizeof(Matrix4));}
	void Identity() {memset(m, 0, sizeof(m)); m[0][0]=m[1][1]=m[2][2]=m[3][3]=1.f;}
//		operator float*() {return &m[0][0];}
	//Vec4 GetRow(int i) {return *(Vec4*)&m[i][0];}
	//Vec4 GetCol(int i) {Vec4 v(m[0][i],m[1][i],m[2][i],m[3][i]); return v;}
	Vec3 GetRowVec3(int i) {return *(Vec3*)&m[i][0];}
	Vec3 GetColVec3(int i) {return Vec3(m[0][i],m[1][i],m[2][i]);}
	void SetRowVec3(int i, Vec3 &v) { m[i][0]=v.x; m[i][1]=v.y; m[i][2]=v.z; }
	void SetColVec3(int i, Vec3 &v) { m[0][i]=v.x; m[1][i]=v.y; m[2][i]=v.z; }
	void operator *=(Matrix4& m1);

	// all of these operators multiply-in their operation to the current matrix
	// they preserve the original contents of the matrix and transform it
	// using right-multiplication
	void Transpose();
	void Translate(float x, float y, float z);
	void Translate(Vec3 &t);
	void Scale(float fSx, float fSy, float fSz);
	void Scale(Vec3 &s);
	void RotateX(float fX);
	void RotateY(float fY);
	void RotateZ(float fZ);
	void RotateAxis(Vec3 &r, float a);
	void ProjectLH(float FOV, float NearZ, float FarZ, float fAspect = 1.f, float fOrthoScale = 1.f, float HorizCenter = 0, float VertCenter = 0);
	void ProjectRH(float FOV, float NearZ, float FarZ, float fAspect = 1.f, float fOrthoScale = 1.f, float HorizCenter = 0, float VertCenter = 0);

	// Get Euler Angles (pitch, roll, yaw)
	Vec3 GetRotation();
	void GetRotationAxisAngle(Vec3 &r, float &a);
	void FastInverse();		//assumes only translates, rotates and reflections
	bool Inverse();			//assumes only translates, rotates and reflections & scales
	Vec3 ProjectCoord(Vec3 v);

	friend Vec3 operator *(Vec3 &v, Matrix4 &m);
	friend Vec3 operator *(Matrix4 &m, Vec3 &v);
	friend Vec3 operator *=(Vec3 &v, Matrix4 &m);

private:
	float m[4][4];
};

///////////////////////////////////////////////////////////////////////////////
// catenates a transformation using right-multiplication
///////////////////////////////////////////////////////////////////////////////
inline void Matrix4::operator *=(Matrix4& m1)
{
	float row[4];

	for (int i=0; i<4; i++)
	{
		row[0] = m[i][0]*m1.m[0][0] + m[i][1]*m1.m[1][0] + m[i][2]*m1.m[2][0] + m[i][3]*m1.m[3][0];
		row[1] = m[i][0]*m1.m[0][1] + m[i][1]*m1.m[1][1] + m[i][2]*m1.m[2][1] + m[i][3]*m1.m[3][1];
		row[2] = m[i][0]*m1.m[0][2] + m[i][1]*m1.m[1][2] + m[i][2]*m1.m[2][2] + m[i][3]*m1.m[3][2];
		row[3] = m[i][0]*m1.m[0][3] + m[i][1]*m1.m[1][3] + m[i][2]*m1.m[2][3] + m[i][3]*m1.m[3][3];

		m[i][0]=row[0];
		m[i][1]=row[1];
		m[i][2]=row[2];
		m[i][3]=row[3];
	}
}

///////////////////////////////////////////////////////////////////////////////
// multiplication operator
///////////////////////////////////////////////////////////////////////////////
inline Matrix4 operator *(Matrix4& m1, Matrix4 &m2)
{
	Matrix4 m=m1;
	m*=m2;
	return m;
}


///////////////////////////////////////////////////////////////////////////////
// translates 'this' matrix by vector t
///////////////////////////////////////////////////////////////////////////////
inline void Matrix4::Translate(Vec3 &t)
{
	Translate(t.x,t.y,t.z);
}

///////////////////////////////////////////////////////////////////////////////
// translates 'this' matrix by three floats
///////////////////////////////////////////////////////////////////////////////
inline void Matrix4::Translate(float x, float y, float z)
{
	m[3][0]+=x;
	m[3][1]+=y;
	m[3][2]+=z;
}

///////////////////////////////////////////////////////////////////////////////
// translates 'this' matrix by vector t
///////////////////////////////////////////////////////////////////////////////
inline void Matrix4::Scale(float fSx, float fSy, float fSz)
{
	for (int i=0; i<4; i++)
	{
		m[i][0] *= fSx;
		m[i][1] *= fSy;
		m[i][2] *= fSz;
	}
}

///////////////////////////////////////////////////////////////////////////////
// translates 'this' matrix by vector t
///////////////////////////////////////////////////////////////////////////////
inline void Matrix4::Scale(Vec3& s)
{
	Scale(s.x, s.y, s.z);
}

///////////////////////////////////////////////////////////////////////////////
// rotates 'this' matrix by angle a around X axis
///////////////////////////////////////////////////////////////////////////////
inline void Matrix4::RotateX(float a)
{
	int i;
	float s = sinf(a);
	float c = cosf(a);
	float t;

	for (i=0; i<4; i++)
	{
		t = m[i][1];
		m[i][1] =  c * m[i][1] - s * m[i][2];
		m[i][2] =  s * t       + c * m[i][2];
	}
}

///////////////////////////////////////////////////////////////////////////////
// rotates 'this' matrix by angle a around Y axis
///////////////////////////////////////////////////////////////////////////////
inline void Matrix4::RotateY(float a)
{
	int i;
	float s = sinf(a);
	float c = cosf(a);
	float t;

	for (i=0; i<4; i++)
	{
		t = m[i][0];
		m[i][0] =  c * m[i][0] + s * m[i][2];
		m[i][2] = -s * t       + c * m[i][2];
	}
}

///////////////////////////////////////////////////////////////////////////////
// rotates 'this' matrix by angle a around Z axis
///////////////////////////////////////////////////////////////////////////////
inline void Matrix4::RotateZ(float a)
{
	int i;
	float s = sinf(a);
	float c = cosf(a);
	float t;

	for (i=0; i<4; i++)
	{
		t = m[i][0];
		m[i][0] =  c * m[i][0] - s * m[i][1];
		m[i][1] =  s * t       + c * m[i][1];
	}
}

///////////////////////////////////////////////////////////////////////////////
// rotates 'this' matrix by angle a around arbitrary axis r
///////////////////////////////////////////////////////////////////////////////
inline void Matrix4::RotateAxis(Vec3 &r, float a)
{
	float c = cosf(a);
	float s = sinf(a);
	float t = 1 - c;

	float m00 = t*r.x*r.x+c;
	float m01 = t*r.x*r.y+s*r.z;
	float m02 = t*r.x*r.z-s*r.y;

	float m10 = t*r.x*r.y-s*r.z;
	float m11 = t*r.y*r.y+c;
	float m12 = t*r.y*r.z+s*r.x;

	float m20 = t*r.x*r.z+s*r.y;
	float m21 = t*r.y*r.z-s*r.x;
	float m22 = t*r.z*r.z+c;

	for (int i=0; i<4; i++)
	{
		float row[3];
		row[0] = m[i][0] * m00 + m[i][1] * m10 + m[i][2] * m20;
		row[1] = m[i][0] * m01 + m[i][1] * m11 + m[i][2] * m21;
		row[2] = m[i][0] * m02 + m[i][1] * m12 + m[i][2] * m22;

		m[i][0] = row[0];
		m[i][1] = row[1];
		m[i][2] = row[2];
	}
}

///////////////////////////////////////////////////////////////////////////////
// returns the rotation stored in this matrix as three angles (Euler angles)
// THIS FUNCTION IS BROKEN!!!
///////////////////////////////////////////////////////////////////////////////
inline Vec3 Matrix4::GetRotation()
{
	Vec3 result;
	float cosY = sqrtf(m[2][1]*m[2][1] + m[2][2]*m[2][2]);
	if (cosY > FLT_EPSILON)
	{
		result.x = atan2f(-m[2][1], m[2][2]);
		result.y = atan2f( m[2][0], cosY);
		result.z = atan2f(-m[1][0], m[0][0]);
	}
	else
	{
		result.x = atan2f(m[1][2], m[1][1]);
		result.y = atan2f(m[2][0], cosY);
		result.z = 0;
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////
// returns the rotation stored in this matrix as a vector and angle
///////////////////////////////////////////////////////////////////////////////
inline void Matrix4::GetRotationAxisAngle(Vec3 &r, float &a)
{
	float c = (m[0][0] + m[1][1] + m[2][2] - 1.f)/2.f;
	if (c > 1)
		c = 1.f;
	if (c < -1)
		c = -1.f;

	a = acosf(c);
	float s = sinf(a);
	if (std::fabsf(s) > .001f)
	{
		r.x = m[1][2] - m[2][1];
		r.y = m[2][0] - m[0][2];
		r.z = m[0][1] - m[1][0];
		r.Norm();
	}
	else
	{
		r.x = 0;
		r.y = 0;
		r.z = 1;
	}
}

///////////////////////////////////////////////////////////////////////////////
// transposes 'this' matrix
///////////////////////////////////////////////////////////////////////////////
inline void Matrix4::Transpose()
{
	float t;

	t = m[0][1];
	m[0][1] = m[1][0];
	m[1][0] = t;

	t = m[0][2];
	m[0][2] = m[2][0];
	m[2][0] = t;

	t = m[0][3];
	m[0][3] = m[3][0];
	m[3][0] = t;

	t = m[1][2];
	m[1][2] = m[2][1];
	m[2][1] = t;

	t = m[1][3];
	m[1][3] = m[3][1];
	m[3][1] = t;

	t = m[2][3];
	m[2][3] = m[3][2];
	m[3][2] = t;
}

///////////////////////////////////////////////////////////////////////////////
// Inverts 'this' matrix assuming only translations & rotations
///////////////////////////////////////////////////////////////////////////////
inline void Matrix4::FastInverse()
{
	float t;

	//transpose the rotation
	t = m[0][1];
	m[0][1] = m[1][0];
	m[1][0] = t;

	t = m[0][2];
	m[0][2] = m[2][0];
	m[2][0] = t;

	t = m[1][2];
	m[1][2] = m[2][1];
	m[2][1] = t;

	//invert the translation
	Vec3 v(m[3][0], m[3][1], m[3][2]);
	Vec3 axis=GetColVec3(0);
	m[3][0] = - (v % axis);
	axis=GetColVec3(1);
	m[3][1] = - (v % axis);
	axis=GetColVec3(2);
	m[3][2] = - (v % axis);
}

///////////////////////////////////////////////////////////////////////////////
// Inverts 'this' matrix assuming only translations & rotations & scales
///////////////////////////////////////////////////////////////////////////////
inline bool Matrix4::Inverse()
{
	// compute determinant of upper 3x3
	float det3x3 =	  m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
					- m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
					+ m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);

	if (std::fabsf(det3x3) < FLT_EPSILON)
		return false;

	// multiply the adjoint (transpose of matrix of cofactors) by the inverse determinant
	float invdet = 1.0f / det3x3;
	float inv[3][3];
	inv[0][0] = invdet * (m[1][1]*m[2][2] - m[1][2]*m[2][1]);
	inv[0][1] = invdet * (m[0][2]*m[2][1] - m[0][1]*m[2][2]);
	inv[0][2] = invdet * (m[0][1]*m[1][2] - m[0][2]*m[1][1]);
	inv[1][0] = invdet * (m[1][2]*m[2][0] - m[1][0]*m[2][2]);
	inv[1][1] = invdet * (m[0][0]*m[2][2] - m[0][2]*m[2][0]);
	inv[1][2] = invdet * (m[0][2]*m[1][0] - m[0][0]*m[1][2]);
	inv[2][0] = invdet * (m[1][0]*m[2][1] - m[1][1]*m[2][0]);
	inv[2][1] = invdet * (m[0][1]*m[2][0] - m[0][0]*m[2][1]);
	inv[2][2] = invdet * (m[0][0]*m[1][1] - m[0][1]*m[1][0]);
		
	// copy the inverted 3x3 back into the matrix
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			m[i][j] = inv[i][j];

	//invert the translation
	Vec3 v(m[3][0], m[3][1], m[3][2]);
	Vec3 axis=GetColVec3(0);
	m[3][0] = - (v % axis);
	axis=GetColVec3(1);
	m[3][1] = - (v % axis);
	axis=GetColVec3(2);
	m[3][2] = - (v % axis);

	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Multiplies in a left-handed Projection matrix defined by FOV, NearZ, FarZ and fAspect
///////////////////////////////////////////////////////////////////////////////
inline void Matrix4::ProjectLH(float FOV, float NearZ, float FarZ, float fAspect, float fOrthoScale, float HorizCenter, float VertCenter)
{
	int i;
	float t,fZRange,fCotFOV,fHorizCotFOV;

	fZRange = 1 / (FarZ - NearZ);

	if (FOV)
	{
		fZRange *= FarZ;
		fCotFOV = 1.f / tanf(FOV / 2);
		fHorizCotFOV = fAspect * fCotFOV;
		for (i=0; i<4; i++)
		{
			m[i][0] = m[i][0]*fHorizCotFOV + m[i][2]*HorizCenter;
			m[i][1] = m[i][1]*fCotFOV + m[i][2]*VertCenter;
			t = m[i][2];
			m[i][2] = (m[i][2] - m[i][3] * NearZ) * fZRange;
			m[i][3] = t;
		}
	}
	else
	{
		for (i=0; i<4; i++)
		{
			m[i][0] *= fAspect * fOrthoScale;
			m[i][1] *= fOrthoScale;
			m[i][2] = (m[i][2] - m[i][3] * NearZ) * fZRange;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Multiplies in a right-handed Projection matrix defined by FOV, NearZ, FarZ and fAspect
// Horiz/Vert Center is for shifting the center of projection
// they are specified in [-1,1] space, so 0,0 is default in the center of the screen
///////////////////////////////////////////////////////////////////////////////
inline void Matrix4::ProjectRH(float FOV, float NearZ, float FarZ, float fAspect, float fOrthoScale, float HorizCenter, float VertCenter)
{
	int i;
	float t,fZRange,fCotFOV,fHorizCotFOV;

	fZRange = 1 / (NearZ - FarZ);

	if (FOV)
	{
		fZRange *= FarZ;
		fCotFOV = 1.f / tanf(FOV / 2);
		fHorizCotFOV = fAspect * fCotFOV;
		for (i=0; i<4; i++)
		{
			m[i][0] = m[i][0]*fHorizCotFOV - m[i][2]*HorizCenter;
			m[i][1] = m[i][1]*fCotFOV - m[i][2]*VertCenter;
			t = m[i][2];
			m[i][2] = (m[i][2] + m[i][3] * NearZ) * fZRange;
			m[i][3] = -t;
		}
	}
	else
	{
		for (i=0; i<4; i++)
		{
			m[i][0] *= fAspect * fOrthoScale;
			m[i][1] *= fOrthoScale;
			m[i][2] = (m[i][2] + m[i][3] * NearZ) * fZRange;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// multiplies a row-vector by a matrix
///////////////////////////////////////////////////////////////////////////////
inline Vec3 operator *(Vec3 &v, Matrix4 &m)
{
	Vec3 result;

	result.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
	result.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
	result.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];

	return result;
}

///////////////////////////////////////////////////////////////////////////////
// multiply-in a row-vector by a matrix
///////////////////////////////////////////////////////////////////////////////
inline Vec3 operator *=(Vec3 &v, Matrix4 &m)
{
	Vec3 result;

	result.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
	result.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
	result.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];

	v = result;

	return result;
}

///////////////////////////////////////////////////////////////////////////////
// Multiplies a column-vector by a matrix
///////////////////////////////////////////////////////////////////////////////
inline Vec3 operator *(Matrix4 &m, Vec3 &v)
{
	Vec3 result;


	result.x = v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + m.m[0][3];
	result.y = v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + m.m[0][3];
	result.z = v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + m.m[0][3];

	return result;
}

///////////////////////////////////////////////////////////////////////////////
// transforms a vector by matrix and divides by w
///////////////////////////////////////////////////////////////////////////////
inline Vec3 Matrix4::ProjectCoord(Vec3 v)
{
	float rhw = 1.0f;
	if (m[2][3] != 0)
		rhw /= (v.z * m[2][3]);
	return Vec3(
					rhw * (v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0]),
					rhw * (v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1]),
					rhw * (v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2]) );
}
