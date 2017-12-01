#pragma once

#include "template.h"
#include "Ray.h"

class Camera
{
public:

	Camera();
	~Camera();
	void Init();
	void GenerateRays();
	void UpdateRays();

	Ray* cameraRays[SCRWIDTH*SCRHEIGHT];

	//Camera(float3 pos, float3 dir, float aspectRatio);   
    //Ray getRay(float x, float y);
	//void calculateScreen();
    //Ray generateRay(float2 p);
	
	float3 pos;
	float3 dir;
    float3 screenCenter;
	float3 p0, p1, p2;

	float d;
    float aspectRatio;
    
    float2 p;

};
