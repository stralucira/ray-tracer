#pragma once
#include "template.h"

class Material
{
public:
	enum class Shader { DIFFUSE, MIRROR, GLASS };

	Material() : texture(nullptr){}

	Material(vec3 diffuseColor)
	{
		this->diffuse = diffuseColor;
		this->texture = nullptr;
	}

	Material(Shader shader)
	{
		this->shader = shader;
		this->texture = nullptr;
	}

	Material(vec3 diffuseColor, Shader shader)
	{
		this->diffuse = diffuseColor;
		this->shader = shader;
		this->texture = nullptr;
	}

	Material(vec3 diffuseColor, vec3 specularColor, float shininess, Shader shader)
	{
		this->diffuse = diffuseColor;
		this->specular = specularColor;
		this->shininess = shininess;
		this->shader = shader;
		this->texture = nullptr;
	}

	Material(vec3 diffuseColor, vec3 specularColor, float shininess, float dissolve)
	{
		this->diffuse = diffuseColor;
		this->specular = specularColor;
		this->shininess = shininess;
		this->dissolve = dissolve;
		this->texture = nullptr;
	}
	
	vec3 ambient = BLACK;		// Ka
	vec3 diffuse = BLACK;		// Kd
	vec3 specular = BLACK;		// Ks
	vec3 transmittance = BLACK;	// Kt, Tf
	vec3 emission = BLACK;		// Ke
	float shininess = 0.f;		// Ns
	float ior = 1.f;			// Ni (index of refraction)
	float dissolve = 1.f;		// d, Tr
	float roughness = 1.f;
	int illum = 0;				// illum model

	Surface* texture;

	Shader shader;
};