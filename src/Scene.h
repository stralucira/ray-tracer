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
	std::vector<Primitive*> primList;

	// bvh helpers
	AABB* CalculateSceneBounds();

	AABB* sceneBounds;
	BVH* bvh;
};
