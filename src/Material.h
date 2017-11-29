#pragma once

#include "template.h"

class Material
{
public:
    Material(){
        this->mat = float4(1, 1, 1, 1);
    }
	float4 mat;
};
