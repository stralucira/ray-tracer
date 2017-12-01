#pragma once

class Material
{
public:
	enum class Shader { DIFFUSE, MIRROR, GLASS };
	float3 color;
	Shader shader;

	Material(float3 color, Shader shader)
	{
		this->color = color;
		this->shader = shader;
	}
	Material();
};
