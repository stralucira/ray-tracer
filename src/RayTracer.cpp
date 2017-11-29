#include "template.h"

RayTracer::RayTracer() {
    
}



float3 RayTracer::getColor(int x, int y, Camera* cam, pointLight* light, Triangle* tri){
	
    float u = x/SCRWIDTH;
    float v = y/SCRHEIGHT;
    
    float2 p = float2(u,v);
    
    Ray r = cam->generateRay(p);
    
    float3 hitPoint = tri->IntersectTriangle(r);
    float3 normal = tri->getNormal(hitPoint);
    Ray shadowRay = Ray(hitPoint, (light->pos - hitPoint).normalized());

    return float3(tri->mat->specs.x, tri->mat->specs.y, tri->mat->specs.z);
}
