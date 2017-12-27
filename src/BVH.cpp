#include "template.h"
#include "BVH.h"
#include "BVHNode.h"

void BVH::ConstructBVH(std::vector<Primitive*>* primList)
{
	//printf("Constructing BVH for %i polygons...\n", primList->size());

	// create index array
	//indices = new uint[primitives->size()];
	//for (uint i = 0; i < primitives->size(); i++) indices[i] = i;

	// allocate BVH root node
	pool = reinterpret_cast<BVHNode**>(_aligned_malloc((primList->size() * 2 - 1) * sizeof(BVHNode), 64));
	//lengthsAxis = new vec4[primList->size() * 2 - 1];
	//distancesAxis = reinterpret_cast<vec4*>(_aligned_malloc((primList->size() * 2 - 1) * sizeof(vec4), 64));
	//pool = new BVHNode*[primList->size() * 2 - 1];
	for (size_t i = 0; i < (primList->size() * 2 - 1); i++)
	{
		pool[i] = new BVHNode();
		//distancesAxis[i] = vec4(0.0f);
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
		// ------------------------------------------------------------------------------
		// Basic Traversal (It gets the job done)
		// ------------------------------------------------------------------------------
		//this->Traverse(ray, pool[node->leftFirst], isShadowRay, depthRender);
		//this->Traverse(ray, pool[node->leftFirst + 1], isShadowRay, depthRender);

		// ------------------------------------------------------------------------------
		// Ordered Traversal (It is actually a bit faster using this)
		// Calculate distance to both child nodes, Traverse the nearest child node first
		// ------------------------------------------------------------------------------
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

		// ------------------------------------------------------------------------------
		// Ordered Traversal (A bit slower then distance calculations?)
		// Determine the axis for which the child node centroids are furthest apart
		// Use ray direction sign for that axis to determine near and far.
		// ------------------------------------------------------------------------------
		/*vec3 leftCentroid = pool[node->leftFirst]->bounds.CalculateCentroid();
		vec3 rightCentroid = pool[node->leftFirst + 1]->bounds.CalculateCentroid();

		vec3 distances = glm::abs(leftCentroid - rightCentroid);
		int axis = ReturnLargest(distances);

		bool index = ray->dir[axis] > 0.0f;
		//distancesAxis[node->leftFirst] = vec4(distances, axis);

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
		}*/
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

float BVH::CalculateDistance2(AABB bounds, vec3 point)
{
	float distanceX = glm::max(glm::max(bounds.min.x - point.x, 0.0f), glm::max(0.0f, point.x - bounds.max.x));
	float distanceY = glm::max(glm::max(bounds.min.y - point.y, 0.0f), glm::max(0.0f, point.y - bounds.max.y));
	float distanceZ = glm::max(glm::max(bounds.min.z - point.z, 0.0f), glm::max(0.0f, point.z - bounds.max.z));

	return distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ;
}

AABB BVH::CalculateBounds(std::vector<Primitive*>* primitives, int first, int last)
{
	//printf("BVH Node %i: Calculating\n", first);

	float maxX = -INFINITY; float minX = INFINITY;
	float maxY = -INFINITY; float minY = INFINITY;
	float maxZ = -INFINITY; float minZ = INFINITY;

	for (int i = first; i < last; i++)
	{
		if ((*primitives)[i]->boundingBox->max.x > maxX) { maxX = (*primitives)[i]->boundingBox->max.x; }
		if ((*primitives)[i]->boundingBox->max.y > maxY) { maxY = (*primitives)[i]->boundingBox->max.y; }
		if ((*primitives)[i]->boundingBox->max.z > maxZ) { maxZ = (*primitives)[i]->boundingBox->max.z; }

		if ((*primitives)[i]->boundingBox->min.x < minX) { minX = (*primitives)[i]->boundingBox->min.x; }
		if ((*primitives)[i]->boundingBox->min.y < minY) { minY = (*primitives)[i]->boundingBox->min.y; }
		if ((*primitives)[i]->boundingBox->min.z < minZ) { minZ = (*primitives)[i]->boundingBox->min.z; }
	}
	return AABB(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));
}

AABB BVH::CalculateBoundsTop(std::vector<BVH*>* bvhList, int first, int last)
{
	//printf("BVH Node %i: Calculating\n", first);

	float maxX = -INFINITY; float minX = INFINITY;
	float maxY = -INFINITY; float minY = INFINITY;
	float maxZ = -INFINITY; float minZ = INFINITY;

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

int BVH::ReturnLargest(vec3 point)
{
	return point.x > point.y ?
		(point.x > point.z ? 0 : 2) :
		(point.y > point.z ? 1 : 2);
}