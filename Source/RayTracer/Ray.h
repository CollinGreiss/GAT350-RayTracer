#pragma once

#include <glm/glm.hpp>

struct ray_t {

	ray_t() = default;
	ray_t( const glm::vec3& origin, const glm::vec3& direction ) :
		origin { origin },
		direction { direction }
	{}

	glm::vec3 At( float distance ) const { return origin + ( distance * direction ); }
	glm::vec3 operator * ( float distance ) const { return origin + distance * direction; }

	glm::vec3 origin;
	glm::vec3 direction;

};

struct raycastHit_t {

	float distance = 0;

	glm::vec3 point { 0 };
	glm::vec3 normal { 0 };

	class Material* material { nullptr };

};