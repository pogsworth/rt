#include "shader.h"
#include "rt.h"
#include "color.h"
#include "renderman_helper.h"

class CheckerSolid: public Shader
{
	float frequency;
	RGBAf DarkChecker;
public:
	CheckerSolid(MATERIAL &mat, RGBAf darkcolor=RGBAf(0,0,0,0), float freq=5):Shader(mat)
	{
		DarkChecker=darkcolor;
		frequency=freq;
	}

	RGBAf Shade(ShaderContext &sc)
	{
		RGBAf c;
		float smod = mod(P.x*frequency,1);
		float tmod = mod(P.y*frequency,1);
		float umod = mod(P.z*frequency,1);

		if ( (smod < .5f) ^ (tmod < .5f) ^ (umod < .5f))
			c=DarkChecker;
		else
			c=coeffs.Diffuse;

		RGBAf result;
		result = c * (coeffs.Ambient * ambient() + coeffs.Diffuse * diffuse(N));
		Vec3 V=-I;
		result += coeffs.Specular * specular(N, V, 1.f/coeffs.Power);
		return result;
	}
};