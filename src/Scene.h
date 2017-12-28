#pragma once
#include "Primitive.h"
#include "Light.h"
#include "Camera.h"
#include "BVH.h"
#include "BVHTop.h"

class Scene
{
public:
	Scene(int scene_id);
	Camera* camera;

	void LoadObject(std::string filename);

	std::vector<Light*> lightList;
	std::vector<Primitive*> primList;
	std::vector<BVH*> bvhList;

	//std::vector<std::vector<Primitive*>> objectList;
	BVHTop* bvhTop;
	BVH* bvh;

	vec3 pos, lookAt;
	float MOVEMODIFIER;
	int index = 0;

	// bvh helpers
	AABB CalculateObjectBounds(std::vector<Primitive*> primList);
	vec3 CalculateObjectCentroid(AABB bounds);
	AABB* sceneBounds;
};
