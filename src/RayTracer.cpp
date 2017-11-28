#include "template.h"

RayTracer::RayTracer() {
    
}

void RayTracer::getColor(int x, int y, Camera* cam , Triangle* tri){
	
    float u = x/SCRWIDTH;
    float v = y/SCRHEIGHT;
    
    float2 p = float2(u,v);
    
    Ray r = cam->generateRay(p);
    
    bool intersects = tri->IntersectTriangle(r);
    if(intersects){
    printf("yes\n");
    } else {
         printf("intersects\n");
    }
}
