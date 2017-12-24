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

	// Variables
	//uint* indices;
	glm::uint poolPtr;
	BVHNode* root;
	BVHNode** pool;
	
	std::vector<Primitive*>* primList;
};