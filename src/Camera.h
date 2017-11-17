#include "template.h"

class Camera
{
public:

	Camera();
	Camera(float3 pos, float3 dir);
	~Camera();
	
	float3 pos;
	float3 dir;
	float focalDistance = 0;
};
