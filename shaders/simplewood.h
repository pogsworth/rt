#include "shader.h"
#include "rt.h"
#include "color.h"
#include "renderman_helper.h"

class SimpleWood: public Shader
{
	RGBAf LightWood,DarkWood;
	float fRingFreq;
public:

	SimpleWood(MATERIAL &mat,
			   RGBAf Clightwood = RGBAf(.5f,.2f,.067f,1.f),
			   RGBAf Cdarkwood = RGBAf(.15f, .077f, .028f,1.f),
			   float freq=10.f)
			   :Shader(mat)
	{
		LightWood=Clightwood;
		DarkWood=Cdarkwood;
		fRingFreq=freq;
	}

	RGBAf Shade(ShaderContext &sc)
	{
		float r2 = P.x*P.x + P.z*P.z;
		float r = sqrtf(r2)*fRingFreq;
		float inring = smoothpulsetrain(.1f,.55f,.7f,.95f,1.f,r);
		return mix(LightWood, DarkWood, inring);
	}
};