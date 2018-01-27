#include "template.h"
#include "Camera.h"
#include "Ray.h"
#include <random>

int iCPU = omp_get_num_procs();
glm::uint seed3 = 2343 * 68365;
glm::uint seed4 = 1853 * 28563;

Camera::Camera()
{
	this->Init(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f), 5.0f, 0.0f);
}

Camera::Camera(vec3 pos, vec3 lookAt, float focalLength, float apertureSize)
{
	this->Init(pos, lookAt, focalLength, apertureSize);
}

// Initialize camera and the transformation matrix
void Camera::Init(vec3 pos, vec3 lookAt, float focalLength, float apertureSize)
{
	this->pos = pos;
	this->SetPosition(pos);
	this->LookAt(lookAt);

	this->magnification = 1.0f;
	this->focalLength = focalLength;
	this->apertureSize = apertureSize;
	this->aspectRatio = (float)SCRHEIGHT / (float)SCRWIDTH;

	CalculateScreen();
}

void Camera::CalculateScreen()
{
	this->SetPosition(pos);

	this->p0 = vec3(transform * vec4(focalLength * vec3(-1.f,  1.f * aspectRatio, -this->magnification), 1.0f)); // top left corner
	this->p1 = vec3(transform * vec4(focalLength * vec3( 1.f,  1.f * aspectRatio, -this->magnification), 1.0f)); // top right corner
	this->p2 = vec3(transform * vec4(focalLength * vec3(-1.f, -1.f * aspectRatio, -this->magnification), 1.0f)); // bottom left corner
	this->p3 = vec3(transform * vec4(focalLength * vec3( 1.f, -1.f * aspectRatio, -this->magnification), 1.0f)); // bottom right corner
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
	if (magnification + increment > EPSILON)
	{
		magnification += increment;
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
	ray->dir = normalize((this->p0 + ((float)x / SCRWIDTH) * (this->p1 - this->p0) + ((float)y / SCRHEIGHT) * (this->p2 - this->p0)) - ray->orig);
	ray->invDir = 1.0f / ray->dir;
}

void Camera::GenerateRayDOF(Ray* ray, int x, int y)
{
	/// fast rectangular plane depth of field
	//vec3 DoF = transform * vec4(RandomFloat(&seed3) * apertureSize - apertureSize * 0.05f, RandomFloat(&seed4) * apertureSize - apertureSize * 0.05f, 0.0f, 0.f);

	/// fast circular plane depth of field
	float dir = RandomFloat(&seed3) * PI * 2.f;
	float len = RandomFloat(&seed4) * apertureSize;
	float dof_x = len * (float)sine(double(dir + PI * 0.5f));
	float dof_y = len * (float)sine(double(dir));
	vec3 DoF = transform * vec4(dof_x, dof_y, 0.f, 0.f);

	ray->t = INFINITY;
	ray->u = INFINITY;
	ray->v = INFINITY;

	ray->orig = this->pos + DoF;
	ray->dir = normalize((this->p0 + ((float)x / SCRWIDTH) * (this->p1 - this->p0) + ((float)y / SCRHEIGHT) * (this->p2 - this->p0)) - ray->orig);
	ray->invDir = 1.0f / ray->dir;
}

glm::uint Camera::RandomInt(glm::uint* seed)
{
	// Marsaglia Xor32; see http://excamera.com/sphinx/article-xorshift.html
	// also see https://github.com/WebDrake/xorshift/blob/master/xorshift.c for higher quality variants
	*seed ^= *seed << 13;
	*seed ^= *seed >> 17;
	*seed ^= *seed << 5;
	return *seed;
}

float Camera::RandomFloat(glm::uint* seed)
{
	return RandomInt(seed) * 2.3283064365387e-10f;
}