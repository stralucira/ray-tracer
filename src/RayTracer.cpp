#include "template.h"
#include "RayTracer.h"

int iCPU2 = omp_get_num_procs();

RayTracer::RayTracer(Scene* scene, Surface* screen)
{
	this->scene = scene;
	this->screen = screen;
	//this->scene->camera->GenerateRays();
}

vec3 RayTracer::GetColor(int x, int y, Ray* ray, unsigned int depth)
{
	if (depth > MAXDEPTH || this->scene->primList.size() == 0)
	{
		return BACKGROUND_COLOR;
	}

	float nearest = INFINITY;
	Primitive* primHit;

	// check intersection
	for (size_t i = 0; i < this->scene->primList.size(); i++)
	{
		if (this->scene->primList[i]->intersect(ray) && nearest > ray->t)
		{
			nearest = ray->t;
			primHit = this->scene->primList[i];
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
			for (size_t i = 0; i < this->scene->lightList.size(); i++)
			{
				vec3 dir = normalize(scene->lightList[i]->pos - hitPoint);
				if (dot(dir, normal) < 0) continue;
				color += DirectIllumination(hitPoint, dir, normal, scene->lightList[i], primHit->material);
			}
			ray->t = INFINITY;
		}

		// MIRROR material shader hit
		if (primHit->material.shader == Material::Shader::MIRROR)
		{
			Ray* reflectRay = new Ray(hitPoint, Reflect(ray->dir, normal));
			color += primHit->material.color * GetColor(x, y, reflectRay, depth += 1);
			ray->t = INFINITY;
			delete reflectRay;
		}

		// GLASS material shader hit
		if (primHit->material.shader == Material::Shader::GLASS)
		{
			vec3 refractColor = BLACK;
			
			float kr = Fresnel(ray->dir, normal, 1.52f);
			bool outside = dot(ray->dir, normal) < 0;
			vec3 bias = 0.001f * primHit->getNormal(hitPoint);

			float cosi = clamp(-1.0f, 1.0f, dot(normal, ray->dir));
			float etai = 1, etat = 1.52f;
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
			
			if (k < 0)
			{
				return color;
			}
			else
			{
				Ray* refractRay = new Ray(hitPoint, eta * ray->dir + (eta * cosi - sqrtf(k)) * n);
				
				vec3 hitEpsilon = refractRay->orig + refractRay->dir * 0.01f;
				refractRay->orig = hitEpsilon;

				refractColor = this->GetColor(x, y, refractRay, depth + 1);
				delete refractRay;
			}
		
			Ray* reflectRay = new Ray(hitPoint, reflect(ray->dir, normal));
			vec3 reflectColor = primHit->material.color * 0.8f * GetColor(x, y, reflectRay, depth += 1);
			ray->t = INFINITY;
			delete reflectRay;

			color += reflectColor * kr + refractColor * (1 - kr);
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
	float tToLight = length(light->pos - hitEpsilon);

	for (size_t i = 0; i < this->scene->primList.size(); i++)
	{
		this->scene->primList[i]->intersect(&shadowRay);
		if (shadowRay.t < tToLight)
		{
			return BLACK;
		}
	}

	float euclidianDistanceToLight = distance(hitPoint, light->pos);
	return light->color * dot(normal, dir) * (1 / (euclidianDistanceToLight*euclidianDistanceToLight)) * (mat.color * INVPI);
}

vec3 RayTracer::Reflect(vec3 dir, vec3 normal)
{
	return dir - 2 * dot(dir, normal) * normal;
}

float RayTracer::Fresnel(vec3 dir, vec3 normal, float index)
{
	float cosi = clamp(dot(normal, dir), -1.0f, 1.0f);
	float etai = 1, etat = index;
	if (cosi > 0)
	{
		std::swap(etai, etat);
	}
    float sint = etai / etat * sqrtf(glm::max(0.0f, 1 - cosi * cosi));
	if (sint >= 1)
	{
		return 1;
	}
	else
	{
        float cost = sqrtf(glm::max(0.0f, 1 - sint * sint));
		cosi = fabsf(cosi);
		float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
		float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		return (Rs * Rs + Rp * Rp) / 2;
	}
}

void RayTracer::Render()
{
	omp_set_num_threads(iCPU2);
	int x = 0;
	
	#pragma omp parallel for private(x)
	for (int y = 0; y < SCRHEIGHT; y++)
	{
		for (x = 0; x < SCRWIDTH; x++)
		{
			//vec3 color = GetColor(x, y, this->scene->camera->cameraRays[y*SCRWIDTH + x], 0);
			vec3 color = GetColor(x, y, this->scene->camera->GenerateRay(x,y), 0);

			color *= 255.0f;
            int r = glm::min((int)color.x, 255);
            int g = glm::min((int)color.y, 255);
            int b = glm::min((int)color.z, 255);
			buffer[y][x] = ((r << 16) + (g << 8) + (b));
		}
	}
	
	#pragma omp parallel for private(x)
	for (int y = 0; y < SCRHEIGHT; y++)
	{
		for (x = 0; x < SCRWIDTH; x++)
		{
			this->screen->Plot(x, y, this->buffer[y][x]);
		}
	}
}
