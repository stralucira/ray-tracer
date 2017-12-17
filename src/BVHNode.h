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
	void AdjustBounds(AABB* bounds, vec3& min, vec3& max);
	void Subdivide(BVHNode** pool, std::vector<Primitive*> primitives, glm::uint& poolPtr);
	bool Partition(BVHNode** pool, std::vector<Primitive*> primitives, glm::uint& poolPtr);
	bool isLeaf();

	// Variables
	AABB bounds; // 24 bytes ( 6 x 4 bytes )
	int leftFirst; // 4 bytes
	int count; // 4 bytes
};