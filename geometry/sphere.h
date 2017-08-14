#include "surface.h"
#include "shaders/shader.h"

class Sphere:public Surface {
	Vec3 ctr;
	float fRadius;
public:
	Sphere::Sphere(Vec3 newCtr, float newRadius, Shader *pMaterial=0)
	{
		ctr=newCtr;
		fRadius=newRadius;
		if (pMaterial)
			SetShader(pMaterial);
	}

	// sign of b is opposite that of the quadratic formula
	bool Sphere::Intersect(const Ray &r, float &tt)
	{
		float b,c,d;
		Vec3 V=ctr - r.pos;

		b = V % r.dir;
		c = V % V - fRadius*fRadius;
		if (c > 0 && b < 0)
			return false;

		d = b*b - c;
		if (d<=0)
			return false;

		if (c < 0)				//ray originates inside the sphere
			tt = b + sqrtf(d);
		else
			tt = b - sqrtf(d);	//return the smallest positive root
		return true;
	}

	RGBAf Sphere::Shade(ShaderContext &sc)
	{
		RGBAf RGBAf(0.f,0.f,0.f,0.f);
		//calculate the normal
		sc.norm = sc.pos - ctr;
		sc.norm *= 1.f/fRadius;
		sc.geonorm = sc.norm;
		sc.u = (float)(atan2f(sc.geonorm.x,sc.geonorm.z)/M_PI+1.f)*.5f;
		sc.v= (float)(acosf(sc.geonorm.y)/M_PI+1.f)*.5f;
		sc.s=sc.u;
		sc.t=sc.v;

		return pSurfaceShader->Shade(sc);
	}
};
