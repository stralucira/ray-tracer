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

	vec3 Reflect(vec3 dir, vec3 normal);
	vec3 Refract(vec3 dir, vec3 normal, float index);

	vec3 GetColor(int x, int y, Ray* ray, unsigned int depth);
	vec3 DirectIllumination(vec3 hitPoint, vec3 dir, vec3 normal, Light* light, Material mat);

	float Fresnel(vec3 dir, vec3 normal, float index);
	void Render();
};
