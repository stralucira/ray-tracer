#include "template.h"
#include "RayTracer.h"

RayTracer::RayTracer(Scene* scene, Surface* screen)
{
	this->scene = scene;
	this->screen = screen;
	this->scene->camera->GenerateRays();
}

vec3 RayTracer::GetColor(int x, int y, Ray* ray)
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

	// ray does not intersect
	if (nearest == INFINITY)
	{
		return vec3(0.0f);
	}

	// ray intersects
	else
	{
		vec3 hitPoint = ray->orig + ray->dir * nearest;
		vec3 normal = primHit->getNormal(hitPoint);
		vec3 color = vec3(0.0f);
		
		// DIFFUSE material shader hit
		if (primHit->material.shader == Material::Shader::DIFFUSE)
		{
			int lightCount = sizeof(this->scene->lights) / sizeof(this->scene->lights[0]);
			for (int i = 0; i < lightCount; i++)
			{
				vec3 dir = normalize(scene->lights[i]->pos - hitPoint);
				if (dot(dir, normal) < 0) continue;
				color += DirectIllumination(hitPoint, dir, normal, scene->lights[i], primHit->material);
			}
			ray->t = INFINITY;
		}

		// MIRROR material shader hit
		if (primHit->material.shader == Material::Shader::MIRROR)
		{
			ray->t = INFINITY;
			return primHit->material.color * GetColor(x, y, &Ray(hitPoint, reflect(ray->dir, normal)));
		}

		// GLASS material shader hit
		if (primHit->material.shader == Material::Shader::GLASS)
		{
		}

		return color;
	}
}

vec3 RayTracer::DirectIllumination(vec3 hitPoint, vec3 dir, vec3 normal, Light* light, Material mat)
{
	vec3 hitEpsilon = hitPoint + dir * 0.01f;
	Ray shadowRay = Ray(hitPoint, dir);
	shadowRay.t = INFINITY;
	
	float lightInt = 0.0f;
	float tToLight = (light->pos.x - hitEpsilon.x) / dir.x;

	int primCount = sizeof(this->scene->primitives) / sizeof(this->scene->primitives[0]);
	for (int i = 0; i < primCount; i++)
	{
		this->scene->primitives[i]->intersect(&shadowRay);
		if (shadowRay.t < tToLight)
		{
			return vec3(0.0f);
		}
	}

	float euclidianDistanceToLight = distance(hitPoint, light->pos);
	return light->color * dot(normal, dir) * (1 / (euclidianDistanceToLight*euclidianDistanceToLight)) * (mat.color / PI);
}

void RayTracer::Render()
{
#pragma omp parallel for
	for (int y = 0; y < SCRHEIGHT; y++)
	{
#pragma omp parallel for
		for (int x = 0; x < SCRWIDTH; x++)
		{
			vec3 color = GetColor(x, y, this->scene->camera->cameraRays[y*SCRWIDTH + x]);

			color *= 256.0f;
			int r = min((int)color.x, 255);
			int g = min((int)color.y, 255);
			int b = min((int)color.z, 255);
			buffer[y][x] = ((r << 16) + (g << 8) + (b));
		}
	}
#pragma omp parallel for
	for (int y = 0; y < SCRHEIGHT; y++)
	{
#pragma omp parallel for
		for (int x = 0; x < SCRWIDTH; x++)
		{
			this->screen->Plot(x, y, this->buffer[y][x]);
		}
	}
}