#include "shaders/color.h"
//#include "scene/transform.h"

class Ray;
class Shader;
class ShaderContext;
#pragma once

class Surface {	//: public Transform {
protected:
	Shader *pSurfaceShader;
public:
	void SetShader(Shader *pShader) {pSurfaceShader=pShader;}
	virtual bool Intersect(const Ray &r, float &t)=0;
	virtual	RGBAf Shade(ShaderContext &result)=0;
};
