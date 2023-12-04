#include "Renderer.h"
#include "Canvas.h"
#include "Random.h"
#include "Scene.h"
#include "Sphere.h"

#include <iostream>


int main( int, char** ) {

	std::cout << "hello world!\n";

	Renderer renderer = Renderer();
	renderer.Initialize();

	renderer.CreateWindow( "RayTracer", 400, 300 );
	Canvas canvas( 400, 300, renderer );

	float aspectRatio = canvas.GetSize().x / (float) canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>( glm::vec3 { 0, 0, 1 }, glm::vec3 { 0, 0, 0 }, glm::vec3 { 0, 1, 0 }, 70.0f, aspectRatio );

	Scene scene;
	scene.SetCamera( camera );

	seedRandom( time(NULL) );

	auto material = std::make_shared<Lambertian>( color3_t { 0, 0, 1 } );

	for ( auto i = 0; i < 10; i++ ) {


		auto sphere = std::make_unique<Sphere>( glm::vec3 { random(-1, 1), random( -1, 1 ), random( -1, 1 ) }, 0.1f, material);
		scene.AddObject( std::move( sphere ) );

	}

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
		scene.Render( canvas );
		canvas.Update();

		renderer.PresentCanvas( canvas );
	
	}
	
	renderer.Shutdown();

	return 0;

}