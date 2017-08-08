#include "shader.h"
#include "rt.h"
#include "color.h"

class UVView: public Shader
{
public:
	UVView(MATERIAL &mat):Shader(mat)	{}

	RGBAf Shade(ShaderContext &sc)
	{
		return RGBAf(u,v,0,1.f);
	}
};