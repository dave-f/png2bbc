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

	bool operator==(const Colour& other) const
	{
		return m_internalColour == other.m_internalColour;
	}

	Colour() = delete;
	Colour(const BBCColour& c) : m_internalColour(c) {}
	Colour(uint32_t rawRGBA) = delete;
	Colour(uint8_t r, uint8_t g, uint8_t b);
	~Colour() {}

	static std::map<uint32_t, BBCColour> m_colourMap;
	BBCColour m_internalColour;
};

class ScreenByte
{
public:
	ScreenByte(uint32_t mode) : m_mode(mode),m_byte(0),m_offset(0) {}
	ScreenByte() = delete;
	~ScreenByte() {}

	bool addPixel(uint8_t pixelIndex)
	{
		// TODO: This currently assumes mode 5
		switch (pixelIndex)
		{
		case 0:
			// Nothing
			break;

		case 1:
			m_byte |= 0b011;
			break;

		case 2:
			m_byte |= 0b110;
			break;

		case 3:
			m_byte |= 0b111;
			break;

		case 4:
		case 5:
		case 6:
		case 7:
		default:
			throw std::runtime_error("Bad pixel for this mode");
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
	static std::map<Colour::BBCColour,uint8_t> m_mode5Map;

	uint8_t m_byte;
	uint32_t m_offset;
	uint32_t m_mode;
};