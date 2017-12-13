#pragma once
#include "AABB.h"
#include "Primitive.h"

struct BVHNode
{
public:
	// Constructors
	BVHNode() {};
	BVHNode(AABB bounds, int leftFirst, int count)
	{
		this->bounds = bounds;
		this->leftFirst = leftFirst;
		this->count = count;
	}

	// Functions
	void Subdivide(BVHNode** pool, Primitive** primitives, uint& poolPtr);
	bool Partition(BVHNode** pool, Primitive** primitives, uint& poolPtr);
	bool isLeaf();

	// Variables
	AABB bounds;
	int leftFirst;
	int count;
};