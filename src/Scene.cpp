#include "template.h"
#include "Scene.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"
#include "tiny_obj_loader.h"

// every primitive should go here
Scene::Scene()
{
	camera = new Camera();

	//lights[0] = new Light(vec3(0, 0, 0), vec3(100, 100, 100));
	//primitives[0] = new Triangle(vec3(0, 1, 2), vec3(1, 0, 2), vec3(-1, 0, 2));
	//primitives[0]->material = Material(vec3(1, 0, 1), Material::Shader::DIFFUSE);

	lights[0] = new Light(vec3(-1.0f, 0.0f, 1.0f), vec3(100.0f, 100.0f, 100.0f));
	lights[1] = new Light(vec3(0.0f,-2.0f, 0.0f), vec3(50.0f, 50.0f, 50.0f));

	primitives[0] = new Sphere(vec3(-0.5f, 0.0f, 3.0f), 1.0f);
	primitives[0]->material = Material(vec3(0.0f, 1.0f, 0.0f), Material::Shader::DIFFUSE);

	primitives[1] = new Sphere(vec3(1.5f, 0.5f, 3.0f), 0.7f);
	primitives[1]->material = Material(vec3(1.0f, 1.0f, 1.0f), Material::Shader::MIRROR);

	primitives[2] = new Triangle(vec3(0,-1.5,2.5), vec3(1.5,-1.5,2.5), vec3(0.75,-1.5,3));
	primitives[2]->material = Material(vec3(0.0f, 0.0f, 1.0f), Material::Shader::DIFFUSE);

	primitives[3] = new Plane(vec3(0, -5, 5), vec3(0, 1, 0));
	primitives[3]->material = Material(vec3(0.8f, 0.8f, 0.8f), Material::Shader::DIFFUSE);
	
	primitives[4] = new Plane(vec3(-5, 0, 5), vec3(1, 0, 0));
	primitives[4]->material = Material(vec3(0.8f, 0.8f, 0.8f), Material::Shader::DIFFUSE);

	primitives[5] = new Plane(vec3(5, 0, 5), vec3(-1, 0, 0));
	primitives[5]->material = Material(vec3(0.7f, 0.8f, 0.8f), Material::Shader::DIFFUSE);

	primitives[6] = new Plane(vec3(0, 5, 5), vec3(0, -1, 0));
	primitives[6]->material = Material(vec3(0.8f, 0.8f, 0.8f), Material::Shader::DIFFUSE);

	primitives[7] = new Plane(vec3(0, 0, 10), vec3(0, 0, -1));
	primitives[7]->material = Material(vec3(0.8f, 0.8f, 0.8f), Material::Shader::DIFFUSE);

}
