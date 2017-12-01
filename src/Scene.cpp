#include "template.h"
#include "Scene.h"
#include "Triangle.h"
#include "Sphere.h"

Scene::Scene()
{
	camera = new Camera();

	/*
	lights[0] = new Light(float3(0, 0, 0), float3(100, 100, 100));
	primitives[0] = new Triangle(float3(0, 1, 2), float3(1, 0, 2), float3(-1, 0, 2));
	primitives[0]->material = Material(float3(1, 0, 1), Material::Shader::DIFFUSE);
	*/

	lights[0] = new Light(float3(0, 0, 1), float3(100, 100, 100));
	lights[1] = new Light(float3(-2, 0, 0), float3(50, 50, 50));

	primitives[0] = new Sphere(float3(-1, 0, 5), 1.0f);
	primitives[0]->material = Material(float3(0, 1, 0), Material::Shader::DIFFUSE);

	primitives[1] = new Sphere(float3(1.5, 0, 5), 0.7f);
	primitives[1]->material = Material(float3(1, 1, 1), Material::Shader::MIRROR);

	primitives[2] = new Triangle(float3(-1, 0, 8), float3(-1, 2, 5), float3(1, 0, 8));
	primitives[2]->material = Material(float3(0, 0, 1), Material::Shader::DIFFUSE);
}