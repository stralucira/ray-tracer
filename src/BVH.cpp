#include "template.h"
#include "BVH.h"

void BVH::ConstructTopBVH(std::vector<std::vector<Primitive*>>* objectList, std::vector<AABB*>* objectBounds)
{
	topPool = new BVHNode*[objectList->size() * 2 - 1];
	for (glm::uint i = 0; i < (objectList->size() * 2 - 1); i++)
	{
		topPool[i] = new BVHNode();
	}
	topRoot = topPool[0];
	topPoolPtr = 2;

	topRoot->leftFirst = 0;
	topRoot->count = objectList->size();
	topRoot->bounds = CalculateTopBounds(objectBounds, 0, objectList->size());
	topRoot->SubdivideTop(topPool, objectList, topPoolPtr);
}

void BVH::ConstructBVH(std::vector<Primitive*>* primList)
{
	//printf("Constructing BVH for %i polygons...\n", primList->size());

	// create index array
	//indices = new uint[primitives->size()];
	//for (uint i = 0; i < primitives->size(); i++) indices[i] = i;

	// allocate BVH root node
	pool = reinterpret_cast<BVHNode**>(_aligned_malloc((primList->size() * 2 - 1) * sizeof(BVHNode), 32));
	//pool = new BVHNode*[primList->size() * 2 - 1];
	for (glm::uint i = 0; i < (primList->size() * 2 - 1); i++)
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

	if (depthRender != NULL) { depthRender[0]++; }

	if (isShadowRay && prevT != INFINITY) { return; }

	if (!ray->Intersect(node->bounds)) { return; }

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
		this->Traverse(ray, pool[node->leftFirst], isShadowRay, depthRender);
		this->Traverse(ray, pool[node->leftFirst + 1], isShadowRay, depthRender);
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

AABB BVH::CalculateBounds(std::vector<Primitive*>* primitives, int first, int last)
{
	//printf("BVH Node %i: Calculating\n", first);
	
	float maxX = -INFINITY;
	float maxY = -INFINITY;
	float maxZ = -INFINITY;

	float minX = INFINITY;
	float minY = INFINITY;
	float minZ = INFINITY;

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

AABB BVH::CalculateTopBounds(std::vector<AABB*>* objectBounds, int first, int last)
{
	float maxX = -INFINITY;
	float maxY = -INFINITY;
	float maxZ = -INFINITY;

	float minX = INFINITY;
	float minY = INFINITY;
	float minZ = INFINITY;

	for (int i = first; i < last; i++)
	{
		if ((*objectBounds)[i]->max.x > maxX) { maxX = (*objectBounds)[i]->max.x; }
		if ((*objectBounds)[i]->max.y > maxY) { maxY = (*objectBounds)[i]->max.y; }
		if ((*objectBounds)[i]->max.z > maxZ) { maxZ = (*objectBounds)[i]->max.z; }

		if ((*objectBounds)[i]->min.x < minX) { minX = (*objectBounds)[i]->min.x; }
		if ((*objectBounds)[i]->min.y < minY) { minY = (*objectBounds)[i]->min.y; }
		if ((*objectBounds)[i]->min.z < minZ) { minZ = (*objectBounds)[i]->min.z; }
	}
	return AABB(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));
}