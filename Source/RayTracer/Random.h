#pragma once

#include <algorithm>
#include <glm/gtx/norm.hpp>

void seedRandom( unsigned int seed ) {

	srand( seed );

}

float randomf() {

	return rand() / (float) RAND_MAX;

}

float random( float min, float max ) {

	if ( min > max ) std::swap( min, max );

	return ( randomf() * ( max - min ) ) + min;

}

float random( float max ) {

	return random( 0, max );

}

inline glm::vec3 random( const glm::vec3& min, const glm::vec3& max ) {

	return { random( min.x, max.x ), random( min.y, max.y ), random( min.z, max.z ) };

}

inline glm::vec3 randomInUnitSphere() {

	glm::vec3 v;

	do {

		v = random( { -1, -1, -1 }, { 1, 1, 1 } );

	} while ( glm::length2( v ) >= 1.0f );

	return v;

}