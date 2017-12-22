#pragma once

class Material
{
public:
	enum class Shader { DIFFUSE, MIRROR, GLASS };

	Material(vec3 diffuseColor, Shader shader)
	{
		this->Kd = diffuseColor;
		this->shader = shader;
	}

	Material(vec3 diffuseColor, vec3 specularColor)
	{
		this->Kd = diffuseColor;
		this->Ks = specularColor;
	}

	Material(Shader shader)
	{
		this->shader = shader;
	}

	vec3 Kd = GRAY;
	vec3 Ks = GRAY;
	Shader shader;
};