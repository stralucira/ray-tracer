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
	case 1: // orignal scene
		pos = vec3(0.0f, 0.0f, -1.0f);
		lookAt = pos + vec3(0.0f, 0.0f, 1.0f);
		camera = new Camera(pos, lookAt);

		MOVEMODIFIER = 0.50f;

		lightList.push_back(new Light(vec3(1.0f, 0.0f, 1.0f), vec3(100.0f, 100.0f, 100.0f)));
		lightList.push_back(new Light(vec3(0.0f, 2.0f, 0.0f), vec3(50.0f, 50.0f, 50.0f)));

		primList.push_back(new Sphere(vec3(0.5f, 0.0f, 3.0f), 0.4f));
		primList.back()->material = Material(vec3(1.0f, 1.0f, 1.0f), Material::Shader::GLASS);
		primList.back()->index = index; index++;

		primList.push_back(new Sphere(vec3(-1.5f, 1.0f, 3.0f), 0.7f));
		primList.back()->material = Material(vec3(0.8f, 0.8f, 0.8f), Material::Shader::MIRROR);
		primList.back()->index = index; index++;

		primList.push_back(new Cylinder(vec3(2.0f, -1.0f, 2.0f), vec3(1.0f, 0.0f, 0.0f), 0.2f, 0.5f));
		primList.back()->material = Material(vec3(0.0f, 0.0f, 1.0f), Material::Shader::DIFFUSE);
		primList.back()->index = index; index++;

		primList.push_back(new Torus(vec3(1.0f, -2.0f, 1.0f), vec3(0.0f, 0.0f, 0.5f), 0.5f, 0.2f));
		primList.back()->material = Material(vec3(0.0f, 1.0f, 0.0f), Material::Shader::GLASS);
		primList.back()->index = index; index++;

		primList.push_back(new Triangle(vec3(-0.1f, -2.0f, 4.0f), vec3(-0.75f, -0.1f, 4.0f), vec3(0.5, -0.5, 3)));
		primList.back()->material = Material(vec3(1.0f, 0.0f, 0.0f), Material::Shader::DIFFUSE);
		primList.back()->index = index; index++;

		//primList.push_back(new Plane(vec3(0, -5, 0), vec3(0, 1, 0))); // bottom plane
		//primList.back()->material = Material(vec3(0.0f, 0.5f, 0.2f), Material::Shader::DIFFUSE);
		//primList.back()->index = index; index++;

		primList.push_back(new Triangle(vec3(-5.0f, -5.0f, 0.0f), vec3(-5.0f, -5.0f, 10.0f), vec3(5.0f, -5.0f, 10.0f)));
		primList.back()->material = Material(vec3(0.0f, 0.5f, 0.2f), Material::Shader::DIFFUSE);
		primList.back()->index = index; index++;

		primList.push_back(new Triangle(vec3(-5.0f, -5.0f, 0.0f), vec3(5.0f, -5.0f, 0.0f), vec3(5.0f, -5.0f, 10.0f), 1));
		primList.back()->material = Material(vec3(0.0f, 0.5f, 0.2f), Material::Shader::DIFFUSE);
		primList.back()->index = index; index++;

		//primList.push_back(new Plane(vec3(0, 5, 0), vec3(0, -1, 0))); // top plane
		//primList.back()->material = Material(vec3(0.8f, 0.8f, 0.8f), Material::Shader::DIFFUSE);
		//primList.back()->index = index; index++;

		primList.push_back(new Triangle(vec3(-5.0f, 5.0f, 0.0f), vec3(-5.0f, 5.0f, 10.0f), vec3(5.0f, 5.0f, 10.0f), 1));
		primList.back()->material = Material(vec3(0.8f, 0.8f, 0.8f), Material::Shader::DIFFUSE);
		primList.back()->index = index; index++;

		primList.push_back(new Triangle(vec3(-5.0f, 5.0f, 0.0f), vec3(5.0f, 5.0f, 0.0f), vec3(5.0f, 5.0f, 10.0f)));
		primList.back()->material = Material(vec3(0.8f, 0.8f, 0.8f), Material::Shader::DIFFUSE);
		primList.back()->index = index; index++;

		//primList.push_back(new Plane(vec3(-5, 0, 0), vec3(1, 0, 0))); // left plane
		//primList.back()->material = Material(vec3(0.95f, 1.0f, 0.95f), Material::Shader::DIFFUSE);
		//primList.back()->index = index; index++;

		primList.push_back(new Triangle(vec3(-5.0f, -5.0f, 0.0f), vec3(-5.0f, -5.0f, 10.0f), vec3(-5.0f, 5.0f, 0.0f), 1));
		primList.back()->material = Material(vec3(0.95f, 1.0f, 0.95f), Material::Shader::MIRROR);
		primList.back()->index = index; index++;

		primList.push_back(new Triangle(vec3(-5.0f, 5.0f, 10.0f), vec3(-5.0f, 5.0f, 0.0f), vec3(-5.0f, -5.0f, 10.0f), 1));
		primList.back()->material = Material(vec3(0.95f, 1.0f, 0.95f), Material::Shader::MIRROR);
		primList.back()->index = index; index++;

		//primList.push_back(new Plane(vec3(5, 0, 0), vec3(-1, 0, 0))); // right plane
		//primList.back()->material = Material(vec3(0.7f, 0.8f, 0.8f), Material::Shader::DIFFUSE);
		//primList.back()->index = index; index++;

		primList.push_back(new Triangle(vec3(5.0f, -5.0f, 0.0f), vec3(5.0f, -5.0f, 10.0f), vec3(5.0f, 5.0f, 0.0f)));
		primList.back()->material = Material(vec3(0.7f, 0.8f, 0.8f), Material::Shader::MIRROR);
		primList.back()->index = index; index++;

		primList.push_back(new Triangle(vec3(5.0f, 5.0f, 10.0f), vec3(5.0f, 5.0f, 0.0f), vec3(5.0f, -5.0f, 10.0f)));
		primList.back()->material = Material(vec3(0.7f, 0.8f, 0.8f), Material::Shader::MIRROR);
		primList.back()->index = index; index++;

		//primList.push_back(new Plane(vec3(0, 0, 10), vec3(0, 0, -1))); // back plane
		//primList.back()->material = Material(vec3(0.2f, 0.7f, 1.0f), Material::Shader::DIFFUSE);
		//primList.back()->index = index; index++;

		primList.push_back(new Triangle(vec3(-5.0f, 5.0f, 10.0f), vec3(-5.0f, -5.0f, 10.0f), vec3(5.0f, -5.0f, 10.0f), 1));
		primList.back()->material = Material(vec3(1.0f, 0.0f, 0.0f), Material::Shader::DIFFUSE);
		primList.back()->index = index; index++;

		primList.push_back(new Triangle(vec3(-5.0f, 5.0f, 10.0f), vec3(5.0f, 5.0f, 10.0f), vec3(5.0f, -5.0f, 10.0f)));
		primList.back()->material = Material(vec3(1.0f, 0.0f, 0.0f), Material::Shader::DIFFUSE);
		primList.back()->index = index; index++;

		this->LoadObject("cube.obj");

		break;
	case 2: // lego han solo scene
		pos = vec3(6.43f, 12.65f, 15.52f);
		lookAt = pos + vec3(-0.51f, -0.14f, -0.85f);
		camera = new Camera(pos, lookAt);

		MOVEMODIFIER = 1.00f;

		lightList.push_back(new Light(vec3(0.50f, 16.77f, 10.85f), vec3(200.0f, 200.0f, 200.0f)));
		lightList.push_back(new Light(vec3(12.37f, 19.72f, 11.91f), vec3(200.0f, 200.0f, 200.0f)));

		this->LoadObject("han.obj");

		break;
	case 3: // x-wing scene
		pos = vec3(1697.0f, 231.0f, -967.0f);
		lookAt = pos + vec3(-0.41f, -0.24f, -0.88f);
		camera = new Camera(pos, lookAt);

		MOVEMODIFIER = 10.00f;

		lightList.push_back(new Light(vec3(1409.98f, 107.04f, -1637.97f), vec3(200.0f, 200.0f, 200.0f)));
		lightList.push_back(new Light(vec3(1467.80f, 111.66f, -1881.94f), vec3(1000.0f, 100.0f, 100.0f)));
		lightList.push_back(new Light(vec3(1468.24f, 42.31f, -1891.45f), vec3(1000.0f, 100.0f, 100.0f)));
		lightList.push_back(new Light(vec3(1351.85f, 42.45f, -1887.67f), vec3(1000.0f, 100.0f, 100.0f)));
		lightList.push_back(new Light(vec3(1351.50f, 112.44f, -1888.60f), vec3(1000.0f, 100.0f, 100.0f)));
		lightList.push_back(new Light(vec3(1411.23f, 134.46f, -1694.33f), vec3(1000.0f, 1000.0f, 1000.0f)));

		this->LoadObject("x-wing.obj");

		break;
	case 4: // tie fighter scene
		pos = vec3(88.94f, 47.57f, -175.54f);
		lookAt = pos + vec3(0.55f, -0.38f, -0.74f);
		camera = new Camera(pos, lookAt);

		MOVEMODIFIER = 10.00f;

		lightList.push_back(new Light(vec3(143.12f, 11.31f, -271.44f), vec3(200.0f, 200.0f, 200.0f)));
		lightList.push_back(new Light(vec3(149.62f, -12.83f, -274.64f), vec3(0.0f, 200.0f, 0.0f)));
		lightList.push_back(new Light(vec3(132.69f, 0.15f, -313.19f), vec3(100.0f, 0.0f, 0.0f)));
		lightList.push_back(new Light(vec3(157.25f, -0.39f, -312.82f), vec3(100.0f, 0.0f, 0.0f)));


		this->LoadObject("TIE-fighter.obj");

		break;
	case 5: // millennium falcon scene
		pos = vec3(0, 1600, -6000);
		lookAt = pos + vec3(0.0f, -0.48f, 0.88f);
		camera = new Camera(pos, lookAt);

		MOVEMODIFIER = 10.00f;

		lightList.push_back(new Light(vec3(180.0f, 0.0f, -2850.0f), vec3(200.0f, 200.0f, 200.0f)));

		this->LoadObject("millenium-falcon.obj");

		break;
	case 0:
		pos = vec3(0.0f, 0.0f, -1.0f);
		lookAt = pos + vec3(0.0f, 0.0f, 1.0f);
		camera = new Camera(pos, lookAt);

		MOVEMODIFIER = 0.10f;

		lightList.push_back(new Light(vec3(0.0f, 0.0f, 0.0f), vec3(100.0f, 100.0f, 100.0f)));

		//this->LoadObject("han.obj");
		this->LoadObject("f-16.obj");
		this->LoadObject("cube.obj");

		break;
	}

	// BVH helpers	
	printf("Constructing BVH for %i polygons...\n", primList.size());

	// Static scene BVH builder
	float lastftime = 0; auto timer = Timer();
	bvh = new BVH(&primList);
	lastftime = timer.elapsed(); timer.reset();
	printf("-----------------------\n BVH constructed in %.3f seconds\n-----------------------\n", lastftime);
#if ENABLETOPBVH
	// Dynamic scene BVH builder
	lastftime = 0;
	bvhTop = new BVHTop(&bvhList);
	lastftime = timer.elapsed();
	printf("-----------------------\n Top level BVH constructed in %.3f seconds\n-----------------------\n", lastftime);
#endif
}

// wavefront .obj file loader
void Scene::LoadObject(std::string inputfile)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

#if ENABLETOPBVH
	std::vector<Primitive*> primLoadList;
#endif //ENABLETOPBVH

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
			//vec2 texcoord;

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
#if ENABLETOPBVH // Experimental top BVH construction helper
			primLoadList.push_back(new Triangle(vertices[0], vertices[1], vertices[2], normal));
			primLoadList.back()->index = index;

			if (materials.size() > 0)
			{
				primLoadList.back()->material = Material(
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
					primLoadList.back()->material.shader = Material::Shader::GLASS;
				}
				else
				{
					primLoadList.back()->material.shader = Material::Shader::DIFFUSE;
				}
			}
		}
	}
	// Top level BVH loader
	objectList.push_back(primLoadList);

	float lastftime = 0; auto timer = Timer();
	bvhList.push_back(new BVH(&objectList.back()));
	lastftime = timer.elapsed();

	printf("-----------------------\n BVH constructed in %.3f seconds\n-----------------------\n", lastftime);
	bvhList.back()->bounds = CalculateObjectBounds(primLoadList);
	bvhList.back()->centroid = CalculateObjectCentroid(bvhList.back()->bounds);
#else
		}
	}
#endif // ENABLETOPBVH
	printf("-----------------------\n Done loading polygons \n-----------------------\n");
}

AABB Scene::CalculateObjectBounds(std::vector<Primitive*> primList)
{
	float maxX = -INFINITY; float minX = INFINITY;
	float maxY = -INFINITY; float minY = INFINITY;
	float maxZ = -INFINITY; float minZ = INFINITY;

	for (size_t i = 0; i < primList.size(); i++)
	{
		if (primList[i]->bounds->max.x > maxX) { maxX = primList[i]->bounds->max.x; }
		if (primList[i]->bounds->max.y > maxY) { maxY = primList[i]->bounds->max.y; }
		if (primList[i]->bounds->max.z > maxZ) { maxZ = primList[i]->bounds->max.z; }

		if (primList[i]->bounds->min.x < minX) { minX = primList[i]->bounds->min.x; }
		if (primList[i]->bounds->min.y < minY) { minY = primList[i]->bounds->min.y; }
		if (primList[i]->bounds->min.z < minZ) { minZ = primList[i]->bounds->min.z; }
	}
	return AABB(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));
}

vec3 Scene::CalculateObjectCentroid(AABB bounds)
{
	return ((bounds.max - bounds.min) * 0.5f);
}