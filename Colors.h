#pragma once
#include "Color.h"

namespace Colors {
#define cx constexpr

	// Utility Colors
	cx Color BLACK{ 0, 0, 0, 255 };
	cx Color WHITE{ 255, 255, 255, 255 };
	cx Color TRANSPARENT{ 0, 0, 0, 0 };

	// Primary Colors
	cx Color RED{ 255, 0, 0, 255 };
	cx Color GREEN{ 0, 255, 0, 255 };
	cx Color BLUE{ 0, 0, 255, 255 };

	// Secondary Colors
	cx Color YELLOW{ 255, 255, 0, 255 };
	cx Color CYAN{ 0, 255, 255, 255 };
	cx Color MAGENTA{ 255, 0, 255, 255 };
	cx Color FUCSHIA{ 255, 0, 255, 255 };

	// Grayscale Colors
	cx Color GRAY{ 128, 128, 128, 255 };
	cx Color LIGHT_GRAY{ 211, 211, 211, 255 };
	cx Color DARK_GRAY{ 64, 64, 64, 255 };
	cx Color SILVER{ 192, 192, 192, 255 };
	cx Color CHARCOAL{ 50, 50, 50, 255 };

	// Tertiary Colors
	cx Color ORANGE{ 255, 165, 0,   255 };
	cx Color CHARTREUSE{ 127, 255, 0,   255 };
	cx Color SPRING_GREEN{ 0,   255, 127, 255 };
	cx Color AZURE{ 0,   127, 255, 255 };
	cx Color VIOLET{ 127, 0,   255, 255 };
	cx Color ROSE{ 255, 0,   127, 255 };
}