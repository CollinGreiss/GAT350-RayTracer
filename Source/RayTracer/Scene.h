#pragma once

#include "Camera.h"
#include "Color.h"
#include "Object.h"

#include <memory>
#include <vector>

class Scene {

public:

	Scene() = default;
	Scene( const color3_t& topColor, const color3_t& bottomColor ) :
		m_topColor { topColor },
		m_bottomColor { bottomColor }
	{}

	void Render( class Canvas& canvas, int numSamples, int depth );

	void InitScene01( Scene& scene, const class Canvas& canvas );
	void InitScene02( Scene& scene, const class Canvas& canvas );
	void InitScene03( Scene& scene, const class Canvas& canvas );
	void InitScene04( Scene& scene, const class Canvas& canvas );

	void SetCamera( std::shared_ptr<Camera> camera ) { m_camera = camera; }

	color3_t Trace( const ray_t& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit, int depth );
	void AddObject( std::unique_ptr<Object> object ) { m_objects.push_back( std::move( object ) ); }

private:

	std::shared_ptr<Camera> m_camera;

	color3_t m_topColor { 0 };
	color3_t m_bottomColor { 1 };
	int m_depth { 5 };

	std::vector<std::unique_ptr<Object>> m_objects;

};

