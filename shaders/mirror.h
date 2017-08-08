#include "shader.h"
#include "rt.h"
#include "color.h"

class Mirror: public Shader
{
	float frequency;
	RGBAf DarkChecker;
public:
	Mirror(MATERIAL &mat):Shader(mat)
	{
	}

	RGBAf Shade(ShaderContext &sc)
	{
		RGBAf c,result;

//		result = coeffs.Ambient * ambient() + coeffs.Diffuse * diffuse(N);

		Ray reflection;
		reflection.dir=reflect(I,N);
		reflection.pos=P + reflection.dir*1e-4f;
		c=sc.pRayTracer->TraceRay(reflection);
		result = c * coeffs.Specular;

		return result;
	}
};