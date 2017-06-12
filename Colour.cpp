#include "Colour.h"

std::map<uint32_t, Colour::BBCColour> Colour::m_colourMap =
{
	{0x000000ff,BBCColour::Black},
	{0xff0000ff,BBCColour::Red},
	{0x00ff00ff,BBCColour::Green},
	{0xffff00ff,BBCColour::Yellow},
	{0x0000ffff,BBCColour::Blue},
	{0xff00ffff,BBCColour::Magenta},
	{0x00ffffff,BBCColour::Cyan},
	{0xffffffff,BBCColour::White}
};

std::map<Colour::BBCColour,uint8_t> ScreenByte::m_mode5Map =
{
	{Colour::BBCColour::Black,	0b00},
	{Colour::BBCColour::Red,	0b00},
	{Colour::BBCColour::Green,	0b00},
	{Colour::BBCColour::Yellow, 0b00},
	{Colour::BBCColour::Blue,	0b00},
	{Colour::BBCColour::Magenta,0b00},
	{Colour::BBCColour::Cyan,	0b00},
	{Colour::BBCColour::White,	0b11}
};

Colour::Colour(uint8_t r, uint8_t g, uint8_t b)
{
	uint32_t key = (r << 24) | (g << 16) | (b << 8) | 0xff;
	
	auto it = m_colourMap.find(key);

	if (it != m_colourMap.end())
	{
		m_internalColour = it->second;
	}
	else
	{
		throw std::runtime_error("Invalid BBC Colour");
	}
}
