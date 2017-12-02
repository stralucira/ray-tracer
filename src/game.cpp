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
	if (a_Key == 79)
	{
		rayTracer->scene->camera->pos.x += 0.1;
	}
	if (a_Key == 80)
	{
		rayTracer->scene->camera->pos.x -= 0.1;
	}
	if (a_Key == 81)
	{
		rayTracer->scene->camera->pos.z -= 0.1;
	}
	if (a_Key == 82)
	{
		rayTracer->scene->camera->pos.z += 0.1;
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