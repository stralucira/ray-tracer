#pragma once
#include "Primitive.h"
#include "Light.h"
#include "Primitive.h"

class Scene
{
public:
	Scene();
	Camera* camera;

	Light* lights[1];
	Primitive* primitives[1];
};