#include "template.h"
#include "Scene.h"
#include "RayTracer.h"

float ROTATEMODIFIER = 0.05f;
float ZOOMMODIFIER = 0.05f;
int FRAMEMODIFIER = 0;

Scene* scene;
RayTracer* rayTracer;
bool keyState[512];
int polyCount = 0;

// -----------------------------------------------------------
// Initialize the game
// -----------------------------------------------------------

void Game::Init()
{
// -----------------------------------------------------------
// 1 The original scene
// 2 LEGO Han Solo, 8k triangles
// 3 X-wing, 34k triangles
// 4 TIE Fighter, 54k triangles
// 5 Millennium Falcon, 178k triangles (better not get near)
// -----------------------------------------------------------
// Fear is the path to the dark side
// -----------------------------------------------------------
	
	LoadScene(1); // <-- Change scene here
}

void Game::LoadScene(int scene_id)
{
	memset(keyState, 0, sizeof(bool) * 512);
	frameCount = 0;
	scene = new Scene(scene_id);
	rayTracer = new RayTracer(scene, screen);
	polyCount += rayTracer->scene->bvh->primList->size();
}

void Game::Shutdown() { }
// -----------------------------------------------------------
// Input handling
// -----------------------------------------------------------
void Game::HandleInput( float dt ) { }

void Game::KeyUp(int a_Key) { keyState[a_Key] = false; }

void Game::KeyDown(int a_Key) { keyState[a_Key] = true; }

//	frameCount = 0;
//	switch (a_Key)
//	{
//		// Scene loader, needs destructor
//	//case SDL_SCANCODE_1:
//	//	LoadScene(1);
//	//	break;
//	//case SDL_SCANCODE_2:
//	//	LoadScene(2);
//	//	break;
//	//case SDL_SCANCODE_3:
//	//	LoadScene(3);
//	//	break;
//
//		// Translation:
//	case SDL_SCANCODE_W:
//		rayTracer->scene->camera->Surge(scene->MOVEMODIFIER);
//		break;
//	case SDL_SCANCODE_A:
//		rayTracer->scene->camera->Sway(-scene->MOVEMODIFIER);
//		break;
//	case SDL_SCANCODE_S:
//		rayTracer->scene->camera->Surge(-scene->MOVEMODIFIER);
//		break;
//	case SDL_SCANCODE_D:
//		rayTracer->scene->camera->Sway(scene->MOVEMODIFIER);
//		break;
//	case SDL_SCANCODE_Q:
//		rayTracer->scene->camera->Heave(-scene->MOVEMODIFIER);
//		break;
//	case SDL_SCANCODE_E:
//		rayTracer->scene->camera->Heave(scene->MOVEMODIFIER);
//		break;
//
//		// Rotation:
//	case SDL_SCANCODE_UP:
//		rayTracer->scene->camera->Pitch(-ROTATEMODIFIER);
//		break;
//	case SDL_SCANCODE_DOWN:
//		rayTracer->scene->camera->Pitch(ROTATEMODIFIER);
//		break;
//	case SDL_SCANCODE_LEFT:
//		rayTracer->scene->camera->Yaw(ROTATEMODIFIER);
//		break;
//	case SDL_SCANCODE_RIGHT:
//		rayTracer->scene->camera->Yaw(-ROTATEMODIFIER);
//		break;
//	//case SDL_SCANCODE_Z:
//	//	rayTracer->scene->camera->Roll(-ROTATEMODIFIER);
//	//	break;
//	//case SDL_SCANCODE_X:
//	//	rayTracer->scene->camera->Roll(ROTATEMODIFIER);
//	//	break;
//
//		// Zooming:
//	case SDL_SCANCODE_EQUALS:
//		rayTracer->scene->camera->Zoom(ZOOMMODIFIER);
//		break;
//	case SDL_SCANCODE_MINUS:
//		rayTracer->scene->camera->Zoom(-ZOOMMODIFIER);
//		break;
//
//		// Print Camera Position:
//	case SDL_SCANCODE_P:
//		rayTracer->scene->camera->PrintPosition();
//		break;
//
//		// Move light to current camera position
//	case SDL_SCANCODE_L:
//		rayTracer->scene->lightList.back()->pos = rayTracer->scene->camera->pos;
//		break;
//
//		// BVH Depth rendering
//	case SDL_SCANCODE_B:
//		rayTracer->depthRendering = !rayTracer->depthRendering;
//		break;
//
//		// Toggle Shadow
//	case SDL_SCANCODE_V:
//		rayTracer->renderShadow = !rayTracer->renderShadow;
//		break;
//
//		// Toggle Traversal Mode
//	case SDL_SCANCODE_T:
//#if ENABLETOPBVH
//		if (rayTracer->scene->bvhTop->traversalMode != 2)
//			rayTracer->scene->bvhTop->traversalMode++;
//		else
//			rayTracer->scene->bvhTop->traversalMode = 0;
//#else
//		if (rayTracer->scene->bvh->traversalMode != 2)
//			rayTracer->scene->bvh->traversalMode++;
//		else
//			rayTracer->scene->bvh->traversalMode = 0;
//#endif
//		break;
//
//		// Reset Camera Position:
//	case SDL_SCANCODE_R:
//		rayTracer->scene->camera->Init(rayTracer->scene->pos, rayTracer->scene->lookAt);
//		break;
//
//	case SDL_SCANCODE_1:
//		LoadScene(1);
//		break;
//	case SDL_SCANCODE_2:
//		LoadScene(2);
//		break;
//	case SDL_SCANCODE_3:
//		LoadScene(3);
//		break;
//	case SDL_SCANCODE_4:
//		LoadScene(4);
//		break;
//	case SDL_SCANCODE_5:
//		LoadScene(5);
//		break;
//

// -----------------------------------------------------------
// Main game tick function
// -----------------------------------------------------------
void Game::Tick(float dt)
{
	// Translation:
	if (keyState[SDL_SCANCODE_W]) { frameCount = FRAMEMODIFIER; rayTracer->scene->camera->Surge(scene->MOVEMODIFIER); }
	if (keyState[SDL_SCANCODE_A]) { frameCount = FRAMEMODIFIER; rayTracer->scene->camera->Sway(-scene->MOVEMODIFIER); }
	if (keyState[SDL_SCANCODE_S]) { frameCount = FRAMEMODIFIER; rayTracer->scene->camera->Surge(-scene->MOVEMODIFIER); }
	if (keyState[SDL_SCANCODE_D]) { frameCount = FRAMEMODIFIER; rayTracer->scene->camera->Sway(scene->MOVEMODIFIER); }
	if (keyState[SDL_SCANCODE_Q]) { frameCount = FRAMEMODIFIER; rayTracer->scene->camera->Heave(-scene->MOVEMODIFIER); }
	if (keyState[SDL_SCANCODE_E]) { frameCount = FRAMEMODIFIER; rayTracer->scene->camera->Heave(scene->MOVEMODIFIER); }

	// Rotation:
	if (keyState[SDL_SCANCODE_UP]) { frameCount = FRAMEMODIFIER; rayTracer->scene->camera->Pitch(-ROTATEMODIFIER); }
	if (keyState[SDL_SCANCODE_DOWN]) { frameCount = FRAMEMODIFIER; rayTracer->scene->camera->Pitch(ROTATEMODIFIER); }
	if (keyState[SDL_SCANCODE_LEFT]) { frameCount = FRAMEMODIFIER; rayTracer->scene->camera->Yaw(ROTATEMODIFIER); }
	if (keyState[SDL_SCANCODE_RIGHT]) { frameCount = FRAMEMODIFIER; rayTracer->scene->camera->Yaw(-ROTATEMODIFIER); }
	//if (keyState[SDL_SCANCODE_Z]) { frameCount = FRAMEMODIFIER; rayTracer->scene->camera->Roll(-ROTATEMODIFIER); }
	//if (keyState[SDL_SCANCODE_X]) { frameCount = FRAMEMODIFIER; rayTracer->scene->camera->Roll(ROTATEMODIFIER); }

	// Zooming:
	if (keyState[SDL_SCANCODE_EQUALS]) { frameCount = FRAMEMODIFIER; rayTracer->scene->camera->Zoom(ZOOMMODIFIER); }
	if (keyState[SDL_SCANCODE_MINUS]) { frameCount = FRAMEMODIFIER; rayTracer->scene->camera->Zoom(-ZOOMMODIFIER); }

	// Print Camera Position:
	if (keyState[SDL_SCANCODE_P]) rayTracer->scene->camera->PrintPosition();

	// Move light to current camera position
	if (keyState[SDL_SCANCODE_L]) { frameCount = FRAMEMODIFIER; rayTracer->scene->lightList.back()->pos = rayTracer->scene->camera->pos; }

	// BVH Depth rendering
	if (keyState[SDL_SCANCODE_B]) { frameCount = FRAMEMODIFIER; rayTracer->depthRendering = !rayTracer->depthRendering; }

	// Toggle Shadow
	if (keyState[SDL_SCANCODE_V]) { frameCount = FRAMEMODIFIER; rayTracer->renderShadow = !rayTracer->renderShadow; }

	// Toggle Traversal Mode
	if (keyState[SDL_SCANCODE_T])
	{
		frameCount = FRAMEMODIFIER;
#if ENABLETOPBVH
		if (rayTracer->scene->bvhTop->traversalMode != 2)
			rayTracer->scene->bvhTop->traversalMode++;
		else
			rayTracer->scene->bvhTop->traversalMode = 0;
#else
		if (rayTracer->scene->bvh->traversalMode != 2)
			rayTracer->scene->bvh->traversalMode++;
		else
			rayTracer->scene->bvh->traversalMode = 0;
#endif
	}

	// Reset Camera Position:
	if (keyState[SDL_SCANCODE_R]) { frameCount = FRAMEMODIFIER; rayTracer->scene->camera->Init(rayTracer->scene->pos, rayTracer->scene->lookAt); }

	++frameCount;

	int pixelCount = rayTracer->Render(frameCount);

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

	sprintf(buffer, "Pixel summed: %i. Frame count: %i", pixelCount, frameCount);
	screen->Print(buffer, 2, 42, 0xffffff);
}