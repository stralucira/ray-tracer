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

	BVH(std::vector<Primitive*>* primList, int dummy)
	{
		this->primList = primList;
		ConstructTopBVH(primList);
	}

	// Functions
	void ConstructBVH(std::vector<Primitive*>* primList);
	void Traverse(Ray* ray, BVHNode* node, bool isShadowRay = false, int* depthRender = NULL);
	float Trace(Ray* ray, BVHNode* node);
	
	static AABB CalculateBounds(std::vector<Primitive*>* primList, int first, int last);
	
	float CalculateDistance(AABB bounds, vec3 point);
	float CalculateDistance2(AABB bounds, vec3 point);
	float IntersectRay(Ray* ray, AABB bounds);
	int ReturnLargest(vec3 point);

	// Variables
	//uint* indices;
	int poolPtr;
	BVHNode** pool;
	BVHNode* root;
	//vec4* distancesAxis;

	int index;
	std::vector<Primitive*>* primList;
	
	void ConstructTopBVH(std::vector<Primitive*>* primList);
};