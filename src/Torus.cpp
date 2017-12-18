#include "template.h"
#include "Torus.h"

bool Torus::intersect(Ray* ray)
{
	vec3 rayOriginPosition = ray->orig;
	vec3 rayDirection = ray->dir;

	vec3 centerToRayOrigin = rayOriginPosition - this->centroid;
	const float centerToRayOriginDotDirection = dot(rayDirection, centerToRayOrigin);
	float centerToRayOriginDotDirectionSquared = dot(centerToRayOrigin, centerToRayOrigin);
	float r2 = r * r;
	float R2 = R * R;

	float axisDotCenterToRayOrigin = dot(axis, centerToRayOrigin);
	float axisDotRayDirection = dot(axis, rayDirection);
	float a = 1 - axisDotRayDirection * axisDotRayDirection;
	float b = 2 * (dot(centerToRayOrigin, rayDirection) - axisDotCenterToRayOrigin * axisDotRayDirection);
	float c = centerToRayOriginDotDirectionSquared - axisDotCenterToRayOrigin * axisDotCenterToRayOrigin;
	float d = centerToRayOriginDotDirectionSquared + R2 - r2;

	// Solve quartic equation with coefficients A, B, C, D and E
	float A = 1;
	float B = 4 * centerToRayOriginDotDirection;
	float C = 2 * d + B * B * 0.25f - 4 * R2 * a;
	float D = B * d - 4 * R2 * b;
	float E = d * d - 4 * R2 * c;

	// Maximum number of roots is 4
	QuarticEquation equation(A, B, C, D, E);
	const int maxRootsCount = 4;
	double roots[maxRootsCount] = { -1.0, -1.0, -1.0, -1.0 };
	int rootsCount = equation.Solve(roots);

	if (rootsCount == 0)
	{
		return false;
	}

	// Find closest to zero positive solution
	float closestRoot = INFINITY;
	for (int idx = 0; idx < maxRootsCount; ++idx)
	{
		float root = (float)roots[idx];
		if (root > 0.001 && root < closestRoot)
		{
			closestRoot = root;
		}
	}

	ray->t = closestRoot;
	return true;
}

vec3 Torus::getNormal(vec3 point)
{
	vec3 centerToPoint = point - this->centroid;
	float centerToPointDotAxis = dot(axis, centerToPoint);
	vec3 direction = normalize(centerToPoint - axis * centerToPointDotAxis);
	vec3 normal = normalize(point - this->centroid + direction * R);
	return normal;
}

AABB* Torus::calculateAABB()
{
	return new AABB(this->centroid - R, this->centroid + R);
}