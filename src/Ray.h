#include "template.h"

class Ray
{
public:

	Ray(float3 orig, float3 dir);
	~Ray();
	
	float3 dir;
	float3 orig;
	float t;
};
