#include "template.h"
#include "BVH.h"

void BVH::ConstructBVH(std::vector<Primitive*>* primitives)
{
	printf("Constructing BVH for %i polygons...\n\n", primitives->size());

	// create index array
	//indices = new uint[primitives->size()];
	//for (uint i = 0; i < primitives->size(); i++) indices[i] = i;

	// allocate BVH root node
	//pool = reinterpret_cast<BVHNode**>(_aligned_malloc((primitives->size() * 2 - 1) * sizeof(BVHNode), 32));
	pool = new BVHNode*[primitives->size() * 2 - 1];
	for (glm::uint i = 0; i < (primitives->size() * 2 - 1); i++)
	{
		pool[i] = new BVHNode();
	}
	root = pool[0];
	poolPtr = 2;

	// subdivide root node
	root->leftFirst = 0;
	root->count = primitives->size();
	root->bounds = CalculateBounds(primitives, 0, primitives->size());
	root->Subdivide(pool, primitives, poolPtr);
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
		//float currentT = IntersectPrim(ray, node);
		if (IntersectPrim(ray, node) > prevT)
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
	printf("BVH Node %i: Calculating\n", first);
	
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