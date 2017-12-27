#pragma once
#include "AABB.h"
#include "Primitive.h"

class BVH;

class BVHNode
{
public:
	// Constructors
	BVHNode() {};
	~BVHNode();
	BVHNode(AABB bounds, int leftFirst, int count)
	{
		this->bounds = bounds;
		this->leftFirst = leftFirst;
		this->count = count;
	}

	// Functions
	void AdjustBounds(AABB* bounds, vec3& min, vec3& max);
	void Subdivide(BVHNode** pool, std::vector<Primitive*>* primList, int& poolPtr);
	bool Partition(BVHNode** pool, std::vector<Primitive*>* primList, int& poolPtr);
	bool isLeaf();

	// Variables
	AABB bounds;	// 24 bytes ( 6 x 4 bytes )
	int leftFirst;	// 4 bytes
	int count;		// 4 bytes

	// Top level BVH functions
	void SubdivideTop(BVHNode** pool, std::vector<BVH*>* primList, int& poolPtr);
	bool PartitionTop(BVHNode** pool, std::vector<BVH*>* primList, int& poolPtr);
};