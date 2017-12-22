#pragma once

class Material
{
public:
	enum class Shader { DIFFUSE, MIRROR, GLASS };

	Material(vec3 diffuseColor, Shader shader)
	{
		this->Kd = diffuseColor;
		this->Ks = vec3(0, 0, 0);
		this->shader = shader;
	}

	vec3 Kd, Ks;
	Shader shader;
};