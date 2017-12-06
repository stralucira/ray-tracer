#include "template.h"
#include "Scene.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

// every primitive should go here
Scene::Scene()
{
	camera = new Camera();

    /*
	lightList.push_back(new Light(vec3(0.0f, 0.0f, 0.0f), vec3(100.0f, 100.0f, 100.0f)));
	
	primList.push_back(new Triangle(vec3(0, 1, 2), vec3(1, 0, 2), vec3(-1, 0, 2)));
	primList.back()->material = Material(vec3(1, 0, 1), Material::Shader::DIFFUSE);
     */

	lightList.push_back(new Light(vec3(-1.0f, 0.0f, 1.0f), vec3(100.0f, 100.0f, 100.0f)));
	lightList.push_back(new Light(vec3(0.0f,-2.0f, 0.0f), vec3(50.0f, 50.0f, 50.0f)));

	primList.push_back(new Sphere(vec3(-0.5f, 0.0f, 3.0f), 0.3f));
	primList.back()->material = Material(vec3(0.0f, 1.0f, 0.0f), Material::Shader::DIFFUSE);

	primList.push_back(new Sphere(vec3(1.5f, -1.0f, 3.0f), 0.7f));
	primList.back()->material = Material(vec3(0.8f, 0.8f, 0.8f), Material::Shader::MIRROR);

	primList.push_back(new Triangle(vec3(0.1f, 2.0f, 4.0f), vec3(0.75f, 0.1f, 4.0f), vec3(-0.5, 0.5, 3)));
	primList.back()->material = Material(vec3(1.0f, 0.0f, 0.0f), Material::Shader::DIFFUSE);

	primList.push_back(new Plane(vec3(0, -5, 5), vec3(0, 1, 0))); // top
	primList.back()->material = Material(vec3(0.0f, 0.5f, 0.7f), Material::Shader::DIFFUSE);

	primList.push_back(new Plane(vec3(0, 5, 5), vec3(0, -1, 0))); // bottom
	primList.back()->material = Material(vec3(0.0f, 0.5f, 0.2f), Material::Shader::DIFFUSE);
	
	primList.push_back(new Plane(vec3(-5, 0, 5), vec3(1, 0, 0))); // left
	primList.back()->material = Material(vec3(0.95f, 1.0f, 0.95f), Material::Shader::MIRROR);

	primList.push_back(new Plane(vec3(5, 0, 5), vec3(-1, 0, 0))); // right
	primList.back()->material = Material(vec3(0.7f, 0.8f, 0.8f), Material::Shader::DIFFUSE);

	primList.push_back(new Plane(vec3(0, 0, 10), vec3(0, 0, -1))); // back
	primList.back()->material = Material(vec3(0.8f, 0.4f, 0.2f), Material::Shader::DIFFUSE);

    /*
	lightList.push_back(new Light(vec3(10, 0, 1), vec3(100, 100, 100)));

	primList.push_back(new Sphere(vec3(0.0f, 0.0f, 0.5f), 1.0f));
	primList.back()->material = Material(vec3(0.0f, 0.0f, 1.0f), Material::Shader::DIFFUSE);

	primList.push_back(new Sphere(vec3(0.0f, 0.0f, 10.0f), 4.0f));
	primList.back()->material = Material(vec3(1.0f, 0.0f, 0.0f), Material::Shader::DIFFUSE);

	primList.push_back(new Triangle(vec3(4, 4, 5), vec3(1, 1, 5), vec3(2, 5, 5)));
	primList.back()->material = Material(vec3(0.0f, 1.0f, 0.0f), Material::Shader::DIFFUSE);

	primList.push_back(new Plane(vec3(0, 0, 15), vec3(0, 0, 1)));
	primList.back()->material = Material(vec3(0.75f, 0.8f, 0.7f), Material::Shader::DIFFUSE);
     */
	
	lightList.push_back(new Light(vec3(-1.0f, 0.0f, 1.0f), vec3(100.0f, 100.0f, 100.0f)));

	std::string inputfile = "cube.obj";
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());

	if (!err.empty()) // `err` may contain warning message.
	{ 
		std::cerr << err << std::endl;
	}

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++)
	{
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			int fv = shapes[s].mesh.num_face_vertices[f];

			vec3 vertices[3];
			vec3 normal;

			// Loop over vertices in the face.
			for (int v = 0; v < fv; v++)
			{
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				float vx = attrib.vertices[3 * idx.vertex_index + 0];
				float vy = attrib.vertices[3 * idx.vertex_index + 1];
				float vz = attrib.vertices[3 * idx.vertex_index + 2];
				float nx = attrib.normals[3 * idx.normal_index + 0];
				float ny = attrib.normals[3 * idx.normal_index + 1];
				float nz = attrib.normals[3 * idx.normal_index + 2];
				//tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
				//tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
				// Optional: vertex colors
				// tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
				// tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
				// tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];

				vertices[v] = vec3(vx, vy, vz);
				normal += vec3(nx, ny, nz);
			}
			index_offset += fv;
			
			// per-face material
			//shapes[s].mesh.material_ids[f];

			primList.push_back(new Triangle(vertices[0], vertices[1], vertices[2]));
			primList.back()->material = Material(vec3(1.0f, 1.0f, 1.0f), Material::Shader::DIFFUSE);
		}
	}
}
