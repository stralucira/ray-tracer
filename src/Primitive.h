#pragma once
#include "Ray.h"
#include "Material.h"

class Primitive
{
public:
	Primitive(vec3 pos);
	void Init(vec3 pos);
	void UpdatePosition();
	vec3 GetPosition();
	vec3 GetDirectionVector();

	// default material color and shader
	Material material = Material(vec3(1.0f, 0.0f, 0.0f), Material::Shader::DIFFUSE);

	virtual bool intersect(Ray* ray) = 0;
	virtual vec3 getNormal(vec3 point) = 0;

	Primitive(glm::mat4 transformMatrix);
	void Init(glm::mat4 transformMatrix);

	glm::mat4 transformMatrix;
	
	vec3 pos;
	vec3 scale;
	vec3 skew;
	vec4 perspective;
	quat rot;
};
