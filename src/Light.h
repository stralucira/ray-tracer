#pragma once
#include "template.h"

class Light
{
public:
	Light(float3 pos, float3 color);

	float3 pos;
	float3 color;
};