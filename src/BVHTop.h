#pragma once
#include "BVH.h"
#include "Primitive.h"

class BVHTop
{
public:
	BVHTop(std::vector<BVH*>* bvhList)
	{
		//this->primList = primList;
		this->bvhList = bvhList;
		ConstructTopBVH(bvhList);
	}

	int* primIndices;
	int* bvhIndices;
	int topPoolPtr;
	BVHNode** topPool;
	BVHNode* topRoot;

	void ConstructTopBVH(std::vector<BVH*>* bvhList);
	void SubdivideTop(BVHNode** pool, std::vector<BVH*>* bvhList, int& poolPtr);
	AABB CalculateTopBounds(std::vector<BVH*>* bvhList, int first, int last);

	//std::vector<Primitive*>* primList;
	std::vector<BVH*>* bvhList;
};