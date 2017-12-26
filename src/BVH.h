#pragma once
#include "Ray.h"
#include "Primitive.h"
#include "BVHNode.h"

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
	void Traverse(Ray* ray, BVHNode* node, bool isShadowRay = false, int* depthRender = NULL);
	float Trace(Ray* ray, BVHNode* node);
	static AABB CalculateBounds(std::vector<Primitive*>* primList, int first, int last);

	float CalculateDistance2(AABB bounds, vec3 point);
	int ReturnLargest(vec3 point);

	// Variables
	//uint* indices;
	unsigned int poolPtr;
	BVHNode* root;
	BVHNode** pool;
	
	std::vector<Primitive*>* primList;
	
	//// Top BVH calculations
	//// Constructors
	//BVH(std::vector<std::vector<Primitive*>>* objectList, std::vector<AABB*>* objectBounds)
	//{
	//	this->objectList = objectList;
	//	ConstructTopBVH(objectBounds);
	//}

	//// Functions
	//void ConstructTopBVH(std::vector<AABB*>* objectBounds);
	//static AABB CalculateTopBounds(std::vector<AABB*>* objectBounds, int first, int last);
	//
	//// Variables
	//unsigned int topPoolPtr = 2;
	//BVHNode* topRoot;
	//BVHNode** topPool;

	//std::vector<std::vector<Primitive*>>* objectList;
};