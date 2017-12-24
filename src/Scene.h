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
	std::vector<std::vector<Primitive*>> objectList;
	std::vector<AABB*> objectBounds;
	std::vector<BVH*> bvh;

	// bvh helpers
	AABB* CalculateObjectBounds(std::vector<Primitive*> primList);

	AABB* sceneBounds;
	BVH* topbvh;
};
