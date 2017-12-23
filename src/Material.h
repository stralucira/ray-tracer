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

	Material(vec3 diffuseColor, vec3 specularColor, float shininess, Shader shader)
	{
		this->Kd = diffuseColor;
		this->Ks = specularColor;
		this->Ns = shininess;
		this->shader = shader;
	}

	Material(Shader shader)
	{
		this->shader = shader;
	}

	vec3 Ka = BLACK; // Ambient color
	vec3 Kd = BLACK; // Diffuse color
	vec3 Ks = BLACK; // Specular color
	float Ns = 0;
	Shader shader;
};