#include "template.h"
#include "RayTracer.h"
#include "Sphere.h"

int iCPU2 = omp_get_num_procs();
glm::uint seed1 = 161219 * 49977;
glm::uint seed2 = 324867 * 67869;

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
	float invSample = 1.0f / samples;

	int halfWidth = SCRWIDTH / 2;

#pragma omp parallel for private(x)
	for (int y = 0; y < SCRHEIGHT; y++)
	{
		for (x = 0; x < SCRWIDTH; x++)
		{
			//Ray ray = Ray(this->scene->camera->GenerateRay(x, y));
			Ray ray; this->scene->camera->GenerateRay(&ray, x, y);
			//vec3 color = SampleWhitted(x, y, &ray, 0);	// Whitted-style ray tracing
			//vec3 color = SampleSimple(&ray, 0);			// Path tracing with importance sampling
			//vec3 color = Sample(&ray, 0);					// Path tracing with importance sampling and next event estimation
			//vec3 color = SampleMIS(&ray);					// Path tracing with multiple importance sampling
			
			vec3 color;
			if (x < halfWidth)
			{
				color = SampleSimple(&ray, 0);
			}
			else
			{
				color = Sample(&ray, 0, true);
			}

			/*color *= 255.0f;
			int r = glm::min((int)color.r, 255);
			int g = glm::min((int)color.g, 255);
			int b = glm::min((int)color.b, 255);*/

			color *= 255.0f;
			accumulator[y][x].r = (accumulator[y][x].r * (1 - invSample) + color.r * invSample);
			accumulator[y][x].g = (accumulator[y][x].g * (1 - invSample) + color.y * invSample);
			accumulator[y][x].b = (accumulator[y][x].b * (1 - invSample) + color.z * invSample);

			int r = glm::min((int)accumulator[y][x].r, 255);
			int g = glm::min((int)accumulator[y][x].g, 255);
			int b = glm::min((int)accumulator[y][x].b, 255);

			// Gamma correction
			/*float gamma = 1.8f;
			r = (int)(pow((float)r / 255, 1 / gamma) * 255);
			g = (int)(pow((float)g / 255, 1 / gamma) * 255);
			b = (int)(pow((float)b / 255, 1 / gamma) * 255);*/

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

void RayTracer::RenderPacket()
{
	omp_set_num_threads(iCPU2);
	int x = 0;

#pragma omp parallel for private(x)
	for (int y = 0; y < SCRHEIGHT / RayPacket::width; y++)
	{
		for (x = 0; x < SCRWIDTH / RayPacket::height; x++)
		{
			RayPacket rays; this->scene->camera->GenerateRays(&rays, x, y);
		}
	}
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
		this->scene->bvhTop->TraverseTop(ray, this->scene->bvhTop->topRoot, 0, &depthRender);
#else
		this->scene->bvh->Traverse(ray, this->scene->bvh->root, 0, &depthRender);
#endif
		return vec3(clamp((depthRender * 0.005f), 0.0f, 1.0f), clamp((0.8f - depthRender * 0.005f), 0.0f, 1.0f), 0.0f);
	}

	// Trace function
#if ENABLEBVH
#if ENABLETOPBVH
	this->scene->bvhTop->TraverseTop(ray, this->scene->bvhTop->topRoot);
#else
	this->scene->bvh->Traverse(ray, this->scene->bvh->root);
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
			hitPoint + normal * EPSILON :
			hitPoint - normal * EPSILON;

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

				diffuseColor += DirectIllumination(hitPoint, lightDir, normal, this->scene->lightList[i], ray);

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
				hitPoint - normal * EPSILON :
				hitPoint + normal * EPSILON;
			vec3 refractionRayOrig = (dot(refractionDirection, normal) < 0) ?
				hitPoint - normal * EPSILON :
				hitPoint + normal * EPSILON;

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
				vec3 dir = normalize(this->scene->lightList[i]->pos - hitPoint);
				if (dot(dir, normal) < 0) continue;
				color += DirectIllumination(hitPoint, dir, normal, this->scene->lightList[i], ray);
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
			vec3 bias = EPSILON * ray->hit->getNormal(hitPoint);

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
	vec3 hitEpsilon = hitPoint + dir * EPSILON;
	Ray shadowRay = Ray(hitPoint, dir);
	shadowRay.t = INFINITY;

	float lightInt = 0.0f;
	float tToLight = length(light->pos - hitEpsilon);

	if (renderShadow)
	{
#if ENABLEBVH
#if ENABLETOPBVH
		this->scene->bvhTop->TraverseTop(&shadowRay, this->scene->bvhTop->topRoot, true);
#else
		this->scene->bvh->Traverse(&shadowRay, this->scene->bvh->root, true);
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
// Path tracing stuff
// -----------------------------------------------------------

vec3 RayTracer::Trace(Ray* ray)
{
	float nearest = INFINITY;

	this->scene->bvh->Traverse(ray, this->scene->bvh->root);
	nearest = ray->t;
	if (nearest == INFINITY)
	{
		return BLACK;
	}
	else return ray->orig + ray->dir * nearest;
}

vec3 RayTracer::SampleSimple(Ray* ray, int depth)
{
	if (depth > MAXDEPTH || this->scene->primList.size() == 0) { return BACKGROUND_COLOR; }

	// trace ray
	vec3 I = Trace(ray); // ray intersection

	// teriminate if ray left the scene
	if (ray->t == INFINITY)
	{
		//return BACKGROUND_COLOR;
		return this->scene->skydome ? SampleSkydome(this->scene->skydome, ray) : BLACK;
	}

	// terminate if we hit a light source
	if (ray->hit->getIsLight()) return ray->hit->material->diffuse;
	/// Emission Le(x, ωo) (outgoing radiance), direct illumination

	vec3 N = ray->hit->getNormal(I); // intersection normal

	// surface interaction
	if (ray->hit->material->shader == Material::Shader::MIRROR)
	{
		// continue in fixed direction
		Ray r = Ray(I, reflect(ray->dir, N));
		return GetColor(ray) * SampleSimple(&r, depth + 1);
	}

	if (ray->hit->material->shader == Material::Shader::GLASS)
	{
		vec3 refractionDirection = normalize(refract(ray->dir, N, 0.8f));
		vec3 refractionRayOrig = (dot(refractionDirection, N) < 0) ?
			I - N * EPSILON :
			I + N * EPSILON;

		Ray r = Ray(refractionRayOrig, refractionDirection);
		return GetColor(ray) * SampleSimple(&r, depth + 1);
	}

	// continue in random direction
	vec3 R = CosineWeightedDiffuseReflection(N);
	Ray r = Ray(I, R);

	// update throughput
	/// x = point, wo = output, wi = input
	vec3 BRDF = GetColor(ray) * INVPI; // bidirectional reflectance distribution function
	/// fr(x, ωo, ωi), indirect illumination or reflected light
	float PDF = dot(N, R) * INVPI; 	if (PDF == 0) PDF = EPSILON; // probability density function
	/// cosθi? solid angle
	vec3 Ei = SampleSimple(&r, depth + 1) * dot(N, R) / PDF; // irradiance
	/// Li(x, ωi) cosθi
	return BRDF * Ei; // outgoing radiance
	/// Lo(x, ωo) = f fr(x, ωo, ωi) Li(x, ωi) cosθi dωi
}

vec3 RayTracer::Sample(Ray* ray, int depth, bool lastSpecular)
{
	if (depth > MAXDEPTH || this->scene->primList.size() == 0) { return BACKGROUND_COLOR; }
	
	// trace ray
	vec3 hitPoint = Trace(ray);

	// teriminate if ray left the scene
	if (ray->t == INFINITY)
	{
		//return BACKGROUND_COLOR;
		return this->scene->skydome ? SampleSkydome(this->scene->skydome, ray) : BLACK;
	}

	// terminate if we hit a light source
	if (ray->hit->getIsLight())
	{
		if (lastSpecular)
		{
			return ray->hit->material->diffuse;
		}
		else
		{
			return BLACK;
		}
	}

	vec3 normal = ray->hit->getNormal(hitPoint);
	vec3 BRDF = GetColor(ray) * INVPI;	// bidirectional reflectance distribution function

	// surface interaction
	if (ray->hit->material->shader == Material::Shader::MIRROR)
	{
		// continue in fixed direction
		Ray r = Ray(hitPoint, reflect(ray->dir, normal));
		return GetColor(ray) * SampleSimple(&r, depth + 1);
	}

	if (ray->hit->material->shader == Material::Shader::GLASS)
	{
		vec3 refractionDirection = normalize(refract(ray->dir, normal, 0.8f));
		vec3 refractionRayOrig = (dot(refractionDirection, normal) < 0) ?
			hitPoint - normal * EPSILON :
			hitPoint + normal * EPSILON;

		Ray r = Ray(refractionRayOrig, refractionDirection);
		return GetColor(ray) * Sample(&r, depth + 1, false);
	}

	// sample a random light source
	// TODO -- Pick a random light and create a random ray towards that light
	//Primitive* randomLight = this->scene->areaLightList.back();

	int lightIndex = rand() % this->scene->areaLightList.size();
	Primitive* randomLight = this->scene->areaLightList[lightIndex];
	vec3 randomPointOnLight = randomLight->randomPointOnPrimitive(hitPoint);
	
	vec3 randomDirToRandomLight = normalize(randomPointOnLight - hitPoint);
	vec3 lightNormal = randomLight->getNormal(randomPointOnLight);
	float dist2 = glm::length2(randomPointOnLight - hitPoint);

	/*vec3 randomDirToRandomLight = randomPointOnLight - hitPoint;
	vec3 lightNormal = randomLight->getNormal(randomPointOnLight);
	float dist = length(randomDirToRandomLight);
	randomDirToRandomLight /= dist;*/

	Ray lr = Ray(hitPoint, randomDirToRandomLight);

	vec3 Ld;
	if (Trace(&lr) != BLACK && dot(normal, randomDirToRandomLight) > 0 && dot(lightNormal, -randomDirToRandomLight) > 0)
	{
		//float solidAngle = (dot(lightNormal, -randomDirToRandomLight) * randomLight->calculateArea()) / (dist * dist);
		//Ld = randomLight->material->diffuse * solidAngle * BRDF * dot(normal, randomDirToRandomLight);

		float solidAngle = (dot(lightNormal, -randomDirToRandomLight) * randomLight->calculateArea()) / dist2;
		float lightPDF = 1 / solidAngle; if (lightPDF == 0) lightPDF = EPSILON;
		Ld = (float)this->scene->areaLightList.size() * randomLight->material->diffuse * BRDF * (dot(normal, randomDirToRandomLight) / lightPDF);
	}
	else
	{
		Ld = BLACK;
	}

	// continue random walk
	vec3 R = CosineWeightedDiffuseReflection(normal);
	Ray r = Ray(hitPoint, R);

	// update throughput
	float PDF = dot(normal, R) * INVPI; if (PDF == 0) PDF = EPSILON;
	vec3 Ei = Sample(&r, depth + 1, false) * dot(normal, R) / PDF; // irradiance
	return BRDF * Ei + Ld;
}

vec3 RayTracer::SampleMIS(Ray* ray)
{
	vec3 T = WHITE, E = BLACK;
	while (1)
	{
		// trace ray
		vec3 I = Trace(ray); // ray intersection (ray hit point)
		if (ray->t == INFINITY) break; // terminate if ray left the scene
		if (ray->hit->getIsLight()) break; // terminate if we hit a light source

		vec3 N = ray->hit->getNormal(I); // primitive normal
		vec3 BRDF = GetColor(ray) * INVPI;	// bidirectional reflectance distribution function

		// sample a random light source
		Primitive* randomLight = this->scene->areaLightList.back();
		vec3 randomPointOnLight = randomLight->randomPointOnPrimitive(I);

		vec3 L = normalize(randomPointOnLight - I); // random direction to random light
		vec3 Nl = randomLight->getNormal(randomPointOnLight); // light normal
		float dist2 = glm::length2(randomPointOnLight - I); // distance squared
		///float A = randomLight->calculateArea();

		Ray lr = Ray(I, L);

		if (dot(N, L) > 0 && dot(Nl, -L) > 0) if (Trace(&lr) != BLACK)
		{
			float solidAngle = (dot(Nl, -L) * randomLight->calculateArea()) / dist2;
			float lightPDF = 1 / solidAngle; if (solidAngle = 0) solidAngle = EPSILON;
			E += T * (dot(N, L) / lightPDF) * BRDF * randomLight->material->diffuse;
		}

		// continue random walk
		vec3 R = CosineWeightedDiffuseReflection(N);
		float hemiPDF = 1 / (PI * 2.0f);
		ray = &Ray(I, R);
		T *= (dot(N, R) / hemiPDF) * BRDF;
	}
	return E;
}

vec3 RayTracer::CosineWeightedDiffuseReflection(vec3 normal)
{
	/// RNG 1
	//float r0 = (float)rand() / RAND_MAX, r1 = (float)rand() / RAND_MAX;
	float r0 = RandomFloat(&seed1), r1 = RandomFloat(&seed2);
	float r = sqrt(r0);
	float theta = 2 * PI * r1;
	float x = r * cosf(theta);
	float y = r * sinf(theta);

	vec3 randomDir = normalize(vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX));
	vec3 t = cross(randomDir, normal);
	vec3 b = cross(normal, t);
	mat3 tangentSpace = mat3(b, t, normal);

	return tangentSpace * vec3(x, y, sqrt(1 - r0));

	/// RNG 2
	/*float r1 = RandomFloat(&seed1), r2 = RandomFloat(&seed1);
	float term1 = 2 * PI * r1;
	float term2 = 2 * sqrt(r2 * (1 - r2));
	vec3 R = vec3(cos(term1) * term2, sin(term1) * term2, 1 - 2 * r2);
	if (R.z < 0) R.z = -R.z;

	vec3 n = normal;
	vec3 axis = fabs(n.x) > 0.1f ? vec3(0.0f, 1.0f, 0.0f) : vec3(1.0f, 0.0f, 0.0f);
	vec3 t = normalize(cross(axis, n));
	vec3 b = cross(n, t);

	return normalize(R.x * t + R.y * b + R.z*n);*/

	/// RNG 3
	/*float r1 = 2.0f * PI * RandomFloat(&seed1);
	float r2 = 1;
	while (r2 == 1) r2 = RandomFloat(&seed1);

	float r2s = sqrt(r2);
	vec3 n = normal;
	vec3 axis = fabs(n.x) > 0.1f ? vec3(0.0f, 1.0f, 0.0f) : vec3(1.0f, 0.0f, 0.0f);
	vec3 t = normalize(cross(axis, n));
	vec3 b = cross(n, t);

	return normalize(t * cos(r1) * r2s + b * sin(r1) * r2s + n * sqrt(1.0f - r2));*/

	/// RNG 4
	/*float r0 = RandomFloat(&seed1), r1 = RandomFloat(&seed2);
	float r = sqrt(r0);
	float theta = 2 * PI * r1;
	float x = r * cosf(theta);
	float y = r * sinf(theta);
	float minus = 1 - r0;
	float sqrtR0 = sqrt(max(0.0f, minus));
	vec3 randomDir = abs(normal.x) > 0.1f ? vec3(0.0f, 1.0f, 0.0f) : vec3(1.0f, 0.0f, 0.0f);
	vec3 direction = normalize(vec3(x, y, sqrtR0));
	vec3 t = cross(randomDir, normal);
	vec3 b = cross(normal, t);
	mat3 tangentSpace = mat3(t, b, normal);
	return = tangentSpace * direction;*/
}

glm::uint RayTracer::RandomInt(glm::uint* seed)
{
	// Marsaglia Xor32; see http://excamera.com/sphinx/article-xorshift.html
	// also see https://github.com/WebDrake/xorshift/blob/master/xorshift.c for higher quality variants
	*seed ^= *seed << 13;
	*seed ^= *seed >> 17;
	*seed ^= *seed << 5;
	return *seed;
}

float RayTracer::RandomFloat(glm::uint* seed)
{
	return RandomInt(seed) * 2.3283064365387e-10f;
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