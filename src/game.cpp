#include "template.h"
#include "Scene.h"
#include "RayTracer.h"

RayTracer* rayTracer;

// -----------------------------------------------------------
// Initialize the game
// -----------------------------------------------------------Zz

void Game::Init()
{    
	Scene* testScene = new Scene();
	rayTracer = new RayTracer(testScene, screen);
}

void Game::Shutdown() { }
// -----------------------------------------------------------
// Input handling
// -----------------------------------------------------------
void Game::HandleInput( float dt ) { }


void Game::KeyDown(int a_Key)
{
	Camera* camera = rayTracer->scene->camera;
	glm::mat4 transform = camera->transMatrix;
	glm::mat4 prevTransform = transform;

	float rotationSpeed = 0.05f;

	if (a_Key == 79)
	{
		transform = rotate(transform, rotationSpeed, camera->rotY);
		camera->TransCamera(transform);
		camera->transMatrix = prevTransform;

	}
	if (a_Key == 80)
	{
		transform = rotate(transform, -rotationSpeed, camera->rotY);
		camera->TransCamera(transform);
		camera->transMatrix = prevTransform;
	}
	if (a_Key == 81)
	{
		transform = rotate(transform, -rotationSpeed, camera->rotX);
		camera->TransCamera(transform);
		camera->transMatrix = prevTransform;
	}
	if (a_Key == 82)
	{
		transform = rotate(transform, rotationSpeed, camera->rotX);
		camera->TransCamera(transform);
		camera->transMatrix = prevTransform;
	}
}

// -----------------------------------------------------------
// Main game tick function
// -----------------------------------------------------------
void Game::Tick( float dt )
{
	rayTracer->Render();

	char buffer[500];
	sprintf(buffer, "FPS: %f \n", 1 / dt);
	screen->Print(buffer, 2, 2, 0xffffff);	
}