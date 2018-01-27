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

	areaLightList.clear();

	switch (scene_id)
	{
	case 1: // orignal scene
		pos = vec3(0.f, 0.f, -4.f);
		lookAt = pos + vec3(0.0f, 0.0f, 1.0f);
		camera = new Camera(pos, lookAt);

		MOVEMODIFIER = 0.50f;

		skydome = new HDRBitmap("space.hdr");

		primList.push_back(new Sphere(vec3(0.f, 0.f, -10.f), .1f, true));
		primList.back()->material = new Material(vec3(20.f, 0.f, 0.f), Material::Shader::DIFFUSE);
		areaLightList.push_back(primList.back());

		primList.push_back(new Sphere(vec3(2.f, 0.f, -10.f), .1f, true));
		primList.back()->material = new Material(vec3(0.f, 20.f, 0.f), Material::Shader::DIFFUSE);
		areaLightList.push_back(primList.back());

		primList.push_back(new Sphere(vec3(0.f, 2.f, -10.f), .1f, true));
		primList.back()->material = new Material(vec3(0.f, 0.f, 20.f), Material::Shader::DIFFUSE);
		areaLightList.push_back(primList.back());

		primList.push_back(new Sphere(vec3(0.f, 0.f, -12.f), .1f, true));
		primList.back()->material = new Material(vec3(20.f, 20.f, 0.f), Material::Shader::DIFFUSE);
		areaLightList.push_back(primList.back());

		primList.push_back(new Sphere(vec3(2.f, 2.f, -10.f), .1f, true));
		primList.back()->material = new Material(vec3(20.f, 0.f, 20.f), Material::Shader::DIFFUSE);
		areaLightList.push_back(primList.back());

		lightList.push_back(new Light(vec3(1.0f, 0.0f, 1.0f), vec3(100.0f, 100.0f, 100.0f)));
		lightList.push_back(new Light(vec3(0.0f, 2.0f, 0.0f), vec3(50.0f, 50.0f, 50.0f)));

		primList.push_back(new Sphere(vec3(0.5f, 0.0f, 3.0f), 0.4f));
		primList.back()->material = new Material(vec3(0.0f, 1.0f, 0.0f), Material::Shader::GLASS);
		primList.back()->material->ior = 1.52f;

		primList.push_back(new Sphere(vec3(-1.5f, 1.0f, 3.0f), 0.7f));
		primList.back()->material = new Material(vec3(0.8f, 0.8f, 0.8f), Material::Shader::MIRROR);

		primList.push_back(new Cylinder(vec3(2.0f, -1.0f, 2.0f), vec3(1.0f, 0.0f, 0.0f), 0.2f, 0.5f));
		primList.back()->material = new Material(vec3(0.0f, 0.0f, 1.0f), Material::Shader::DIFFUSE);

		primList.push_back(new Torus(vec3(1.0f, -2.0f, 1.0f), vec3(0.0f, 0.0f, 0.5f), 0.5f, 0.2f));
		primList.back()->material = new Material(vec3(0.f, 1.0f, 0.0f), Material::Shader::DIFFUSE);

		primList.push_back(new Triangle(vec3(-0.1f, -2.0f, 4.0f), vec3(-0.75f, -0.1f, 4.0f), vec3(0.5, -0.5, 3)));
		primList.back()->material = new Material(vec3(1.0f, 0.0f, 0.0f), Material::Shader::DIFFUSE);

		//primList.push_back(new Plane(vec3(0, -5, 0), vec3(0, 1, 0))); // bottom plane
		//primList.back()->material = Material(vec3(0.0f, 0.5f, 0.2f), Material::Shader::DIFFUSE);

		primList.push_back(new Triangle(vec3(-5.0f, -5.0f, 0.0f), vec3(-5.0f, -5.0f, 10.0f), vec3(5.0f, -5.0f, 10.0f)));
		primList.back()->material = new Material(vec3(0.0f, 0.5f, 0.2f), Material::Shader::DIFFUSE);
		primList.push_back(new Triangle(vec3(-5.0f, -5.0f, 0.0f), vec3(5.0f, -5.0f, 0.0f), vec3(5.0f, -5.0f, 10.0f), 1));
		primList.back()->material = new Material(vec3(0.0f, 0.5f, 0.2f), Material::Shader::DIFFUSE);

		/// top plane
		//primList.push_back(new Plane(vec3(0, 5, 0), vec3(0, -1, 0))); 
		//primList.back()->material = Material(vec3(0.8f, 0.8f, 0.8f), Material::Shader::DIFFUSE);

		primList.push_back(new Triangle(vec3(-5.0f, 5.0f, 0.0f), vec3(-5.0f, 5.0f, 10.0f), vec3(5.0f, 5.0f, 10.0f), 1, true));
		//primList.push_back(new Triangle(vec3(-5.0f, 5.0f, 0.0f), vec3(-5.0f, 5.0f, 10.0f), vec3(5.0f, 5.0f, 10.0f), 1));
		primList.back()->material = new Material(vec3(1.f), Material::Shader::DIFFUSE);
		areaLightList.push_back(primList.back());

		primList.push_back(new Triangle(vec3(-5.0f, 5.0f, 0.0f), vec3(5.0f, 5.0f, 0.0f), vec3(5.0f, 5.0f, 10.0f), 0, true));
		//primList.push_back(new Triangle(vec3(-5.0f, 5.0f, 0.0f), vec3(5.0f, 5.0f, 0.0f), vec3(5.0f, 5.0f, 10.0f), 0));
		primList.back()->material = new Material(vec3(1.f), Material::Shader::DIFFUSE);
		areaLightList.push_back(primList.back());

		/// left plane
		//primList.push_back(new Plane(vec3(-5, 0, 0), vec3(1, 0, 0))); 
		//primList.back()->material = Material(vec3(0.95f, 1.0f, 0.95f), Material::Shader::MIRROR);

		primList.push_back(new Triangle(vec3(-5.0f, -5.0f, 0.0f), vec3(-5.0f, -5.0f, 10.0f), vec3(-5.0f, 5.0f, 0.0f), 1));
		primList.back()->material = new Material(vec3(0.95f, 1.0f, 0.95f), Material::Shader::MIRROR);
		primList.push_back(new Triangle(vec3(-5.0f, 5.0f, 10.0f), vec3(-5.0f, 5.0f, 0.0f), vec3(-5.0f, -5.0f, 10.0f), 1));
		primList.back()->material = new Material(vec3(0.95f, 1.0f, 0.95f), Material::Shader::MIRROR);

		/// right plane
		//primList.push_back(new Plane(vec3(5, 0, 0), vec3(-1, 0, 0)));
		//primList.back()->material = Material(vec3(0.7f, 0.8f, 0.8f), Material::Shader::MIRROR);

		primList.push_back(new Triangle(vec3(5.0f, -5.0f, 0.0f), vec3(5.0f, -5.0f, 10.0f), vec3(5.0f, 5.0f, 0.0f)));
		primList.back()->material = new Material(vec3(0.7f, 0.8f, 0.8f), Material::Shader::MIRROR);
		primList.push_back(new Triangle(vec3(5.0f, 5.0f, 10.0f), vec3(5.0f, 5.0f, 0.0f), vec3(5.0f, -5.0f, 10.0f)));
		primList.back()->material = new Material(vec3(0.7f, 0.8f, 0.8f), Material::Shader::MIRROR);

		/// back plane
		//primList.push_back(new Plane(vec3(0, 0, 10), vec3(0, 0, -1)));
		//primList.back()->material = Material(vec3(0.2f, 0.7f, 1.0f), Material::Shader::DIFFUSE);

		primList.push_back(new Triangle(vec3(-5.0f, 5.0f, 10.0f), vec3(-5.0f, -5.0f, 10.0f), vec3(5.0f, -5.0f, 10.0f), 1));
		primList.back()->material = new Material(vec3(0.2f, 0.7f, 1.0f), Material::Shader::DIFFUSE);
		primList.push_back(new Triangle(vec3(-5.0f, 5.0f, 10.0f), vec3(5.0f, 5.0f, 10.0f), vec3(5.0f, -5.0f, 10.0f)));
		primList.back()->material = new Material(vec3(0.2f, 0.7f, 1.0f), Material::Shader::DIFFUSE);

		this->LoadObject("cube.obj", true);

		break;
	case 2: // lego han solo scene
		pos = vec3(6.43f, 12.65f, 15.52f);
		lookAt = pos + vec3(-0.51f, -0.14f, -0.85f);
		camera = new Camera(pos, lookAt);

		MOVEMODIFIER = 1.00f;

		skydome = new HDRBitmap("space.hdr");

		primList.push_back(new Triangle(vec3(-5.0f, 5.0f, 0.0f), vec3(-5.0f, 5.0f, 10.0f), vec3(5.0f, 5.0f, 10.0f), 0, true));
		//primList.push_back(new Triangle(vec3(-5.0f, 5.0f, 0.0f), vec3(-5.0f, 5.0f, 10.0f), vec3(5.0f, 5.0f, 10.0f), 1));
		primList.back()->material = new Material(vec3(1.f), Material::Shader::DIFFUSE);
		areaLightList.push_back(primList.back());

		lightList.push_back(new Light(vec3(0.50f, 16.77f, 10.85f), vec3(200.0f, 200.0f, 200.0f)));
		lightList.push_back(new Light(vec3(12.37f, 19.72f, 11.91f), vec3(200.0f, 200.0f, 200.0f)));

		this->LoadObject("han.obj");

		break;
	case 3: // x-wing scene
		pos = vec3(1697.0f, 231.0f, -967.0f);
		lookAt = pos + vec3(-0.41f, -0.24f, -0.88f);
		camera = new Camera(pos, lookAt);

		MOVEMODIFIER = 10.00f;

		skydome = new HDRBitmap("equi.hdr");

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

		skydome = new HDRBitmap("space.hdr");

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

		skydome = new HDRBitmap("space.hdr");

		lightList.push_back(new Light(vec3(180.0f, 0.0f, -2850.0f), vec3(200.0f, 200.0f, 200.0f)));

		this->LoadObject("millenium-falcon.obj");

		break;
	case 0:
		pos = vec3(0.0f, 2.0f, -1.0f);
		lookAt = pos + vec3(0.0f, 0.0f, 1.0f);
		focalLength = 7.43219f;
		apertureSize = 0.53288f;
		camera = new Camera(pos, lookAt, focalLength, apertureSize);

		MOVEMODIFIER = 0.10f;

		skydome = new HDRBitmap("equi.hdr");

		lightList.push_back(new Light(vec3(0.0f, 4.7f, 7.45f), vec3(50.0f, 50.0f, 50.0f)));

		primList.push_back(new Triangle(vec3(-1.5, 4.9, 5), vec3(-1.5, 4.9, 9.9), vec3(1.5, 4.9, 5), 1, true));
		primList.back()->material = new Material(vec3(5.0f), Material::Shader::DIFFUSE);
		areaLightList.push_back(primList.back());
		primList.push_back(new Triangle(vec3(-1.5, 4.9, 9.9), vec3(1.5, 4.9, 9.9), vec3(1.5, 4.9, 5), 1, true));
		primList.back()->material = new Material(vec3(5.0f), Material::Shader::DIFFUSE);
		areaLightList.push_back(primList.back());

		primList.push_back(new Sphere(vec3(-2.0f, 2.0f, 7.0f), 1.0f));
		primList.back()->material = new Material(vec3(0.9f, 0.6f, 0.6f), Material::Shader::GLASS);
		primList.back()->material->ior = 1.52f;

		primList.push_back(new Sphere(vec3(-2.0f, 0.0f, 4.0f), 1.0f));
		primList.back()->material = new Material(vec3(0.1f, 0.3f, 0.9f), Material::Shader::MIRROR);

		primList.push_back(new Sphere(vec3(0.5f, 0.0f, 9.0f), 1.0f));
		primList.back()->material = new Material(vec3(0.3f, 0.9f, 0.3f), Material::Shader::DIFFUSE);

		primList.push_back(new Triangle(vec3(-3, 5, -5), vec3(-3, 5, 10), vec3(3, 5, -5), 1));
		primList.back()->material = new Material(vec3(0.7), Material::Shader::DIFFUSE);		
		primList.push_back(new Triangle(vec3(3, 5, -5), vec3(-3, 5, 10), vec3(3, 5, 10), 1));
		primList.back()->material = new Material(vec3(0.7), Material::Shader::DIFFUSE);

		primList.push_back(new Triangle(vec3(-3, 5, 10), vec3(-3, 5, -5), vec3(-3, -3, -5), 1));
		primList.back()->material = new Material(vec3(0.7), Material::Shader::DIFFUSE);
		primList.push_back(new Triangle(vec3(-3, 5, 10), vec3(-3, -3, -5), vec3(-3, -3, 10), 1));
		primList.back()->material = new Material(vec3(0.7), Material::Shader::DIFFUSE);

		primList.push_back(new Triangle(vec3(3, 5, -5), vec3(3, 5, 10), vec3(3, -3, -5), 1));
		primList.back()->material = new Material(vec3(0.7), Material::Shader::DIFFUSE);
		primList.push_back(new Triangle(vec3(3, -3, -5), vec3(3, 5, 10), vec3(3, -3, 10), 1));
		primList.back()->material = new Material(vec3(0.7), Material::Shader::DIFFUSE);

		primList.push_back(new Triangle(vec3(-3, -3, 10), vec3(-3, -3, -5), vec3(3, -3, -5), 1));
		primList.back()->material = new Material(vec3(0.7), Material::Shader::DIFFUSE);
		primList.push_back(new Triangle(vec3(3, -3, 10), vec3(-3, -3, 10), vec3(3, -3, -5), 1));
		primList.back()->material = new Material(vec3(0.7), Material::Shader::DIFFUSE);

		primList.push_back(new Triangle(vec3(-3, 5, 10), vec3(-3, -3, 10), vec3(3, -3, 10), 1));
		primList.back()->material = new Material(vec3(0.7), Material::Shader::DIFFUSE);
		primList.push_back(new Triangle(vec3(3, 5, 10), vec3(-3, 5, 10), vec3(3, -3, 10), 1));
		primList.back()->material = new Material(vec3(0.7), Material::Shader::DIFFUSE);

		this->LoadObject("cube.obj", true);

		break;
	}

	// BVH helpers	
	printf("----------------------------------------------\nConstructing static BVH for %i polygons...\n", primList.size());

	// Static scene BVH builder
	float lastftime = 0; auto timer = Timer();
	bvh = new BVH(&primList);
	lastftime = timer.elapsed(); timer.reset();
	printf("BVH constructed in %.3f seconds\n----------------------------------------------\n", lastftime);
#if ENABLETOPBVH
	// Dynamic scene BVH builder
	for (size_t i = 0; i < objectList.size(); i++)
	{
		printf("Constructing object %i BVH for %i polygons...\n", i + 1, objectList[i].size());
		lastftime = 0;
		bvhList.push_back(new BVH(&objectList[i]));
		lastftime = timer.elapsed(); timer.reset();
		bvhList[i]->bounds = CalculateObjectBounds(objectList[i]);
		bvhList[i]->centroid = CalculateObjectCentroid(bvhList[i]->bounds);
		printf("BVH for object %i constructed in %.3f seconds\n----------------------------------------------\n", i + 1, lastftime);
	}
	lastftime = 0;
	bvhTop = new BVHTop(&bvhList);
	lastftime = timer.elapsed();
	printf("Top level BVH constructed in %.3f seconds\n----------------------------------------------\n", lastftime);
#endif
}

// wavefront .obj file loader
void Scene::LoadObject(std::string inputfile, bool simple)
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

	// Loop over materials
	for (size_t i = 0; i < materials.size(); i++)
	{
		matList.push_back(new Material());
		matList.back()->diffuse = vec3(materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
		matList.back()->specular = vec3(materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
		matList.back()->shininess = materials[i].shininess;
		matList.back()->dissolve = materials[i].dissolve;
		
		if (materials[i].dissolve < 1.0f)
		{
			matList.back()->shader = Material::Shader::GLASS;
			matList.back()->ior = 1.0;
		}
		else
		{
			matList.back()->shader = Material::Shader::DIFFUSE;
		}
		if (!materials[i].diffuse_texname.empty())
		{
			auto lastSlash = materials[i].diffuse_texname.find_last_of('/');
			std::string texturePath;
			if (lastSlash != std::string::npos) texturePath = materials[i].diffuse_texname.substr(0, lastSlash + 1);
			else texturePath = materials[i].diffuse_texname;
			matList.back()->texture = new Surface(const_cast<char*>(materials[i].diffuse_texname.c_str()));
		}
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
			vec3 normals[3];

			if (!simple)
			{
				vec2 texcoord[3];
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
					float tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					float ty = attrib.texcoords[2 * idx.texcoord_index + 1];
					// Optional: vertex colors
					//tinyobj::real_t red = attrib.colors[3 * idx.vertex_index + 0];
					//tinyobj::real_t green = attrib.colors[3 * idx.vertex_index + 1];
					//tinyobj::real_t blue = attrib.colors[3 * idx.vertex_index + 2];

					vertices[v] = vec3(vx, vy, vz);
					normals[v] = vec3(nx, ny, nz);
					texcoord[v] = vec2(tx, ty);
				}
				index_offset += fv;

				// per-face material
				// shapes[s].mesh.material_ids[f];

				primList.push_back(new Triangle(vertices[0], vertices[1], vertices[2],normals[0], normals[1], normals[2],texcoord[0], texcoord[1], texcoord[2]));
				if (shapes[s].mesh.material_ids[f] < 0) primList.back()->material = new Material(GRAY);
				else primList.back()->material = matList[shapes[s].mesh.material_ids[f]];
			}
			else // simple obj loader
			{
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

					vertices[v] = vec3(vx, vy, vz);
					normals[v] = vec3(nx, ny, nz);
		}
				index_offset += fv;

				primList.push_back(new Triangle(vertices[0], vertices[1], vertices[2], normals[0], normals[1], normals[2]));
				if (shapes[s].mesh.material_ids[f] < 0) primList.back()->material = new Material(GRAY);
				else primList.back()->material = matList[shapes[s].mesh.material_ids[f]];
			}
#if ENABLETOPBVH // Experimental top BVH construction helper
			primLoadList.push_back(new Triangle(vertices[0], vertices[1], vertices[2], normal));
			primLoadList.back()->index = index;

			primList.back()->material = matList[shapes[s].mesh.material_ids[f]];
		}
	}
	// Top level BVH loader
	objectList.push_back(primLoadList);
#else
		}
	}
#endif // ENABLETOPBVH
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