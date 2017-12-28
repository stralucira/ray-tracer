#pragma once
//#include "Ray.h"
#include "Primitive.h"
#include "BVHNode.h"

class BVHNode;

class BVH
{
public:
	// Contructors
	BVH(std::vector<Primitive*>* primList)
	{
		this->primList = primList;
		ConstructBVH(primList);
	}

	// Functions
	void ConstructBVH(std::vector<Primitive*>* primList);
	void Subdivide(BVH** pool, std::vector<BVH*>* bvhList, int& poolPtr);
	bool Partition(BVH** pool, std::vector<BVH*>* bvhList, int& poolPtr);
	void Traverse(Ray* ray, BVHNode* node, bool isShadowRay = false, int* depthRender = NULL);
	float Trace(Ray* ray, BVHNode* node);
	float IntersectRay(Ray* ray, AABB bounds);
	
	static AABB CalculateBounds(std::vector<Primitive*>* primList, int first, int last);
	static AABB CalculateBoundsTop(std::vector<BVH*>* bvhList, int first, int last);
	
	bool isLeaf();

	// Variables
	int poolPtr;
	BVHNode** pool;
	BVHNode* root;
	AABB bounds;

	vec3 centroid;
	int leftFirst;
	int count;

	std::vector<Primitive*>* primList;

	int traversalMode = 1;
};