#include "template.h"
#include "BVH.h"

void BVH::ConstructBVH(Primitive** primitives)
{
	// create index array
	//indices = new uint[N];
	//for (int i = 0; i < N; i++) indices[i] = i;

	// allocate BVH root node
	pool = new BVHNode*[N * 2 - 1];
	root = pool[0];
	poolPtr = 2;

	// subdivide root node
	root->leftFirst = 0;
	root->count = N;
	root->bounds = CalculateBounds(primitives, 0, N);
	root->Subdivide(pool, primitives, poolPtr);
}

AABB BVH::CalculateBounds(Primitive** primitives, int first, int last)
{
	float maxX = -INFINITY;
	float maxY = -INFINITY;
	float maxZ = -INFINITY;

	float minX = INFINITY;
	float minY = INFINITY;
	float minZ = INFINITY;

	for (int i = first; i < last; i++)
	{
		if (primitives[i]->boundingBox->max.x > maxX) { maxX = primitives[i]->boundingBox->max.x; }
		if (primitives[i]->boundingBox->max.y > maxY) { maxY = primitives[i]->boundingBox->max.y; }
		if (primitives[i]->boundingBox->max.z > maxZ) { maxZ = primitives[i]->boundingBox->max.z; }
		
		if (primitives[i]->boundingBox->min.x < minX) { minX = primitives[i]->boundingBox->min.x; }
		if (primitives[i]->boundingBox->min.y < minY) { minY = primitives[i]->boundingBox->min.y; }
		if (primitives[i]->boundingBox->min.z < minZ) { minZ = primitives[i]->boundingBox->min.z; }
	}
	return AABB(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));
}

void BVH::Traverse(Ray* ray)
{
	float prevRayT = ray->t;
}