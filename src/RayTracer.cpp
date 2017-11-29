#include "template.h"

RayTracer::RayTracer() {
    
}



void RayTracer::getColor(int x, int y, Camera* cam, pointLight* light, Triangle* tri){
	
    float u = x/SCRWIDTH;
    float v = y/SCRHEIGHT;
    
    float2 p = float2(u,v);
    
    Ray r = cam->generateRay(p);
    
    float3 hitPoint = tri->IntersectTriangle(r);
    float3 normal = tri->getNormal(hitPoint);
    Ray shadowRay = Ray(hitPoint, (light->pos - hitPoint).normalized());

}
