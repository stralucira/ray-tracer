#pragma once
#include "Ray.h"
#include "Primitive.h"
#include "BVHNode.h"

class BVH
{
public:
	// Contructors
	BVH(Primitive** primitives, uint N)
	{
		this->N = N;
		this->primitives = primitives;
		ConstructBVH(primitives);
	}

	// Functions
	void ConstructBVH(Primitive** primitives);
	void Traverse(Ray* ray, BVHNode* node, bool isShadowRay);
	float IntersectPrim(Ray* ray, BVHNode* node);
	static AABB CalculateBounds(Primitive** primitives, int first, int last);

	// Variables
	uint N, poolPtr;
	BVHNode* root;
	BVHNode** pool;
	Primitive** primitives;
};