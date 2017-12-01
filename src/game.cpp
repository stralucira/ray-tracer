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
//void Game::KeyDown(int a_Key)
//{
//	if (a_Key == 79)
//	{
//		cam->pos.x += 0.1;
//	}
//	if (a_Key == 80)
//	{
//		cam->pos.x -= 0.1;
//	}
//	if (a_Key == 81)
//	{
//		cam->pos.z -= 0.1;
//	}
//	if (a_Key == 82)
//	{
//		cam->pos.z += 0.1;
//	}
//}

// -----------------------------------------------------------
// Main game tick function
// -----------------------------------------------------------
void Game::Tick( float dt )
{
}