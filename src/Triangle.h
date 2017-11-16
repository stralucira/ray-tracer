#include "Primitive.h"
/// <summary>
/// Defines a Triangle object.
/// </summary>
/// <seealso cref="Primitive" />
class Triangle : public Primitive
{
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="Triangle"/> class.
	/// The min and max bounds are both (0,0,0) and the points a,b,c are set to
	/// (0, 1, 0), (1, 0, 0), (-1, 0, 0) respectively. Material is not set.
	/// </summary>
	Triangle();

	Triangle(float3 a, float3 b, float3 c, Material mat);
	~Triangle();
	//float intersects(Ray r) override;
	//Ray getNormal(float3 point) override;

private:
	float3 a;
	float3 b;
	float3 c;
	float3 normal;
};