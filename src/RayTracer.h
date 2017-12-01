#pragma once
#include "template.h"
#include "scene.h"
#include "Light.h"

class RayTracer
{
public:
	RayTracer();
	~RayTracer();

	Surface* screen;
	Scene* scene;
	Pixel buffer[SCRHEIGHT][SCRWIDTH];

	RayTracer(Scene* scene, Surface* screen);

	void getColor(int x, int y, Ray* ray);
	float3 DirectIllumination(float3 hitPoint, float3 direction, float3 normal, Light* light, Material mat);
    
    //float3 getColor(int x, int y, Camera* cam, pointLight* light, std::vector<Primitive*> primList);

    
};
