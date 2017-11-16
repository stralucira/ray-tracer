#include "template.h"

class Ray
{
public:

	Ray(float3 orig, float3 dir);
	~Ray();
	
	float3 getDirection() const;
	float3 getOrigin() const;

private:
	
	float3 direction;
	float3 origin;
};
