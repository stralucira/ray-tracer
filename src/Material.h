#pragma once

class Material
{
public:
	enum class Shader { DIFFUSE, MIRROR, GLASS };

	Material() : texture(nullptr){}

	Material(vec3 diffuseColor, Shader shader)
	{
		this->diffuse = diffuseColor;
		this->shader = shader;
	}

	Material(vec3 diffuseColor, vec3 specularColor, float shininess, Shader shader)
	{
		this->diffuse = diffuseColor;
		this->specular = specularColor;
		this->shininess = shininess;
		this->shader = shader;
	}

	Material(vec3 diffuseColor, vec3 specularColor, float shininess, float dissolve)
	{
		this->diffuse = diffuseColor;
		this->specular = specularColor;
		this->shininess = shininess;
		this->dissolve = dissolve;
	}

	Material(Shader shader)
	{
		this->shader = shader;
	}
	
	vec3 ambient = BLACK;	// Ka
	vec3 diffuse = BLACK;	// Kd
	vec3 specular = BLACK;	// Ks
	vec3 transmittance;		// Kt, Tf
	vec3 emission;			// Ke
	float shininess = 0;	// Ns
	float ior;				// Ni (index of refraction)
	float dissolve;			// d, Tr
	int illum;				// illum model

	Surface* texture;

	Shader shader;
};