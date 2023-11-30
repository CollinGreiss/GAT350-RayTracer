#pragma once

#include <glm/glm.hpp>

using color3_t = glm::vec3;
using color4_t = glm::vec4;
using rgba_t   = uint32_t;

inline color4_t RGBAToColor( const rgba_t& rgba ) {

	return {

		( ( rgba >> 24 ) & 0xff ) / 255,
		( ( rgba >> 16 ) & 0xff ) / 255,
		( ( rgba >> 8  ) & 0xff ) / 255,
		( ( rgba >> 0  ) & 0xff ) / 255

	};

}

inline rgba_t ColorToRGBA( const color4_t& color ) {

	rgba_t rgba{};

	rgba |= uint8_t( glm::clamp( (int) ( color.r * 255 ), 0, 255 ) ) << 24;
	rgba |= uint8_t( glm::clamp( (int) ( color.g * 255 ), 0, 255 ) ) << 16;
	rgba |= uint8_t( glm::clamp( (int) ( color.b * 255 ), 0, 255 ) ) <<  8;
	rgba |= uint8_t( glm::clamp( (int) ( color.a * 255 ), 0, 255 ) ) <<  0;

	return rgba;

}