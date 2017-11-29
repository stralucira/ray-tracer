#include "template.h"

RayTracer::RayTracer() {
    
}



float3 RayTracer::getColor(int x, int y, Camera* cam, pointLight* light, Primitive* prim){
	
	float u = (float)x / (float)SCRWIDTH;
	float v = (float)y / (float)SCRHEIGHT;
    
    float2 p = float2(u,v);
    
    Ray r = cam->generateRay(p);
    
    float3 hitPoint = prim->intersect(r);
	if (hitPoint.x == -1)
	{
		return float3(0, 0, 0);
	}
	else
	{
		float3 normal = prim->getNormal(hitPoint);
		Ray shadowRay = Ray(hitPoint, (light->pos - hitPoint).normalized());
        
		return float3(prim->mat->specs.x, prim->mat->specs.y, prim->mat->specs.z);
	}
}
