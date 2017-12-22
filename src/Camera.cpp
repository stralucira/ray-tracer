#include "template.h"
#include "Camera.h"
#include "Ray.h"
#include <random>

#define CAMERADEBUG 0

int iCPU = omp_get_num_procs();

Camera::Camera()
{
	this->Init();
}

// Initialize camera and the transformation matrix
void Camera::Init()
{
	this->pos = vec3(0.0f, 0.0f, -1.0f);
	this->viewDir = vec3(0.0f, 0.0f, 1.0f);
	this->dir = glm::normalize(viewDir - pos);
	this->d = 1.0f;
	this->up = vec3(0.0f, 1.0f, 0.0f);
	this->aspectRatio = (float)SCRHEIGHT / (float)SCRWIDTH;

	CalculateScreen();
}

void Camera::CalculateScreen()
{

	this->right = glm::cross(this->up, this->dir);
	this->up = glm::cross(this->dir, this->right);

	vec3 screenCenter = this->pos + this->d * this->dir;

	this->p0 = screenCenter - this->right + this->up * this->aspectRatio; // top left corner
	this->p1 = screenCenter + this->right + this->up * this->aspectRatio; // top right corner
	this->p2 = screenCenter - this->right - this->up * this->aspectRatio; // bottom left corner
}

void Camera::PrintPosition()
{
	printf("-----------------------\n Camera Position \n-----------------------\n");
	printf("dir: %.2f %.2f %.2f\n", dir.x, dir.y, dir.z);
	printf("pos: %.2f %.2f %.2f\n", pos.x, pos.y, pos.z);
	printf("\n");
	printf("right: %.2f %.2f %.2f \n", right.x, right.y, right.z);
	printf("up: %.2f %.2f %.2f \n", up.x, up.y, up.z);
	printf("\n");
	printf("p0: %.2f %.2f %.2f \n", p0.x, p0.y, p0.z);
	printf("p1: %.2f %.2f %.2f \n", p1.x, p1.y, p1.z);
	printf("p2: %.2f %.2f %.2f \n", p2.x, p2.y, p2.z);
	printf("-----------------------\n");
}

// Camera Movement
// Translations:
void Camera::Surge(float speed)
{
	pos += dir * speed;
	CalculateScreen();
}
void Camera::Sway(float speed)
{
	pos += right * speed;
	CalculateScreen();
}
void Camera::Heave(float speed)
{
	pos += up * speed;
	CalculateScreen();
}
// Rotations:
void Camera::Roll(float angle)
{
	float x = dir.x;
	float y = dir.y;

	float rad = angle * PIOVER180;
	float c = glm::cos(rad);
	float s = glm::sin(rad);

	dir.x = x * c - y * s;
	dir.y = y * c + x * s;

	CalculateScreen();
}
void Camera::Pitch(float angle)
{
	float y = dir.y;
	float z = dir.z;

	float rad = angle * PIOVER180;
	float c = glm::cos(rad);
	float s = glm::sin(rad);

	dir.y = y * c - z * s;
	dir.z = z * c + y * s;

	CalculateScreen();
}
void Camera::Yaw(float angle)
{
	float x = dir.x;
	float z = dir.z;

	float rad = angle * PIOVER180;
	float c = glm::cos(rad);
	float s = glm::sin(rad);

	dir.x = x * c - z * s;
	dir.z = z * c + x * s;

	CalculateScreen();
}
// Zooming:
void Camera::Zoom(float distance)
{
	d += distance;
	CalculateScreen();
}

// Generate ray
Ray Camera::GenerateRay(int x, int y)
{
	return Ray(this->pos, normalize((this->p0 + ((float)x / (float)SCRWIDTH) * (this->p1 - this->p0) + ((float)y / (float)SCRHEIGHT) * (this->p2 - this->p0)) - this->pos));
}