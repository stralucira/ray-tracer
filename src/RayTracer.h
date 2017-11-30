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
    
    float3 getColor(int x, int y, Camera* cam, pointLight* light, std::vector<Primitive*> primList);

    float3 DirectIllumination(float3 hitPoint, float3 direction, float3 normal, std::vector<Primitive*> primList );
};
