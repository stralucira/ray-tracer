﻿#include "template.h"
#include "Camera.h"
#include "Ray.h"
#include <random>

int iCPU = omp_get_num_procs();

Camera::Camera()
{
	this->Init(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f));
}

Camera::Camera(vec3 pos, vec3 lookAt)
{
	this->Init(pos, lookAt);
}

// Initialize camera and the transformation matrix
void Camera::Init(vec3 pos, vec3 lookAt)
{
	this->pos = pos;
	this->SetPosition(pos);
	this->LookAt(lookAt);

	this->d = 1.0f;
	this->aspectRatio = (float)SCRHEIGHT / (float)SCRWIDTH;

	CalculateScreen();
}

void Camera::CalculateScreen()
{
	this->SetPosition(pos);
	
	vec3 screenCenter = this->pos + this->d * this->GetForward();

	this->p0 = screenCenter - this->GetRight() + this->GetUp() * this->aspectRatio; // top left corner
	this->p1 = screenCenter + this->GetRight() + this->GetUp() * this->aspectRatio; // top right corner
	this->p2 = screenCenter - this->GetRight() - this->GetUp() * this->aspectRatio; // bottom left corner
}

void Camera::PrintPosition()
{
	printf("----------------------------------------------\n Camera Position \n----------------------------------------------\n");
	printf("Position:  %.2ff, %.2ff, %.2ff\n", GetPosition().x, GetPosition().y, GetPosition().z);
	printf("Direction: %.2ff, %.2ff, %.2ff\n", GetForward().x, GetForward().y, GetForward().z);
	printf("\n");
	printf("Right: %.2f %.2f %.2f \n", GetRight().x, GetRight().y, GetRight().z);
	printf("Up: %.2f %.2f %.2f \n", GetUp().x, GetUp().y, GetUp().z);
	printf("\n");
	printf("Top left corner: %.2f %.2f %.2f \n", p0.x, p0.y, p0.z);
	printf("Top right corner: %.2f %.2f %.2f \n", p1.x, p1.y, p1.z);
	printf("Bottom left corner: %.2f %.2f %.2f \n", p2.x, p2.y, p2.z);
	printf("----------------------------------------------\n");
}

// Camera Movement
// Translations:
void Camera::Surge(float speed)
{
	pos += GetForward() * speed;
	CalculateScreen();
}
void Camera::Sway(float speed)
{
	pos += GetRight() * speed;
	CalculateScreen();
}
void Camera::Heave(float speed)
{
	pos += GetUp() * speed;
	CalculateScreen();
}
// Rotations:
void Camera::Roll(float angle)
{
	LookAt(GetPosition() + GetUp() - angle * GetRight());
	CalculateScreen();
}
void Camera::Pitch(float angle)
{
	LookAt(GetPosition() + GetForward() - angle * GetUp());
	CalculateScreen();
}
void Camera::Yaw(float angle)
{
	LookAt(GetPosition() + GetForward() - angle * GetRight());
	CalculateScreen();
}
// Zooming:
void Camera::Zoom(float increment)
{
	if (d + increment > 0)
	{
		d += increment;
		CalculateScreen();
	}
}

// Generate ray
Ray Camera::GenerateRay(int x, int y)
{
	return Ray(this->pos, normalize((this->p0 + ((float)x / (float)SCRWIDTH) * (this->p1 - this->p0) + ((float)y / (float)SCRHEIGHT) * (this->p2 - this->p0)) - this->pos));
	//return Ray(this->pos, normalizeSSE((this->p0 + ((float)x / (float)SCRWIDTH) * (this->p1 - this->p0) + ((float)y / (float)SCRHEIGHT) * (this->p2 - this->p0)) - this->pos));
}