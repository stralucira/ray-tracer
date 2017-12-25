#include "template.h"
#include "Scene.h"
#include "RayTracer.h"

float MOVEMODIFIER = 10.00f;
float ROTATEMODIFIER = 1.50f;
float ZOOMMODIFIER = 1.10f;

RayTracer* rayTracer;
int polyCount;

// -----------------------------------------------------------
// Initialize the game
// -----------------------------------------------------------

void Game::Init()
{    
	Scene* testScene = new Scene();
	rayTracer = new RayTracer(testScene, screen);

	for (size_t i = 0; i < rayTracer->scene->bvh.size(); i++)
	{
		polyCount += rayTracer->scene->bvh[i]->primList->size();
	}
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

		// Print Camera Position:
	case SDL_SCANCODE_P:
		rayTracer->scene->camera->PrintPosition();
		break;

		// Move light to current camera position
	case SDL_SCANCODE_L:
		rayTracer->scene->lightList.back()->pos = rayTracer->scene->camera->pos;
		break;

		// BVH Depth rendering
	case SDL_SCANCODE_B:
		rayTracer->depthRendering = !rayTracer->depthRendering;
		break;

		// Toggle Shadow
	case SDL_SCANCODE_V:
		rayTracer->renderShadow = !rayTracer->renderShadow;
		break;

		// Reset Camera Position:
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
	sprintf(buffer, "FPS: %f Polygons: %i \n", 1 / dt, polyCount);
	screen->Print(buffer, 2, 2, 0xffffff);
	sprintf(buffer, "Position: %.2f %.2f %.2f \n", rayTracer->scene->camera->pos.x, rayTracer->scene->camera->pos.y, rayTracer->scene->camera->pos.z);
	screen->Print(buffer, 2, 12, 0xffffff);
	sprintf(buffer, "Direction: %.2f %.2f %.2f \n", rayTracer->scene->camera->dir.x, rayTracer->scene->camera->dir.y, rayTracer->scene->camera->dir.z);
	screen->Print(buffer, 2, 22, 0xffffff);	
}