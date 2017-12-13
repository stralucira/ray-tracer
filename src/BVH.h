#pragma once
#include "Ray.h"
#include "Primitive.h"
#include "BVHNode.h"

class BVH
{
public:
	void ConstructBVH(Primitive** primitives);
	static AABB CalculateBounds(Primitive** primitives, int first, int last);
	void Traverse(Ray* ray);

	uint N, poolPtr;
	
	BVHNode* root;
	BVHNode** pool;
};