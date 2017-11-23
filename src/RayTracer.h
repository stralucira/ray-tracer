#include "template.h"
#include "Triangle.h"
#include "Camera.h"

class RayTracer
{
public:

	RayTracer();
	RayTracer(float3 pos, float3 dir);
	~RayTracer();
    
    float3 getColor(int x, int y, Camera cam , Triangle tri);
};
