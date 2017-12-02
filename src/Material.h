#pragma once

class Material
{
public:
	enum class Shader { DIFFUSE, MIRROR, GLASS };
	vec3 color;
	Shader shader;

	Material(vec3 color, Shader shader)
	{
		this->color = color;
		this->shader = shader;
	}
};