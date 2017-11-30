#include "template.h"

RayTracer::RayTracer() {
   
}

float3 RayTracer::getColor(int x, int y, Camera* cam, pointLight* light, std::vector<Primitive*> primList){
	
	float nearest = INFINITY;
	Primitive* hitPrim;
	float3 hitPoint;
	float distance;

	float u = (float)x / SCRWIDTH;
	float v = (float)y / SCRHEIGHT;
    
	float2 p = float2(u, v);
    
    Ray r = cam->generateRay(p);
    
	float3 color = float3(0, 0, 0);
    Primitive* test;
    
	for (size_t k = 0; k < primList.size(); k++)
	{
		test = primList[k];
		distance = test->intersect(r);

		if(distance < nearest & distance != -1) {
			nearest = distance;
			hitPrim = test;
		}
    }

    if (nearest == INFINITY) {
    	return float3(0,0,0);
    } else {
    	hitPoint = r.orig + r.dir*distance;
    	float3 normal = hitPrim->getNormal(hitPoint);


    	color += DirectIllumination(hitPoint, r.dir, normal, primList);
    }


    if (hitPoint.x != -1)
		{
			float3 normal = test->getNormal(hitPoint);
			Ray shadowRay = Ray(hitPoint, (light->pos - hitPoint).normalized());
        
			color = color + float3(test->mat->specs.x, test->mat->specs.y, test->mat->specs.z);
        }
    return color;
}


float3 RayTracer::DirectIllumination(float3 hitPoint, float3 direction, float3 normal, std::vector<Primitive*> primList ){
	return float3(1,1,1);
}