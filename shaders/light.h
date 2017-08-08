#ifndef LIGHT_H
#define LIGHT_H

#include "math/vec3.h"
#include "shaders/color.h"

enum LIGHTTYPE {
	LIGHT_POINT = 1,
	LIGHT_SPOT,
	LIGHT_DIRECTIONAL,
	LIGHT_PADDING = 0x7fffffff
};

struct LIGHT {
  LIGHTTYPE	Type;
  RGBAf		Diffuse;
  RGBAf		Specular;
  RGBAf		Ambient;
  Vec3		Position;
  Vec3		Direction;
  float		Range;
  float		Falloff;
  float		Attenuation0;
  float		Attenuation1;
  float		Attenuation2;
  float		Theta;
  float		Phi;
};

#endif // LIGHT_H