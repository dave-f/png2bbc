#pragma once

#include <array>

#include "Colour.h"

class ScreenByte
{
public:
    ScreenByte(uint32_t mode) : m_mode(mode),m_byte(0),m_offset(0) {}
    ScreenByte() = delete;
    ~ScreenByte() {}

    static constexpr std::array<uint8_t, 2> twoColourModeValues      = { 0, 1 };
    static constexpr std::array<uint8_t, 4> fourColourModeValues     = { 0b00000, 0b00001, 0b10000, 0b10001 };
    static constexpr std::array<uint8_t, 16> sixteenColourModeValues = { 0b00000000, 0b00000001, 0b00000100, 0b00000101, 0b00010000, 0b00010001, 0b00010100, 0b00010101, 
                                                                         0b01000000, 0b01000001, 0b01000100, 0b01000101, 0b01010000, 0b01010001, 0b01010100, 0b01010101 };

    bool addPixel(uint8_t pixelValue)
    {
        if (pixelValue > Colour::getNumberOfColoursForMode(m_mode))
        {
            throw std::runtime_error("Pixel value too high for this mode");
        }

        switch (m_mode)
        {
        case 0:
        case 4:
            m_byte |= twoColourModeValues[pixelValue];
            break;

        case 1:
        case 5:
            m_byte |= fourColourModeValues[pixelValue];
            break;

        case 2:
            m_byte |= sixteenColourModeValues[pixelValue];
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

    bool isEmpty() const
    {
        return (m_offset==0);
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
        return Colour::getPixelsPerByteForMode(m_mode);
    }

private:
    uint8_t m_byte;
    uint32_t m_offset;
    uint32_t m_mode;
};
