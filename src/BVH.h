#pragma once
#include "Ray.h"
#include "Primitive.h"
#include "BVHNode.h"

class BVHNode;

class BVH
{
public:
	// Contructors
	BVH() {};
	BVH(std::vector<Primitive*>* primList)
	{
		this->primList = primList;
		ConstructBVH(primList);
	}

	// Functions
	void ConstructBVH(std::vector<Primitive*>* primList);
	void Traverse(Ray* ray, BVHNode* node, bool isShadowRay = false, int* depthRender = NULL);
	float Trace(Ray* ray, BVHNode* node);
	
	static AABB CalculateBounds(std::vector<Primitive*>* primList, int first, int last);
	static AABB CalculateBoundsTop(std::vector<BVH*>* bvhList, int first, int last);
	
	float CalculateDistance2(AABB bounds, vec3 point);
	float IntersectRay(Ray* ray, AABB bounds);
	bool isLeaf();

	void Subdivide(BVH** pool, std::vector<BVH*>* bvhList, int& poolPtr);
	bool Partition(BVH** pool, std::vector<BVH*>* bvhList, int& poolPtr);

	// Variables
	//uint* indices;
	int poolPtr;
	BVHNode** pool;
	BVHNode* root;
	AABB boundingBox;

	int leftFirst;	// 4 bytes
	int count; //4 bytes

	std::vector<Primitive*>* primList;

	vec3 centroid;
	int index;
	int traversalMode = 1;
};