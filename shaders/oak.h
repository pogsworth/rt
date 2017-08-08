#include "shader.h"
#include "rt.h"
#include "color.h"
#include "renderman_helper.h"

class Oak: public Shader
{
	RGBAf LightWood,DarkWood;
	float fRingFreq;
public:
	Oak(MATERIAL &mat,
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
		r += smoothstep(0,5,r) * noise(P*2.5f);
		float inring = smoothpulsetrain(.1f,.55f,.7f,.95f,1.f,r);

		RGBAf Cwood = mix(LightWood, DarkWood, inring);

		RGBAf result = Cwood * (coeffs.Ambient * ambient() + coeffs.Diffuse * diffuse(N));
		Vec3 V=-I;
		result += (1.f - .5f*inring) * coeffs.Specular * specular(N, V, 1.f/coeffs.Power);

		return result;
	}
};