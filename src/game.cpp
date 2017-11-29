#include "template.h"

// -----------------------------------------------------------
// Initialize the game
// -----------------------------------------------------------
RayTracer* rayTracer;
Camera* cam;
Triangle* testTriangle;
int c;
float3 color;
std::vector<pointLight*> pointLights;

void Game::Init() {
    
    rayTracer = new RayTracer();
    cam = new Camera();
    
    testTriangle = new Triangle();
    
    pointLights = std::vector<pointLight*>();
    pointLights.reserve(3);
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
	screen->Print( "Hello World", 100, 100, 0xffffff );
	screen->Line( 2, 10, 66, 10, 0xff0000 );
	screen->Line( 30, 30, 30, 50, 0xff0000  );
	screen->Plot(50, 50 , 0xff0000 );

	//color in plot is integer 32 bit
	//store ray tracer colors in vec3
	// (int)(cc.x*255)
	// c = b + ( g << 8 ) + ( r << 16 )
    
    for(int x=0; x< SCRWIDTH ; x++){
        for(int y=0 ; y < SCRHEIGHT ; y++){
            rayTracer->getColor(x, y, cam, pointLights, testTriangle);
            c = color.z + ( (int) color.y << 8 ) + ( (int) color.x << 16 );
            //screen->Plot(x, y ,c );
        }
    }
}
