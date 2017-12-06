#pragma once

class Material
{
public:
	enum class Shader { DIFFUSE, MIRROR, GLASS };

	Material(vec3 color, Shader shader)
	{
		this->color = color;
		this->shader = shader;
	}

	vec3 color;
	Shader shader;
};