#include "template.h"
#include "RayTracer.h"

#define ENABLEBVH 1
#define ALTERNATERENDERMODE 0

int iCPU2 = omp_get_num_procs();

RayTracer::RayTracer(Scene* scene, Surface* screen)
{
	this->scene = scene;
	this->screen = screen;
}

vec3 RayTracer::GetColor(int x, int y, Ray* ray, int depth)
{
	if (depth > MAXDEPTH || this->scene->primList.size() == 0)
	{
		return BACKGROUND_COLOR;
	}

	float nearest = INFINITY;

	// BVH depth rendering
	if (depthRendering)
	{
		int depthRender = 0;
		scene->bvh->Traverse(ray, scene->bvh->root, 0, &depthRender);
		return vec3(clamp((depthRender * 0.001f), 0.0f, 1.0f), clamp((0.8f - depthRender * 0.001f), 0.0f, 1.0f), 0.0f);
	}

	// Trace function
#if ENABLEBVH
	scene->bvh->Traverse(ray, scene->bvh->root);
	nearest = ray->t;
#else
	for (size_t i = 0; i < this->scene->primList.size(); i++)
	{
		if (this->scene->primList[i]->intersect(ray) && nearest > ray->t)
		{
			nearest = ray->t;
			ray->hit = this->scene->primList[i];
		}
	}
#endif // ENABLEBVH

	// If ray does not intersect
	if (nearest == INFINITY)
	{
		return BACKGROUND_COLOR;
	}

#if ALTERNATERENDERMODE // Diffuse + Shininess + Dissolve
	else
	{
		if (renderShadow)
		{
			vec3 hitPoint = ray->orig + ray->dir * nearest;
			vec3 normal = ray->hit->getNormal(hitPoint);
			vec3 color = BLACK;

			vec3 shadowPointOrig = (dot(ray->dir, normal) < 0) ?
				hitPoint + normal * 0.0001f :
				hitPoint - normal * 0.0001f;

			if (ray->hit->material.shader == Material::Shader::DIFFUSE)
			{
				for (size_t i = 0; i < this->scene->lightList.size(); i++)
				{
					vec3 diffuseColor = BLACK;
					vec3 specularColor = BLACK;

					vec3 lightDir = this->scene->lightList[i]->pos - hitPoint;
					//float lightDistance2 = dot(lightDir, lightDir);
					lightDir = normalize(lightDir);

					if (dot(lightDir, normal) < 0) { continue; }

					diffuseColor += DirectIllumination(hitPoint, lightDir, normal, scene->lightList[i], ray->hit->material);

					vec3 reflectionDirection = reflect(-lightDir, normal);
					specularColor += powf(glm::max(0.0f, -dot(reflectionDirection, ray->dir)), ray->hit->material.Ns);// * this->scene->lightList[i]->color;

					color += diffuseColor + specularColor * ray->hit->material.Ks;
				}
				//ray->t = INFINITY;
			}

			if (ray->hit->material.shader == Material::Shader::MIRROR)
			{
				Ray reflectRay = Ray(hitPoint, Reflect(ray->dir, normal));
				color += ray->hit->material.Kd * GetColor(x, y, &reflectRay, depth += 1);
				//ray->t = INFINITY;
			}
			
			if (ray->hit->material.shader == Material::Shader::GLASS)
			{
				vec3 reflectionDirection = normalize(reflect(ray->dir, normal));
				vec3 refractionDirection = normalize(refract(ray->dir, normal, 1.0f));
				vec3 reflectionRayOrig = (dot(reflectionDirection, normal) < 0) ?
					hitPoint - normal * 0.0001f :
					hitPoint + normal * 0.0001f;
				vec3 refractionRayOrig = (dot(refractionDirection, normal) < 0) ?
					hitPoint - normal * 0.0001f :
					hitPoint + normal * 0.0001f;
				
				Ray reflectRay = Ray(reflectionRayOrig, reflectionDirection);
				vec3 reflectionColor = GetColor(x, y, &reflectRay, depth += 1);
				
				Ray refractRay = Ray(refractionRayOrig, refractionDirection);
				vec3 refractionColor = GetColor(x, y, &refractRay, depth += 1);

				color += reflectionColor * (ray->hit->material.dissolve) + refractionColor * (1 - ray->hit->material.dissolve);
			}
			return color;

		}
		else
		{
			return ray->hit->material.Kd;
		}
	}
#else
	// If ray intersects
	else
	{
		vec3 hitPoint = ray->orig + ray->dir * nearest;
		vec3 normal = ray->hit->getNormal(hitPoint);
		vec3 color = BLACK;
		
		// DIFFUSE material shader hit
		if (ray->hit->material.shader == Material::Shader::DIFFUSE)
		{
			for (size_t i = 0; i < this->scene->lightList.size(); i++)
			{
				vec3 dir = normalize(scene->lightList[i]->pos - hitPoint);
				if (dot(dir, normal) < 0) continue;
				color += DirectIllumination(hitPoint, dir, normal, scene->lightList[i], ray->hit->material);
			}
			ray->t = INFINITY;
		}

		// MIRROR material shader hit
		if (ray->hit->material.shader == Material::Shader::MIRROR)
		{
			Ray reflectRay = Ray(hitPoint, Reflect(ray->dir, normal));
			color += ray->hit->material.Kd * GetColor(x, y, &reflectRay, depth += 1);
			ray->t = INFINITY;
		}

		// GLASS material shader hit
		if (ray->hit->material.shader == Material::Shader::GLASS)
		{
			vec3 refractColor = BLACK;
			
			float kr = Fresnel(ray->dir, normal, 1.52f);
			bool outside = dot(ray->dir, normal) < 0;
			vec3 bias = 0.0001f * ray->hit->getNormal(hitPoint);

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
				Ray refractRay = Ray(hitPoint, eta * ray->dir + (eta * cosi - sqrtf(k)) * n);
				
				vec3 hitEpsilon = refractRay.orig + refractRay.dir * 0.01f;
				refractRay.orig = hitEpsilon;

				refractColor = this->GetColor(x, y, &refractRay, depth += 1);
			}
		
			Ray reflectRay = Ray(hitPoint, reflect(ray->dir, normal));
			vec3 reflectColor = ray->hit->material.Kd * GetColor(x, y, &reflectRay, depth += 1);
			ray->t = INFINITY;

			color += reflectColor * kr + refractColor * (1 - kr);
		}
		return color;
	}
#endif
}

vec3 RayTracer::DirectIllumination(vec3 hitPoint, vec3 dir, vec3 normal, Light* light, Material mat)
{
	vec3 hitEpsilon = hitPoint + dir * 0.0001f;
	Ray shadowRay = Ray(hitPoint, dir);
	shadowRay.t = INFINITY;

	float lightInt = 0.0f;
	float tToLight = length(light->pos - hitEpsilon);

	if (renderShadow)
	{
#if ENABLEBVH
	//scene->topbvh->TraverseTop(&shadowRay, scene->topbvh->topRoot, true);
	//if (shadowRay.t < tToLight) { return BLACK; }

	scene->bvh->Traverse(&shadowRay, scene->bvh->root, true);
	if (shadowRay.t < tToLight) { return BLACK; }
#else
		for (size_t i = 0; i < this->scene->primList.size(); i++)
		{
			this->scene->primList[i]->intersect(&shadowRay);
			if (shadowRay.t < tToLight) { return BLACK; }
		}
#endif
	}

	float euclidianDistanceToLight = distance(hitPoint, light->pos);
	return light->color * dot(normal, dir) * (1 / (euclidianDistanceToLight*euclidianDistanceToLight)) * (mat.Kd * INVPI);
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
			Ray ray = Ray(this->scene->camera->GenerateRay(x, y));
			vec3 color = GetColor(x, y, &ray, 0);

			color *= 255.0f;
            int r = glm::min((int)color.x, 255);
            int g = glm::min((int)color.y, 255);
            int b = glm::min((int)color.z, 255);
			
			buffer[y][x] = ((r << 16) + (g << 8) + (b));
			this->screen->Plot(x, y, ((r << 16) + (g << 8) + (b)));
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
