#include "template.h"
#include "Camera.h"
#include "Ray.h"
#include <random>

#define CAMERADEBUG 1

int iCPU = omp_get_num_procs();

Camera::Camera()
{
	this->Init();
}

// Initialize camera and the transformation matrix
void Camera::Init()
{
	this->pos = vec3(0.0f, 0.0f, -1.0f);
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

	vec3 screenCenter = this->pos + this->d * this->viewDir;

	this->p0 = screenCenter - this->right + this->up * this->aspectRatio; // top left corner
	this->p1 = screenCenter + this->right + this->up * this->aspectRatio; // top right corner
	this->p2 = screenCenter - this->right - this->up * this->aspectRatio; // bottom left corner

#if CAMERADEBUG
	printf("------------------------------\n");
	printf("dir: %.2f %.2f %.2f\n", dir.x, dir.y, dir.z);
	printf("pos: %.2f %.2f %.2f\n", pos.x, pos.y, pos.z);
	printf("\n");
	printf("viewDir: %.2f %.2f %.2f \n", viewDir.x, viewDir.y, viewDir.z);
	printf("right: %.2f %.2f %.2f \n", right.x, right.y, right.z);
	printf("up: %.2f %.2f %.2f \n", up.x, up.y, up.z);
	printf("\n");
	printf("screenCenter: %.2f %.2f %2f \n", screenCenter.x, screenCenter.y, screenCenter.z);
	printf("\n");
	printf("p0: %.2f %.2f %.2f \n", p0.x, p0.y, p0.z);
	printf("p1: %.2f %.2f %.2f \n", p1.x, p1.y, p1.z);
	printf("p2: %.2f %.2f %.2f \n", p2.x, p2.y, p2.z);
	printf("\n");
#endif
}

// Generate ray
Ray Camera::GenerateRay(int x, int y)
{
	return Ray(this->pos, normalize((this->p0 + ((float)x / (float)SCRWIDTH) * (this->p1 - this->p0) + ((float)y / (float)SCRHEIGHT) * (this->p2 - this->p0)) - this->pos));
}