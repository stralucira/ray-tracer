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
	this->pos = vec3(0.5f, 0.0f, -3.0f);
	this->dir = vec3(0.0f, 0.0f, 1.0f);
	this->d = 1.0f;
	this->up = vec3(0.0f, -1.0f, 0.0f);
	this->right = vec3(1.0f, 0.0f, 0.0f);
	this->aspectRatio = (float)SCRHEIGHT / (float)SCRWIDTH;

	this->screenCenter = pos + dir * d;

	CalculateScreen();
}

void Camera::CalculateScreen()
{
	this->viewDirNorm = glm::normalize(this->dir - this->pos);
	this->right = glm::cross(this->up, this->viewDirNorm);
	this->up = glm::cross(this->viewDirNorm, this->right);

	vec3 center = this->pos + d * this->viewDirNorm;

	this->topLeft = center - this->right + this->up * aspectRatio;
	this->topRight = center + this->right + this->up * aspectRatio;
	this->bottomLeft = center - this->right - this->up * aspectRatio;
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
			u = (float)x / SCRWIDTH;
			v = (float)y / SCRHEIGHT;

			vec3 rayDir = normalize((this->topLeft + ((float)x / (float)SCRWIDTH) * (this->topRight - this->topLeft) + ((float)y / (float)SCRHEIGHT) * (this->bottomLeft - this->topLeft)) - this->pos);

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

			vec3 rayDir = normalize((this->topLeft + ((float)x / (float)SCRWIDTH) * (this->topRight - this->topLeft) + ((float)y / (float)SCRHEIGHT) * (this->bottomLeft - this->topLeft)) - this->pos);

			Ray* ray = cameraRays[y * SCRWIDTH + x];

			ray->dir = rayDir;
			ray->orig = this->pos;
			ray->t = INFINITY;
		}
	}
}