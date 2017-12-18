#include "template.h"
#include "BVH.h"

void BVH::ConstructBVH(std::vector<Primitive*>* primitives)
{
	// create index array
	//indices = new uint[N];
	//for (int i = 0; i < N; i++) indices[i] = i;

	// allocate BVH root node
	pool = new BVHNode*[N * 2 - 1];
	for (glm::uint i = 0; i < (N * 2 - 1); i++)
	{
		pool[i] = new BVHNode();
	}
	root = pool[0];
	poolPtr = 2;

	// subdivide root node
	root->leftFirst = 0;
	root->count = N;
	root->bounds = CalculateBounds(primitives, 0, N);
	root->Subdivide(pool, primitives, poolPtr);
}

void BVH::Traverse(Ray* ray, BVHNode* node, bool isShadowRay)
{
	float prevT = ray->t;
	Primitive* prevHit = ray->hit;

	if (isShadowRay && prevT != INFINITY) { return; }

	if (!ray->Intersect(node->bounds)) { return; }

	if (node->isLeaf())
	{
		//float currentT = IntersectPrim(ray, node);
		if (IntersectPrim(ray, node) > prevT)
		{
			ray->t = prevT;
			ray->hit = prevHit;
		}
	}

	else
	{
		this->Traverse(ray, pool[node->leftFirst], isShadowRay);
		this->Traverse(ray, pool[node->leftFirst + 1], isShadowRay);
	}
}

float BVH::IntersectPrim(Ray* ray, BVHNode* node)
{
	float nearest = INFINITY;

	for (int i = node->leftFirst; i < node->count; i++)
	{
		if ((*primitives)[i]->intersect(ray) && nearest > ray->t)
		{
			nearest = ray->t;
			ray->hit = (*primitives)[i];
		}
	}

	if (ray->t > nearest) { ray->t = nearest; }

	return ray->t;
}

AABB BVH::CalculateBounds(std::vector<Primitive*>* primitives, int first, int last)
{
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