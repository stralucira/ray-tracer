// Basar Oguz - 6084990
// Satwiko Wirawan Indrawanto - 6201539

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

	vector<vector<vec3>> accumulator;
	
	RayTracer(Scene* scene, Surface* screen);

	vec3 GetColor(int x, int y, Ray* ray, int depth);
	vec3 DirectIllumination(vec3 hitPoint, vec3 dir, vec3 normal, Light* light, Material mat);

	vec3 Reflect(vec3 dir, vec3 normal);
	float Fresnel(vec3 dir, vec3 normal, float index);
	void Render(int samples);

	bool depthRendering = false;
	bool renderShadow = true;
	bool inShadow;

	vec3 Sample(Ray* ray, int depth);
	vec3 CosineWeightedDiffuseReflection(vec3 normal);
	vec3 Trace(Ray* ray);

	vec3 SampleSkydome(HDRBitmap* skydome, Ray* ray);
};
