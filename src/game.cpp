#include "template.h"
#include "Scene.h"
#include "RayTracer.h"

#define MOVEMODIFIER	0.10f
#define ROTATEMODIFIER	0.50f
#define ZOOMMODIFIER	0.10f

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
		// Translation:
	case SDL_SCANCODE_W:
		rayTracer->scene->camera->Surge(MOVEMODIFIER);
		break;
	case SDL_SCANCODE_A:
		rayTracer->scene->camera->Sway(-MOVEMODIFIER);
		break;
	case SDL_SCANCODE_S:
		rayTracer->scene->camera->Surge(-MOVEMODIFIER);
		break;
	case SDL_SCANCODE_D:
		rayTracer->scene->camera->Sway(MOVEMODIFIER);
		break;
	case SDL_SCANCODE_Q:
		rayTracer->scene->camera->Heave(-MOVEMODIFIER);
		break;
	case SDL_SCANCODE_E:
		rayTracer->scene->camera->Heave(MOVEMODIFIER);
		break;

		// Rotation:
	case SDL_SCANCODE_UP:
		rayTracer->scene->camera->Pitch(-ROTATEMODIFIER);
		break;
	case SDL_SCANCODE_DOWN:
		rayTracer->scene->camera->Pitch(ROTATEMODIFIER);
		break;
	case SDL_SCANCODE_LEFT:
		rayTracer->scene->camera->Yaw(ROTATEMODIFIER);
		break;
	case SDL_SCANCODE_RIGHT:
		rayTracer->scene->camera->Yaw(-ROTATEMODIFIER);
		break;
	//case SDL_SCANCODE_Z:
	//	rayTracer->scene->camera->Roll(-ROTATEMODIFIER);
	//	break;
	//case SDL_SCANCODE_X:
	//	rayTracer->scene->camera->Roll(ROTATEMODIFIER);
	//	break;

		// Zooming:
	case SDL_SCANCODE_EQUALS:
		rayTracer->scene->camera->Zoom(ZOOMMODIFIER);
		break;
	case SDL_SCANCODE_MINUS:
		rayTracer->scene->camera->Zoom(-ZOOMMODIFIER);
		break;

		// Reset Camera:
	case SDL_SCANCODE_R:
		rayTracer->scene->camera->Init();
		break;
	}
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