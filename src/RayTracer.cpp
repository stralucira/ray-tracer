#include "template.h"
#include "RayTracer.h"

RayTracer::RayTracer(Scene* scene, Surface* screen)
{
	this->scene = scene;
	this->screen = screen;
	this->scene->camera->GenerateRays();
}

float3 RayTracer::getColor(int x, int y, Ray* ray)
{
	float nearest = INFINITY;
	Primitive* primHit;

	// check intersection
	int primCount = sizeof(this->scene->primitives) / sizeof(this->scene->primitives[0]);
	for (int i = 0; i < primCount; i++)
	{
		if (this->scene->primitives[i]->intersect(ray) && nearest > ray->t)
		{
			nearest = ray->t;
			primHit = this->scene->primitives[i];
		}
	}

	if (nearest == INFINITY) // if ray does not intersect
	{
		return float3(0.0f, 0.0f, 0.0f);
	}
	else // if ray intersect
	{
		float3 hitPoint = ray->orig + ray->dir * nearest;
		float3 normal = primHit->getNormal(hitPoint);
		float3 color = float3(0.0f, 0.0f, 0.0f);

		// DIFFUSE material shader hit
		if (primHit->material.shader == Material::Shader::DIFFUSE)
		{
			int lightCount = sizeof(this->scene->lights) / sizeof(this->scene->lights[0]);
			for (int i = 0; i < lightCount; i++)
			{
				float3 dir = (scene->lights[i]->pos - hitPoint);
				dir.normalize();

				if (dot(dir, normal) < 0)
				{
					continue;
				}

				color += DirectIllumination(hitPoint, dir, normal, scene->lights[i], primHit->material);
			}
			ray->t = INFINITY;
		}

		// MIRROR material shader hit
		if (primHit->material.shader == Material::Shader::MIRROR)
		{
			ray->t = INFINITY;
			return primHit->material.color * getColor(x, y, &Ray(hitPoint, reflect(ray->dir, normal)));
		}

		// GLASS material shader hit
		if (primHit->material.shader == Material::Shader::GLASS)
		{
		}

		return color;
	}

}

float3 RayTracer::DirectIllumination(float3 hitPoint, float3 dir, float3 normal, Light* light, Material mat)
{
	float3 hitEpsilon = hitPoint + dir * 0.01f;
	Ray shadowRay = Ray(hitEpsilon, dir);
	shadowRay.t = INFINITY;

	float lightInt = 0.0f;
	float tToLight = (light->pos.x - hitEpsilon.x) / dir.x;

	int primCount = sizeof(this->scene->primitives) / sizeof(this->scene->primitives[0]);
	for (int i = 0; i < primCount; i++)
	{
		this->scene->primitives[i]->intersect(&shadowRay);
		if (shadowRay.t < tToLight)
		{
			return float3(0.0f, 0.0f, 0.0f);
		}
	}

	float euclidianDistanceToLight = (hitPoint, light->pos).length();
	float3 matPI = float3(mat.color.x / PI, mat.color.y / PI, mat.color.z / PI);

	return light->color * dot(normal, dir) * (1 / (euclidianDistanceToLight * euclidianDistanceToLight)) * matPI;
}

void RayTracer::Render()
{
	// render color
	for (int y = 0; y < SCRHEIGHT; y++)
	{
		for (int x = 0; x < SCRWIDTH; x++)
		{
			float3 color = getColor(x, y, this->scene->camera->cameraRays[y * SCRWIDTH + x]);

			color *= 256.0f;
			int r = min((int)color.x, 255);
			int g = min((int)color.y, 255);
			int b = min((int)color.z, 255);
			buffer[y][x] = ((r << 16) + (g << 8) + b);
		}
	}

	for (int y = 0; y < SCRHEIGHT; y++)
	{
		for (int x = 0; x < SCRWIDTH; x++)
		{
			this->screen->Plot(x, y, this->buffer[y][x]);
		}
	}
}