#pragma once
#include "template.h"

class Light
{
public:
	Light(vec3 position, vec3 color)
	{
		this->pos = position;
		this->color = color;
	}
	~Light();

	vec3 pos;
	vec3 color;
};
