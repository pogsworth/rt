#pragma once

class Scene;
class Camera;
class Ray;

const int MaxRecursionDepth=6;

class RayTracer {
	Scene *pScene;
	Camera *pCamera;
	int iWidth,iHeight;
	RGBAf *pTarget;
	int iRecursionDepth,iNumOverflows;
public:
	RayTracer(Scene *pNewScene, Camera *pNewCamera, int iWidth, int iHeight);
	Scene *GetScene() {return pScene;}
	Camera *GetCamera() {return pCamera;}
	bool CastRay(const Ray& r, float &t);
	RGBAf TraceRay(const Ray& r);
	bool IsShadowed(const Vec3 &pos, LIGHT &l);
	void TraceScene();
	void WriteFrame(char *pFileName);
	void ExportTarga(char *);
};
