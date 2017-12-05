#include "template.h"
#include "RayTracer.h"

RayTracer::RayTracer(Scene* scene, Surface* screen)
{
	this->scene = scene;
	this->screen = screen;
	this->scene->camera->GenerateRays();
}

vec3 RayTracer::GetColor(int x, int y, Ray* ray, unsigned int depth)
{
	if (depth > MAXDEPTH)
	{
		return BACKGROUND_COLOR;
	}

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
		return BACKGROUND_COLOR;
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
			Ray* reflectRay = new Ray(hitPoint, Reflect(ray->dir, normal));
			color += primHit->material.color * 0.8f * GetColor(x, y, reflectRay, depth += 1);
			ray->t = INFINITY;
			delete reflectRay;
		}

		// GLASS material shader hit
		if (primHit->material.shader == Material::Shader::GLASS)
		{
			vec3 refractColor = vec3(0.0f);
			// compute fresnel
			float kr = Fresnel(ray->dir, normal, 1.33f);
			bool outside = dot(ray->dir, normal) < 0;
			vec3 bias = 0.001f * primHit->getNormal(hitPoint);
			// compute refraction if it is not a case of total internal reflection
			if (kr < 1)
			{
				vec3 refractDir = normalize(refract(ray->dir, normal, 1.33f));
				vec3 refractRayOrig = outside ? hitPoint - bias : hitPoint + bias;
				
				Ray* refractRay = new Ray(hitPoint, refract(refractRayOrig, refractDir, 1.33f));
				refractColor = GetColor(x, y, refractRay, depth += 1);
				delete refractRay;

				//refractColor = GetColor(refractionRayOrig, refractionDirection, objects, lights, options, depth + 1);
			}

			vec3 reflectDir = normalize(reflect(ray->dir, normal));
			vec3 reflectRayOrig = outside ? hitPoint + bias : hitPoint - bias;
			
			Ray* reflectRay = new Ray(hitPoint, reflect(ray->dir, normal));
			vec3 reflectColor = GetColor(x, y, reflectRay, depth += 1);
			delete reflectRay;

			//vec3 reflectColor = GetColor(reflectionRayOrig, reflectionDirection, objects, lights, options, depth + 1);

			// mix the two
			//color += reflectColor * kr + refractColor * (1 - kr);
			color += refractColor * (1 - kr);
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

vec3 RayTracer::Reflect(vec3 dir, vec3 normal)
{
	return dir - 2 * dot(dir, normal) * normal;
}

vec3 RayTracer::Refract(vec3 dir, vec3 normal, float index)
{
	float cosi = clamp(dot(dir,normal),-1.0f,1.0f);
	float etai = 1, etat = index;
	vec3 n = normal;

	if (cosi < 0)
	{
		cosi = -cosi;
	}
	else
	{
		std::swap(etai, etat);
		n = -normal;
	}

	float eta = etai / etat;
	float k = 1 - eta * eta * (1 - cosi * cosi);
	//return k < 0.0f ? 0.0f : eta * dir + (eta * cosi - sqrtf(k)) * n;
	return vec3(1.0f);
}

float RayTracer::Fresnel(vec3 dir, vec3 normal, float index)
{
	float cosi = clamp(dot(normal, dir), -1.0f, 1.0f);
	float etai = 1, etat = index;
	if (cosi > 0)
	{
		std::swap(etai, etat);
	}
	float sint = etai / etat * sqrtf(max(0.0f, 1 - cosi * cosi));
	if (sint >= 1)
	{
		return 1;
	}
	else
	{
		float cost = sqrtf(max(0.0f, 1 - sint * sint));
		cosi = fabsf(cosi);
		float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
		float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		return (Rs * Rs + Rp * Rp) / 2;
	}
}

void RayTracer::Render()
{
#pragma omp parallel for
	for (int y = 0; y < SCRHEIGHT; y++)
	{
#pragma omp parallel for
		for (int x = 0; x < SCRWIDTH; x++)
		{
			vec3 color = GetColor(x, y, this->scene->camera->cameraRays[y*SCRWIDTH + x], 0);

			color *= 255.0f;
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