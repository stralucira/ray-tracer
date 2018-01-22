#include "template.h"
#include "Torus.h"

bool Torus::intersect(Ray* ray)
{
	vec3 rayOriginPosition = ray->orig;
	vec3 rayDirection = ray->dir;

	vec3 centerToRayOrigin = rayOriginPosition - this->centroid;
	const double centerToRayOriginDotDirection = dot(rayDirection, centerToRayOrigin);
	double centerToRayOriginDotDirectionSquared = dot(centerToRayOrigin, centerToRayOrigin);
	double r2 = r * r;
	double R2 = R * R;

	double axisDotCenterToRayOrigin = dot(axis, centerToRayOrigin);
	double axisDotRayDirection = dot(axis, rayDirection);
	double a = 1 - axisDotRayDirection * axisDotRayDirection;
	double b = 2 * (dot(centerToRayOrigin, rayDirection) - axisDotCenterToRayOrigin * axisDotRayDirection);
	double c = centerToRayOriginDotDirectionSquared - axisDotCenterToRayOrigin * axisDotCenterToRayOrigin;
	double d = centerToRayOriginDotDirectionSquared + R2 - r2;

	// Solve quartic equation with coefficients A, B, C, D and E
	double A = 1;
	double B = 4 * centerToRayOriginDotDirection;
	double C = 2 * d + B * B * 0.25f - 4 * R2 * a;
	double D = B * d - 4 * R2 * b;
	double E = d * d - 4 * R2 * c;

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
	double closestRoot = INFINITY;
	for (int idx = 0; idx < maxRootsCount; ++idx)
	{
		double root = roots[idx];
		if (root > EPSILON && root < closestRoot)
		{
			closestRoot = root;
		}
	}

	ray->t = (float)closestRoot;
	return true;
}

vec2 Torus::getTexCoord(Ray* ray)
{
	return vec2(0.0f);
}

bool Torus::getIsLight()
{
	return this->isLight;
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
	return new AABB(this->centroid - R - r, this->centroid + R + r);
}

float Torus::calculateArea()
{
	return (2.0f * PI * R) * (2.0f * PI * r);
}

vec3 Torus::randomPointOnPrimitive(vec3 point)
{
	return point;
}