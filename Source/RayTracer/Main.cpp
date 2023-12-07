#include "Renderer.h"
#include "Random.h"
#include "Canvas.h"
#include "Camera.h"
#include "Scene.h"
#include "Material.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Mesh.h"

#include <iostream>
#include <memory>

const int width = 400;
const int height = 300;
const int samples = 20;
const int depth = 5;

int main( int argc, char* argv[] ) {

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow( "Ray Tracer", width, height );

	Canvas canvas( width, height, renderer );

	float aspectRatio = canvas.GetSize().x / (float) canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>( glm::vec3 { 0, 2, 10 }, glm::vec3 { 0, 0, 0 }, glm::vec3 { 0, 1, 0 }, 20.0f, aspectRatio );

	Scene scene( glm::vec3 { 1.0f }, glm::vec3 { 0.5f, 0.7f, 1.0f } );
	scene.SetCamera( camera );

	scene.InitScene01( scene, canvas );

	canvas.Clear( { 0, 0, 0, 1 } );
	scene.Render( canvas, samples, depth );
	canvas.Update();

	bool quit = false;
	while ( !quit ) {

		SDL_Event event;
		SDL_PollEvent( &event );
		switch ( event.type ) {
		
		case SDL_QUIT:
			quit = true;
			break;
		
		case SDL_KEYDOWN:
		
			switch ( event.key.keysym.sym ) {
			
			case SDLK_ESCAPE:
				quit = true;
				break;
			
			}
			
			break;
		
		}

		renderer.PresentCanvas( canvas );
	
	}

	renderer.Shutdown();

	return 0;

}