#pragma once
#include "Primitive.h"
#include "Light.h"
#include "Camera.h"
#include "BVH.h"
#include "BVHTop.h"
#include "HDRBitmap.h"

class Scene
{
public:
	Scene(int scene_id);
	Camera* camera;

	void LoadObject(std::string filename, bool simple = false);

	std::vector<Light*> lightList;
	std::vector<Primitive*> primList;
	std::vector<Material*> matList;

	std::vector<Primitive*> areaLightList;

	std::vector<std::vector<Primitive*>> objectList;
	std::vector<BVH*> bvhList;

	HDRBitmap* skydome;

	BVHTop* bvhTop;
	BVH* bvh;

	vec3 pos, lookAt;
	float focalLength = 5.f;
	float apertureSize = 0.f;
	float MOVEMODIFIER;
	int index = 0;

	// bvh helpers
	AABB CalculateObjectBounds(std::vector<Primitive*> primList);
	vec3 CalculateObjectCentroid(AABB bounds);
	AABB* sceneBounds;
};
