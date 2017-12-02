#include "template.h"
#include "Camera.h"
#include "Ray.h"
#include <random>

Camera::Camera()
{
	this->Init();
}

// Initialize camera and the transformation matrix
void Camera::Init()
{
	this->pos = vec3(0.0f);
	this->dir = vec3(0.0f, 0.0f, 1.0f);
	this->d = 1.0f;
	this->aspectRatio = (float)SCRHEIGHT / (float)SCRWIDTH;

	this->screenCenter = pos + dir * d;
	
	this->p0 = screenCenter + vec3(-1.0f, -aspectRatio, 0.0f);
	this->p1 = screenCenter + vec3(1.0f, -aspectRatio, 0.0f);
	this->p2 = screenCenter + vec3(-1.0f, aspectRatio, 0.0f);

	this->wUp = vec3(0.0f, 1.0f, 0.0f);
	this->transMatrix = inverse(lookAt(pos, screenCenter, wUp));
	this->rRight = vec3(this->transMatrix[0]);
	this->rUp = vec3(this->transMatrix[1]);
	this->dir = vec3(this->transMatrix[2]);
}

void Camera::UpdatePosition()
{
	this->pos = vec3(this->transMatrix[3]);
	this->dir = vec3(this->transMatrix[2]);
	this->rUp = vec3(this->transMatrix[1]);
	this->rRight = vec3(this->transMatrix[0]);

	this->screenCenter = pos + dir * d;

	this->p0 = (transMatrix * vec4(p0, 1));
	this->p1 = (transMatrix * vec4(p1, 1));
	this->p2 = (transMatrix * vec4(p2, 1));
}

// Generate rays for every screen pixel and store them inside cameraRays
void Camera::GenerateRays()
{
	float u, v = 0.0f;

	#pragma omp parallel for
	for (int y = 0; y < SCRHEIGHT; y++)
	{
		#pragma omp parallel for
		for (int x = 0; x < SCRWIDTH; x++)
		{
			u = (float) x / SCRWIDTH;
			v = (float) y / SCRHEIGHT;

			vec3 point = p0 + (p1 - p0) * u + (p2 - p0) * v;
			vec3 rayDir = normalize(point - pos);
			Ray* ray = new Ray(pos, rayDir);

			cameraRays[y * SCRWIDTH + x] = ray;
		}
	}
}

void Camera::UpdateRays()
{
	float u, v = 0.0f;

	#pragma omp parallel for
	for (int y = 0; y < SCRHEIGHT; y++)
	{
		#pragma omp parallel for
		for (int x = 0; x < SCRWIDTH; x++)
		{
			u = (float)x / SCRWIDTH;
			v = (float)y / SCRHEIGHT;

			vec3 point = p0 + (p1 - p0) * u + (p2 - p0) * v;
			vec3 rayDir = normalize(point - pos);
			Ray* ray = cameraRays[y * SCRWIDTH + x];

			ray->dir = rayDir;
			ray->orig = pos;
			ray->t = INFINITY;
		}
	}
}

void Camera::TransformCamera(glm::mat4 transMatrix)
{
	this->transMatrix = transMatrix;
	this->UpdatePosition();
	this->UpdateRays();
}

