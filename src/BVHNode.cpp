#include "template.h"
#include "BVH.h"
#include "BVHNode.h"

#define ENABLEBINNING 1

void BVHNode::Subdivide(BVHNode** pool, std::vector<Primitive*>* primList, int& poolPtr)
{
	//printf("BVH Node %i: Subdividing\n", poolPtr);
	if (count - this->leftFirst < 5) return;
	
	int tempPoolPtr = poolPtr;

	BVHNode* left = pool[poolPtr]; //this.left = pool[poolPtr++];
	BVHNode* right = pool[poolPtr + 1]; //this.right = pool[poolPtr++];

	if (!Partition(pool, primList, poolPtr)) return; //Partition();

	left->Subdivide(pool, primList, poolPtr); //this.left->Subdivide();
	right->Subdivide(pool, primList, poolPtr); //this.right->Subdivide();

	this->leftFirst = tempPoolPtr; count = 0; //this.isLeaf = false;
}

bool BVHNode::Partition(BVHNode** pool, std::vector<Primitive*>* primList, int& poolPtr)
{
	//printf("BVH Node %i: Partitioning \n", poolPtr);
	// A * N
	float parentNodeCost = this->bounds.CalculateVolume() * (count - leftFirst);
	float lowestNodeCost = INFINITY;
	float bestCoordinate;
	int bestAxis;

	if (ENABLEBINNING) // SAH with binning
	{
		vec3 lengths = vec3(this->bounds.max - this->bounds.min);
		for (int i = 1; i < 7; i++)
		{
			for (int axis = 0; axis < 3; axis++)
			{
				float splitCoordinate = this->bounds.min[axis] + lengths[axis] * (i / 7.0f);

				int leftCounter = 0, rightCounter = 0;
				vec3 maxLeft = vec3(-INFINITY), maxRight = vec3(-INFINITY);
				vec3 minLeft = vec3(INFINITY), minRight = vec3(INFINITY);

				for (int i = leftFirst; i < count; i++)
				{
					if ((*primList)[i]->centroid[axis] < splitCoordinate)
					{
						AdjustBounds((*primList)[i]->boundingBox, minLeft, maxLeft); leftCounter++;
					}
					else
					{
						AdjustBounds((*primList)[i]->boundingBox, minRight, maxRight); rightCounter++;
					}
				}

				// Aleft * Nleft + Aright * Nright
				float splitNodeCost = AABB(minLeft, maxLeft).CalculateVolume() * leftCounter + AABB(minRight, maxRight).CalculateVolume() * rightCounter;
				if (splitNodeCost < lowestNodeCost)
				{
					lowestNodeCost = splitNodeCost;
					bestAxis = axis;
					bestCoordinate = splitCoordinate;
				}
			}
		}
	}
	else // SAH without binning
	{
		for (int i = leftFirst; i < count; i++)
		{
			for (int axis = 0; axis < 3; axis++)
			{
				float splitCoordinate = (*primList)[i]->centroid[axis];

				int leftCounter = 0, rightCounter = 0;
				vec3 maxLeft = vec3(-INFINITY), maxRight = vec3(-INFINITY);
				vec3 minLeft = vec3(INFINITY), minRight = vec3(INFINITY);

				for (int i = leftFirst; i < count; i++)
				{
					if ((*primList)[i]->centroid[axis] < splitCoordinate)
					{
						AdjustBounds((*primList)[i]->boundingBox, minLeft, maxLeft); leftCounter++;
					}
					else
					{
						AdjustBounds((*primList)[i]->boundingBox, minRight, maxRight); rightCounter++;
					}
				}

				// Aleft * Nleft + Aright * Nright
				float splitNodeCost = AABB(minLeft, maxLeft).CalculateVolume() * leftCounter + AABB(minRight, maxRight).CalculateVolume() * rightCounter;
				if (splitNodeCost < lowestNodeCost)
				{
					lowestNodeCost = splitNodeCost;
					bestAxis = axis;
					bestCoordinate = splitCoordinate;
				}
			}
		}
	}

	// Aleft * Nleft + Aright * Nright >= A * N
	if (lowestNodeCost >= parentNodeCost) return false;

	int mid = leftFirst;
	for (int i = leftFirst; i < count; i++)
	{
		if ((*primList)[i]->centroid[bestAxis] < bestCoordinate)
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

void BVHNode::SubdivideTop(BVHNode** pool, std::vector<BVH*>* bvhList, int& poolPtr)
{
	//printf("BVH Node %i: Subdividing\n", poolPtr);
	if (count - this->leftFirst < 5) return;

	int tempPoolPtr = poolPtr;

	BVHNode* left = pool[poolPtr]; //this.left = pool[poolPtr++];
	BVHNode* right = pool[poolPtr + 1]; //this.right = pool[poolPtr++];

	if (!PartitionTop(pool, bvhList, poolPtr)) return; //Partition();

	left->SubdivideTop(pool, bvhList, poolPtr); //this.left->Subdivide();
	right->SubdivideTop(pool, bvhList, poolPtr); //this.right->Subdivide();

	this->leftFirst = tempPoolPtr; count = 0; //this.isLeaf = false;
}

bool BVHNode::PartitionTop(BVHNode** pool, std::vector<BVH*>* bvhList, int & poolPtr)
{
	vec3 lengths = bounds.max - bounds.min;

	int axis;

	float longestAxis = max(lengths.x, max(lengths.y, lengths.z));
	if (longestAxis == lengths.x) axis = 0;
	else if (longestAxis == lengths.y) axis = 1;
	else axis = 2;

	float splitCoordinate = (bounds.max[axis] + bounds.min[axis]) * 0.5f;

	int mid = leftFirst;
	for (int i = leftFirst; i < count; i++)
	{
		if ((*bvhList)[i]->centroid[axis] < splitCoordinate)
		{
			std::swap(bvhList[i], bvhList[mid]);
			mid++;
		}
	}

	//Left node.
	pool[poolPtr]->leftFirst = leftFirst;
	pool[poolPtr]->count = mid;
	pool[poolPtr]->bounds = BVH::CalculateBoundsTop(bvhList, pool[poolPtr]->leftFirst, pool[poolPtr]->count);

	poolPtr++;

	//Right node.
	pool[poolPtr]->leftFirst = mid;
	pool[poolPtr]->count = count;
	pool[poolPtr]->bounds = BVH::CalculateBoundsTop(bvhList, pool[poolPtr]->leftFirst, pool[poolPtr]->count);

	poolPtr++;

	return true;
}
