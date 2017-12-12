#pragma once
#include "AABB.h"

struct BVHNode
{
public:
	BVHNode() {};
	BVHNode(AABB bounds, int leftFirst, int count)
	{
		this->bounds = bounds;
		this->leftFirst = leftFirst;
		this->count = count;
	}
	void Subdivide();
	void Partition();

	bool isLeaf();

	AABB bounds;
	int leftFirst;
	int count;
};