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

	float rotSpeed = 0.05f;
	float movSpeed = 0.1f;

	if (a_Key == SDL_SCANCODE_RIGHT)
	{
		transform = rotate(transform, rotSpeed, camera->rotY);
		camera->TransCamera(transform);
		camera->transMatrix = prevTransform;

	}
	if (a_Key == SDL_SCANCODE_LEFT)
	{
		transform = rotate(transform, -rotSpeed, camera->rotY);
		camera->TransCamera(transform);
		camera->transMatrix = prevTransform;
	}
	if (a_Key == SDL_SCANCODE_DOWN)
	{
		transform = rotate(transform, -rotSpeed, camera->rotX);
		camera->TransCamera(transform);
		camera->transMatrix = prevTransform;
	}
	if (a_Key == SDL_SCANCODE_UP)
	{
		transform = rotate(transform, rotSpeed, camera->rotX);
		camera->TransCamera(transform);
		camera->transMatrix = prevTransform;
	}

	if (a_Key == SDL_SCANCODE_W)
	{
		camera->pos.z += movSpeed;
		camera->CalculateScreen();
		camera->UpdateRays();
	}
	if (a_Key == SDL_SCANCODE_A)
	{
		camera->pos.x -= movSpeed;
		camera->CalculateScreen();
		camera->UpdateRays();
	}
	if (a_Key == SDL_SCANCODE_S)
	{
		camera->pos.z -= movSpeed;
		camera->CalculateScreen();
		camera->UpdateRays();
	}
	if (a_Key == SDL_SCANCODE_D)
	{
		camera->pos.x += movSpeed;
		camera->CalculateScreen();
		camera->UpdateRays();
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