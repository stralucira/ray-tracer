#include "RayTracer.h"

float3 RayTracer::getColor(int x, int y, Camera cam , Triangle tri){
	
    float u = x/SCRWIDTH;
    float v = y/SCRHEIGHT;
    
    float2 p = new float2(u,v);
    
    Ray r = cam.generateRay(p);
    
    bool intersects = tri.IntersectTriangle(r);
    printf(intersects);
}
