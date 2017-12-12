#pragma once

struct BVHNode
{
public:
	AABB bounds;
	int leftFirst;
	int count;

	BVHNode(AABB bounds, int leftFirst, int count)
	{
		this->bounds = bounds;
		this->leftFirst = leftFirst;
		this->count = count;
	}
}