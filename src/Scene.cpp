#include "template.h"
#include "Scene.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"
#include "Cylinder.h"
#include "Torus.h"

#define EXPERIMENTAL 0

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader_exp.h"
#include "tiny_obj_loader.h"

// every primitive should go here
Scene::Scene()
{
	camera = new Camera();

	// static scene primitives
	//lightList.push_back(new Light(vec3(1.0f, 0.0f, 1.0f), vec3(100.0f, 100.0f, 100.0f)));
	//lightList.push_back(new Light(vec3(0.0f,2.0f, 0.0f), vec3(50.0f, 50.0f, 50.0f)));

	//lightList.push_back(new Light(vec3(0, 20, 5), vec3(100.0f, 100.0f, 100.0f)));
	//lightList.push_back(new Light(vec3(150, 20, 300), vec3(100.0f, 100.0f, 100.0f)));
	lightList.push_back(new Light(vec3(3.0f, -3.0f, -5.0f), vec3(100.0f, 100.0f, 100.0f)));
	//lightList.push_back(new Light(vec3(0.0f, 500.0f, 0.0f), vec3(100.0f, 100.0f, 100.0f)));

	//primList.push_back(new Sphere(vec3(0.5f, 0.0f, 3.0f), 0.4f));
	//primList.back()->material = Material(vec3(0.0f, 1.0f, 0.0f), Material::Shader::GLASS);

	//primList.push_back(new Sphere(vec3(-1.5f, 1.0f, 3.0f), 0.7f));
	//primList.back()->material = Material(vec3(0.8f, 0.8f, 0.8f), Material::Shader::MIRROR);

	//primList.push_back(new Cylinder(vec3(2.0f, -1.0f, 2.0f), vec3(1.0f, 0.0f, 0.0f), 0.2f, 0.5f));
	//primList.back()->material = Material(vec3(0.0f, 0.0f, 1.0f), Material::Shader::DIFFUSE);
	
	//primList.push_back(new Torus(vec3(1.0f, -2.0f, 1.0f), vec3(0.0f, 0.0f, 0.5f), 0.5f, 0.2f));
	//primList.back()->material = Material(vec3(0.0f, 1.0f, 0.0f), Material::Shader::DIFFUSE);

	//primList.push_back(new Triangle(vec3(-0.1f, -2.0f, 4.0f), vec3(-0.75f, -0.1f, 4.0f), vec3(0.5, -0.5, 3)));
	//primList.back()->material = Material(vec3(1.0f, 0.0f, 0.0f), Material::Shader::DIFFUSE);

	//primList.push_back(new Plane(vec3(0, -5, 5), vec3(0, 1, 0))); // bottom plane
	//primList.back()->material = Material(vec3(0.0f, 0.5f, 0.2f), Material::Shader::DIFFUSE);

	//primList.push_back(new Plane(vec3(0, 5, 5), vec3(0, -1, 0))); // top plane
	//primList.back()->material = Material(vec3(0.8f, 0.8f, 0.8f), Material::Shader::DIFFUSE);

	//primList.push_back(new Plane(vec3(-5, 0, 5), vec3(1, 0, 0))); // left plane
	//primList.back()->material = Material(vec3(0.95f, 1.0f, 0.95f), Material::Shader::DIFFUSE);

	//primList.push_back(new Plane(vec3(5, 0, 5), vec3(-1, 0, 0))); // right plane
	//primList.back()->material = Material(vec3(0.7f, 0.8f, 0.8f), Material::Shader::DIFFUSE);

	//primList.push_back(new Plane(vec3(0, 0, 10), vec3(0, 0, -1))); // back plane
	//primList.back()->material = Material(vec3(0.2f, 0.7f, 1.0f), Material::Shader::DIFFUSE);

#if EXPERIMENTAL
	// wavefront .obj file loader experimental
	std::string inputfile = "f-16.obj";
	std::vector<tinyobjexp::shape_t> shapes;
	std::vector<tinyobjexp::material_t> materials;

	std::vector<uvec3> faces;
	std::vector<vec3> normals;
	std::vector<vec3> vertices;

	std::string err = tinyobjexp::LoadObj(shapes, materials, inputfile.c_str());

	if (!err.empty()) {
		std::cerr << err << std::endl;
		exit(1);
	}

	std::cout << "# of shapes    : " << shapes.size() << std::endl;
	std::cout << "# of materials : " << materials.size() << std::endl;

	for (size_t i = 0; i < shapes.size(); i++)
	{
		printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
		printf("Size of shape[%ld].indices: %ld\n", i, shapes[i].mesh.indices.size());
		printf("Size of shape[%ld].material_ids: %ld\n", i, shapes[i].mesh.material_ids.size());
		assert((shapes[i].mesh.indices.size() % 3) == 0);
		
		for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++)
		{
			printf("  idx[%ld] = %d, %d, %d. mat_id = %d\n", f, shapes[i].mesh.indices[3 * f + 0], shapes[i].mesh.indices[3 * f + 1], shapes[i].mesh.indices[3 * f + 2], shapes[i].mesh.material_ids[f]);
			
			faces.push_back(vec3(shapes[i].mesh.indices[3 * f + 0], shapes[i].mesh.indices[3 * f + 1], shapes[i].mesh.indices[3 * f + 2]));
		}

		printf("shape[%ld].vertices: %ld\n", i, shapes[i].mesh.positions.size());
		assert((shapes[i].mesh.positions.size() % 3) == 0);
		for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++)
		{
			printf("  v[%ld] = (%f, %f, %f)\n", v, shapes[i].mesh.positions[3 * v + 0], shapes[i].mesh.positions[3 * v + 1], shapes[i].mesh.positions[3 * v + 2]);
			
			vertices.push_back(vec3(shapes[i].mesh.positions[3 * v + 0], shapes[i].mesh.positions[3 * v + 1], shapes[i].mesh.positions[3 * v + 2]));
			//normals.push_back(vec3(shapes[i].mesh.normals[3 * v + 0], shapes[i].mesh.normals[3 * v + 1], shapes[i].mesh.normals[3 * v + 2]));
		}

		for (size_t s = 0; s < faces.size(); s++)
		{
			primList.push_back(new Triangle(vertices[faces[s].x], vertices[faces[s].y], vertices[faces[s].z]));
		}

		faces.clear();
		vertices.clear();

		printf("");
	}

	for (size_t i = 0; i < materials.size(); i++) {
		printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
		printf("  material.Ka = (%f, %f ,%f)\n", materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
		printf("  material.Kd = (%f, %f ,%f)\n", materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
		printf("  material.Ks = (%f, %f ,%f)\n", materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
		printf("  material.Tr = (%f, %f ,%f)\n", materials[i].transmittance[0], materials[i].transmittance[1], materials[i].transmittance[2]);
		printf("  material.Ke = (%f, %f ,%f)\n", materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]);
		printf("  material.Ns = %f\n", materials[i].shininess);
		printf("  material.Ni = %f\n", materials[i].ior);
		printf("  material.dissolve = %f\n", materials[i].dissolve);
		printf("  material.illum = %d\n", materials[i].illum);
		printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
		printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
		printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
		printf("  material.map_Ns = %s\n", materials[i].normal_texname.c_str());
		std::map<std::string, std::string>::const_iterator it(materials[i].unknown_parameter.begin());
		std::map<std::string, std::string>::const_iterator itEnd(materials[i].unknown_parameter.end());
		for (; it != itEnd; it++) {
			printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
		}
		printf("\n");
	}
#else
	// wavefront .obj file loader
	std::string inputfile = "han.obj";
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
			//vec3 normal;
			//vec2 texcoord;

			// Loop over vertices in the face.
			for (int v = 0; v < fv; v++)
			{

				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				float vx = attrib.vertices[3 * idx.vertex_index + 0];
				float vy = attrib.vertices[3 * idx.vertex_index + 1];
				float vz = attrib.vertices[3 * idx.vertex_index + 2];
				//float nx = attrib.normals[3 * idx.normal_index + 0];
				//float ny = attrib.normals[3 * idx.normal_index + 1];
				//float nz = attrib.normals[3 * idx.normal_index + 2];
				//float tx = attrib.texcoords[2 * idx.texcoord_index + 0];
				//float ty = attrib.texcoords[2 * idx.texcoord_index + 1];
				// Optional: vertex colors
				//tinyobj::real_t red = attrib.colors[3 * idx.vertex_index + 0];
				//tinyobj::real_t green = attrib.colors[3 * idx.vertex_index + 1];
				//tinyobj::real_t blue = attrib.colors[3 * idx.vertex_index + 2];

				vertices[v] = vec3(vx, vy, vz);
				//normal = vec3(nx, ny, nz);
				//texcoord = vec2(tx, ty);
			}
			index_offset += fv;
			
			// per-face material
			// shapes[s].mesh.material_ids[f];
			
			primList.push_back(new Triangle(vertices[0], vertices[1], vertices[2]));

			if (materials.size() > 0)
			{
				primList.back()->material = Material(vec3(
					materials[shapes[s].mesh.material_ids[f]].diffuse[0],
					materials[shapes[s].mesh.material_ids[f]].diffuse[1],
					materials[shapes[s].mesh.material_ids[f]].diffuse[2]),
					Material::Shader::DIFFUSE);
			}
			
			//primList.back()->material = Material(vec3(1.0f, 1.0f, 1.0f), Material::Shader::DIFFUSE);
			printf("Loading triangle %i \n", primList.size());
		}
	}
	printf("----------------------\nDone loading triangles \n----------------------\n");
#endif

	// BVH helpers
	sceneBounds = this->CalculateSceneBounds();
	bvh = new BVH(&primList);
	printf("----------------------\nDone\n----------------------\n");
}

AABB* Scene::CalculateSceneBounds()
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