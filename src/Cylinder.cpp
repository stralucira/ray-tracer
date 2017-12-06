#include "template.h"
#include "Cylinder.h"

bool Cylinder::intersect(Ray* ray)
{
	vector<float> points;
	vec3 alpha = up * dot(ray->dir, up);
	vec3 deltaP = (ray->orig - pos);
	vec3 beta = up * dot(deltaP, up);
	vec3 pos2 = pos + up * height;

	float a = length2(ray->dir - alpha);
	float b = 2 * dot((ray->dir - alpha), (deltaP - beta));
	float c = length2(deltaP - beta) - radius * radius;

	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) return false;
	else
	{
		discriminant = sqrt(discriminant);
		float t1 = ((-1 * b) + discriminant) / (2 * a);
		float t2 = ((-1 * b) - discriminant) / (2 * a);
		if (t1 >= 0)
		{
			if (dot(up, ((ray->orig - pos) + ray->dir * t1)) > 0 && dot(up, ((ray->orig - pos2) + ray->dir * t1)) < 0)
			{
				points.push_back(t1);
			}
		}
		if (t2 >= 0)
		{
			if (dot(up, ((ray->orig - pos) + ray->dir * t2)) > 0 && dot(up, ((ray->orig - pos2) + ray->dir * t2)) < 0)
			{
				points.push_back(t2);
			}
		}
	}

	float denom = dot(ray->dir, up);
	if (denom > 1e-6)
	{
		vec3 co = pos - ray->orig;
		float t3 = dot(co, up) / denom;
		if (t3 > 0 && length2(ray->dir * t3 - co) <= radius * radius)
		{
			points.push_back(t3);
		}
	}
	else if (denom < 1e-6)
	{
		vec3 co2 = pos2 - ray->orig;
		float t4 = dot(co2, up) / denom;
		if (t4 > 0 && length2(ray->dir * t4 - co2) <= radius * radius)
		{
			points.push_back(t4);
		}
	}

	float minT = INFINITY;
	bool intersect = false;
	for (size_t i = 0; i < points.size(); i++)
	{
		if (minT > points[i] && points[i] >= 0.0001f)
		{
			minT = points[i];
			intersect = true;
		}
	}
	if (intersect)
	{
		ray->t = minT / length(ray->dir);
		return true;
	}
	else
	{
		return false;
	}
}

vec3 Cylinder::getNormal(vec3 point)
{
	vec3 co = point - pos;
	vec3 co2 = co - up * height;
	if (length2(co) <= radius * radius)
	{
		return up;
	}
	if (length2(co2) <= radius * radius)
	{
		return up;
	}
	vec3 normal = co - (up * (dot(co, up) / length2(up)));

	return normalize(normal);
}
