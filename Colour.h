#pragma once

#include <cstdint>
#include <map>

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

	Colour() = delete;
	Colour(const BBCColour&) = delete;
	Colour(uint32_t rawRGBA) = delete;
	Colour(uint8_t r, uint8_t g, uint8_t b);
	~Colour() {}

	static std::map<uint32_t, BBCColour> m_colourMap;
	BBCColour m_thisColour;
};

class ScreenByte
{
public:
	ScreenByte(uint32_t mode) : m_mode(mode),m_byte(0),m_offset(0) {}
	ScreenByte() = delete;
	~ScreenByte() {}

	// This checks the colour passed in
	uint8_t getPixelMask(const Colour& pixelColour)
	{
		return 0;
	}

	bool addPixel(const Colour& pixelColour)
	{
		switch (m_mode)
		{
		case 2:
			break;

		case 5:
			auto mask = getPixelMask(pixelColour);
			// uint8_t mask = 0b11 << m_offset;
			break;
		}

		if (++m_offset == getPixelsPerByte())
		{
			m_offset = 0;
			return true;
		}
		else
		{
			return false;
		}
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
	static std::map<Colour::BBCColour,uint8_t> m_pop;

	uint8_t m_byte;
	uint32_t m_offset;
	uint32_t m_mode;
};