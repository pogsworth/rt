#include "shader.h"
#include "rt.h"
#include "color.h"
#include "renderman_helper.h"

class CheckerBoard: public Shader
{
	float frequency;
	RGBAf DarkChecker;

public:
	CheckerBoard(MATERIAL &mat, RGBAf darkcolor=RGBAf(0,0,0,0), float freq=10):Shader(mat)
	{
		DarkChecker=darkcolor;
		frequency=freq;
	}

	RGBAf Shade(ShaderContext &sc)
	{
		RGBAf c;
		float smod = mod(s*frequency,1);
		float tmod = mod(t*frequency,1);

		if ( (smod < .5f) ^ (tmod < .5f))
			c=DarkChecker;
		else
			c=coeffs.Diffuse;

		RGBAf result;
		Vec3 norm=faceforward(N,I);
		result = c * (coeffs.Ambient * ambient() + coeffs.Diffuse * diffuse(norm));
		result += coeffs.Specular * specular(norm, -I, 1.f/coeffs.Power);
		return result;
	}
};