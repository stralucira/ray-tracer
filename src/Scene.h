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

	void LoadObject(std::string filename);

	std::vector<Light*> lightList;
	std::vector<Primitive*> primList;

	// bvh helpers
	AABB* CalculateSceneBounds();

	AABB* sceneBounds;
	BVH* bvh;
};
