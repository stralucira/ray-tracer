#include "Primitive.h"
#include "template.h"

class Sphere :
	public Primitive
{
public:
	Sphere();

	
	Sphere(float3 c, float r, Material mat);
	~Sphere();

	
	bool IntersectSphere( Ray ray );
	
	//Ray getNormal(Vector3 point) override;

private:
	float3 center;
	float radius;
	float radius2;
};