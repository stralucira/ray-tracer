#pragma once
#include "Ray.h"
#include "template.h"
#include "Scene.h"

class RayTracer
{
public:
	Surface* screen;
	Scene* scene;
	Pixel buffer[SCRHEIGHT][SCRWIDTH];
	
	RayTracer(Scene* scene, Surface* screen);

	vec3 GetColor(int x, int y, Ray* ray);
	vec3 DirectIllumination(vec3 hitPoint, vec3 dir, vec3 normal, Light* light, Material mat);

	void Render();
};
