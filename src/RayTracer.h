#pragma once

#include "template.h"

struct pointLight
{
    Material* mat;
    float3 pos;
    pointLight(float3 p, Material* mt)
    {
        this->mat = mt;
        this->pos = p;
    };
};

class RayTracer
{
public:

	RayTracer();
	RayTracer(float3 pos, float3 dir);
	~RayTracer();
    
    void getColor(int x, int y, Camera* cam, pointLight* light, Triangle* tri);
};
