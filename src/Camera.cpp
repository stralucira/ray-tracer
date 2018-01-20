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
	this->p3 = screenCenter + this->GetRight() + this->GetUp() * this->aspectRatio; // bottom right corner
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
void Camera::GenerateRay(Ray* ray, int x, int y)
{
	//return Ray(this->pos, normalize((this->p0 + ((float)x / (float)SCRWIDTH) * (this->p1 - this->p0) + ((float)y / (float)SCRHEIGHT) * (this->p2 - this->p0)) - this->pos));
	//return Ray(this->pos, normalizeSSE((this->p0 + ((float)x / (float)SCRWIDTH) * (this->p1 - this->p0) + ((float)y / (float)SCRHEIGHT) * (this->p2 - this->p0)) - this->pos));

	ray->t = INFINITY;
	ray->u = INFINITY;
	ray->v = INFINITY;

	ray->orig = this->pos;
	ray->dir = normalize((this->p0 + (x / (float)SCRWIDTH) * (this->p1 - this->p0) + (y / (float)SCRHEIGHT) * (this->p2 - this->p0)) - this->pos);
	ray->invDir = 1.0f / ray->dir;
}

void Camera::GenerateRays(RayPacket* rays, int x, int y)
{
	static __m128i offsetX4 = _mm_set_epi32(1, 0, 1, 0);
	static __m128i offsetY4 = _mm_set_epi32(0, 1, 0, 1);
	
	vec3 pos = GetPosition();
	vec3 horDir = (p1 - p0);
	vec3 verDir = (p2 - p0);

	for (int i = 0; i < RayPacket::packetCount; i++)
	{
		rays->t4[i] = _mm_set_ps1(1e34f);

		rays->origX4[i] = _mm_set_ps1(pos.x);
		rays->origY4[i] = _mm_set_ps1(pos.y);
		rays->origZ4[i] = _mm_set_ps1(pos.z);

		__m128i screenX4 = _mm_add_epi32(_mm_set1_epi32(x * RayPacket::width + (i % (RayPacket::width / 2)) * 2), offsetX4);
		__m128i screenY4 = _mm_add_epi32(_mm_set1_epi32(y * RayPacket::height + (i / (RayPacket::height / 2)) * 2), offsetY4);
		__m128 pX4 = _mm_cvtepi32_ps(screenX4), pY4 = _mm_cvtepi32_ps(screenY4);
		pX4 = _mm_div_ps(pX4, _mm_set_ps1(SCRWIDTH)); pY4 = _mm_div_ps(pY4, _mm_set_ps1(SCRHEIGHT));
		__m128 p0X4 = _mm_set_ps1(p0.x), p0Y4 = _mm_set_ps1(p0.y), p0Z4 = _mm_set_ps1(p0.z);
		__m128 horOffsetX4 = _mm_set_ps1(horDir.x), horOffsetY4 = _mm_set_ps1(horDir.y), horOffsetZ4 = _mm_set_ps1(horDir.z);
		__m128 verOffsetX4 = _mm_set_ps1(verDir.x), verOffsetY4 = _mm_set_ps1(verDir.y), verOffsetZ4 = _mm_set_ps1(verDir.z);
		__m128 x4 = _mm_add_ps(_mm_add_ps(p0X4, _mm_mul_ps(pX4, horOffsetX4)), _mm_mul_ps(pY4, verOffsetX4));
		__m128 y4 = _mm_add_ps(_mm_add_ps(p0Y4, _mm_mul_ps(pX4, horOffsetY4)), _mm_mul_ps(pY4, verOffsetY4));
		__m128 z4 = _mm_add_ps(_mm_add_ps(p0Z4, _mm_mul_ps(pX4, horOffsetZ4)), _mm_mul_ps(pY4, verOffsetZ4));
		rays->dirX4[i] = _mm_sub_ps(x4, rays->origX4[i]);
		rays->dirY4[i] = _mm_sub_ps(y4, rays->origY4[i]);
		rays->dirZ4[i] = _mm_sub_ps(z4, rays->origZ4[i]);
		__m128 invlen4 = _mm_div_ps(_mm_set_ps1(1.0f), _mm_sqrt_ps(_mm_dot_ps(rays->dirX4[i], rays->dirY4[i], rays->dirZ4[i], rays->dirX4[i], rays->dirY4[i], rays->dirZ4[i])));
		rays->dirX4[i] = _mm_mul_ps(rays->dirX4[i], invlen4);
		rays->dirY4[i] = _mm_mul_ps(rays->dirY4[i], invlen4);
		rays->dirZ4[i] = _mm_mul_ps(rays->dirZ4[i], invlen4);

		rays->hit4[i] = _mm_setzero_si128();

		for (int j = 0; j < 4; j++)
		{
			rays->address[i * 4 + j] = reinterpret_cast<uint*>(&screenX4)[j] + reinterpret_cast<uint*>(&screenY4)[j] * SCRWIDTH;
		}
	}

	float offsetLeft = (x * RayPacket::width) / float(SCRWIDTH);
	float offsetRight = ((x + 1) * RayPacket::width) / float(SCRWIDTH);
	float offsetUp = (y * RayPacket::width) / float(SCRHEIGHT);
	float offsetDown = ((y + 1) * RayPacket::height) / float(SCRHEIGHT);

	vec3 c0 = normalize(p0 + offsetLeft * horDir + offsetUp * verDir - pos);
}