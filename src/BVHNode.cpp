#include "template.h"
#include "BVH.h"
#include "BVHNode.h"

void BVHNode::Subdivide(BVHNode** pool, std::vector<Primitive*> primitives, uint& poolPtr)
{
	//if (count - this->leftFirst < 5) return;

	uint tempPoolPtr = poolPtr;

	BVHNode* left = pool[poolPtr]; //this.left = pool[poolPtr++];
	BVHNode* right = pool[poolPtr + 1]; //this.right = pool[poolPtr++];
	
	if (!Partition(pool, primitives, poolPtr)) return; //Partition();
	
	left->Subdivide(pool, primitives, poolPtr); //this.left->Subdivide();
	right->Subdivide(pool, primitives, poolPtr); //this.right->Subdivide();
	
	this->leftFirst = tempPoolPtr; count = 0; //this.isLeaf = false;
}

bool BVHNode::Partition(BVHNode** pool, std::vector<Primitive*> primitives, uint& poolPtr)
{
	float parentNodeCost = this->bounds.CalculateVolume() * (count - leftFirst);
	float lowestNodeCost = INFINITY;

	float bestCoordinate;
	int bestDimension;

	for (int i = leftFirst; i < count; i++)
	{
		for (int dimension = 0; dimension < 3; dimension++)
		{
			float splitCoordinate = primitives[i]->centroid[dimension];
			int leftCounter = 0;
			int rightCounter = 0;

			vec3 maxLeft = vec3(-INFINITY);
			vec3 maxRight = vec3(-INFINITY);
			vec3 minLeft= vec3(INFINITY);
			vec3 minRight = vec3(INFINITY);

			for (int i = leftFirst; i < count; i++)
			{
				if (primitives[i]->centroid[dimension] < splitCoordinate)
				{
					AABB* bounds = primitives[i]->boundingBox;
					AdjustBounds(bounds, minLeft, maxLeft);
					leftCounter++;
				}
				else
				{
					AABB* bounds = primitives[i]->boundingBox;
					AdjustBounds(bounds, minRight, maxRight);
					rightCounter++;
				}
			}

			// Aleft * Nleft + Aright * Nright >= A * N
			float splitNodeCost = AABB(minLeft, maxLeft).CalculateVolume() * leftCounter + AABB(minRight, maxRight).CalculateVolume() * rightCounter;
			if (splitNodeCost < lowestNodeCost)
			{
				lowestNodeCost = splitNodeCost;
				bestDimension = dimension;
				bestCoordinate = splitCoordinate; 
			}
		}
	}
	
	if (lowestNodeCost >= parentNodeCost) { return false; }

	int mid = leftFirst;
	for (int i = leftFirst; i < count; i++)
	{
		if (primitives[i]->centroid[bestDimension] < bestCoordinate)
		{
			std::swap(primitives[i], primitives[mid]);
			mid++;
		}
	}

	//Left node.
	pool[poolPtr]->leftFirst = leftFirst;
	pool[poolPtr]->count = mid;
	pool[poolPtr]->bounds = BVH::CalculateBounds(primitives, pool[poolPtr]->leftFirst, pool[poolPtr]->count);

	poolPtr++;

	//Right node.
	pool[poolPtr]->leftFirst = mid;
	pool[poolPtr]->count = count;
	pool[poolPtr]->bounds = BVH::CalculateBounds(primitives, pool[poolPtr]->leftFirst, pool[poolPtr]->count);

	poolPtr++;

	return true;

/*	float lengthX = this->bounds.max.x - this->bounds.min.x;
	float lengthY = this->bounds.max.y - this->bounds.min.y;
	float lengthZ = this->bounds.max.z - this->bounds.min.z;

	int dimension;

	float biggest = glm::max(lengthX, glm::max(lengthY, lengthZ));
	if (biggest == lengthX)
	{
		dimension = 0;
	}
	else if (biggest == lengthY)
	{
		dimension = 1;
	}
	else 
	{
		dimension = 2;
	}

	float splitPlane = (bounds.max[dimension] + bounds.min[dimension]) * 0.5f;

	int mid = leftFirst;
	for (int i = leftFirst; i < count; i++)
	{
		if (primitives[i]->centroid[dimension] < splitPlane)
		{
			std::swap(primitives[i], primitives[mid]);
			mid++;
		}
	}

	//Left node.
	pool[poolPtr]->leftFirst = leftFirst;
	pool[poolPtr]->count = mid;
	pool[poolPtr]->bounds = BVH::CalculateBounds(primitives, pool[poolPtr]->leftFirst, pool[poolPtr]->count);

	poolPtr++;

	//Right node.
	pool[poolPtr]->leftFirst = mid;
	pool[poolPtr]->count = count;
	pool[poolPtr]->bounds = BVH::CalculateBounds(primitives, pool[poolPtr]->leftFirst, pool[poolPtr]->count);

	poolPtr++;

	return true;*/
}

void BVHNode::AdjustBounds(AABB* bounds, vec3& min, vec3& max)
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