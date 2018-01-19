#include "template.h"
#include "RayTracer.h"

int iCPU2 = omp_get_num_procs();

RayTracer::RayTracer(Scene* scene, Surface* screen)
{
	this->scene = scene;
	this->screen = screen;	
}

int RayTracer::Render(int samples)
{
	omp_set_num_threads(iCPU2);
	int x = 0;

	int pixelCount = 0;
	float invSample = 1 / (float)samples;

#pragma omp parallel for private(x)
	for (int y = 0; y < SCRHEIGHT; y++)
	{
		for (x = 0; x < SCRWIDTH; x++)
		{
			Ray ray = Ray(this->scene->camera->GenerateRay(x, y));
			//vec3 color = SampleWhitted(x, y, &ray, 0);	// Whitted-style ray tracing
			vec3 color = Sample(&ray, 0);					// Pathtracing

			accumulator[y][x].r = (accumulator[y][x].r * (1 - invSample) + color.r * invSample * 255.0f);
			accumulator[y][x].g = (accumulator[y][x].g * (1 - invSample) + color.y * invSample * 255.0f);
			accumulator[y][x].b = (accumulator[y][x].b * (1 - invSample) + color.z * invSample * 255.0f);

			int r = glm::min((int)accumulator[y][x].r, 255);
			int g = glm::min((int)accumulator[y][x].g, 255);
			int b = glm::min((int)accumulator[y][x].b, 255);

			pixelCount += r + g + b;

			buffer[y][x] = ((r << 16) + (g << 8) + b);
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

	return pixelCount;
}

// -----------------------------------------------------------
// Whitted-style ray tracing stuff
// -----------------------------------------------------------

vec3 RayTracer::SampleWhitted(int x, int y, Ray* ray, int depth)
{
	if (depth > MAXDEPTH || this->scene->primList.size() == 0) return BACKGROUND_COLOR;
	float nearest = INFINITY;

	// BVH depth rendering
	if (depthRendering)
	{
		int depthRender = 0;
#if ENABLETOPBVH
		scene->bvhTop->TraverseTop(ray, scene->bvhTop->topRoot, 0, &depthRender);
#else
		scene->bvh->Traverse(ray, scene->bvh->root, 0, &depthRender);
#endif
		return vec3(clamp((depthRender * 0.005f), 0.0f, 1.0f), clamp((0.8f - depthRender * 0.005f), 0.0f, 1.0f), 0.0f);
	}

	// Trace function
#if ENABLEBVH
#if ENABLETOPBVH
	scene->bvhTop->TraverseTop(ray, scene->bvhTop->topRoot);
#else
	scene->bvh->Traverse(ray, scene->bvh->root);
#endif // ENABLETOPBVH
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

	if (nearest == INFINITY) return BACKGROUND_COLOR; 	// If ray does not intersect
	if (!renderShadow) return GetColor(ray);			// Toggle shadows on / off

#if ALTERNATERENDERMODE // pseudo diffuse + shininess + dissolve rendering method (used for loaded obj files)
	else
	{
		vec3 hitPoint = ray->orig + ray->dir * nearest;
		vec3 normal = ray->hit->getNormal(hitPoint);
		vec3 color = BLACK;

		vec3 shadowPointOrig = (dot(ray->dir, normal) < 0) ?
			hitPoint + normal * 0.0001f :
			hitPoint - normal * 0.0001f;

		if (ray->hit->material->shader == Material::Shader::DIFFUSE)
		{
			for (size_t i = 0; i < this->scene->lightList.size(); i++)
			{
				vec3 diffuseColor = BLACK;
				vec3 specularColor = BLACK;

				vec3 lightDir = this->scene->lightList[i]->pos - hitPoint;
				//float lightDistance2 = dot(lightDir, lightDir);
				lightDir = normalize(lightDir);

				if (dot(lightDir, normal) < 0) { continue; }

				diffuseColor += DirectIllumination(hitPoint, lightDir, normal, scene->lightList[i], ray);

				vec3 reflectionDirection = reflect(-lightDir, normal);
				specularColor += powf(glm::max(0.0f, -dot(reflectionDirection, ray->dir)), ray->hit->material->shininess);// * this->scene->lightList[i]->color;

				color += diffuseColor + specularColor * ray->hit->material->shininess;
			}
		}

		if (ray->hit->material->shader == Material::Shader::MIRROR)
		{
			Ray reflectRay = Ray(hitPoint, Reflect(ray->dir, normal));
			color += GetColor(ray) * SampleWhitted(x, y, &reflectRay, depth += 1);
		}

		if (ray->hit->material->shader == Material::Shader::GLASS)
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
			vec3 reflectionColor = SampleWhitted(x, y, &reflectRay, depth += 1);

			Ray refractRay = Ray(refractionRayOrig, refractionDirection);
			vec3 refractionColor = SampleWhitted(x, y, &refractRay, depth += 1);

			color += reflectionColor * (ray->hit->material->dissolve) + refractionColor * (1 - ray->hit->material->dissolve);
		}
		return color;
	}

#else
	// If ray intersects
	else
	{
		vec3 hitPoint = ray->orig + ray->dir * nearest;
		vec3 normal = ray->hit->getNormal(hitPoint);
		vec3 color = BLACK;

		// DIFFUSE material shader hit
		if (ray->hit->material->shader == Material::Shader::DIFFUSE)
		{
			for (size_t i = 0; i < this->scene->lightList.size(); i++)
			{
				vec3 dir = normalize(scene->lightList[i]->pos - hitPoint);
				if (dot(dir, normal) < 0) continue;
				color += DirectIllumination(hitPoint, dir, normal, scene->lightList[i], ray);
			}
			ray->t = INFINITY;
		}

		// MIRROR material shader hit
		if (ray->hit->material->shader == Material::Shader::MIRROR)
		{
			Ray reflectRay = Ray(hitPoint, Reflect(ray->dir, normal));
			color += GetColor(ray) * SampleWhitted(x, y, &reflectRay, depth += 1);
			ray->t = INFINITY;
		}

		// GLASS material shader hit
		if (ray->hit->material->shader == Material::Shader::GLASS)
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

				refractColor = this->SampleWhitted(x, y, &refractRay, depth += 1);
			}

			Ray reflectRay = Ray(hitPoint, reflect(ray->dir, normal));
			vec3 reflectColor = GetColor(ray) * SampleWhitted(x, y, &reflectRay, depth += 1);
			ray->t = INFINITY;

			color += reflectColor * kr + refractColor * (1 - kr);
		}
		return color;
	}
#endif
}

vec3 RayTracer::DirectIllumination(vec3 hitPoint, vec3 dir, vec3 normal, Light* light, Ray* ray)
{
	vec3 hitEpsilon = hitPoint + dir * 0.0001f;
	Ray shadowRay = Ray(hitPoint, dir);
	shadowRay.t = INFINITY;

	float lightInt = 0.0f;
	float tToLight = length(light->pos - hitEpsilon);

	if (renderShadow)
	{
#if ENABLEBVH
#if ENABLETOPBVH
		scene->bvhTop->TraverseTop(&shadowRay, scene->bvhTop->topRoot, true);
#else
		scene->bvh->Traverse(&shadowRay, scene->bvh->root, true);
#endif
		if (shadowRay.t < tToLight) { return BLACK; }
#else
		for (size_t i = 0; i < this->scene->primList.size(); i++)
		{
			this->scene->primList[i]->intersect(&shadowRay);
			if (shadowRay.t < tToLight) { return BLACK; }
		}
#endif // ENABLEBVH
	}

	float euclidianDistanceToLight = distance(hitPoint, light->pos);
	return light->color * dot(normal, dir) * (1 / (euclidianDistanceToLight*euclidianDistanceToLight)) * (GetColor(ray) * INVPI);
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

// -----------------------------------------------------------
// Pathtracing stuff
// -----------------------------------------------------------

vec3 RayTracer::Trace(Ray* ray)
{
	float nearest = INFINITY;

	scene->bvh->Traverse(ray, scene->bvh->root);
	nearest = ray->t;

	return ray->orig + ray->dir * nearest;
}

vec3 RayTracer::Sample(Ray* ray, int depth)
{
	if (depth > MAXDEPTH || this->scene->primList.size() == 0)
	{
		return BACKGROUND_COLOR;
	}
	
	// trace ray
	vec3 hitPoint = Trace(ray);

	// teriminate if ray left the scene
	if (ray->t == INFINITY)
	{
		//return BACKGROUND_COLOR;
		return scene->skydome ? SampleSkydome(scene->skydome, ray) : BLACK;
	}

	// terminate if we hit a light source
	if (ray->hit->getIsLight())
	{
		return ray->hit->material->diffuse;
	}

	vec3 normal = ray->hit->getNormal(hitPoint);

	// continue in random direction
	vec3 R = CosineWeightedDiffuseReflection(normal);
	Ray newRay = Ray(hitPoint, R);

	// update throughput
	vec3 BRDF = GetColor(ray) * INVPI;	// bidirectional reflectance distribution function
	vec3 Ei = Sample(&newRay, depth + 1) * dot(normal, R); // irradiance
	return PI * 2.0f * BRDF * Ei;
}

vec3 RayTracer::CosineWeightedDiffuseReflection(vec3 normal)
{
	float r0 = (float)rand() / RAND_MAX, r1 = (float)rand() / RAND_MAX;
	float r = sqrt(r0);
	float theta = 2 * PI * r1;
	float x = r * cosf(theta);
	float y = r * sinf(theta);

	vec3 dir = vec3(x, y, sqrt(1 - r0));

	vec3 randomDir = normalize(vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX));
	vec3 t = cross(randomDir, normal);
	vec3 b = cross(normal, t);
	mat3 tangentSpace = mat3(b, t, normal);

	return tangentSpace * dir;
}

// -----------------------------------------------------------
// Global stuff
// -----------------------------------------------------------

vec3 RayTracer::GetColor(Ray* ray)
{
	if (ray->hit->material->texture)
	{
		vec2 uv = ray->hit->getTexCoord(ray);
		return SampleTexturePoint(ray->hit->material->texture, uv);
	}
	else
	{
		return ray->hit->material->diffuse;
	}
}

vec3 RayTracer::SampleTexturePoint(Surface* tex, vec2 uv)
{
	const vec2 nuv(fmodf(uv.x + 1000.0f, 1.0f), fmodf(uv.y + 1000.0f, 1.0f));
	const int x = (int)(nuv.x * (float)(tex->GetWidth() - 1));
	const int y = tex->GetHeight() - 1 - (int)(nuv.y * (float)(tex->GetHeight() - 1));
	const Pixel color = tex->GetBuffer()[y*tex->GetPitch() + x];
	return vec3((float)((color >> 16) & 0xff), (float)((color >> 8) & 0xff), (float)((color) & 0xff)) / 255.0f;
}

vec3 RayTracer::SampleSkydome(HDRBitmap* skydome, Ray* ray)
{
	float u = fmodf(0.5f * (1.0f + atan2(ray->dir.x, -ray->dir.z) * INVPI), 1.0f);
	float v = acosf(ray->dir.y) * INVPI;
	int pixel = (int)(u * (float)(skydome->width - 1)) + ((int)(v * (float)(skydome->height - 1)) * skydome->width);
	return skydome->buffer[pixel];
}