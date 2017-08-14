#pragma once

#include "scene/scene.h"
#include "math/ray.h"
#include "rt.h"
class Surface;

class ShaderContext {
public:
	Vec3 pos;		//position of intersection
	Vec3 norm;		//shading normal at point of intersection
	Vec3 geonorm;	//surface normal at point of intersection
	Ray input;				//the incoming ray
	float tray;				//how far along the ray it struck the object
	float u,v;	//surface parameters
	float s,t;	//texture parameters
	Vec3 dPdu;		//partial derivative of P with respect to u
	Vec3 dPdv;		//partial derivative of P with respect to v
	float du,dv;			//derivatives of surface parameters
	float time;				//what time this sample is being taken
	Surface *obj;			//object that was hit
	RayTracer *pRayTracer;	//the raytracer - so we can cast more rays while shading

	RGBAf ambient()
	{
		int i,n=pRayTracer->GetScene()->GetNumLights();
		RGBAf c,result(0,0,0,0);

		for (i=0; i<n; i++) {
			LIGHT *l=pRayTracer->GetScene()->GetLight(i);
			c = l->Ambient;
			result += c;
		}

		return result;
	}

	RGBAf diffuse(Vec3 &Norm)
	{
		int i,n=pRayTracer->GetScene()->GetNumLights();
		RGBAf result(0,0,0,0);
		Ray r;

		for (i=0; i<n; i++) {
			LIGHT &l=*pRayTracer->GetScene()->GetLight(i);
			//check for shadow...
			if (!pRayTracer->IsShadowed(pos, l)) {
				//calculate diffuse factor
				Vec3 light_dir = l.Direction;
				if (l.Type == LIGHT_POINT)
				{
					light_dir = l.Position - pos;
					light_dir.Norm();
				}
				float shade= Norm % light_dir;
				if (shade < 0)
					shade=0;
				else 
					result += shade * l.Diffuse;
			}
		}
		return result;
	}

	RGBAf specular(Vec3 &Norm, Vec3 &V, float fRoughness)
	{
		int i,n=pRayTracer->GetScene()->GetNumLights();
		RGBAf result(0,0,0,0);
		float fPower=1.f/fRoughness;

		for (i=0; i<n; i++) {
			LIGHT &l=*pRayTracer->GetScene()->GetLight(i);
			if (!pRayTracer->IsShadowed(pos, l)) {
				Vec3 light_dir = l.Direction;
				//calculate specular factor
				if (l.Type == LIGHT_POINT)
				{
					light_dir = l.Position - pos;
					light_dir.Norm();
				}
				Vec3 Half=V+light_dir;
				Half.Norm();
				float spec= Half % Norm;
				if (spec < 0)
					spec=0;
				else {
					spec=powf(spec,fPower);
					result+=spec*l.Specular;
				}
			}
		}
		return result;
	}
};

class Shader {
protected:
	MATERIAL coeffs;
public:
	Shader(MATERIAL &mat) {coeffs=mat;}
	virtual RGBAf Shade(ShaderContext &sc)=0;
};

inline float comp(Vec3 &vec, float i)
{
	int index=(int)i;
	return (&vec.x)[index];
}

inline void setcomp(Vec3 &vec, float i, float x)
{
	int index=(int)i;
	(&vec.x)[index]=x;
}

inline float xcomp(Vec3 &vec)
{
	return vec.x;
}

inline float ycomp(Vec3 &vec)
{
	return vec.y;
}

inline float zcomp(Vec3 &vec)
{
	return vec.z;
}

inline void setxcomp(Vec3 &vec, float x)
{
	vec.x=x;
}

inline void setycomp(Vec3 &vec, float y)
{
	vec.y=y;
}

inline void setzcomp(Vec3 &vec, float z)
{
	vec.z=z;
}

inline float length(Vec3 &vec)
{
	return vec.Length();
}

inline float distance(Vec3 &p0, Vec3 &p1)
{
	Vec3 vec=p1-p0;
	return length(vec);
}

inline float ptlined(Vec3 &p0, Vec3 &p1, Vec3 &q)
{
	Vec3 vec=p1-p0, d=q-p0;
	float dot=vec % d;

	if (dot <0.f)
		return ::distance(p0,q);

	float v2=vec.LenSqr();

	float p=dot/v2;

	if (p > 1.f)
		return ::distance(p1,q);

	float dot2= d.LenSqr();
	return sqrtf(dot2 - dot*p);
}

inline void normalize(Vec3 &V)
{
	V.Norm();
}

inline Vec3 reflect(Vec3 &Incident, Vec3 &Norm)
{
	Vec3 R;

	R = Incident - 2.f * (Norm % Incident) * Norm;
	return R;
}

inline Vec3 refract(Vec3 &Incident, Vec3 &Norm, float eta)
{
	return Incident;		//TODO - calculate refracted vector...
}

inline Vec3 faceforward(Vec3& Norm, Vec3& Incident, Vec3& Nref)
{
	if (Nref % Incident > 0)
		return -Norm;
	return Norm;
}

inline Vec3 faceforward(Vec3 &Norm, Vec3 &Incident)
{
	return ( (Norm % Incident) > 0) ? -Norm : Norm;
}

float const LOG05 = -0.693147180559945f;  /* log(0.5) */

inline float bias(float b, float x)
{
    return powf(x, logf(b)/LOG05);
}


inline float clamp(float x, float a, float b)
{
    return (x < a ? a : (x > b ? b : x));
}

inline float gain(float g, float x)
{
	if (x < 0.5f)
		return bias(1.f-g, 2.f*x)*.5f;
	else
		return 1.f - bias(1.f-g, 2.f - 2.f*x)*.5f;
}

inline float gammacorrect(float gamma, float x)
{
    return powf(x, 1/gamma);
}

inline float mod(float a, float b)
{
    a=fmodf(a,b);
    if (a < 0.f)
        a += b;
    return a;
}

template <class T>
inline T mix(T x, T y, float alpha)
{
	return x + (y-x)*alpha;
}

inline float step(float a, float x)
{
    return (float)(x >= a);
}

inline float smoothstep(float a, float b, float x)
{
    if (x < a)
        return 0.f;
    if (x >= b)
        return 1.f;
    x = (x - a)/(b - a); /* normalize to [0:1] */
    return (x*x * (3.f - 2.f*x));
}


inline float spline(float x, int nknots, float *knot)
{
    int span;
    int nspans = nknots - 3;
    float /*c0,*/ c1, c2, c3;	/* coefficients of the cubic.*/

    if (nspans < 1) {  /* illegal */
        //fprintf(stderr, "Spline has too few knots.\n");
        return 0;
    }

    /* Find the appropriate 4-point span of the spline. */
    x = clamp(x, 0, 1) * nspans;
    span = (int) x;
    if (span >= nknots - 3)
        span = nknots - 3;
    x -= span;
    knot += span;

    /* Evaluate the span cubic at x using Horner's rule. */
    c3 =  -.5f * knot[0] +  1.5f * knot[1]
       + -1.5f * knot[2] +   .5f * knot[3];
    c2 =   1.f * knot[0] + -2.5f * knot[1]
       +   2.f * knot[2] +  -.5f * knot[3];
    c1 =  -.5f * knot[0] +   .5f * knot[2];

    return ((c3*x + c2)*x + c1)*x + knot[1];
}


float noise(float);
//float noise(D3DXVECTOR2);
float noise(Vec3);

inline float smoothpulse(float e0, float e1, float e2, float e3, float x)
{
	return smoothstep(e0,e1,x) - smoothstep(e2,e3,x);
}

inline float smoothpulsetrain(float e0, float e1, float e2, float e3, float period, float x)
{
	return smoothpulse(e0, e1, e2, e3, mod(x, period));
}

inline float turbulence(Vec3 vec, float freq)
{
	float tt;
	Vec3 vecout;

	for (tt = 0. ; freq >= 1. ; freq /= 2) {
		vecout = freq * vec;
		tt += fabs(noise(vecout)) / freq;
	}
	return tt;
}
