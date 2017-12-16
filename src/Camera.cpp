#include "template.h"
#include "Camera.h"
#include "Ray.h"
#include <random>

int iCPU = omp_get_num_procs();

Camera::Camera()
{
	this->Init();
}

// Initialize camera and the transformation matrix
void Camera::Init()
{
	this->pos = vec3(-0.5f, 0.0f, -3.0f);
	this->dir = vec3(0.0f, 0.0f, 1.0f);
	this->d = 1.0f;
	this->up = vec3(0.0f, 1.0f, 0.0f);
	this->right = vec3(1.0f, 0.0f, 0.0f);
	this->aspectRatio = (float)SCRHEIGHT / (float)SCRWIDTH;

	CalculateScreen();
}

void Camera::CalculateScreen()
{
	this->viewDir = glm::normalize(this->dir - this->pos);
	this->right = glm::cross(this->up, this->viewDir);
	this->up = glm::cross(this->viewDir, this->right);

	vec3 screenCenter = this->pos + d * this->viewDir;

	this->p0 = screenCenter - this->right + this->up * aspectRatio; // top left corner
	this->p1 = screenCenter + this->right + this->up * aspectRatio; // top right corner
	this->p2 = screenCenter - this->right - this->up * aspectRatio; // bottom left corner
}

// Generate rays for every screen pixel and store them inside cameraRays (obsolete)
/*void Camera::GenerateRays()
{
	omp_set_num_threads(iCPU);
	float u, v = 0.0f;
	int x = 0;

	#pragma omp parallel for private(x)
	for (int y = 0; y < SCRHEIGHT; y++)
	{
		for (x = 0; x < SCRWIDTH; x++)
		{
			u = (float)x / SCRWIDTH;
			v = (float)y / SCRHEIGHT;

			vec3 rayDir = normalize((this->p0 + ((float)x / (float)SCRWIDTH) * (this->p1 - this->p0) + ((float)y / (float)SCRHEIGHT) * (this->p2 - this->p0)) - this->pos);
			Ray* ray = new Ray(pos, rayDir);

			cameraRays[y * SCRWIDTH + x] = ray;
		}
	}
}

void Camera::UpdateRays()
{
	omp_set_num_threads(iCPU);
	float u, v = 0.0f;
	int x = 0;

	#pragma omp parallel for private(x)
	for (int y = 0; y < SCRHEIGHT; y++)
	{
		for (x = 0; x < SCRWIDTH; x++)
		{
			u = (float)x / SCRWIDTH;
			v = (float)y / SCRHEIGHT;

			vec3 rayDir = normalize((this->p0 + ((float)x / (float)SCRWIDTH) * (this->p1 - this->p0) + ((float)y / (float)SCRHEIGHT) * (this->p2 - this->p0)) - this->pos);
			Ray* ray = cameraRays[y * SCRWIDTH + x];

			ray->dir = rayDir;
			ray->orig = this->pos;
			ray->t = INFINITY;
		}
	}
}*/

// Generate ray
Ray Camera::GenerateRay(int x, int y)
{
	//vec3 rayDir = normalize((this->p0 + ((float)x / (float)SCRWIDTH) * (this->p1 - this->p0) + ((float)y / (float)SCRHEIGHT) * (this->p2 - this->p0)) - this->pos);
	//Ray* ray = new Ray(this->pos, rayDir);
	//ray->t = INFINITY;

	//return ray;
	return Ray(this->pos, normalize((this->p0 + ((float)x / (float)SCRWIDTH) * (this->p1 - this->p0) + ((float)y / (float)SCRHEIGHT) * (this->p2 - this->p0)) - this->pos));
}