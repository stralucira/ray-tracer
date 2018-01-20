#pragma once

#include "Sphere.h"
#include "Light.h"

class SphericalLight : public Light {
public:
	SphericalLight(vec3 position, vec3 color, float radius) 
	: Light(position, color) {
		this->pos = position;
		this->sph = new Sphere(position, radius, true);
		this->color = color;
	}
	~SphericalLight(){
		delete sph;
	};

	Sphere* sph;
	vec3 color;

	inline vec3 randomPointOnSphericalLight(vec3 p) {
		
		vec3 e2 = vec3(1.0f,0.0f,0.0f);
	  	vec3 e3 = cross((this->pos-p) , e2);
	  	normalize(e3);
	  	e3 = e3*this->sph->radius;
	  	normalize(e2);
	  	e2 = e2*this->sph->radius;

	  	//std::cout << e2 << ", " << e3 << std::endl;
	  	//srand(time(NULL));
	  	float x, y;
	  	do {
	   		x = ((float)rand())/((float)RAND_MAX)*2 - 1;
	    	y = ((float)rand())/((float)RAND_MAX)*2 - 1;
	  	} while (x*x + y*y > 1);

	  	return this->pos + (x * e2 + e3*y);
	} 
};
