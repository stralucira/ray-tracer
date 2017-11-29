#pragma once

#include "template.h"

#define STDASPECTRATIO 4/3

class Camera
{
public:

	Camera();
	Camera(float3 pos, float3 dir, float aspectRatio);
	~Camera();
    
    Ray getRay(float x, float y);
    
    void calculateScreen();
    Ray generateRay(float2 p);
	
	float3 pos;
	float3 dir;
	float d = 1;
    float3 screenCenter;
    float3 p0,p1,p2;
    float aspectRatio;
    
    float2 p;
};
