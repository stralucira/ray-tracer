#pragma once
#include "template.h"
#include "Ray.h"

class Camera
{
public:
	Camera();
	void Init();
	void CalculateScreen();
	void PrintPosition();

	Ray GenerateRay(int x, int y);
	
	// Camera movement
	// Translation:
	void Surge(float speed);	// Moving forward and backward on the X-axis.
	void Sway(float speed);		// Moving left and right on the Y-axis.
	void Heave(float speed);	// Moving up and down on the Z-axis.
	// Rotation:
	void Roll(float angle);		// Tilting side to side on the X-axis.
	void Pitch(float angle);	// Tilting forward and backward on the Y-axis.
	void Yaw(float angle);		// Turning left and right on the Z-axis.
	// Zooming:
	void Zoom(float distance);

	vec3 pos;
	vec3 dir;
	vec3 p0, p1, p2;

	vec3 viewDir;
	vec3 up;
	vec3 right;

	float d;
	float aspectRatio;
};
