#ifndef _COLOR_H
#define _COLOR_H

#include <cstdint>

struct Color
{
	uint8_t r, g, b;

	Color();
	Color(uint8_t, uint8_t, uint8_t);
};

#endif
