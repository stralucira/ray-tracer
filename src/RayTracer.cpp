#include "template.h"
#include "RayTracer.h"
#include "Sphere.h"

#define RR 0

int iCPU2 = omp_get_num_procs();
glm::uint seed1 = 1219 * 49977;
glm::uint seed2 = 3267 * 67869;

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
			Ray ray;
			if (!depthOfField)
			{
				this->scene->camera->GenerateRay(&ray, x, y); // generate ray
			}
			else
			{
				this->scene->camera->GenerateRayDOF(&ray, x, y); // generate ray with randomization
			}
			
			vec3 color;
			if (!renderShadow)
			{
				color = SampleBasic(x, y, &ray, 0);
			}
			else if (depthRendering)
			{
				color = SampleDepth(x, y, &ray, 0); // depth renderer
			}
			else
			{
				//color = SampleWhitted(x, y, &ray, 0);	// whitted-style ray tracing
				//color = SampleSimple(&ray, 0);			// path tracing with importance sampling
				//color = Sample(&ray, 0, true);			// path tracing with importance sampling and next event estimation
				//color = SampleMIS(&ray);					// path tracing with multiple importance sampling
				color = SampleEX(&ray, 0, true);			// DEBUG STUFF

				/*if (x < halfWidth)
				{
					color = Sample(&ray, 0, true);
				}
				else
				{
					color = SampleEX(&ray, 0, true);
				}*/
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
			/*float gamma = 2.2f;
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

// -----------------------------------------------------------
// Whitted-style ray tracing stuff
// -----------------------------------------------------------

vec3 RayTracer::SampleWhitted(int x, int y, Ray* ray, int depth)
{
	if (depth > MAXDEPTH || this->scene->primList.size() == 0) return BACKGROUND_COLOR;

#pragma region trace function
	float nearest = INFINITY;
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
	if (nearest == INFINITY) return BACKGROUND_COLOR; 	// ray does not intersect
#pragma endregion

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

				refractColor = GetColor(ray) * SampleWhitted(x, y, &refractRay, depth += 1);
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

vec3 RayTracer::SampleBasic(int x, int y, Ray* ray, int depth)
{
	if (depth > MAXDEPTH || this->scene->primList.size() == 0) return BACKGROUND_COLOR;

#pragma region trace function
	float nearest = INFINITY;
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
	if (nearest == INFINITY) return BACKGROUND_COLOR; 	// ray does not intersect
#pragma endregion

	return GetColor(ray);
}

vec3 RayTracer::SampleDepth(int x, int y, Ray* ray, int depth)
{
	if (depth > MAXDEPTH || this->scene->primList.size() == 0) return BACKGROUND_COLOR;

	int depthRender = 0;
#if ENABLETOPBVH
	this->scene->bvhTop->TraverseTop(ray, this->scene->bvhTop->topRoot, 0, &depthRender);
#else
	this->scene->bvh->Traverse(ray, this->scene->bvh->root, 0, &depthRender);
#endif
	return vec3(clamp((depthRender * 0.005f), 0.0f, 1.0f), clamp((0.8f - depthRender * 0.005f), 0.0f, 1.0f), 0.0f);
}

vec3 RayTracer::DirectIllumination(vec3 hitPoint, vec3 dir, vec3 normal, Light* light, Ray* ray)
{
	vec3 hitEpsilon = hitPoint + dir * EPSILON;
	Ray shadowRay = Ray(hitPoint, dir);
	shadowRay.t = INFINITY;

	float lightInt = 0.0f;
	float tToLight = length(light->pos - hitEpsilon);

#pragma region trace shadow ray
#if ENABLEBVH
#if ENABLETOPBVH
	this->scene->bvhTop->TraverseTop(&shadowRay, this->scene->bvhTop->topRoot, true);
#else
	this->scene->bvh->Traverse(&shadowRay, this->scene->bvh->root, true);
#endif
	if (shadowRay.t < tToLight) return BLACK;
#else
	for (size_t i = 0; i < this->scene->primList.size(); i++)
	{
		this->scene->primList[i]->intersect(&shadowRay);
		if (shadowRay.t < tToLight) return BLACK;
	}
#endif // ENABLEBVH
#pragma endregion

	float euclidianDistanceToLight = distance(hitPoint, light->pos);
	return light->color * dot(normal, dir) * (1 / (euclidianDistanceToLight*euclidianDistanceToLight)) * (GetColor(ray) * INVPI);
}

// -----------------------------------------------------------
// Path tracing stuff
// -----------------------------------------------------------

vec3 RayTracer::Trace(Ray* ray, bool isShadowRay)
{
	float nearest = INFINITY;

	if (!isShadowRay)
	{
		this->scene->bvh->Traverse(ray, this->scene->bvh->root);
		nearest = ray->t;

	}
	else
	{
		float tToLight = ray->t;
		ray->t = INFINITY;
		this->scene->bvh->Traverse(ray, this->scene->bvh->root, true);

		if (ray->t < tToLight)
		{
			return BLACK; // false
		}
		else
		{
			return WHITE; // true
		}
	}

	if (nearest == INFINITY)
	{
		return BLACK;
	}
	else
	{
		ray->t = nearest;
		return ray->orig + ray->dir * nearest;
	}
}

vec3 RayTracer::SampleSimple(Ray* ray, int depth)
{
	if (depth > MAXDEPTH || this->scene->primList.size() == 0) return BACKGROUND_COLOR;

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

#pragma region surface interaction
	// surface interaction
	if (ray->hit->material->shader == Material::Shader::MIRROR)
	{
		// continue in fixed direction
		Ray r = Ray(I, reflect(ray->dir, N));
		return GetColor(ray) * SampleSimple(&r, depth + 1);
	}

	if (ray->hit->material->shader == Material::Shader::GLASS)
	{
		vec3 refractColor = BLACK;

		float kr = Fresnel(ray->dir, N, 1.52f);
		bool outside = dot(ray->dir, N) < 0;
		vec3 bias = EPSILON * ray->hit->getNormal(I);

		float cosi = clamp(-1.0f, 1.0f, dot(N, ray->dir));
		float etai = 1, etat = 1.52f;
		vec3 n = N;

		if (cosi < 0)
		{
			cosi = -cosi;
		}
		else
		{
			std::swap(etai, etat);
			n = -N;
		}
		float eta = etai / etat;
		float k = 1 - eta * eta * (1 - cosi * cosi);

		if (k < 0)
		{
			return GetColor(ray);
		}
		else
		{
			Ray refractRay = Ray(I, eta * ray->dir + (eta * cosi - sqrtf(k)) * n);
			vec3 hitEpsilon = refractRay.orig + refractRay.dir * EPSILON;
			refractRay.orig = hitEpsilon;
			refractColor = SampleSimple(&refractRay, depth + 1);
		}

		Ray reflectRay = Ray(I, reflect(ray->dir, N));
		vec3 reflectColor = GetColor(ray) * SampleSimple(&reflectRay, depth + 1);
		ray->t = INFINITY;

		return reflectColor * kr + refractColor * (1 - kr);
	}
#pragma endregion

	// continue in random direction
	vec3 R = CosineWeightedDiffuseReflection(N);
	Ray r = Ray(I, R);

	// update throughput
	/// x = point, wo = output, wi = input
	vec3 BRDF = GetColor(ray) * INVPI; // bidirectional reflectance distribution function
	/// fr(x, ωo, ωi), indirect illumination or reflected light
	float PDF = dot(N, R) * INVPI; // probability density function
	/// cosθi? solid angle
	if (PDF == 0) return BRDF;
	else
	{
		vec3 Ei = SampleSimple(&r, depth + 1) * dot(N, R) / PDF; // irradiance
		/// Li(x, ωi) cosθi
		return BRDF * Ei; // outgoing radiance
		/// Lo(x, ωo) = f fr(x, ωo, ωi) Li(x, ωi) cosθi dωi
	}
}

vec3 RayTracer::Sample(Ray* ray, int depth, bool lastSpecular)
{
#if !RR
	if (depth > MAXDEPTH || this->scene->primList.size() == 0) return BACKGROUND_COLOR;
#endif

	// trace ray
	vec3 I = Trace(ray);

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
			//return ray->hit->material->diffuse;
			if (dot(ray->hit->getNormal(I), ray->dir) > 0)
			{
				return BLACK;
			}
			else
			{
				return ray->hit->material->diffuse;
			}
		}
		else
		{
			return BLACK;
		}
	}

#if RR // russian roulette
	float a = RandomFloat(&seed1);
	float surviveProb = glm::min(.5f, glm::max(glm::max(ray->hit->material->diffuse.r, ray->hit->material->diffuse.g), ray->hit->material->diffuse.b));
	if (a > surviveProb) return BLACK;
	float survivePower = 1.0f / surviveProb;
#else
	float survivePower = 1.0f;
#endif

	vec3 N = ray->hit->getNormal(I);
	vec3 BRDF = GetColor(ray) * INVPI;	// bidirectional reflectance distribution function

#pragma region surface interaction
	// surface interaction
	/// dissolve property from OBJ
	if (ray->hit->material->dissolve < 1.0f)
	{
		float kr = Fresnel(ray->dir, N, ray->hit->material->ior);
		bool outside = dot(ray->dir, N) < 0;
		vec3 bias = EPSILON * ray->hit->getNormal(I);

		float path = RandomFloat(&seed1);

		if (path < ray->hit->material->dissolve)
		{
			Ray r = Ray(I, reflect(ray->dir, N));
			return GetColor(ray) * Sample(&r, depth + 1, true) * survivePower;
		}

		Ray r = Ray(I, ray->dir);
		return GetColor(ray) * Sample(&r, depth + 1, false) * survivePower;
	}

	/// reflection
	if (ray->hit->material->shininess > EPSILON || ray->hit->material->shader == Material::Shader::MIRROR)
	{
		// continue in direction depending on material shininess
		float shininess = 1.f - (ray->hit->material->shininess / 1000.f);
		float path = RandomFloat(&seed1);

		if (path < 1 - shininess)
		{
			vec3 R = reflect(ray->dir, N);
			vec3 P = I + R + (CosineWeightedDiffuseReflection(R) * shininess);
			R = normalize(P - I);

			Ray r = Ray(I, R);
			return GetColor(ray) * Sample(&r, depth + 1, true) * survivePower;
		}
	}

	/// reflection + refraction
	if (ray->hit->material->ior > EPSILON)
	{
		float kr = Fresnel(ray->dir, N, ray->hit->material->ior);
		bool outside = dot(ray->dir, N) < 0;
		vec3 bias = EPSILON * ray->hit->getNormal(I);

		float path = RandomFloat(&seed1);

		if (path < (1 - kr))
		{
			vec3 refractDir = Refract(ray->dir, N, ray->hit->material->ior);
			vec3 refractOrig = outside ? I - bias : I + bias;
			Ray r = Ray(refractOrig, refractDir);
			return GetColor(ray) * Sample(&r, depth + 1, true) * survivePower;
		}

		Ray r = Ray(I, reflect(ray->dir, N));
		return GetColor(ray) * Sample(&r, depth + 1, true) * survivePower;
	}
#pragma endregion

#pragma region direct illumination
	vec3 Ld;

	// sample a random light source
	int lightIndex = rand() % this->scene->areaLightList.size();
	Primitive* randomLight = this->scene->areaLightList[lightIndex];
	vec3 randomPointOnLight = randomLight->randomPointOnPrimitive(I);

	vec3 L = normalize(randomPointOnLight - I);
	vec3 lightNormal = randomLight->getNormal(randomPointOnLight);
	float dist2 = glm::length2(randomPointOnLight - I);

	Ray lr = Ray(I, L);
	lr.t = length(randomPointOnLight - (I + L));

	if (dot(N, L) > 0 && dot(lightNormal, -L) > 0) if (Trace(&lr, true) != BLACK)
	{
		/*float solidAngle = (dot(lightNormal, -L) * randomLight->calculateArea()) / dist2;
		Ld = (float)this->scene->areaLightList.size() * randomLight->material->diffuse * solidAngle * BRDF * dot(N, L);*/

		float solidAngle = (dot(lightNormal, -L) * randomLight->calculateArea()) / dist2;
		if (solidAngle != 0)
		{
			float lightPDF = 1 / solidAngle;
			Ld = (float)scene->areaLightList.size() * randomLight->material->diffuse * BRDF * (dot(N, L) / lightPDF);
		}
	}

#pragma endregion

	// continue random walk
	vec3 R = CosineWeightedDiffuseReflection(N);
	Ray r = Ray(I, R);

	// update throughput
	float PDF = dot(N, R) * INVPI;
	vec3 Ei = BLACK;
	if (PDF != 0)
	{
		Ei = Sample(&r, depth + 1, false) * dot(N, R) / PDF ; // irradiance
	}

#if RR 
	return BRDF * Ei * survivePower + Ld;
#else
	return BRDF * Ei + Ld;
#endif
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
		int lightIndex = rand() % this->scene->areaLightList.size();
		Primitive* randomLight = this->scene->areaLightList[lightIndex];
		vec3 randomPointOnLight = randomLight->randomPointOnPrimitive(I);

		vec3 L = normalize(randomPointOnLight - I); // random direction to random light
		vec3 Nl = randomLight->getNormal(randomPointOnLight); // light normal
		float dist2 = glm::length2(randomPointOnLight - I); // distance squared
		///float A = randomLight->calculateArea();

		Ray lr = Ray(I, L);

		if (dot(N, L) > 0 && dot(Nl, -L) > 0) if (Trace(&lr) != BLACK)
		{
			float solidAngle = (dot(Nl, -L) * randomLight->calculateArea()) / dist2;
			float lightPDF = 1.0f / solidAngle;
			float brdfPDF = 0.5f * INVPI;
			float misPDF = lightPDF + brdfPDF;
			E += T * (dot(N, L) / misPDF) * BRDF * randomLight->material->diffuse;
		}

		// continue random walk
		vec3 R = CosineWeightedDiffuseReflection(N);
		float hemiPDF = 1 / (PI * 2.0f);
		//ray = &Ray(I, R);
		T *= (dot(N, R) / hemiPDF) * BRDF;
	}
	return E;
}

vec3 RayTracer::SampleEX(Ray* ray, int depth, bool lastSpecular)
{
#if !RR
	if (depth > MAXDEPTH || this->scene->primList.size() == 0) return BACKGROUND_COLOR;
#endif

	// trace ray
	vec3 I = Trace(ray);

	// teriminate if ray left the scene
	if (ray->t == INFINITY)
	{
		//return BLACK;
		return this->scene->skydome ? SampleSkydome(this->scene->skydome, ray) : BACKGROUND_COLOR;
	}

	// terminate if we hit a light source
	if (ray->hit->getIsLight())
	{
		if (lastSpecular)
		{
			//return ray->hit->material->diffuse;
			if (dot(ray->hit->getNormal(I), ray->dir) > 0)
			{
				return BLACK;
			}
			else
			{
				return ray->hit->material->diffuse;
			}
		}
		else
		{
			return BLACK;
		}
	}

#if RR // russian roulette
	float a = RandomFloat(&seed1);
	float surviveProb = glm::min(1.0f, glm::max(glm::max(ray->hit->material->diffuse.r, ray->hit->material->diffuse.g), ray->hit->material->diffuse.b));
	if (a > surviveProb) return BLACK;
	float survivePower = 1.0f / surviveProb;
#else
	float survivePower = 1.0f;
#endif

	vec3 N = ray->hit->getNormal(I);
	vec3 BRDF = GetColor(ray) * INVPI;	// bidirectional reflectance distribution function

#pragma region material interaction
	/// reflection
	if (ray->hit->material->shininess > EPSILON || ray->hit->material->shader == Material::Shader::MIRROR)
	{
		// continue in direction depending on material shininess
		float shininess = 1.f - (ray->hit->material->shininess / 1000.f);
		float path = RandomFloat(&seed1);

		if (path < 1 - shininess)
		{
			vec3 R = reflect(ray->dir, N);
			vec3 P = I + R + (CosineWeightedDiffuseReflection(R) * shininess);
			R = normalize(P - I);

			Ray r = Ray(I, R);
			return GetColor(ray) * Sample(&r, depth + 1, true) * survivePower;
		}
	}
	/// reflection + refraction
	if (ray->hit->material->ior > EPSILON)
	{
		float kr = Fresnel(ray->dir, N, ray->hit->material->ior);
		bool outside = dot(ray->dir, N) < 0;
		vec3 bias = EPSILON * ray->hit->getNormal(I);

		float path = RandomFloat(&seed1);

		if (path < (1 - kr))
		{
			vec3 refractDir = Refract(ray->dir, N, ray->hit->material->ior);
			vec3 refractOrig = outside ? I - bias : I + bias;
			Ray r = Ray(refractOrig, refractDir);
			return GetColor(ray) * SampleEX(&r, depth + 1, true) * survivePower;
		}

		Ray r = Ray(I, reflect(ray->dir, N));
		return GetColor(ray) * SampleEX(&r, depth + 1, true) * survivePower;
	}
#pragma endregion

	vec3 Ld = BLACK;

	// sample a random light source
	int lightIndex = rand() % this->scene->areaLightList.size();
	Primitive* randomLight = this->scene->areaLightList[lightIndex];
	vec3 randomPointOnLight = randomLight->randomPointOnPrimitive(I);

	vec3 L = normalize(randomPointOnLight - I);
	vec3 lightNormal = randomLight->getNormal(randomPointOnLight);
	float dist2 = glm::length2(randomPointOnLight - I);

	Ray lr = Ray(I, L);
	lr.t = length(randomPointOnLight - (I + L));

#pragma region microfacet BRDF
	// microfacet BRDF
	vec3 V = -ray->dir;
	float alpha = .2f;
	vec3 kS = ray->hit->material->specular;
	/// halfway vector
	vec3 H = normalize(V + L);
	/// normal distribution
	/*float D = 0.f;
	if (dot(N, H) > EPSILON)
	{
	D = ((alpha + 2.0f) / (2.0f * PI) * pow(dot(N, H), alpha));
	}*/
	float D_GGX = GGX_Distribution(N, H, alpha);
	/// geometry term
	float Gdenom = dot(V, H);
	//float G1_GGX = GGX_PartialGeometryTerm(V, N, H, alpha);
	//float G2_GGX = GGX_PartialGeometryTerm(L, N, H, alpha);
	float G1 = (2.0f * dot(N, H) * dot(N, V)) / Gdenom;
	float G2 = (2.0f * dot(N, H) * dot(N, L)) / Gdenom;
	float G = glm::min(1.0f, glm::min(G1, G2));
	/// fresnel term
	vec3 F = kS + (vec3(1.0f) - kS) * pow((1.0f - dot(L, H)), 5.0f);
	/// microfacet BRDF
	float denom = 4.0f * dot(N, L) * dot(N, V);
	//vec3 mfBRDF = (D * G * F) / denom;
	vec3 mfBRDF = (D_GGX * G* F) / denom;
#pragma endregion

	if (dot(N, L) > 0 && dot(lightNormal, -L) > 0) if (Trace(&lr, true) != BLACK)
	{
		/*float solidAngle = (dot(lightNormal, -L) * randomLight->calculateArea()) / dist2;
		Ld = (float)this->scene->areaLightList.size() * randomLight->material->diffuse * solidAngle * BRDF * dot(N, L);*/

		float solidAngle = (dot(lightNormal, -L) * randomLight->calculateArea()) / dist2;
		if (solidAngle != 0)
		{
			float lightPDF = 1 / solidAngle;
			Ld = (float)scene->areaLightList.size() * randomLight->material->diffuse * mfBRDF * (dot(N, L) / lightPDF);
		}
	}

	// continue random walk
	vec3 R = CosineWeightedDiffuseReflection(N);
	Ray r = Ray(I, R);

	// update throughput
	float PDF = dot(N, R) * INVPI;
	vec3 Ei = BLACK;
	if (PDF != 0)
	{
		Ei = Sample(&r, depth + 1, false) * dot(N, R) / PDF; // irradiance
	}

#if RR 
	return BRDF * Ei * survivePower + Ld;
#else
	return BRDF * Ei + Ld;
#endif
}

vec3 RayTracer::CosineWeightedDiffuseReflection(vec3 normal)
{
	/// RNG 1
	//float r0 = (float)rand() / RAND_MAX, r1 = (float)rand() / RAND_MAX;
	float r0 = RandomFloat(&seed1), r1 = RandomFloat(&seed2);
	float r = sqrt(r0);
	float theta = 2 * PI * r1;
	float x = r * sine(theta + PI * 0.05); // cosf(theta);
	float y = r * sine(theta); // sinf(theta);

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

vec3 RayTracer::Reflect(vec3 dir, vec3 normal)
{
	return dir - 2 * dot(dir, normal) * normal;
}

vec3 RayTracer::Refract(vec3 dir, vec3 normal, float ior)
{
	float cosi = clamp(-1.0f, 1.0f, dot(normal, dir));
	float etai = 1, etat = ior;
	vec3 n = normal;
	if (cosi < 0) { cosi = -cosi; } else { std::swap(etai, etat); n = -normal; }
	float eta = etai / etat;
	float k = 1 - eta * eta * (1 - cosi * cosi);
	return k < 0 ? BLACK : eta * dir + (eta * cosi - sqrtf(k)) * n;
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

vec3 RayTracer::Fresnel_Schlick(float cosT, vec3 F0)
{
	return F0 + (1.0f - F0) * pow(1.0f - cosT, 5.0f);
}

vec3 RayTracer::GetColor(Ray* ray)
{
	if (ray->hit->material->texture)
	{
		vec2 uv = ray->hit->getTexCoord(ray);
		//return SampleTexturePoint(ray->hit->material->texture, uv);
		return SampleTextureBilinear(ray->hit->material->texture, uv);
	}
	else
	{
		return ray->hit->material->diffuse;
	}
}

vec3 RayTracer::SampleTexturePoint(Surface* texture, vec2 uv)
{
	const vec2 nUV(fmodf(uv.x + 1000.0f, 1.0f), fmodf(uv.y + 1000.0f, 1.0f));
	const int x = (int)(nUV.x * (float)(texture->GetWidth() - 1));
	const int y = texture->GetHeight() - 1 - (int)(nUV.y * (float)(texture->GetHeight() - 1));
	const Pixel color = texture->GetBuffer()[y * texture->GetPitch() + x];
	
	return vec3(
		(float)((color >> 16) & 0xff),
		(float)((color >> 8) & 0xff),
		(float)((color) & 0xff)
	) / 255.0f;
}

vec3 RayTracer::SampleTextureBilinear(Surface* texture, const vec2 uv) {
	const float u = fmodf(uv.x + 1000.0f, 1.0f) * (float)texture->GetWidth() - 0.5f;
	const float v = fmodf(uv.y + 1000.0f, 1.0f) * (float)texture->GetHeight() - 0.5f;
	const int x = (int)u; const int y = (int)v;

	const float uFrac = u - (float)x;
	const float vFrac = v - (float)y;
	const float invUFrac = 1 - uFrac;
	const float invVFrac = 1 - vFrac;

	const float w0 = invUFrac * invUFrac;
	const float w1 = uFrac * invVFrac;
	const float w2 = invVFrac * vFrac;
	const float w3 = 1 - (w0 + w1 + w2);

	const int invY = texture->GetHeight() - 1 - y;
	const int x2 = (x + 1) % texture->GetWidth();
	const int y2 = abs((invY - 1) % texture->GetHeight());
	
	const ivec4 color(
		texture->GetBuffer()[x + invY * texture->GetPitch()],
		texture->GetBuffer()[x2 + invY * texture->GetPitch()],
		texture->GetBuffer()[x + y2 * texture->GetPitch()],
		texture->GetBuffer()[x2 + y2 * texture->GetPitch()]);
	
	return vec3(
		w0 * (float)((color.w >> 16) & 0xff) + w1 * (float)((color.x >> 16) & 0xff) + w2 * (float)((color.y >> 16) & 0xff) + w3 * (float)((color.z >> 16) & 0xff),
		w0 * (float)((color.w >> 8) & 0xff) + w1 * (float)((color.x >> 8) & 0xff) + w2 * (float)((color.y >> 8) & 0xff) + w3 * (float)((color.z >> 8) & 0xff),
		w0 * (float)((color.w) & 0xff) + w1 * (float)((color.x) & 0xff) + w2 * (float)((color.y) & 0xff) + w3 * (float)((color.z) & 0xff)
	) / 255.0f;
}

vec3 RayTracer::SampleSkydome(HDRBitmap* skydome, Ray* ray)
{
	float u = fmodf(0.5f * (1.0f + atan2(ray->dir.x, -ray->dir.z) * INVPI), 1.0f);
	float v = acosf(ray->dir.y) * INVPI;
	int pixel = (int)(u * (float)(skydome->width - 1)) + ((int)(v * (float)(skydome->height - 1)) * skydome->width);
	return skydome->buffer[pixel];
}

void RayTracer::Focus(int x, int y)
{
	Ray ray; scene->camera->GenerateRay(&ray, x, y);

	this->scene->bvh->Traverse(&ray, this->scene->bvh->root);
	if (ray.t != INFINITY)
	{
		scene->camera->focalLength = ray.t / scene->camera->magnification;
		scene->camera->CalculateScreen();
	}
}

float RayTracer::chiGGX(float v)
{
	return v > 0 ? 1 : 0;
}

float RayTracer::GGX_Distribution(vec3 n, vec3 h, float alpha)
{
	float NoH = dot(n, h);
	float alpha2 = alpha * alpha;
	float NoH2 = NoH * NoH;
	float den = NoH2 * alpha2 + (1 - NoH2);
	return (chiGGX(NoH) * alpha2) / (PI * den * den);
}

float RayTracer::GGX_PartialGeometryTerm(vec3 v, vec3 n, vec3 h, float alpha)
{
	float VoH2 = clamp(dot(v, h), 0.0f, 1.0f);
	float chi = chiGGX(VoH2 / clamp(dot(v, n), 0.0f, 1.0f));
	VoH2 = VoH2 * VoH2;
	float tan2 = (1 - VoH2) / VoH2;
	return (chi * 2) / (1 + sqrt(1 + alpha * alpha * tan2));
}