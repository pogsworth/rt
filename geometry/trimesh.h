#include "surface.h"
#include "shaders/shader.h"

class TriMesh:public Surface {
	int iNumVerts;
	Vec3 *pVerts;
	Vec3 *pNormals;
	int iNumTris;
	short *pTriangles;
	short iTriangleHit;
	float uHit,vHit;
public:

	TriMesh::TriMesh(int iVertCount, Vec3 *pVertList, int iTriCount, short *pTriList, Shader *pMaterial=0)
	{
		iNumVerts=iVertCount;
		pVerts=pVertList;
		iNumTris=iTriCount;
		pTriangles=pTriList;
		iTriangleHit=0;
		pNormals=new Vec3[iTriCount];
		int i;
		short *pTris=pTriangles;
		Vec3 tri[3],v1,v2;
		for (i=0; i<iTriCount; i++) {
			tri[0]=pVerts[*pTris++];
			tri[1]=pVerts[*pTris++];
			tri[2]=pVerts[*pTris++];
			v1=tri[1]-tri[0];
			v2=tri[2]-tri[1];
			pNormals[i] = v1 ^ v2;
			pNormals[i].Norm();
		}
		if (pMaterial)
			SetShader(pMaterial);
	}

	bool TriMesh::RayTriIntersect(const Ray &r, const Vec3 *tri, float &tt, float &uu, float &vv)
	{
		Vec3 e1,e2,p,q,ss;
		float a,f;

		e1.x=tri[1].x-tri[0].x;
		e1.y=tri[1].y-tri[0].y;
		e1.z=tri[1].z-tri[0].z;

		e2.x=tri[2].x-tri[0].x;
		e2.y=tri[2].y-tri[0].y;
		e2.z=tri[2].z-tri[0].z;

		p = r.dir ^ e2;
		a = e1 % p;
		if (-FLT_EPSILON < a && a < FLT_EPSILON)
			return false;

		f=1.f/a;

		ss.x=r.pos.x-tri[0].x;
		ss.y=r.pos.y-tri[0].y;
		ss.z=r.pos.z-tri[0].z;

		uu=f * (ss % p);
		if (uu<0.f || uu>1.f)
			return false;

		q = ss ^ e1;
		vv=f*(r.dir % q);
		if (vv<0.f || uu+vv>1.f+FLT_EPSILON)
			return false;

		tt=f*(e2 % q);
		if (tt<0.f)
			return false;

		return true;
	}

	bool TriMesh::Intersect(const Ray &r, float &tt)
	{
		int i;
		Vec3 tri[3];
		short *pTriangle=pTriangles;
		float localT,uu,vv;
		bool bHit=false;

		for (i=0; i<iNumTris; i++) {
			tri[0]=pVerts[*pTriangle++];
			tri[1]=pVerts[*pTriangle++];
			tri[2]=pVerts[*pTriangle++];

			if (RayTriIntersect(r, tri, localT, uu, vv)) {
				if (localT < tt) {
					bHit=true;
					tt=localT;
					iTriangleHit=i;
					uHit=uu;
					vHit=vv;
				}
			}
		}
		return bHit;
	}

	RGBAf TriMesh::Shade(ShaderContext &sc)
	{
		RGBAf color(0.f,0.f,0.f,0.f);
		//calculate the normal
		Vec3 tri[3],v1,v2;

		short *pTri=pTriangles+iTriangleHit*3;
		sc.norm=pNormals[iTriangleHit];
		sc.u=uHit;
		sc.v=vHit;
		sc.s=sc.u;
		sc.t=sc.v;

		return pSurfaceShader->Shade(sc);
	}

};


inline TriMesh *Tetra(Vec3 p, Shader *pMat=0)
{
	const int iNumTetraVerts=4;
	const int iNumTetraTris=4;

	static Vec3 TetraVerts[iNumTetraVerts]={ 
		Vec3(-1.f,-1.f,-1.f),
		Vec3(1.f,-1.f,1.f),
		Vec3(1.f,1.f,-1.f),
		Vec3(-1.f,1.f,1.f)
	};

	for (int i=0; i<iNumTetraVerts; i++)
		TetraVerts[i] += p;

	static short TetraTris[3*iNumTetraTris]={
		0,3,2,
		1,2,3,
		2,1,0,
		3,0,1
	};

	return new TriMesh(iNumTetraVerts, TetraVerts, iNumTetraTris, TetraTris, pMat);
}


inline TriMesh *Octa(Vec3 p, Shader *pMat=0)
{
	const int iNumOctaVerts=6;
	const int iNumOctaTris=8;

	static Vec3 OctaVerts[iNumOctaVerts]={
		Vec3(1.f,0.f,0.f),
		Vec3(-1.f,0.f,0.f),
		Vec3(0.f,1.f,0.f),
		Vec3(0.f,-1.f,0.f),
		Vec3(0.f,0.f,1.f),
		Vec3(0.f,0.f,-1.f)
	};

	for (int i=0; i<iNumOctaVerts; i++)
		OctaVerts[i]+=p;

	static short OctaTris[3*iNumOctaTris]={
		0,5,2,		1,2,5,
		0,2,4,		1,5,3,
		0,3,5,		1,3,4,
		0,4,3,		1,4,2
	};

	return new TriMesh(iNumOctaVerts, OctaVerts, iNumOctaTris, OctaTris, pMat);
}


inline TriMesh *Cube(float fXScale=1.f, float fYScale=1.f, float fZScale=1.f, Shader *pMat=0)
{
	const int iNumCubeVerts=8;
	const int iNumCubeTris=2*6;

	static Vec3 CubeVerts[iNumCubeVerts]={
		Vec3(-1.f,-1.f,-1.f),
		Vec3( 1.f,-1.f,-1.f),
		Vec3(-1.f, 1.f,-1.f),
		Vec3( 1.f, 1.f,-1.f),
		Vec3(-1.f,-1.f, 1.f),
		Vec3( 1.f,-1.f, 1.f),
		Vec3(-1.f, 1.f, 1.f),
		Vec3( 1.f, 1.f, 1.f)
	};

	for (int i=0; i<iNumCubeVerts; i++) {
		CubeVerts[i].x*=fXScale;
		CubeVerts[i].y*=fYScale;
		CubeVerts[i].z*=fZScale;
	}
	static short CubeTris[3*iNumCubeTris]={
		0,2,1,		3,1,2,
		0,4,2,		6,2,4,
		0,1,4,		5,1,4,

		7,3,6,		2,6,3,
		7,5,3,		1,3,5,
		7,6,5,		4,5,6
	};

	return new TriMesh(iNumCubeVerts, CubeVerts, iNumCubeTris, CubeTris,pMat);
}


inline TriMesh *Icosa(Vec3 p, Shader *pMat=0)
{
	const float G=1.618034f;		//golden mean
	const int iNumIcosaVerts=12;
	const int iNumIcosaTris=20;

	static Vec3 IcosaVerts[iNumIcosaVerts]={
		Vec3(G,1.f,0.f),
		Vec3(-G,1.f,0.f),
		Vec3(-G,-1.f,0.f),
		Vec3(G,-1.f,0.f),
		Vec3(0.f,G,1.f),
		Vec3(0.f,G,-1.f),
		Vec3(0.f,-G,-1.f),
		Vec3(0.f,-G,1.f),
		Vec3(1.f,0.f,G),
		Vec3(-1.f,0.f,G),
		Vec3(-1.f,0.f,-G),
		Vec3(1.f,0.f,-G)
	};

	for (int i=0; i<iNumIcosaVerts; i++)
		IcosaVerts[i]+=p;

	static short IcosaTris[3*iNumIcosaTris]={
		0,5,4,
		0,4,8,
		0,8,3,
		0,3,11,
		0,11,5,
		2,6,7,
		2,7,9,
		2,9,1,
		2,1,10,
		2,10,6,
		6,11,3,
		11,6,10,
		10,5,11,
		5,10,1,
		1,4,5,
		4,1,9,
		9,8,4,
		8,9,3,
		3,8,7,
		7,6,3
	};

	return new TriMesh(iNumIcosaVerts, IcosaVerts, iNumIcosaTris, IcosaTris, pMat);
}


inline TriMesh *Dodeca(Vec3 p, Shader *pMat=0)
{
	const float G=1.618034f;		//golden mean
	const float GG=G-1;
	const int iNumDodecaVerts=20;
	const int iNumDodecaTris=3*12;

	static Vec3 DodecaVerts[iNumDodecaVerts]={
		Vec3(-1.f,-1.f,-1.f),
		Vec3( 1.f,-1.f,-1.f),
		Vec3(-1.f, 1.f,-1.f),
		Vec3( 1.f, 1.f,-1.f),
		Vec3(-1.f,-1.f, 1.f),
		Vec3( 1.f,-1.f, 1.f),
		Vec3(-1.f, 1.f, 1.f),
		Vec3( 1.f, 1.f, 1.f),
		Vec3(G,GG,0.f),
		Vec3(-G,GG,0.f),
		Vec3(-G,-GG,0.f),
		Vec3(G,-GG,0.f),
		Vec3(0,G,GG),
		Vec3(0,G,-GG),
		Vec3(0,-G,-GG),
		Vec3(0,-G,GG),
		Vec3(GG,0.f,G),
		Vec3(-GG,0.f,G),
		Vec3(-GG,0.f,-G),
		Vec3(GG,0.f,-G)
	};

	for (int i=0; i<iNumDodecaVerts; i++)
		DodecaVerts[i]+=p;

	static short DodecaTris[3*iNumDodecaTris]={
		8,3,13,		8,13,12,	8,12,7,
		9,6,12,		9,12,13,	9,13,2,
		12,6,17,	12,17,16,	12,16,7,
		13,3,19,	13,19,18,	13,18,2,
		19,3,8,		19,8,11,	19,11,1,
		18,0,10,	18,10,9,	18,9,2,
		16,5,11,	16,11,8,	16,8,7,
		17,6,9,		17,9,10,	17,10,4,
		11,5,15,	11,15,14,	11,14,1,
		10,0,14,	10,14,15,	10,15,4,
		14,0,18,	14,18,19,	14,19,1,
		15,5,16,	15,16,17,	15,17,4
	};

	return new TriMesh(iNumDodecaVerts, DodecaVerts, iNumDodecaTris, DodecaTris, pMat);
}