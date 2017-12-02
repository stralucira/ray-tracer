#pragma once
#include "Primitive.h"
#include "Light.h"
#include "Camera.h"

class Scene
{
public:
	Scene();
	Camera* camera;

	//Light* lights[1];
	//Primitive* primitives[1];

	Light* lights[2];
	Primitive* primitives[7];
};
