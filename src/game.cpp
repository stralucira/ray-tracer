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
	//Camera* camera = rayTracer->scene->camera;
	bool update = false;

	// rotate camera
	if (a_Key == SDL_SCANCODE_RIGHT)
	{
		rayTracer->scene->camera->dir += rayTracer->scene->camera->right * 0.05f;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_LEFT)
	{
		rayTracer->scene->camera->dir -= rayTracer->scene->camera->right * 0.05f;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_DOWN)
	{
		rayTracer->scene->camera->dir -= rayTracer->scene->camera->up * 0.05f;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_UP)
	{
		rayTracer->scene->camera->dir += rayTracer->scene->camera->up * 0.05f;
		update = true;
	}

	// move camera
	if (a_Key == SDL_SCANCODE_W)
	{
		rayTracer->scene->camera->pos += rayTracer->scene->camera->viewDir * 0.10f;
		update = true;
		
	}
	if (a_Key == SDL_SCANCODE_A)
	{
		rayTracer->scene->camera->pos -= rayTracer->scene->camera->right * 0.10f;
		rayTracer->scene->camera->dir -= rayTracer->scene->camera->right * 0.10f;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_S)
	{
		rayTracer->scene->camera->pos -= rayTracer->scene->camera->viewDir * 0.10f;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_D)
	{
		rayTracer->scene->camera->pos += rayTracer->scene->camera->right * 0.10f;
		rayTracer->scene->camera->dir += rayTracer->scene->camera->right * 0.10f;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_Q)
	{
		rayTracer->scene->camera->pos += rayTracer->scene->camera->up * 0.10f;
		rayTracer->scene->camera->dir += rayTracer->scene->camera->up * 0.10f;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_E)
	{
		rayTracer->scene->camera->pos -= rayTracer->scene->camera->up * 0.10f;
		rayTracer->scene->camera->dir -= rayTracer->scene->camera->up * 0.10f;
		update = true;
	}

	// zoom camera
	if (a_Key == SDL_SCANCODE_EQUALS)
	{
		rayTracer->scene->camera->d += 0.05f;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_MINUS)
	{
		rayTracer->scene->camera->d -= 0.05f;
		update = true;
	}

	// update screen and rays
	if (update)
	{
		rayTracer->scene->camera->CalculateScreen();
		//camera->UpdateRays();
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