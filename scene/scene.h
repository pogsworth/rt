#include "shaders/light.h"
#include "geometry/surface.h"
#include <vector>

#pragma once

using namespace std;

class Scene {
	vector<Surface *> ObjectList;
	vector<LIGHT> LightList;
public:
	void AddObject(Surface *newobj) {ObjectList.push_back(newobj);}
	void AddLight(LIGHT light) {LightList.push_back(light);}
	size_t GetNumObjects() {return ObjectList.size();}
	size_t GetNumLights() {return LightList.size();}
	Surface *GetObject(unsigned i) {if (i<0 || i>=ObjectList.size()) return NULL; return ObjectList[i];}
	LIGHT *GetLight(unsigned i) {if (i<0 || i>=LightList.size()) return NULL; return &LightList[i];}
};

