#include "shader.h"
#include "rt.h"
#include "color.h"

class Woodgrain: public Shader
{
public:
	Woodgrain(D3DMATERIAL8 &mat):Shader(mat) {}
	D3DXCOLOR Shade(ShaderContext &sc)
	{
		
	}
};