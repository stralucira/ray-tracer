#pragma once

#include "template.h"

class Material
{
public:
    Material(){
        this->specs = float4(1, 1, 1, 1);
    }
    
    Material(float4 specs) {
        this->specs = specs;
    }
    
	float4 specs;
};
