#pragma once
#include "AABB.h"
#include "Primitive.h"

class BVHNode
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
	void Subdivide(BVHNode** pool, std::vector<Primitive*>* primitives, glm::uint& poolPtr);
	bool Partition(BVHNode** pool, std::vector<Primitive*>* primitives, glm::uint& poolPtr);
	bool isLeaf();

	// Variables
	AABB bounds;
	int leftFirst;
	int count;
};