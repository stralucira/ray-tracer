#include "template.h"
#include "BVHTop.h"

// Top BVH calculations
void BVHTop::ConstructTopBVH(std::vector<BVH*>* bvhList)
{
	/*this->primIndices = new int[primList->size()];
	for (size_t i = 0; i < primList->size(); i++)
	{
		this->primIndices[i] = (*primList)[i]->index;
	}*/

	topPool = reinterpret_cast<BVHNode**>(_aligned_malloc((bvhList->size() * 2 - 1) * sizeof(BVHNode), 64));
	//topPool = new BVHNode*[bvhList->size() * 2 - 1];
	//this->bvhIndices = new int[bvhList->size()];
	for (size_t i = 0; i < bvhList->size(); i++)
	{
		topPool[i] = new BVHNode();
		//this->bvhIndices[i] = (*bvhList)[i]->index;
	}
	topRoot = topPool[0];
	topPoolPtr = 2;

	topRoot->leftFirst = 0;
	topRoot->count = bvhList->size();
	topRoot->bounds = CalculateTopBounds(bvhList, 0, bvhList->size());
	topRoot->SubdivideTop(topPool, bvhList, topPoolPtr);

	printf("");
}

AABB BVHTop::CalculateTopBounds(std::vector<BVH*>* bvhList, int first, int last)
{
	float maxX = -INFINITY; float minX = INFINITY;
	float maxY = -INFINITY; float minY = INFINITY;
	float maxZ = -INFINITY;	float minZ = INFINITY;

	for (int i = first; i < last; i++)
	{
		if ((*bvhList)[i]->boundingBox->max.x > maxX) { maxX = (*bvhList)[i]->boundingBox->max.x; }
		if ((*bvhList)[i]->boundingBox->max.y > maxY) { maxY = (*bvhList)[i]->boundingBox->max.y; }
		if ((*bvhList)[i]->boundingBox->max.z > maxZ) { maxZ = (*bvhList)[i]->boundingBox->max.z; }

		if ((*bvhList)[i]->boundingBox->min.x < minX) { minX = (*bvhList)[i]->boundingBox->min.x; }
		if ((*bvhList)[i]->boundingBox->min.y < minY) { minY = (*bvhList)[i]->boundingBox->min.y; }
		if ((*bvhList)[i]->boundingBox->min.z < minZ) { minZ = (*bvhList)[i]->boundingBox->min.z; }
	}
	return AABB(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));
}