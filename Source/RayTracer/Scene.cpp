#include "Scene.h"

#include "Canvas.h"
#include "MathUtils.h"

void Scene::Render( Canvas& canvas ) {

	for ( int y = 0; y < canvas.GetSize().y; y++ ) {

		for ( int x = 0; x < canvas.GetSize().x; x++ ) {

			glm::vec2 pixel = { x, y };
			glm::vec2 point = { pixel.x / canvas.GetSize().x, pixel.y / canvas.GetSize().y };


			point.y = 1.0f - point.y;

			ray_t ray = m_camera->GetRay(point);

			raycastHit_t raycastHit;
			color3_t color = Trace( ray, 0, 100, raycastHit );

			canvas.DrawPoint( pixel, color4_t( color, 1 ) );

		}

	}

}

color3_t Scene::Trace( const ray_t& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit ) {

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

		if ( raycastHit.material->Scatter( ray, raycastHit, color, scattered ) ) {
			
			return raycastHit.normal;

		} else {

			return color3_t { 0, 0, 0 };

		}

	}


	glm::vec3 direction = glm::normalize( ray.direction );
	float t = ( direction.y + 1 ) * 0.5f;
	color3_t color = lerp( m_bottomColor, m_topColor, t );

	return color;
}