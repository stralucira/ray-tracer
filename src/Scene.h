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

	std::vector<Primitive*> primList;
	std::vector<Light*> lightList;

	// bvh helpers
	AABB* sceneBounds;
	BVH* bvh;
};
