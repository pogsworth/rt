#include "surface.h"
#include "shaders/shader.h"

class Plane:public Surface {
	D3DXPLANE plane;
public:
	Plane(D3DXPLANE newplane, Shader *pShader=0)
	{
		plane=newplane;
		if (pMaterial)
			SetShader(*pShader);

		if (plane.a > plane.b) {
			if (plane.a > plane.c) {
				udir=inplane.y;
				sc.v=inplane.z;
			} else {
				sc.u=inplane.x;
				sc.v=inplane.y;
			}
		} else if (sc.norm.y > sc.norm.z) {
			sc.u = inplane.x;
			sc.v = inplane.z;
		} else {
			sc.u = inplane.x;
			sc.v = inplane.y;
		}
	}

	bool Intersect(const Ray r, float &t)
	{
		float vd,v0;

		vd=(plane.a*r.dir.x + plane.b*r.dir.y + plane.c*r.dir.z);
		if (vd>=0)        //ray parallel or behind plane
			return false;
		v0=-(plane.a*r.pos.x + plane.b*r.pos.y + plane.c*r.pos.z + plane.d);
		t = v0/vd;
		return true;
	}

	COLOR Shade(ShaderContext &sc)
	{
		sc.norm = plane;
		Vec3 toplane = sc.norm*D3DXVec3Dot(&sc.pos, &sc.norm);
		Vec3 inplane = sc.pos - toplane;

		pSurfaceShader->Shade(sc);
	}
};
