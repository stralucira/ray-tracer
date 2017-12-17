#pragma once
#include "Ray.h"
#include "Primitive.h"
#include "BVHNode.h"

class BVH
{
public:
	// Contructors
	BVH(std::vector<Primitive*> primitives, uint N)
	{
		this->N = N;
		this->primitives = primitives;
		ConstructBVH(primitives);
	}

	// Functions
	void ConstructBVH(std::vector<Primitive*> primitives);
	void Traverse(Ray* ray, BVHNode* node, bool isShadowRay = false);
	float IntersectPrim(Ray* ray, BVHNode* node);
	static AABB CalculateBounds(std::vector<Primitive*> primitives, int first, int last);

	// Variables
	uint N, poolPtr;
	BVHNode* root;
	BVHNode** pool;
	std::vector<Primitive*> primitives;
	Primitive** primitives2;

	byte* whichChildFirst;
	vec4* dists;
};