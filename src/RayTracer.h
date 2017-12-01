#pragma once
#include "template.h"
#include "scene.h"
#include "Light.h"

class RayTracer
{
public:
	Surface* screen;
	Scene* scene;
	Pixel buffer[SCRHEIGHT][SCRWIDTH];

	RayTracer(Scene* scene, Surface* screen);

	float3 getColor(int x, int y, Ray* ray);
	float3 DirectIllumination(float3 hitPoint, float3 dir, float3 normal, Light* light, Material mat);

	void Render();
    //float3 getColor(int x, int y, Camera* cam, pointLight* light, std::vector<Primitive*> primList);
};
