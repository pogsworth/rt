
#pragma once

#include "vec3.h"
#include <float.h>

///////////////////////////////////////////////////////////////////////////////
// Ray class
///////////////////////////////////////////////////////////////////////////////
class Ray
{
public:

	Ray() {}
	Ray(Vec3 &_pos, Vec3 &_dir) {pos = _pos; dir = _dir;}

	bool IntersectTriangle(Vec3&, Vec3&, Vec3&, float&, float&, float&);

	Vec3 pos,dir;
};

inline bool Ray::IntersectTriangle(Vec3& vert0, Vec3& vert1, Vec3& vert2, float& t, float& u, float& v)
{
	float edge1[3];
	float edge2[3];
	float pvec[3];
	float det, invDet;

	t = 0; u = 0; v = 0;
	// find vectors for two edges sharing vert0
	edge1[0] = vert1.x - vert0.x;
	edge1[1] = vert1.y - vert0.y;
	edge1[2] = vert1.z - vert0.z;
	edge2[0] = vert2.x - vert0.x;
	edge2[1] = vert2.y - vert0.y;
	edge2[2] = vert2.z - vert0.z;

	// begin calculating determinant - also used to calculate u param
	pvec[0] = dir.y * edge2[2] - dir.z * edge2[1];
	pvec[1] = dir.z * edge2[0] - dir.x * edge2[2];
	pvec[2] = dir.x * edge2[1] - dir.y * edge2[0];

	// if determinant is near zero, ray lies in plane of triangle
	det = edge1[0]*pvec[0] + edge1[1]*pvec[1] + edge1[2]*pvec[2];
	if (det > -FLT_EPSILON && det < FLT_EPSILON)
		return false;

	invDet = 1.0f / det;

	float tvec[3];
	// calculate distance from vert0 to ray origin
	tvec[0] = pos.x - vert0.x;
	tvec[1] = pos.y - vert0.y;
	tvec[2] = pos.z - vert0.z;

	// calculate U paramater and test bounds
	u = (tvec[0]*pvec[0] + tvec[1]*pvec[1] + tvec[2]*pvec[2]) * invDet;
	if( u < 0.0f || u > 1.0f )
		return false;

	// prepare to test V parameter
	pvec[0] = tvec[1] * edge1[2] - tvec[2] * edge1[1];
	pvec[1] = tvec[2] * edge1[0] - tvec[0] * edge1[2];
	pvec[2] = tvec[0] * edge1[1] - tvec[1] * edge1[0];

	// calculate V parameter and test bounds
	v = (dir.x*pvec[0] + dir.y*pvec[1] + dir.z*pvec[2]) * invDet;
	if( v < 0.0f || u + v > 1.0f )
		return false;

	// calculate t, ray intersects triangle
	t = (edge2[0]*pvec[0] + edge2[1]*pvec[1] + edge2[2]*pvec[2]) * invDet;

	// throw out anything behind the ray origin
	if( t < 0.0f )
		return false;

	//// test if it is a backface
	//if( det < 0.0f )
	//    return -1;

	return true;
}


///////////////////////////////////////////////////////////////////////////////
// HitInfo class for intersection results
///////////////////////////////////////////////////////////////////////////////
class HitInfo
{
public:
	float Dist;
	float u,v;
	int faceIndex;
	Vec3 pos;
	Vec3 norm;
	Vec3 faceNorm;
	int surfaceCode;
};
