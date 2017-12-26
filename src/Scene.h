#pragma once
#include "Primitive.h"
#include "Light.h"
#include "Camera.h"
#include "BVH.h"

class Scene
{
public:
	Scene(int scene_id);
	Camera* camera;

	void LoadObject(std::string filename);

	std::vector<Light*> lightList;
	std::vector<Primitive*> primList;
	std::vector<std::vector<Primitive*>> objectList;
	std::vector<AABB*> objectBounds;
	BVH* bvh;

	vec3 pos, lookAt;

	// bvh helpers
	AABB* CalculateObjectBounds(std::vector<Primitive*> primList);

	AABB* sceneBounds;
	BVH* topbvh;
};
