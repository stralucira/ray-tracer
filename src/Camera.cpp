#include "template.h"
#include "Camera.h"
#include "Ray.h"

Camera::Camera()
{
	this->Init();
}

void Camera::Init()
{
	this->pos = float3(0.0f, 0.0f, 0.0f);
	this->dir = float3(0.0f, 0.0f, 1.0f);
	this->d = 1.0f;
	this->aspectRatio = (float)SCRHEIGHT / (float)SCRWIDTH;

	this->screenCenter = pos + dir*d;

	this->p0 = screenCenter + float3(-1, -aspectRatio, 0);
	this->p1 = screenCenter + float3(1, -aspectRatio, 0);
	this->p2 = screenCenter + float3(-1, aspectRatio, 0);
}

void Camera::GenerateRays()
{    
	float u, v = 0.0f;

	for (int y = 0; y < SCRHEIGHT; y++)
	{
		for (int x = 0; x < SCRWIDTH; x++)
		{
			u = (float)x / SCRWIDTH;
			v = (float)y / SCRHEIGHT;

			float3 rayDir = p0 + (p1 - p0)*u + (p2 - p0)*v;
			rayDir.normalize();
			
			Ray* ray = new Ray(pos, rayDir);

			cameraRays[y * SCRWIDTH + x] = ray;
		}
	}
}

void Camera::UpdateRays()
{
	float u, v = 0.0f;

	for (int y = 0; y < SCRHEIGHT; y++)
	{
		for (int x = 0; x < SCRWIDTH; x++)
		{
			u = (float)x / SCRWIDTH;
			v = (float)y / SCRHEIGHT;

			float3 rayDir = p0 + (p1 - p0)*u + (p2 - p0)*v;
			rayDir.normalize();

			Ray* ray = cameraRays[y * SCRWIDTH + x];

			ray->dir = rayDir;
			ray->orig = pos;
			ray->t = INFINITY;
		}
	}
}

