#include "template.h"

RayTracer::RayTracer() {
    
}



float3 RayTracer::getColor(int x, int y, Camera* cam, pointLight* light, std::vector<Primitive*> primList){
	
	float u = (float)x / (float)SCRWIDTH;
	float v = (float)y / (float)SCRHEIGHT;
    
    float2 p = float2(u,v);
    
    Ray r = cam->generateRay(p);
    
    Primitive* test = primList[0];
    
    float3 hitPoint = test->intersect(r);
	if (hitPoint.x == -1)
	{
		return float3(0, 0, 0);
	}
	else
	{
		float3 normal = test->getNormal(hitPoint);
		Ray shadowRay = Ray(hitPoint, (light->pos - hitPoint).normalized());
        
		return float3(test->mat->specs.x, test->mat->specs.y, test->mat->specs.z);
	}
}
