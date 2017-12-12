#include "template.h"
#include "BVHNode.h"

bool BVHNode::isLeaf()
{
	return count != 0;
}