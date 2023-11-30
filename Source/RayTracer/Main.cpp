#include "Renderer.h"
#include "Canvas.h"
#include "Random.h"

#include <iostream>


int main( int, char** ) {

	std::cout << "hello world!\n";

	Renderer renderer = Renderer();
	renderer.Initialize();

	renderer.CreateWindow( "RayTracer", 400, 300 );
	Canvas canvas( 400, 300, renderer );

	seedRandom( time(NULL) );

	bool quit = false;
	
	while ( !quit ) {
	
		SDL_Event event;
		SDL_PollEvent( &event );
		
		switch ( event.type ) {
		
		case SDL_QUIT:
			quit = true;
			break;
		
		}

		canvas.Clear( { 0, 0, 0, 1 } );
		for ( int i = 0; i < 1000; i++ ) canvas.DrawPoint( { random(400), random(300) }, { randomf(), randomf(), randomf(), 1});
		canvas.Update();

		renderer.PresentCanvas( canvas );
	
	}
	
	renderer.Shutdown();

	return 0;

}