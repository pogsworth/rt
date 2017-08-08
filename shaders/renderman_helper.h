
//Renderman SL defines...
typedef Vec3 point;
//typedef Vec3 vector;
typedef Vec3 normal;
typedef RGBAf color;
#define P  sc.pos
#define N  sc.norm
#define Ng sc.geonorm
#define I  sc.input.dir
#define u  sc.u
#define v  sc.v
#define s  sc.s
#define t  sc.t
#define ambient sc.ambient
#define diffuse sc.diffuse
#define specular sc.specular
//#define illuminance(pos, Norm, fAngle) \
//		for (int i=0; i<pRayTracer->GetScene()->GetNumLights(); i++) \
//			if (pRayTracer->GetScene()->GetLight(i)->Shade(pos,Norm,fAngle))


