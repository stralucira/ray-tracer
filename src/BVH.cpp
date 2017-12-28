#include "template.h"
#include "BVH.h"
#include "BVHNode.h"

void BVH::ConstructBVH(std::vector<Primitive*>* primList)
{
	// allocate BVH root node
	//pool = new BVHNode*[primList->size() * 2 - 1];
	pool = reinterpret_cast<BVHNode**>(_mm_malloc((primList->size() * 2 - 1) * sizeof(BVHNode), 64));
	for (size_t i = 0; i < (primList->size() * 2 - 1); i++)
	{
		pool[i] = new BVHNode();
	}
	root = pool[0];
	poolPtr = 2;

	// subdivide root node
	root->leftFirst = 0;
	root->count = primList->size();
	root->bounds = CalculateBounds(primList, 0, primList->size());
	root->Subdivide(pool, primList, poolPtr);
}

void BVH::Traverse(Ray* ray, BVHNode* node, bool isShadowRay, int* depthRender)
{
	float prevT = ray->t;
	Primitive* prevHit = ray->hit;

	if (depthRender != NULL) depthRender[0]++;
	if (isShadowRay && prevT != INFINITY) return;
	if (IntersectRay(ray, node->bounds) == INFINITY) return;

	if (node->isLeaf())
	{
		if (Trace(ray, node) > prevT)
		{
			ray->t = prevT;
			ray->hit = prevHit;
		}
	}

	else
	{
		switch (traversalMode)
		{
		// ------------------------------------------------------------------------------
		// Basic Traversal (It gets the job done)
		// ------------------------------------------------------------------------------
		case 0:
		{
			this->Traverse(ray, pool[node->leftFirst], isShadowRay, depthRender);
			this->Traverse(ray, pool[node->leftFirst + 1], isShadowRay, depthRender);
		}
		break;
		// ------------------------------------------------------------------------------
		// Ordered Traversal (Preferred method, it is actually faster using this)
		// Calculate distance to both child nodes, Traverse the nearest child node first
		// ------------------------------------------------------------------------------
		case 1:
		{
			float leftDistance2 = CalculateDistance2(pool[node->leftFirst]->bounds, ray->orig);
			float rightDistance2 = CalculateDistance2(pool[node->leftFirst + 1]->bounds, ray->orig);

			if (leftDistance2 < rightDistance2)
			{
				Traverse(ray, pool[node->leftFirst], isShadowRay, depthRender);
				if (ray->t * ray->t < rightDistance2) return;
				Traverse(ray, pool[node->leftFirst + 1], isShadowRay, depthRender);
			}
			else
			{
				Traverse(ray, pool[node->leftFirst + 1], isShadowRay, depthRender);
				if (ray->t * ray->t < leftDistance2) return;
				Traverse(ray, pool[node->leftFirst], isShadowRay, depthRender);
			}
		}
		break;
		// ------------------------------------------------------------------------------
		// Ordered Traversal (Least depth but a bit slower than distance calculation)
		// Determine the axis for which the child node centroids are furthest apart
		// Use ray direction sign for that axis to determine near and far.
		// ------------------------------------------------------------------------------
		case 2:
		{
			vec3 leftCentroid = pool[node->leftFirst]->bounds.CalculateCentroid();
			vec3 rightCentroid = pool[node->leftFirst + 1]->bounds.CalculateCentroid();

			vec3 distances = glm::abs(leftCentroid - rightCentroid);
			int axis = returnLargest(distances);

			bool index = ray->dir[axis] > 0.0f;

			int closeIndex = index ? node->leftFirst : node->leftFirst + 1;
			int farIndex = index ? node->leftFirst + 1 : node->leftFirst;

			float closeT = ray->t;
			float farT = ray->t;

			float closeAABBdistance = IntersectRay(ray, pool[closeIndex]->bounds);
			float farAABBdistance = IntersectRay(ray, pool[farIndex]->bounds);

			if (closeAABBdistance < INFINITY)
			{
				this->Traverse(ray, pool[closeIndex], isShadowRay, depthRender);
				closeT = ray->t;
				if (closeT < farAABBdistance) return;
			}

			if (farAABBdistance < INFINITY)
			{
				this->Traverse(ray, pool[farIndex], isShadowRay, depthRender);
				farT = ray->t;
				if (closeT < farT) ray->t = closeT;
				else ray->t = farT;
			}
		}
		break;
		}
	}
}

float BVH::Trace(Ray* ray, BVHNode* node)
{
	float nearest = INFINITY;

	for (int i = node->leftFirst; i < node->count; i++)
	{
		if ((*this->primList)[i]->intersect(ray) && nearest > ray->t)
		{
			nearest = ray->t;
			ray->hit = (*this->primList)[i];
		}
	}

	if (ray->t > nearest) { ray->t = nearest; }

	return ray->t;
}

float BVH::IntersectRay(Ray* ray, AABB bounds)
{
	float tmin, tmax, txmin, txmax, tymin, tymax, tzmin, tzmax;

	txmin = (bounds.min.x - ray->orig.x) * ray->invDir.x;
	txmax = (bounds.max.x - ray->orig.x) * ray->invDir.x;
	tymin = (bounds.min.y - ray->orig.y) * ray->invDir.y;
	tymax = (bounds.max.y - ray->orig.y) * ray->invDir.y;

	tmin = glm::min(txmin, txmax);
	tmax = glm::max(txmin, txmax);

	tmin = glm::max(tmin, glm::min(tymin, tymax));
	tmax = glm::min(tmax, glm::max(tymin, tymax));

	tzmin = (bounds.min.z - ray->orig.z) * ray->invDir.z;
	tzmax = (bounds.max.z - ray->orig.z) * ray->invDir.z;

	tmin = glm::max(tmin, glm::min(tzmin, tzmax));
	tmax = glm::min(tmax, glm::max(tzmin, tzmax));

	if (tmax < 0.0f) return INFINITY;
	if (tmin > tmax) return INFINITY;

	return tmin;
}

AABB BVH::CalculateBounds(std::vector<Primitive*>* primitives, int first, int last)
{
	float maxX = -INFINITY; float minX = INFINITY;
	float maxY = -INFINITY; float minY = INFINITY;
	float maxZ = -INFINITY; float minZ = INFINITY;

	for (int i = first; i < last; i++)
	{
		if ((*primitives)[i]->bounds->max.x > maxX) { maxX = (*primitives)[i]->bounds->max.x; }
		if ((*primitives)[i]->bounds->max.y > maxY) { maxY = (*primitives)[i]->bounds->max.y; }
		if ((*primitives)[i]->bounds->max.z > maxZ) { maxZ = (*primitives)[i]->bounds->max.z; }

		if ((*primitives)[i]->bounds->min.x < minX) { minX = (*primitives)[i]->bounds->min.x; }
		if ((*primitives)[i]->bounds->min.y < minY) { minY = (*primitives)[i]->bounds->min.y; }
		if ((*primitives)[i]->bounds->min.z < minZ) { minZ = (*primitives)[i]->bounds->min.z; }
	}
	return AABB(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));
}

AABB BVH::CalculateBoundsTop(std::vector<BVH*>* bvhList, int first, int last)
{
	float maxX = -INFINITY; float minX = INFINITY;
	float maxY = -INFINITY; float minY = INFINITY;
	float maxZ = -INFINITY; float minZ = INFINITY;

	for (int i = first; i < last; i++)
	{
		if ((*bvhList)[i]->bounds.max.x > maxX) { maxX = (*bvhList)[i]->bounds.max.x; }
		if ((*bvhList)[i]->bounds.max.y > maxY) { maxY = (*bvhList)[i]->bounds.max.y; }
		if ((*bvhList)[i]->bounds.max.z > maxZ) { maxZ = (*bvhList)[i]->bounds.max.z; }

		if ((*bvhList)[i]->bounds.min.x < minX) { minX = (*bvhList)[i]->bounds.min.x; }
		if ((*bvhList)[i]->bounds.min.y < minY) { minY = (*bvhList)[i]->bounds.min.y; }
		if ((*bvhList)[i]->bounds.min.z < minZ) { minZ = (*bvhList)[i]->bounds.min.z; }
	}
	return AABB(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));
}

bool BVH::isLeaf()
{
	return count != 0;
}

void BVH::Subdivide(BVH** pool, std::vector<BVH*>* bvhList, int& poolPtr)
{
	if (count - this->leftFirst < 5) return;

	int tempPoolPtr = poolPtr;

	BVH* left = pool[poolPtr]; //this.left = pool[poolPtr++];
	BVH* right = pool[poolPtr + 1]; //this.right = pool[poolPtr++];

	if (!Partition(pool, bvhList, poolPtr)) return; //Partition();

	left->Subdivide(pool, bvhList, poolPtr); //this.left->Subdivide();
	right->Subdivide(pool, bvhList, poolPtr); //this.right->Subdivide();

	this->leftFirst = tempPoolPtr; count = 0; //this.isLeaf = false;
}

bool BVH::Partition(BVH** pool, std::vector<BVH*>* bvhList, int & poolPtr)
{
	vec3 lengths = bounds.max - bounds.min;
	int axis = returnLargest(lengths);
	
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

	// Left node
	pool[poolPtr]->leftFirst = leftFirst;
	pool[poolPtr]->count = mid;
	pool[poolPtr]->bounds = BVH::CalculateBoundsTop(bvhList, pool[poolPtr]->leftFirst, pool[poolPtr]->count);

	poolPtr++;

	// Right node
	pool[poolPtr]->leftFirst = mid;
	pool[poolPtr]->count = count;
	pool[poolPtr]->bounds = BVH::CalculateBoundsTop(bvhList, pool[poolPtr]->leftFirst, pool[poolPtr]->count);

	poolPtr++;
	
	return true;
}
