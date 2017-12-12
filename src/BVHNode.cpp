#include "template.h"
#include "BVHNode.h"

void BVHNode::Subdivide()
{
	if (count - this->leftFirst < 5) return;
}

bool BVHNode::isLeaf()
{
	return count != 0;
}