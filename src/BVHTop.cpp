#include "template.h"
#include "BVHTop.h"

// Top BVH calculations
void BVHTop::ConstructBVHTop(std::vector<BVH*>* bvhList)
{
	// allocate BVH root node
	//topPool = new BVH*[bvhList->size() * 2 - 1];
	topPool = reinterpret_cast<BVH**>(_mm_malloc((bvhList->size() * 2 - 1) * sizeof(BVH), 64));
	for (size_t i = 0; i < bvhList->size(); i++)
	{
		topPool[i] = (*bvhList)[i];
	}
	topRoot = topPool[0];
	topPoolPtr = 2;

	// subdivide root node
	topRoot->leftFirst = 0;
	topRoot->count = bvhList->size();
	topRoot->bounds = BVH::CalculateBoundsTop(bvhList, 0, bvhList->size());
	topRoot->Subdivide(topPool, bvhList, topPoolPtr);
}

void BVHTop::TraverseTop(Ray* ray, BVH* node, bool isShadowRay, int* depthRender)
{
	float prevT = ray->t;
	Primitive* prevHit = ray->hit;

	if (depthRender != NULL) depthRender[0]++;
	if (isShadowRay && prevT != INFINITY) return;
	if (IntersectRay(ray, node->bounds) == INFINITY) return;

	if (node->isLeaf())
	{
		node->Traverse(ray, node->root, 0, depthRender); // we need to go deeper
		if (ray->t > prevT)
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
			this->TraverseTop(ray, topPool[node->leftFirst], isShadowRay, depthRender);
			this->TraverseTop(ray, topPool[node->leftFirst + 1], isShadowRay, depthRender);
		}
		break;
		// ------------------------------------------------------------------------------
		// Ordered Traversal (Preferred method, it is actually faster using this)
		// Calculate distance to both child nodes, Traverse the nearest child node first
		// ------------------------------------------------------------------------------
		case 1:
		{
			float leftDistance2 = CalculateDistance2(topPool[node->leftFirst]->bounds, ray->orig);
			float rightDistance2 = CalculateDistance2(topPool[node->leftFirst + 1]->bounds, ray->orig);

			if (leftDistance2 < rightDistance2)
			{
				TraverseTop(ray, topPool[node->leftFirst], isShadowRay, depthRender);
				if (ray->t * ray->t < rightDistance2) return;
				TraverseTop(ray, topPool[node->leftFirst + 1], isShadowRay, depthRender);
			}
			else
			{
				TraverseTop(ray, topPool[node->leftFirst + 1], isShadowRay, depthRender);
				if (ray->t * ray->t < leftDistance2) return;
				TraverseTop(ray, topPool[node->leftFirst], isShadowRay, depthRender);
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
			vec3 leftCentroid = topPool[node->leftFirst]->bounds.CalculateCentroid();
			vec3 rightCentroid = topPool[node->leftFirst + 1]->bounds.CalculateCentroid();

			vec3 distances = glm::abs(leftCentroid - rightCentroid);
			int axis = returnLargest(distances);

			bool index = ray->dir[axis] > 0.0f;

			int closeIndex = index ? node->leftFirst : node->leftFirst + 1;
			int farIndex = index ? node->leftFirst + 1 : node->leftFirst;

			float closeT = ray->t;
			float farT = ray->t;

			float closeAABBdistance = IntersectRay(ray, topPool[closeIndex]->bounds);
			float farAABBdistance = IntersectRay(ray, topPool[farIndex]->bounds);

			if (closeAABBdistance < INFINITY)
			{
				this->TraverseTop(ray, topPool[closeIndex], isShadowRay, depthRender);
				closeT = ray->t;
				if (closeT < farAABBdistance) return;
			}

			if (farAABBdistance < INFINITY)
			{
				this->TraverseTop(ray, topPool[farIndex], isShadowRay, depthRender);
				farT = ray->t;
				if (closeT < farT) ray->t = closeT;
				else ray->t = farT;
			}
		}
		break;
		}
	}
}

float BVHTop::IntersectRay(Ray* ray, AABB bounds)
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

