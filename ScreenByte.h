#pragma once

#include <array>

class ScreenByte
{
public:
	ScreenByte(uint32_t mode) : m_mode(mode),m_byte(0),m_offset(0) {}
	ScreenByte() = delete;
	~ScreenByte() {}

	static constexpr std::array<uint8_t, 2> twoColourModeValues   = { 0b0, 0b1 };
	static constexpr std::array<uint8_t, 4> fourColourModeValues  = { 0b00000, 0b00001, 0b10000, 0b10001 };
	static constexpr std::array<uint8_t, 8> eightColourModeValues = { 0b00000000, 0b00000001, 0b00000100, 0b00000101, 0b00010000, 0b00010001, 0b00010100, 0b00010101 };

	bool addPixel(uint8_t pixelValue)
	{
		if (pixelValue > Colour::getNumberOfColoursForMode(m_mode))
		{
			throw std::exception("Pixel value too high for this mode");
		}

		switch (m_mode)
		{
		case 0:
		case 3:
		case 4:
			m_byte |= twoColourModeValues[pixelValue];
			break;

		case 1:
		case 5:
			m_byte |= fourColourModeValues[pixelValue];
			break;

		case 2:
			m_byte |= eightColourModeValues[pixelValue];
			break;

		default:
			throw std::runtime_error("Bad mode");
		}

		if (++m_offset == getPixelsPerByte())
		{
			m_offset = 0;

			return true;
		}
		else
		{
			m_byte <<= 1;

			return false;
		}
	}

	uint8_t readByte()
	{
		uint8_t r = m_byte;
		m_byte    = 0;
		m_offset  = 0;

		return r;
	}

	uint32_t getPixelsPerByte() const
	{
		switch (m_mode)
		{
		case 0:
			return 8;
			break;

		case 1:
			return 4;
			break;

		case 2:
			return 2;
			break;

		case 3:
			return 8;
			break;

		case 4:
			return 8;
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
