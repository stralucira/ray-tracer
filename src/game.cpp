#include "template.h"
#include "Scene.h"
#include "RayTracer.h"

#define ROTATEMODIFIER 0.05f
#define MOVEMODIFIER 0.10f



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
	switch (a_Key)
	{
	case SDL_SCANCODE_W:
		rayTracer->scene->camera->Axial(MOVEMODIFIER);
		break;
	case SDL_SCANCODE_A:
		rayTracer->scene->camera->Horizontal(-MOVEMODIFIER);
		break;
	case SDL_SCANCODE_S:
		rayTracer->scene->camera->Axial(-MOVEMODIFIER);
		break;
	case SDL_SCANCODE_D:
		rayTracer->scene->camera->Horizontal(MOVEMODIFIER);
		break;
	}

	/*
	//Camera* camera = rayTracer->scene->camera;
	bool update = false;

	// rotate camera
	if (a_Key == SDL_SCANCODE_RIGHT)
	{
		rayTracer->scene->camera->dir += rayTracer->scene->camera->right * ROTATEMODIFIER;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_LEFT)
	{
		rayTracer->scene->camera->dir -= rayTracer->scene->camera->right * ROTATEMODIFIER;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_DOWN)
	{
		rayTracer->scene->camera->dir -= rayTracer->scene->camera->up * ROTATEMODIFIER;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_UP)
	{
		rayTracer->scene->camera->dir += rayTracer->scene->camera->up * ROTATEMODIFIER;
		update = true;
	}

	// move camera
	if (a_Key == SDL_SCANCODE_W)
	{
		rayTracer->scene->camera->pos += rayTracer->scene->camera->dir * MOVEMODIFIER;
		update = true;
		
	}
	if (a_Key == SDL_SCANCODE_A)
	{
		rayTracer->scene->camera->pos -= rayTracer->scene->camera->right * MOVEMODIFIER;
		rayTracer->scene->camera->dir -= rayTracer->scene->camera->right * MOVEMODIFIER;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_S)
	{
		rayTracer->scene->camera->pos -= rayTracer->scene->camera->dir * MOVEMODIFIER;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_D)
	{
		rayTracer->scene->camera->pos += rayTracer->scene->camera->right * MOVEMODIFIER;
		rayTracer->scene->camera->dir += rayTracer->scene->camera->right * MOVEMODIFIER;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_Q)
	{
		rayTracer->scene->camera->pos += rayTracer->scene->camera->up * MOVEMODIFIER;
		rayTracer->scene->camera->dir += rayTracer->scene->camera->up * MOVEMODIFIER;
		update = true;
	}
	if (a_Key == SDL_SCANCODE_E)
	{
		rayTracer->scene->camera->pos -= rayTracer->scene->camera->up * MOVEMODIFIER;
		rayTracer->scene->camera->dir -= rayTracer->scene->camera->up * MOVEMODIFIER;
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

	if (a_Key = SDL_SCANCODE_P)
	{
		//rayTracer->scene->camera->pos = vec3(0.0f, 100.0f, -300.0f);
		//rayTracer->scene->camera->dir = vec3(0.0f, 0.0f, 1.0f);
		//rayTracer->scene->camera->d = 1.0f;
		//rayTracer->scene->camera->up = vec3(0.0f, 1.0f, 0.0f);
		//rayTracer->scene->camera->right = vec3(1.0f, 0.0f, 0.0f);
		update = true;
	}

	// update screen and rays
	if (update)
	{
		rayTracer->scene->camera->CalculateScreen();
	}*/
}

// -----------------------------------------------------------
// Main game tick function
// -----------------------------------------------------------
void Game::Tick( float dt )
{
	rayTracer->Render();

	char buffer[500];
	sprintf(buffer, "FPS: %f TRIANGLES: %i \n", 1 / dt, rayTracer->scene->bvh->primitives->size());
	screen->Print(buffer, 2, 2, 0xffffff);	
}