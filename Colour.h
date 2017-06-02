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

class ScreenByte
{
public:
	ScreenByte(uint32_t mode) : m_mode(mode),m_byte(0),m_offset(0) {}
	ScreenByte() = delete;
	~ScreenByte() {}

	bool addPixel(const Colour& pixelColour)
	{
		// or in at appropriate place, inc offset
		++m_offset;
		return m_offset == getPixelsPerByte();
	}

	uint8_t getByte() const
	{
		return m_byte;
	}

	uint32_t getPixelsPerByte() const
	{
		switch (m_mode)
		{
		case 2:
			return 2;
			break;

		case 5:
			return 4;
			break;

		default:
			return 0;
			break;
		}
	}

private:
	uint8_t m_byte;
	uint32_t m_offset;
	uint32_t m_mode;
};