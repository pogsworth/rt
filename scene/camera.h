#pragma once
#include "math/Matrix4.h"

class Camera {
	Matrix4 view;	//contains pos, dir, up, right vectors
	float fHorizFOV;	//what angle is horizontal screen extent
public:
	Camera(Matrix4 &newview, float fFOV) {view=newview; fHorizFOV=fFOV;}
	void SetFOV(float fHFOV) {fHorizFOV=fHFOV;}
	float GetFOV() {return fHorizFOV;}
	void SetViewMatrix(Matrix4 &matrix);
	Vec3 GetPosition() {return view.GetRowVec3(3);}
	Vec3 GetDirection() {return view.GetColVec3(2);}
	Vec3 GetUp() {return view.GetColVec3(1);}
	Vec3 GetRight() {return view.GetColVec3(0);}
};
