#pragma once

#include <algorithm>

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