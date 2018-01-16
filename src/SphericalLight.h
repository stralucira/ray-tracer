#pragma once

#include "Sphere.h"
#include "Light.h"

class SphericalLight : public Light {
public:
	SphericalLight(vec3 position, vec3 color, float radius) 
	: Light(position, color) {
		this->pos = position;
		this->sph = new Sphere(position, radius);
		this->color = color;
	}
	~SphericalLight(){
		delete sph;
	};

	Sphere* sph;
	vec3 color;
};
