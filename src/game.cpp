#include "template.h"

// -----------------------------------------------------------
// Initialize the game
// -----------------------------------------------------------
void Game::Init() { }

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
}