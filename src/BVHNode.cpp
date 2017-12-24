#include "template.h"
#include "BVH.h"
#include "BVHNode.h"

#define ENABLEBINNING 1

void BVHNode::Subdivide(BVHNode** pool, std::vector<Primitive*>* primList, glm::uint& poolPtr)
{
	//printf("BVH Node %i: Subdividing\n", poolPtr);
	//if (count - this->leftFirst < 5) return;

	uint tempPoolPtr = poolPtr;

	BVHNode* left = pool[poolPtr]; //this.left = pool[poolPtr++];
	BVHNode* right = pool[poolPtr + 1]; //this.right = pool[poolPtr++];

	if (!Partition(pool, primList, poolPtr)) return; //Partition();

	left->Subdivide(pool, primList, poolPtr); //this.left->Subdivide();
	right->Subdivide(pool, primList, poolPtr); //this.right->Subdivide();

	this->leftFirst = tempPoolPtr; count = 0; //this.isLeaf = false;
}

void BVHNode::SubdivideTop(BVHNode** topPool, std::vector<AABB*>* objectBounds, glm::uint& topPoolPtr)
{
	if (count - this->leftFirst < 5) return;

	uint tempTopPoolPtr = topPoolPtr;

	BVHNode* topLeft = topPool[topPoolPtr];
	BVHNode* topRight = topPool[topPoolPtr];

	if (!PartitionTop(topPool, objectBounds, topPoolPtr)) return;

	topLeft->SubdivideTop(topPool, objectBounds, topPoolPtr);
	topRight->SubdivideTop(topPool, objectBounds, topPoolPtr);

	this->leftFirst = tempTopPoolPtr; count = 0;
}

bool BVHNode::Partition(BVHNode** pool, std::vector<Primitive*>* primList, glm::uint& poolPtr)
{
	//printf("BVH Node %i: Partitioning \n", poolPtr);
	// A * N
	float parentNodeCost = this->bounds.CalculateVolume() * (count - leftFirst);
	float lowestNodeCost = INFINITY;
	float bestCoordinate;
	int bestDimension;

#if !ENABLEBINNING
	for (int i = leftFirst; i < count; i++)
	{
		for (int dimension = 0; dimension < 3; dimension++)
		{
			float splitCoordinate = (*primList)[i]->centroid[dimension];
#else
	vec3 lengths = vec3(this->bounds.max - this->bounds.min);
	for (int i = 1; i < 7; i++)
	{
		for (int dimension = 0; dimension < 3; dimension++)
		{
			float splitCoordinate = this->bounds.min[dimension] + lengths[dimension] * (i / 7.0f);
#endif // ENABLEBINNING
			int leftCounter = 0;
			int rightCounter = 0;

			vec3 maxLeft = vec3(-INFINITY);
			vec3 maxRight = vec3(-INFINITY);
			vec3 minLeft = vec3(INFINITY);
			vec3 minRight = vec3(INFINITY);

			for (int i = leftFirst; i < count; i++)
			{
				if ((*primList)[i]->centroid[dimension] < splitCoordinate)
				{
					AdjustBounds((*primList)[i]->boundingBox, minLeft, maxLeft);
					leftCounter++;
				}
				else
				{
					AdjustBounds((*primList)[i]->boundingBox, minRight, maxRight);
					rightCounter++;
				}
			}

			// Aleft * Nleft + Aright * Nright
			float splitNodeCost = AABB(minLeft, maxLeft).CalculateVolume() * leftCounter + AABB(minRight, maxRight).CalculateVolume() * rightCounter;
			if (splitNodeCost < lowestNodeCost)
			{
				lowestNodeCost = splitNodeCost;
				bestDimension = dimension;
				bestCoordinate = splitCoordinate;
			}
		}
	}

	// Aleft * Nleft + Aright * Nright >= A * N
	if (lowestNodeCost >= parentNodeCost) { return false; }

	int mid = leftFirst;
	for (int i = leftFirst; i < count; i++)
	{
		if ((*primList)[i]->centroid[bestDimension] < bestCoordinate)
		{
			std::swap((*primList)[i], (*primList)[mid]);
			mid++;
		}
	}

	//Left node.
	pool[poolPtr]->leftFirst = leftFirst;
	pool[poolPtr]->count = mid;
	pool[poolPtr]->bounds = BVH::CalculateBounds(primList, pool[poolPtr]->leftFirst, pool[poolPtr]->count);

	poolPtr++;

	//Right node.
	pool[poolPtr]->leftFirst = mid;
	pool[poolPtr]->count = count;
	pool[poolPtr]->bounds = BVH::CalculateBounds(primList, pool[poolPtr]->leftFirst, pool[poolPtr]->count);

	poolPtr++;

	return true;
}

bool BVHNode::PartitionTop(BVHNode** topPool, std::vector<AABB*>* objectBounds, glm::uint& topPoolPtr)
{
	//Median split
	float xwidth = this->bounds.max.x - this->bounds.min.x;
	float ywidth = this->bounds.max.y - this->bounds.min.y;
	float zwidth = this->bounds.max.z - this->bounds.min.z;

	int dimension;

	float biggest = max(xwidth, max(ywidth, zwidth));
	if (biggest == xwidth)
		dimension = 0;
	else if (biggest == ywidth)
		dimension = 1;
	else
		dimension = 2;

	float splitCoord = 0.5f*(bounds.max[dimension] + bounds.min[dimension]);

	uint32_t mid = leftFirst;
	for (uint32_t i = leftFirst; i < count; i++)
	{
		if (primitives[i]->centroid[dimension] < splitCoord)
		{
			std::swap(primitives[i], primitives[mid]);
			mid++;
		}
	}

	//Left node.
	pool[poolPtr]->leftFirst = leftFirst;
	pool[poolPtr]->count = mid;
	pool[poolPtr]->bounds = BVH::CalculateTopBounds(primitives, pool[poolPtr]->leftFirst, pool[poolPtr]->count);

	poolPtr++;

	//Right node.
	pool[poolPtr]->leftFirst = mid;
	pool[poolPtr]->count = count;
	pool[poolPtr]->bounds = BVH::CalculateTopBounds(primitives, pool[poolPtr]->leftFirst, pool[poolPtr]->count);

	poolPtr++;

	return true;
}

void BVHNode::AdjustBounds(AABB * bounds, vec3& min, vec3& max)
{
	if (bounds->max.x > max.x) { max.x = bounds->max.x; }
	if (bounds->max.y > max.y) { max.y = bounds->max.y; }
	if (bounds->max.z > max.z) { max.z = bounds->max.z; }

	if (bounds->min.x < min.x) { min.x = bounds->min.x; }
	if (bounds->min.y < min.y) { min.y = bounds->min.y; }
	if (bounds->min.z < min.z) { min.z = bounds->min.z; }
}

bool BVHNode::isLeaf()
{
	return count != 0;
}