#include "shader.h"
#include "rt.h"
#include "color.h"
#include "renderman_helper.h"

class Plastic: public Shader
{
public:
	Plastic(MATERIAL &mat):Shader(mat) {}
	RGBAf Shade(ShaderContext &sc)
	{
		return	coeffs.Ambient * ambient() + 
				coeffs.Diffuse * diffuse(N) + 
				coeffs.Specular * specular(N, -I, 1.f/coeffs.Power);
	}
};