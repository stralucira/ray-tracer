#include "template.h"

// -----------------------------------------------------------
// Initialize the game
// -----------------------------------------------------------
RayTracer* rayTracer;
Camera* cam;
Primitive* testTriangle;
Primitive* testSphere;
Material* defaultMaterial;
int c;
float3 color;
pointLight* light;
std::vector<Primitive*> primList;

float3 aspectRatio = SCRHEIGHT/SCRWIDTH;

void Game::Init() {
    
    rayTracer = new RayTracer();
    cam = new Camera();
    defaultMaterial = new Material();
    
    testTriangle = new Triangle();
    
    testSphere = new Sphere();
    
    light = new pointLight(float3(0,0,0), defaultMaterial);
    
    primList.push_back(testSphere);
    primList.push_back(testTriangle);
    
}

void Game::Shutdown() { }
// -----------------------------------------------------------
// Input handling
// -----------------------------------------------------------
void Game::HandleInput( float dt ) { }

// -----------------------------------------------------------
// Main game tick function
// -----------------------------------------------------------
void Game::Tick( float dt )
{
	screen->Clear( 0 );
    
    for(int x=0; x < SCRWIDTH ; x++){
        for(int y=0 ; y < SCRHEIGHT ; y++){
            
            color = rayTracer->getColor(x, y, cam, light, primList);
			
			c = (int)(color.z*255.0f) + ((int)(color.y*255.0f) << 8) + ((int)(color.x*255.0) << 16);
            screen->Plot(x, y, c);
        }
    }
}
