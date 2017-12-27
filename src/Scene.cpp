#include "template.h"
#include "Scene.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"
#include "Cylinder.h"
#include "Torus.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

// every primitive should go here
Scene::Scene(int scene_id)
{
	primList.clear();
	lightList.clear();

	switch (scene_id)
	{
	case 1:
		pos = vec3(0.0f, 0.0f, -1.0f);
		lookAt = vec3(0.0f, 0.0f, 1.0f);
		camera = new Camera(pos, lookAt);

		lightList.push_back(new Light(vec3(1.0f, 0.0f, 1.0f), vec3(100.0f, 100.0f, 100.0f)));
		lightList.push_back(new Light(vec3(0.0f, 2.0f, 0.0f), vec3(50.0f, 50.0f, 50.0f)));

		primList.push_back(new Sphere(vec3(0.5f, 0.0f, 3.0f), 0.4f));
		primList.back()->material = Material(vec3(1.0f, 1.0f, 1.0f), Material::Shader::DIFFUSE);
		primList.back()->index = index; index++;

		primList.push_back(new Sphere(vec3(-1.5f, 1.0f, 3.0f), 0.7f));
		primList.back()->material = Material(vec3(0.8f, 0.8f, 0.8f), Material::Shader::GLASS);
		primList.back()->index = index; index++;

		primList.push_back(new Cylinder(vec3(2.0f, -1.0f, 2.0f), vec3(1.0f, 0.0f, 0.0f), 0.2f, 0.5f));
		primList.back()->material = Material(vec3(0.0f, 0.0f, 1.0f), Material::Shader::DIFFUSE);
		primList.back()->index = index; index++;

		primList.push_back(new Torus(vec3(1.0f, -2.0f, 1.0f), vec3(0.0f, 0.0f, 0.5f), 0.5f, 0.2f));
		primList.back()->material = Material(vec3(0.0f, 1.0f, 0.0f), Material::Shader::DIFFUSE);
		primList.back()->index = index; index++;

		primList.push_back(new Triangle(vec3(-0.1f, -2.0f, 4.0f), vec3(-0.75f, -0.1f, 4.0f), vec3(0.5, -0.5, 3)));
		primList.back()->material = Material(vec3(1.0f, 0.0f, 0.0f), Material::Shader::DIFFUSE);
		primList.back()->index = index; index++;

		primList.push_back(new Plane(vec3(0, -5, 5), vec3(0, 1, 0))); // bottom plane
		primList.back()->material = Material(vec3(0.0f, 0.5f, 0.2f), Material::Shader::DIFFUSE);
		primList.back()->index = index; index++;

		primList.push_back(new Plane(vec3(0, 5, 5), vec3(0, -1, 0))); // top plane
		primList.back()->material = Material(vec3(0.8f, 0.8f, 0.8f), Material::Shader::DIFFUSE);
		primList.back()->index = index; index++;

		primList.push_back(new Plane(vec3(-5, 0, 5), vec3(1, 0, 0))); // left plane
		primList.back()->material = Material(vec3(0.95f, 1.0f, 0.95f), Material::Shader::DIFFUSE);
		primList.back()->index = index; index++;

		primList.push_back(new Plane(vec3(5, 0, 5), vec3(-1, 0, 0))); // right plane
		primList.back()->material = Material(vec3(0.7f, 0.8f, 0.8f), Material::Shader::DIFFUSE);
		primList.back()->index = index; index++;

		primList.push_back(new Plane(vec3(0, 0, 10), vec3(0, 0, -1))); // back plane
		primList.back()->material = Material(vec3(0.2f, 0.7f, 1.0f), Material::Shader::DIFFUSE);
		primList.back()->index = index; index++;

		this->LoadObject("cube.obj");

		break;
	case 2:
		pos = vec3(0.0f, 0.0f, 1.0f);
		lookAt = vec3(0.0f, 0.0f, -1.0f);
		camera = new Camera(pos, lookAt);

		lightList.push_back(new Light(vec3(3.0f, -3.0f, -5.0f), vec3(100.0f, 100.0f, 100.0f)));
		lightList.push_back(new Light(vec3(150.0f, 0.0f, -270.0f), vec3(1000.0f, 1000.0f, 1000.0f)));

		this->LoadObject("cube.obj");
		this->LoadObject("f-16.obj");

		break;
	}

	// BVH helpers	
	printf("Constructing BVH for %i polygons...\n", primList.size());

	// Static scene BVH builder
	float lastftime = 0;
	auto timer = Timer();	
	bvh = new BVH(&primList);
	lastftime = timer.elapsed();

	// Dynamic scene BVH builder
	bvhTop = new BVHTop(&bvhList);

	printf("-----------------------\n Done constructing BVH in %.2f seconds\n-----------------------\n", lastftime);
}

// wavefront .obj file loader
void Scene::LoadObject(std::string inputfile)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector<Primitive*> primLoadList;

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
			vec2 texcoord;

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
				//float tx = attrib.texcoords[2 * idx.texcoord_index + 0];
				//float ty = attrib.texcoords[2 * idx.texcoord_index + 1];
				// Optional: vertex colors
				//tinyobj::real_t red = attrib.colors[3 * idx.vertex_index + 0];
				//tinyobj::real_t green = attrib.colors[3 * idx.vertex_index + 1];
				//tinyobj::real_t blue = attrib.colors[3 * idx.vertex_index + 2];

				vertices[v] = vec3(vx, vy, vz);
				normal = vec3(nx, ny, nz);
				//texcoord = vec2(tx, ty);
			}
			index_offset += fv;

			// per-face material
			// shapes[s].mesh.material_ids[f];

			primList.push_back(new Triangle(vertices[0], vertices[1], vertices[2], normal));
			primList.back()->index = index;

			primLoadList.push_back(new Triangle(vertices[0], vertices[1], vertices[2], normal));
			primLoadList.back()->index = index;

			if (materials.size() > 0)
			{ 
				primList.back()->material = Material(
					vec3(
						materials[shapes[s].mesh.material_ids[f]].diffuse[0],	// Kd red
						materials[shapes[s].mesh.material_ids[f]].diffuse[1],	// Kd green
						materials[shapes[s].mesh.material_ids[f]].diffuse[2]),	// Kd blue
					vec3(
						materials[shapes[s].mesh.material_ids[f]].specular[0],	// Ks red
						materials[shapes[s].mesh.material_ids[f]].specular[0],	// Ks green
						materials[shapes[s].mesh.material_ids[f]].specular[0]),	// Kd blue
					materials[shapes[s].mesh.material_ids[f]].shininess,		// Ns
					materials[shapes[s].mesh.material_ids[f]].dissolve			//d
				);

				if (materials[shapes[s].mesh.material_ids[f]].dissolve < 1.0f)
				{
					primList.back()->material.shader = Material::Shader::GLASS;
				}
				else
				{
					primList.back()->material.shader = Material::Shader::DIFFUSE;
				}
			}
			//primList.back()->material = Material(vec3(1.0f, 1.0f, 1.0f), Material::Shader::DIFFUSE);
			//printf("Loading polygon %i \n", primList.size());
		}
	}

	// Top level BVH loader
	bvhList.push_back(new BVH(&primLoadList));
	bvhList.back()->boundingBox = CalculateObjectBounds(primLoadList);
	bvhList.back()->centroid = CalculateObjectCentroid(bvhList.back()->boundingBox);
	bvhList.back()->index = index;
	index++;

	printf("-----------------------\n Done loading polygons \n-----------------------\n");
}

AABB* Scene::CalculateObjectBounds(std::vector<Primitive*> primList)
{
	float maxX = -INFINITY;
	float maxY = -INFINITY;
	float maxZ = -INFINITY;

	float minX = INFINITY;
	float minY = INFINITY;
	float minZ = INFINITY;

	for (size_t i = 0; i < primList.size(); i++)
	{
		if (primList[i]->boundingBox->max.x > maxX) { maxX = primList[i]->boundingBox->max.x; }
		if (primList[i]->boundingBox->max.y > maxY) { maxY = primList[i]->boundingBox->max.y; }
		if (primList[i]->boundingBox->max.z > maxZ) { maxZ = primList[i]->boundingBox->max.z; }

		if (primList[i]->boundingBox->min.x < minX) { minX = primList[i]->boundingBox->min.x; }
		if (primList[i]->boundingBox->min.y < minY) { minY = primList[i]->boundingBox->min.y; }
		if (primList[i]->boundingBox->min.z < minZ) { minZ = primList[i]->boundingBox->min.z; }
	}
	return new AABB(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));
}

vec3 Scene::CalculateObjectCentroid(AABB* bounds)
{
	return ((bounds->max - bounds->min) * 0.5f);
}