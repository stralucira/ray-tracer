#pragma once
#include "Primitive.h"
#include "BVHNode.h"
#include "BVH.h"

class BVHTop
{
public:
	// Constructors
	BVHTop(std::vector<BVH*>* bvhList)
	{
		this->bvhList = bvhList;
		ConstructBVHTop(bvhList);
	}

	// Functions
	void ConstructBVHTop(std::vector<BVH*>* bvhList);
	void TraverseTop(Ray * ray, BVH* node, bool isShadowRay = false, int * depthRender = FALSE);
	float IntersectRay(Ray * ray, AABB bounds);

	// Variables
	int topPoolPtr;
	BVH** topPool;
	BVH* topRoot;

	std::vector<BVH*>* bvhList;

	int traversalMode = 1;
};