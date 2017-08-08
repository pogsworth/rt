#include <math.h>
#include <float.h>			//for FLT_ limits
#include "shaders/shader.h"
#include "scene/camera.h"

RayTracer::RayTracer(Scene *pNewScene, Camera *pNewCamera, int iNewWidth, int iNewHeight)
{
	pScene=pNewScene;
	pCamera=pNewCamera;
	iWidth=iNewWidth;
	iHeight=iNewHeight;
	pTarget=new RGBAf[iWidth*iHeight];
	iRecursionDepth=0;
	iNumOverflows=0;
}

void RayTracer::TraceScene()
{
	int x,y;
	float fHalfW,fHalfH,Xinc,Yinc,rx,ry;
	Ray Eye;

	Eye.pos=pCamera->GetPosition();
	
	fHalfW=tanf(pCamera->GetFOV()/2.f);
	fHalfH=fHalfW*iHeight/iWidth;
	Xinc=2*fHalfW/iWidth;
	Yinc=2*fHalfH/iHeight;
	RGBAf *pDest=pTarget;
	for (y=0, ry=fHalfH-Yinc/2;  y<iHeight;  y++, ry-=Yinc) {
		for (x=0, rx=-fHalfW+Xinc/2; x<iWidth; x++, rx+=Xinc, pDest++) {
			//normalize (unitize) eye ray
			Eye.dir=pCamera->GetDirection() + rx*pCamera->GetRight() + ry*pCamera->GetUp();
			Eye.dir.Norm();
			iRecursionDepth=0;
			*pDest=TraceRay(Eye);
		}
		printf("Tracing %03d of %d lines\r",y,iHeight);
	}
}

const float fShadowEpsilon=1e-4f;
bool RayTracer::IsShadowed(const Vec3 &pos, LIGHT &l)
{
	unsigned i;
	float tt,tmax;
	Ray r;

	if (l.Type == LIGHT_DIRECTIONAL) {
		tmax=FLT_MAX;
		r.dir=l.Direction;
		r.dir.Norm();
	} else {
		r.dir=l.Position-pos;
		tmax=r.dir.Norm();
		tmax-=fShadowEpsilon;
	}
	r.pos=pos+fShadowEpsilon*r.dir;

	for (i=0; i<pScene->GetNumObjects(); i++) {
		Surface *pObject=pScene->GetObject(i);
		if (pObject->Intersect(r, tt)) {
			if (tt > 0 && tt < tmax)	//only catch objects between pos and the light
				return true;
		}
	}
	return false;
}


RGBAf RayTracer::TraceRay(const Ray &r)
{
	unsigned i;
	float tt=FLT_MAX;
	RGBAf color(0.f,0.f,0.f,0.f);
	Vec3 Half;
	ShaderContext sc;

	iRecursionDepth++;
	if (iRecursionDepth >= MaxRecursionDepth) {
		iNumOverflows++;
		return color;
	}

	sc.pRayTracer=this;
	sc.tray=FLT_MAX;	//this will keep track of our minimum t
	sc.obj=0;			//keep track of our closest object
	sc.input.dir=r.dir;

	for (i=0; i<pScene->GetNumObjects(); i++) {
		Surface *pObject=pScene->GetObject(i);
		if (pObject->Intersect(r, tt)) {
			if (tt > 0 && tt < sc.tray) {	//only catch objects in front of us
				sc.tray=tt;
				sc.obj=pObject;
			}
		}
	}

	if (sc.obj) {
		sc.pos=r.pos+sc.tray*r.dir;		//store the position of intersection

		//have the object shade itself
		color=sc.obj->Shade(sc);
	}
	return color;
}

inline unsigned char clamptobyte(float input)
{
	unsigned component;
	component=(unsigned)(input*255);
	if (component < 0)
		component = 0;
	if (component > 255)
		component = 255;
	return (unsigned char)component;
}

void RayTracer::ExportTarga(char *pFileName)
{

#pragma pack(push, 1)			//force this structure to byte align
	struct TargaHeader {
		char	IDLen;			//length of ID field
		char	MapType;		//0=none, 1=present
		char	ImgType;		//0=none, 1=c-map, 2=true, 3=b&w, 9=rle c-map, 10=rle true, 11=rle b&w
		short	FirstColor;		//where to start in colormap
		short	MapLen;			//number of colors to set
		char	EntrySize;		//number of bits per color entry
		short	Xorigin;
		short	Yorigin;
		short	width;
		short	height;
		char	depth;
		char	descriptor;		//put 32 here to say that it's right-side-up
	};
#pragma pack(pop)

	FILE *fp;
	fopen_s(&fp, pFileName,"wb");
	if (fp) {
		TargaHeader header={0,0,2,0,0,0,0,0,(short)iWidth,(short)iHeight,24,32};
		unsigned char *pImage=new unsigned char[iWidth*iHeight*3];
		if (pImage) {
			unsigned char *pDest=pImage;
			RGBAf *pSource=pTarget;
			int i,j;

			for (i=0; i<iHeight; i++)
				for (j=0; j<iWidth; j++,pSource++) {
					*pDest++=clamptobyte(pSource->b);
					*pDest++=clamptobyte(pSource->g);
					*pDest++=clamptobyte(pSource->r);
				}
			fwrite(&header,sizeof(header),1,fp);
			fwrite(pImage,iWidth*iHeight*3,1,fp);
			fclose(fp);
			delete [] pImage;
		}
	}
}
