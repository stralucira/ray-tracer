#pragma once
#include "template.h"
#include "Ray.h"

class Camera
{
public:
	Camera();
	Camera(vec3 pos, vec3 lookAt);
	void Init(vec3 pos, vec3 lookAt);
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
	vec3 p0, p1, p2;

	float d;
	float aspectRatio;

	// Camera matrix
	void SetPosition(vec3& pos)
	{
		glm::mat4& M = transform;
		M[3][0] = pos.x, M[3][1] = pos.y, M[3][2] = pos.z;
	}

	vec3 GetPosition()
	{
		glm::mat4& M = transform;
		return vec3(M[3][0], M[3][1], M[3][2]);
	}

	vec3 GetRight()
	{
		glm::mat4& M = transform;
		return vec3(M[0][0], M[0][1], M[0][2]);
	}

	vec3 GetUp()
	{
		glm::mat4& M = transform;
		return vec3(M[1][0], M[1][1], M[1][2]);
	}

	vec3 GetForward()
	{
		glm::mat4& M = transform;
		return -vec3(M[2][0], M[2][1], M[2][2]);
	}
	
	void LookAt(vec3 target)
	{
		transform = inverse(lookAt(GetPosition(), target, vec3(0, 1, 0)));
	}

	glm::mat4 transform;
};
