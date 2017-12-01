#include "template.h"
#include "Scene.h"
#include "Triangle.h"

Scene::Scene()
{
	camera = new Camera();

	lights[0] = new Light(float3(0, 0, 0), float3(100, 100, 100));
	primitives[0] = new Triangle(float3(0, 1, 2), float3(1, 0, 2), float3(-1, 0, 2));
	primitives[0]->material = Material(float3(1, 0, 1), Material::Shader::DIFFUSE);
}