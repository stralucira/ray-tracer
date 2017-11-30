#include "template.h"

// -----------------------------------------------------------
// Initialize the game
// -----------------------------------------------------------
RayTracer* rayTracer;
Camera* cam;
Primitive* testTriangle;
Primitive* testSphere;
Primitive* testPlane;
Material* defaultMaterial;
int c;
float3 color;
pointLight* light;
std::vector<Primitive*> primList;

//float3 aspectRatio = SCRHEIGHT/SCRWIDTH;

void Game::Init() {
    
    rayTracer = new RayTracer();
    cam = new Camera();
    defaultMaterial = new Material();
    
    testTriangle = new Triangle();
    testPlane = new Plane();
    testSphere = new Sphere();
    
    light = new pointLight(float3(0,0,0), defaultMaterial);
    
    primList.push_back(testSphere);
    primList.push_back(testTriangle);
    primList.push_back(testPlane);
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
		cam->pos.x += 0.1;
	}
	if (a_Key == 80)
	{
		cam->pos.x -= 0.1;
	}
	if (a_Key == 81)
	{
		cam->pos.z -= 0.1;
	}
	if (a_Key == 82)
	{
		cam->pos.z += 0.1;
	}
}

// -----------------------------------------------------------
// Main game tick function
// -----------------------------------------------------------
void Game::Tick( float dt )
{
	//moveCamera();
	screen->Clear( 0 );
    
    for(int x=0; x < SCRWIDTH ; x++){
        for(int y=0 ; y < SCRHEIGHT ; y++){
            
            color = rayTracer->getColor(x, y, cam, light, primList);
			
            // Clamped color values transformed to int
			c = min((int)(color.z*255.0f),255) + (min((int)(color.y*255.0f),255) << 8) + (min((int)(color.x*255.0),255) << 16);
            screen->Plot(x, y, c);
        }
    }
}

/*
void Game::moveCamera()
{
	if (GetAsyncKeyState(VK_UP))
	{
		cam->pos.z += 0.1;
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		cam->pos.z -= 0.1;
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		cam->pos.x -= 0.1;
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		cam->pos.x += 0.1;
	}
	if (GetAsyncKeyState(0x57))
	{
		cam->pos.y -= 0.1;
	}
	if (GetAsyncKeyState(0x53))
	{
		cam->pos.y += 0.1;
	}
	if (GetAsyncKeyState(VK_OEM_PLUS))
	{
		cam->d += 0.1;
		cam->calculateScreen();
	}
	if (GetAsyncKeyState(VK_OEM_MINUS))
	{
		cam->d -= 0.1;
		cam->calculateScreen();
	}
}
*/