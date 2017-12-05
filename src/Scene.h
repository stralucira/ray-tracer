#pragma once
#include "Primitive.h"
#include "Light.h"
#include "Camera.h"

#define SCENE1	0;
#define SCENE2	1;
#define SCENE3	0;
#define TINYOBJLOADER 0;

class Scene
{
public:
	Scene();
	Camera* camera;

	std::vector<Primitive*> primList;
	std::vector<Light*> lightList;

#if SCENE1
	Light* lights[1];
	Primitive* primitives[1];
#elif SCENE2
	Light* lights[2];
	Primitive* primitives[8];
#elif SCENE3
	Light* lights[1];
	Primitive* primitives[2];
#elif TINYOBJLOADER
	//Light* lights[1];
	//Primitive* primitives[12];
#endif
};
