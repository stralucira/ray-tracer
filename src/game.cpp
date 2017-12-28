#include "template.h"
#include "Scene.h"
#include "RayTracer.h"

float ROTATEMODIFIER = 0.05f;
float ZOOMMODIFIER = 0.05f;

Scene* scene;
RayTracer* rayTracer;
int polyCount;

// -----------------------------------------------------------
// Initialize the game
// -----------------------------------------------------------

void Game::Init()
{
	// --------------------------------------------------------------
	// 1 the original scene, better to disable alternate render mode
	// 2 lego han solo, 8k triangles
	// 3 x-wing, 34k triangles
	// 4 tie fighter, 54k triangles
	// 5 millennium falcon, 178k triangles (better not get near)
	// Fear is the path to the dark side
	// --------------------------------------------------------------
	
	LoadScene(0); // <-- Change scene here
}

void Game::LoadScene(int scene_id)
{
	scene = new Scene(scene_id);
	rayTracer = new RayTracer(scene, screen);
	polyCount += rayTracer->scene->bvh->primList->size();
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
		// Scene loader, needs destructor
	//case SDL_SCANCODE_1:
	//	LoadScene(1);
	//	break;
	//case SDL_SCANCODE_2:
	//	LoadScene(2);
	//	break;
	//case SDL_SCANCODE_3:
	//	LoadScene(3);
	//	break;

		// Translation:
	case SDL_SCANCODE_W:
		rayTracer->scene->camera->Surge(scene->MOVEMODIFIER);
		break;
	case SDL_SCANCODE_A:
		rayTracer->scene->camera->Sway(-scene->MOVEMODIFIER);
		break;
	case SDL_SCANCODE_S:
		rayTracer->scene->camera->Surge(-scene->MOVEMODIFIER);
		break;
	case SDL_SCANCODE_D:
		rayTracer->scene->camera->Sway(scene->MOVEMODIFIER);
		break;
	case SDL_SCANCODE_Q:
		rayTracer->scene->camera->Heave(-scene->MOVEMODIFIER);
		break;
	case SDL_SCANCODE_E:
		rayTracer->scene->camera->Heave(scene->MOVEMODIFIER);
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

		// Toggle Traversal Mode
	case SDL_SCANCODE_T:
		if (rayTracer->scene->bvh->traversalMode != 2)
			rayTracer->scene->bvh->traversalMode++;
		else
			rayTracer->scene->bvh->traversalMode = 0;
		break;

		// Reset Camera Position:
	case SDL_SCANCODE_R:
		rayTracer->scene->camera->Init(rayTracer->scene->pos, rayTracer->scene->lookAt);
		break;

	case SDL_SCANCODE_1:
		LoadScene(1);
		break;
	case SDL_SCANCODE_2:
		LoadScene(2);
		break;
	case SDL_SCANCODE_3:
		LoadScene(3);
		break;
	case SDL_SCANCODE_4:
		LoadScene(4);
		break;
	case SDL_SCANCODE_5:
		LoadScene(5);
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
	sprintf(buffer, "FPS: %f Polygons: %i Position: %.2f %.2f %.2f Direction: %.2f %.2f %.2f \n", 1 / dt,
		polyCount,
		rayTracer->scene->camera->pos.x, rayTracer->scene->camera->pos.y, rayTracer->scene->camera->pos.z,
		rayTracer->scene->camera->GetForward().x, rayTracer->scene->camera->GetForward().y, rayTracer->scene->camera->GetForward().z);
	screen->Print(buffer, 2, 2, 0xffffff);
	
	sprintf(buffer, "Press B to toggle depth rendering.");
	screen->Print(buffer, 2, 12, 0xffffff);

	switch (rayTracer->scene->bvh->traversalMode)
	{
	case 0:
		sprintf(buffer, "Press T to toggle traversal modes. Traversal mode: Standard.");
		break;
	case 1:
		sprintf(buffer, "Press T to toggle traversal modes. Traversal mode: Distance calculation.");
		break;
	case 2:
		sprintf(buffer, "Press T to toggle traversal modes. Traversal mode: Ray direction sign.");
		break;
	}
	screen->Print(buffer, 2, 22, 0xffffff);

	sprintf(buffer, "Press V to toggle shadows.");
	screen->Print(buffer, 2, 32, 0xffffff);
	
}