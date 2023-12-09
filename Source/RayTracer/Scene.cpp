#include "Scene.h"

#include "Canvas.h"
#include "MathUtils.h"
#include "Random.h"
#include "Sphere.h"
#include "Plane.h"
#include "Mesh.h"

#include <iomanip>
#include <iostream>
#include "Triangle.h"

void Scene::Render( Canvas& canvas, int numSamples, int depth ) {

	for ( int y = 0; y < canvas.GetSize().y; y++ ) {

		for ( int x = 0; x < canvas.GetSize().x; x++ ) {

			glm::vec2 pixel = glm::vec2 { x, y };

			color3_t color { 0 };

			for ( int sample = 0; sample < numSamples; sample++ ) {

				glm::vec2 point = ( pixel + glm::vec2 { randomf(), randomf() } ) / glm::vec2 { canvas.GetSize().x, canvas.GetSize().y };
				point.y = 1.0f - point.y;

				ray_t ray = m_camera->GetRay( point );

				raycastHit_t raycastHit;
				color += Trace( ray, 0, 100, raycastHit, depth );

			}

			color /= numSamples;
			canvas.DrawPoint( pixel, color4_t( color, 1 ) );;

		}

		std::cout << std::setprecision( 2 ) << std::setw( 5 ) << ( ( y / (float) canvas.GetSize().y ) * 100 ) << "%\n";


	}

}

color3_t Scene::Trace( const ray_t& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit, int depth ) {

	bool rayHit = false;
	float closestDistance = maxDistance;

	for ( auto& object : m_objects ) {

		if ( object->Hit( ray, minDistance, closestDistance, raycastHit ) ) {

			rayHit = true;
			closestDistance = raycastHit.distance;

		}

	}

	if ( rayHit ) {

		ray_t scattered;
		color3_t color;


		if ( depth > 0 && raycastHit.material->Scatter( ray, raycastHit, color, scattered ) )
			return color * Trace( scattered, minDistance, maxDistance, raycastHit, depth - 1 );
		else
			return raycastHit.material->GetEmissive();

	}

	glm::vec3 direction = glm::normalize( ray.direction );
	float t = ( direction.y + 1 ) * 0.5f;
	color3_t color = lerp( m_bottomColor, m_topColor, t );

	return color;

}

void Scene::InitScene01( Scene& scene, const Canvas& canvas ) {

	float aspectRatio = canvas.GetSize().x / canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>( glm::vec3 { 0, 2, 10 }, glm::vec3 { 0, 1, 0 }, glm::vec3 { 0, 1, 0 }, 20.0f, aspectRatio );
	scene.SetCamera( camera );

	for ( int x = -10; x < 10; x++ ) {

		for ( int z = -10; z < 10; z++ ) {

			std::shared_ptr<Material> material;

			float r = randomf();
			if ( r < 0.3f )		material = std::make_shared<Lambertian>( glm::rgbColor( glm::vec3 { random( 0, 360 ), 1.0f, 1.0f } ) );
			else if ( r < 0.6f )	material = std::make_shared<Metal>( color3_t { random( 0.5f, 1.0f ) }, random( 0, 0.5f ) );
			else if ( r < 0.9f )	material = std::make_shared<Dielectric>( color3_t { 1.0f }, random( 1.1f, 2 ) );
			else				material = std::make_shared<Emissive>( glm::rgbColor( glm::vec3 { random( 0, 360 ), 1.0f, 1.0f } ), 5.0f );


			float radius = random( 0.2f, 0.3f );
			auto sphere = std::make_unique<Sphere>( glm::vec3 { x + random( -0.5f, 0.5f ), radius, z + random( -0.5f, 0.5f ) }, radius, material );
			scene.AddObject( std::move( sphere ) );

		}

	}

	auto plane = std::make_unique<Plane>( glm::vec3 { 0, 0, 0 }, glm::vec3 { 0, 1, 0 }, std::make_shared<Lambertian>( color3_t { 0.2f } ) );
	scene.AddObject( std::move( plane ) );

}

void Scene::InitScene02( Scene& scene, const Canvas& canvas ) {

	float aspectRatio = canvas.GetSize().x / canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>( glm::vec3 { 0, 2, 10 }, glm::vec3 { 0, 1, 0 }, glm::vec3 { 0, 1, 0 }, 20.0f, aspectRatio );
	scene.SetCamera( camera );

	auto plane = std::make_unique<Plane>( glm::vec3 { 0, 0, 0 }, glm::vec3 { 0, 1, 0 }, std::make_shared<Lambertian>( color3_t { 0.2f } ) );
	scene.AddObject( std::move( plane ) );

	auto mesh = std::make_unique<Mesh>( std::make_shared<Lambertian>( color3_t { 0, 0, 1 } ) );
	mesh->Load( "models/cube.obj", glm::vec3 { 0, 0.5f, 0 }, glm::vec3 { 0, 45, 0 } );
	scene.AddObject( std::move( mesh ) );

}

void Scene::InitScene03( Scene& scene, const Canvas& canvas ) {

	float aspectRatio = canvas.GetSize().x / canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>( glm::vec3 { 0, 0, 1 }, glm::vec3 { 0, 0, 0 }, glm::vec3 { 0, 1, 0 }, 20.0f, aspectRatio );
	scene.SetCamera( camera );

	auto floor = std::make_unique<Plane>( glm::vec3 { 0, -2, 0 }, glm::vec3 { 0, 1, 0 }, std::make_shared<Lambertian>( color3_t { 1.0f } ) );
	auto backWall = std::make_unique<Plane>( glm::vec3 { 0, 0, -20 }, glm::vec3 { 0, 0, 1 }, std::make_shared<Lambertian>( color3_t { 1 } ) );
	auto ceiling = std::make_unique<Plane>( glm::vec3 { 0, 2, 0 }, glm::vec3 { 0, -1, 0 }, std::make_shared<Lambertian>( color3_t { 1 } ) );
	auto leftWall = std::make_unique<Plane>( glm::vec3 { -4, 0, 0 }, glm::vec3 { 1, 0, 0 }, std::make_shared<Lambertian>( color3_t { 1, 0, 0 } ) );
	auto rightWall = std::make_unique<Plane>( glm::vec3 { 4, 0, 0 }, glm::vec3 { -1, 0, 0 }, std::make_shared<Lambertian>( color3_t { 0, 1, 0 } ) );


	scene.AddObject( std::move( floor ) );
	scene.AddObject( std::move( ceiling ) );
	scene.AddObject( std::move( leftWall ) );
	scene.AddObject( std::move( rightWall ) );
	scene.AddObject( std::move( backWall ) );


	auto light = std::make_unique<Mesh>( std::make_shared<Emissive>( color3_t { 1.0f, 0.85f, .64f }, 5.0f ) );
	light->Load( "models/quad.obj", glm::vec3 { 0.0f, 2.0f, -15.0f }, glm::vec3 { 75, 0, 0 }, glm::vec3 { 1.0f, 1.0f, 1.0f } );
	scene.AddObject( std::move( light ) );

	auto cube = std::make_unique<Mesh>( std::make_shared<Lambertian>( color3_t { 1 } ) );
	cube->Load( "models/cube.obj", glm::vec3 { -1.0f, -1.5f, -16.0f }, glm::vec3 { 0, 45, 0 } );
	scene.AddObject( std::move( cube ) );

	auto sphere = std::make_unique<Sphere>( glm::vec3 { 0.5f, -1.5f, -14.0f }, 0.5f, std::make_shared<Lambertian>( color3_t { 1 } ) );
	scene.AddObject( std::move( sphere ) );

}

void Scene::InitScene04( Scene& scene, const Canvas& canvas ) {

	float aspectRatio = canvas.GetSize().x / canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>( glm::vec3 { 0, 2, 10 }, glm::vec3 { 0, 1, 0 }, glm::vec3 { 0, 1, 0 }, 20.0f, aspectRatio );
	scene.SetCamera( camera );

	for ( int x = -10; x < 10; x++ ) {

		for ( int z = -10; z < 10; z++ ) {

			std::shared_ptr<Material> material;

			float r = randomf();
			if ( r < 0.3f )			material = std::make_shared<Lambertian>( color3_t { randomf(), randomf(), randomf() } );
			else if ( r < 0.6f )	material = std::make_shared<Metal>( color3_t { randomf(), randomf(), randomf() }, random( 0, 0.5f ) );
			else if ( r < 0.9f )	material = std::make_shared<Dielectric>( color3_t { randomf(), randomf(), randomf() }, random( 1.1f, 2 ) );
			else					material = std::make_shared<Emissive>( color3_t { randomf(), randomf(), randomf() }, 5.0f );


			r = randomf();

			if ( r < 0.33f ) {

				float radius = random( 0.2f, 0.3f );
				auto sphere = std::make_unique<Sphere>( glm::vec3 { x + random( -0.5f, 0.5f ), radius, z + random( -0.5f, 0.5f ) }, radius, material );
				scene.AddObject( std::move( sphere ) );

			} else if ( r < 0.66f ) {

				auto cube = std::make_unique<Mesh>( material );
				cube->Load( "models/cube.obj", glm::vec3 { -1.0f, -2.0f, -16.0f }, glm::vec3 { 0, random( 0, 360 ), 0 } );
				scene.AddObject( std::move( cube ) );


			} else {

				auto triangle = std::make_unique<Triangle>(glm::vec3{ -1, 0, 0 }, glm::vec3{ 1, 0, 0 }, glm::vec3{ 0, 2, 0 }, material);
				scene.AddObject(std::move(triangle));

			}


		}

	}

	auto plane = std::make_unique<Plane>( glm::vec3 { 0, 0, 0 }, glm::vec3 { 0, 1, 0 }, std::make_shared<Lambertian>( color3_t { 0.2f } ) );
	scene.AddObject( std::move( plane ) );

}