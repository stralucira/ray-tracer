#pragma once

#include "template.h"

class RayTracer
{
public:

	RayTracer();
	RayTracer(float3 pos, float3 dir);
	~RayTracer();
    
    void getColor(int x, int y, Camera* cam, Triangle* tri);
};
