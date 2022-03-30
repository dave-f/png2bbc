#pragma once

#include <cstdint>
#include <string>
#include <map>
#include <stdexcept>

class Colour
{
public:
    enum class BBCColour : uint32_t
    {
        Black   = 0x000000,
        Red     = 0xff0000,
        Green   = 0x00ff00,
        Yellow  = 0xffff00,
        Blue    = 0x0000ff,
        Magenta = 0xff00ff,
        Cyan    = 0x00ffff,
        White   = 0xffffff,
    };

    bool operator==(const Colour& other) const
    {
        return m_internalColour == other.m_internalColour;
    }

    Colour() = delete;
    Colour(const std::string& colourName);
    Colour(const BBCColour& c) : m_internalColour(c) {}
    Colour(uint8_t r, uint8_t g, uint8_t b);
    Colour(uint32_t rawRGB);
    ~Colour() = default;

    uint32_t getRawRGB() const
    {
        return static_cast<uint32_t>(m_internalColour);
    }

    static uint32_t getNumberOfColoursForMode(uint32_t mode);
    static uint32_t getPixelsPerByteForMode(uint32_t mode);

private:

    static std::map<uint32_t, BBCColour> m_colourMap;
    static std::map<std::string, BBCColour> m_colourMapStr;

    BBCColour m_internalColour;
};
