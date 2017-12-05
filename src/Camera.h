#pragma once
#include "template.h"
#include "Ray.h"

class Camera
{
public:
	Camera();
	void Init();
	void UpdatePosition();
	void GenerateRays();
	void UpdateRays();

	Ray* cameraRays[SCRWIDTH*SCRHEIGHT];

	vec3 pos;
	vec3 dir;
	vec3 screenCenter;
	vec3 p0, p1, p2;

	float d;
	float aspectRatio;
	float width, height;

	void CalculateScreen();
	void TransCamera(glm::mat4 transMatrix);

	glm::mat4 transMatrix;

	vec3 up;
	vec3 rotY;
	vec3 rotX;

};
