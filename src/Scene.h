#pragma once
#include "Primitive.h"
#include "Light.h"
#include "Camera.h"
#include "BVH.h"

class Scene
{
public:
	Scene();
	Camera* camera;

	std::vector<Light*> lightList;

#if USEVECTOR
	std::vector<Primitive*> primList;
#else
	Primitive* primList[4056];
#endif

	// bvh helpers
	AABB* CalculateSceneBounds();

	AABB* sceneBounds;
	BVH* bvh;
};
