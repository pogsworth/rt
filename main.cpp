
//math
#include "math/vec3.h"
#include "math/matrix4.h"

//objects
#include "geometry/sphere.h"
#include "geometry/trimesh.h"

//shaders
#include "shaders/plastic.h"
#include "shaders/checkerboard.h"
#include "shaders/checkersolid.h"
#include "shaders/uvview.h"
#include "shaders/simplewood.h"
#include "shaders/oak.h"
#include "shaders/mirror.h"
#include "shaders/color.h"
#include "shaders/light.h"

//scene
#include "scene/camera.h"
#include "scene/scene.h"
#define DTOR 0.01745329f

void main()
{
	//make matrix and camera for viewing
	Matrix4 view,rotate;
	rotate.RotateY(-(float)M_PI/4.f);
	rotate.RotateX(-(float)M_PI/4.f);
	view.Translate(.5,0,-5);
	Camera camera(view, DTOR*45.f);

	//create the scene
	Scene scene;
	RGBAf cWhite(1.f, 1.f, 1.f, 1.f);
	RGBAf cBlack(0.f, 0.f, 0.f, 0.f);
	RGBAf cPurple(.8f, .2f, .8f, 1.f);
	RGBAf cYellow(.8f, .8f, .2f, 1.f);
	RGBAf cSlightYellow(.8f, .8f, .5f, 1.f);
	RGBAf cRed(1.f, 0.f, 0.f, 1.f);
	RGBAf cBlue(0.f, 0.f, 1.f, 1.f);
	MATERIAL mat;

	mat.Ambient =.2f*cWhite;
	mat.Diffuse =.7f*cRed;
	mat.Specular = .3f*cWhite;
	mat.Power = 32;
	float x=0,y=0,z=0,r=1;
	scene.AddObject(new Sphere(Vec3(1.5,0,0),1,new Oak(mat)));
	mat.Ambient=.2f*cWhite;
	mat.Diffuse=.5f*cRed;
//	scene.AddObject(new Sphere(Vec3(x,y,z), .5f, new Plastic(mat)));
//	CheckerBoard *p=new CheckerBoard(mat);
//	scene.AddObject(Tetra(Vec3(2,1,5),p));
//	scene.AddObject(Octa(Vec3(5,1,7),p));
//	scene.AddObject(Icosa(Vec3(7,1,3),p));
//	scene.AddObject(Dodeca(Vec3(2,1,7),p));

	mat.Ambient=.4f*cWhite;
	mat.Diffuse=.5f*cWhite;
	mat.Specular = .8f*cSlightYellow;
	scene.AddObject(Cube(.5,.5,.5, new CheckerBoard(mat, cRed, 2)));
//	scene.AddObject(new Sphere(Vec3(0,1,1.5), 1.5f, new Mirror(mat)));

	//add a white light  to the scene
	LIGHT Light;
	Light.Type=LIGHT_DIRECTIONAL;
	Light.Direction=Vec3(.577f, .577f, -.577f);
	Light.Ambient=.4f*cWhite;
	Light.Diffuse=cWhite;
	Light.Specular=1.5f*cWhite;
	scene.AddLight(Light);
	Light.Direction=Vec3(-.707f,0.f,.707f);
	scene.AddLight(Light);

	//set up the tracer with the scene and the camera
	RayTracer tracer(&scene, &camera, 640, 640);

	//render the scene
	tracer.TraceScene();

	//output a picture
	tracer.ExportTarga("testscene.tga");
}
