#include "template.h"

RayTracer::RayTracer() {
   
}

float3 RayTracer::getColor(int x, int y, Camera* cam, pointLight* light, std::vector<Primitive*> primList){
	
	float u = (float)x / SCRWIDTH;
	float v = (float)y / SCRHEIGHT;
    
	float2 p = float2(u, v);
    
    Ray r = cam->generateRay(p);
    
	float3 color = float3(0, 0, 0);
    Primitive* test;
    
	for (size_t k = 0; k < primList.size(); k++)
	{
		test = primList[k];
		float3 hitPoint = test->intersect(r);
        if (hitPoint.x != -1)
		{
			float3 normal = test->getNormal(hitPoint);
			Ray shadowRay = Ray(hitPoint, (light->pos - hitPoint).normalized());
        
			color = color + float3(test->mat->specs.x, test->mat->specs.y, test->mat->specs.z);
        }
    }
    return color;
}
