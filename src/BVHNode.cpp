#include "template.h"
#include "BVH.h"
#include "BVHNode.h"

void BVHNode::Subdivide(BVHNode** pool, std::vector<Primitive*>* primitives, uint& poolPtr)
{
	if (count - this->leftFirst < 5) return;

	uint tempPoolPtr = poolPtr;

	BVHNode* left = pool[poolPtr]; //this.left = pool[poolPtr++];
	BVHNode* right = pool[poolPtr]; //this.right = pool[poolPtr++];
	
	if (!Partition(pool, primitives, poolPtr)) return; //Partition();
	
	left->Subdivide(pool, primitives, poolPtr); //this.left->Subdivide();
	right->Subdivide(pool, primitives, poolPtr); //this.right->Subdivide();
	
	this->leftFirst = tempPoolPtr; count = 0; //this.isLeaf = false;
}

bool BVHNode::Partition(BVHNode** pool, std::vector<Primitive*>* primitives, uint& poolPtr)
{
	float lengthX = this->bounds.max.x - this->bounds.min.x;
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
		if ((*primitives)[i]->centroid[dimension] < splitPlane)
		{
			std::swap((*primitives)[i], (*primitives)[mid]);
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
}

bool BVHNode::isLeaf()
{
	return count != 0;
}