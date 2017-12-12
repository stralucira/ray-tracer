#pragma once
#include "template.h"
#include "Ray.h"

class Camera
{
public:
	Camera();
	void Init();
	void CalculateScreen();

	Ray* GenerateRay(int x, int y);

	//void GenerateRays();
	//void UpdateRays();
	//Ray* cameraRays[SCRWIDTH*SCRHEIGHT];

	vec3 pos;
	vec3 dir;
	vec3 p0, p1, p2;

	vec3 viewDir;
	vec3 up;
	vec3 right;

	float d;
	float aspectRatio;
};
