#pragma once

#include "template.h"

class Material
{
public:
	enum class Shader {DIFFUSE, MIRROR, GLASS};
	float4 color;
	Shader shader;

	Material(float4 color, Shader shader)
	{
		this->color = color;
		this->shader = shader;
	}

	/*
    Material(){
        this->specs = float4(1, 1, 1, 1);
    }
    
    Material(float4 specs) {
        this->specs = specs;
    }
    
	float4 specs;
	*/
};
