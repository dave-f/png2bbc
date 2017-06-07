#include "Colour.h"

std::map<uint32_t, Colour::BBCColour> Colour::m_colourMap =
{
	{0x000000,BBCColour::Black},
	{0xff0000,BBCColour::Red},
	{0x00ff00,BBCColour::Green},
	{0xffff00,BBCColour::Yellow},
	{0x0000ff,BBCColour::Blue},
	{0xff00ff,BBCColour::Magenta},
	{0x00ffff,BBCColour::Cyan},
	{0xffffff,BBCColour::White}
};

Colour::Colour(uint8_t r, uint8_t g, uint8_t b)
{
	uint32_t key = (r << 24) | (g << 16) | (b << 8);
	
	auto it = m_colourMap.find(key);

	if (it != m_colourMap.end())
	{
		m_thisColour = it->second;
	}
	else
	{
		throw std::runtime_error("Invalid BBC Colour");
	}
}
