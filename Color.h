#pragma once
#include <cstdint>
#include <stdexcept>
namespace raylib {
	#include "raylib/raylib.h"
	#undef LIGHTGRAY
	#undef GRAY
	#undef DARKGRAY
	#undef YELLOW   
	#undef GOLD     
	#undef ORANGE   
	#undef PINK     
	#undef RED      
	#undef MAROON   
	#undef GREEN    
	#undef LIME     
	#undef DARKGREEN
	#undef SKYBLUE  
	#undef BLUE     
	#undef DARKBLUE 
	#undef PURPLE   
	#undef VIOLET   
	#undef DARKPURPL
	#undef BEIGE    
	#undef BROWN    
	#undef DARKBROWN
	#undef WHITE      
	#undef BLACK      
	#undef BLANK      
	#undef MAGENTA    
	#undef RAYWHITE   
	#undef TRANSPARENT
}

struct Color {
	uint8_t r, g, b, a;
	operator raylib::Color() const { return { r, g, b, a }; }

	constexpr Color() : r(0), g(0), b(0), a(255), m_isEmpty(true) {}
	constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
		: r(red), g(green), b(blue), a(alpha), m_isEmpty(false) {
	}

	constexpr bool operator==(const Color& other) const {
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}
	constexpr bool operator!=(const Color& other) const { return !(*this == other); }

	Color Opacity(float opacity) const {
		if (opacity < 0.0f || opacity > 1.0f)
			throw std::out_of_range("Opacity must be between 0.0 and 1.0");
		return { r, g, b, static_cast<uint8_t>(a * opacity) };
	}

	constexpr bool IsEmpty() const { return m_isEmpty; }
	static constexpr Color Empty() { return Color(); }

	explicit constexpr operator bool() const { return !m_isEmpty; } // true if not empty

private:
	bool m_isEmpty;
};