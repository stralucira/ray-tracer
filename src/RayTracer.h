// Basar Oguz - 6084990
// Satwiko Wirawan Indrawanto - 6201539

#pragma once
#include "Ray.h"
#include "template.h"
#include "Scene.h"

class RayTracer
{
public:
	RayTracer(Scene* scene, Surface* screen);

	Surface* screen;
	Scene* scene;
	Pixel buffer[SCRHEIGHT][SCRWIDTH];
	vec3 accumulator[SCRHEIGHT][SCRWIDTH];
	
	int Render(int samples);
	void RenderPacket();

	// Whitted-style ray tracing stuff
	vec3 SampleWhitted(int x, int y, Ray* ray, int depth);
	vec3 DirectIllumination(vec3 hitPoint, vec3 dir, vec3 normal, Light* light, Ray* ray);
	vec3 Reflect(vec3 dir, vec3 normal);
	vec3 Refract(vec3 dir, vec3 normal, bool inside);
	float Fresnel(vec3 dir, vec3 normal, float index);

	// Path tracing stuff
	vec3 Trace(Ray* ray, bool isShadowRay = false);
	vec3 Sample(Ray* ray, int depth, bool lastSpecular = false);
	vec3 SampleMIS(Ray* ray);
	vec3 SampleSimple(Ray* ray, int depth);
	vec3 SampleEX(Ray* ray, int depth, bool secondaryRay);
	vec3 SampleDirectLight(vec3 intersect, vec3 normal, Ray* ray, bool isMIS = false);
	vec3 CosineWeightedDiffuseReflection(vec3 normal);

	glm::uint RandomInt(glm::uint* seed);
	float RandomFloat(glm::uint* seed);

	// Global stuff
	vec3 GetColor(Ray* ray);
	vec3 SampleTexturePoint(Surface* tex, vec2 uv);
	vec3 SampleSkydome(HDRBitmap* skydome, Ray* ray);

	bool depthRendering = false;
	bool renderShadow = true;
	bool inShadow;
};
