#include "Primitive.h"

class Sphere :
	public Primitive
{
public:
	Sphere();

	
	Sphere(float3 c, float r, Material mat);
	~Sphere();

	
	//float intersects(Ray r) override;

	
	//Ray getNormal(Vector3 point) override;

private:
	float3 center;
	float radius;
};