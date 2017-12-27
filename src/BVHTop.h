#pragma once
#include "BVH.h"
#include "BVHNode.h"
#include "Primitive.h"

class BVHTop
{
public:
	BVHTop(std::vector<Primitive*>* primList, std::vector<BVH*>* bvhList)
	{
		this->primList = primList;
		this->bvhList = bvhList;
		ConstructBVHTop(bvhList);
	}

	//int* primIndices;
	//int* bvhIndices;
	int topPoolPtr;
	BVHNode** topPool;
	BVHNode* topRoot;

	void ConstructBVHTop(std::vector<BVH*>* bvhList);
	void TraverseTop(Ray * ray, BVHNode * node, bool isShadowRay = false, int * depthRender = FALSE);
	float TraceTop(Ray * ray, BVHNode * node);
	float IntersectRay(Ray * ray, AABB bounds);
	void SubdivideTop(BVHNode** pool, std::vector<BVH*>* bvhList, int& poolPtr);
	AABB CalculateBoundsTop(std::vector<BVH*>* bvhList, int first, int last);

	std::vector<Primitive*>* primList;
	std::vector<BVH*>* bvhList;
};