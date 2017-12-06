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
	bool update = false;

	// rotate camera
	if (a_Key == SDL_SCANCODE_RIGHT)
	{
		camera->dir += camera->right * 0.05f;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_LEFT)
	{
		camera->dir -= camera->right * 0.05f;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_DOWN)
	{
		camera->dir -= camera->up * 0.05f;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_UP)
	{
		camera->dir += camera->up * 0.05f;
		update = true;
	}

	// move camera
	if (a_Key == SDL_SCANCODE_W)
	{
		camera->pos += camera->viewDir * 0.10f;
		update = true;
		
	}
	if (a_Key == SDL_SCANCODE_A)
	{
		camera->pos -= camera->right * 0.10f;
		camera->dir -= camera->right * 0.10f;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_S)
	{
		camera->pos -= camera->viewDir * 0.10f;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_D)
	{
		camera->pos += camera->right * 0.10f;
		camera->dir += camera->right * 0.10f;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_Q)
	{
		camera->pos += camera->up * 0.10f;
		camera->dir += camera->up * 0.10f;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_E)
	{
		camera->pos -= camera->up * 0.10f;
		camera->dir -= camera->up * 0.10f;
		update = true;
	}

	// zoom camera
	if (a_Key == SDL_SCANCODE_EQUALS)
	{
		camera->d += 0.05f;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_MINUS)
	{
		camera->d -= 0.05f;
		update = true;
	}

	// update screen and rays
	if (update)
	{
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