#include "template.h"
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
	this->focalLength = 5.0f;
	this->apertureSize = 0.f;
	this->aspectRatio = (float)SCRHEIGHT / (float)SCRWIDTH;

	CalculateScreen();
}

void Camera::CalculateScreen()
{
	this->SetPosition(pos);

	this->p0 = *(vec3*)&(transform * vec4(focalLength * vec3(-1.f,  1.f * aspectRatio, -this->d), 1.0f)); // top left corner
	this->p1 = *(vec3*)&(transform * vec4(focalLength * vec3( 1.f,  1.f * aspectRatio, -this->d), 1.0f)); // top right corner
	this->p2 = *(vec3*)&(transform * vec4(focalLength * vec3(-1.f, -1.f * aspectRatio, -this->d), 1.0f)); // bottom left corner
	this->p3 = *(vec3*)&(transform * vec4(focalLength * vec3( 1.f, -1.f * aspectRatio, -this->d), 1.0f)); // bottom right corner
}

void Camera::PrintPosition()
{
	printf("----------------------------------------------\n Camera Position \n----------------------------------------------\n");
	printf("Position:  %.2ff, %.2ff, %.2ff\n", GetPosition().x, GetPosition().y, GetPosition().z);
	printf("Direction: %.2ff, %.2ff, %.2ff\n", GetForward().x, GetForward().y, GetForward().z);
	printf("\n");
	printf("Focus distance: %.5ff\nAperture Size: %.5ff\n", focalLength, apertureSize);
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
	if (d + increment > EPSILON)
	{
		d += increment;
		CalculateScreen();
	}
}
// Focusing:
void Camera::Focus(float increment)
{
	if (focalLength + increment > EPSILON)
	{
		focalLength += increment;
		CalculateScreen();
	}
}
// Aperture size: DAT SWEET SWEET BOKEH
void Camera::Aperture(float increment)
{
	if (apertureSize + increment > EPSILON)
	{
		apertureSize += increment;
		CalculateScreen();
	}
}

// Generate ray
void Camera::GenerateRay(Ray* ray, int x, int y)
{
	ray->t = INFINITY;
	ray->u = INFINITY;
	ray->v = INFINITY;

	ray->orig = this->pos;
	ray->dir = normalize((this->p0 + ((float)x / SCRWIDTH) * (this->p1 - this->p0) + ((float)y / SCRHEIGHT) * (this->p2 - this->p0)) - this->pos);
	ray->invDir = 1.0f / ray->dir;
}

void Camera::GenerateRayDOF(Ray* ray, int x, int y)
{
	vec3 DoF = vec3(((float)rand() / RAND_MAX) * apertureSize - apertureSize * 0.5f, ((float)rand() / RAND_MAX) * apertureSize - apertureSize * 0.5f, 0.0f);
	
	float fx = ((float)x + (rand() / RAND_MAX)) / SCRWIDTH;
	float fy = ((float)y + (rand() / RAND_MAX)) / SCRHEIGHT;

	ray->t = INFINITY;
	ray->u = INFINITY;
	ray->v = INFINITY;

	ray->orig = this->pos + DoF;
	ray->dir = normalize(this->p0 + fx * (this->p1 - this->p0) + fy * (this->p2 - this->p0) - (this->pos + DoF));
	ray->invDir = 1.0f / ray->dir;
}