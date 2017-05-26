#pragma once

#include <cstdint>

class Colour
{
public:
	enum class BBCColour : uint32_t
	{
		Black	= 0x000000,
		Red		= 0xff0000,
		Green	= 0x00ff00,
		Yellow	= 0xffff00,
		Blue	= 0x0000ff,
		Magenta = 0xff00ff,
		Cyan	= 0x00ffff,
		White	= 0xffffff,
	};

	Colour() {}
	Colour(const BBCColour&) {}
	Colour(uint32_t rawRGBA) {}
	Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff) {}
	~Colour() {}
};
