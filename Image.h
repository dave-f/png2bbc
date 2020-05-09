#pragma once

#include <vector>
#include <stdexcept>

#include "lodepng.h"
#include "Colour.h"

class Image
{
public:
    Image(const std::string& filename) : m_width(0), m_height(0)
    {
        if (lodepng::decode(m_pixels, m_width, m_height, filename) != 0)
        {
            throw std::runtime_error("Bad Image");
        }
    }
    Image() = delete;
    ~Image() = default;

public:
    Colour      getPixel(uint32_t x, uint32_t y) const;
    uint32_t    getPixelRGB(uint32_t x, uint32_t y) const;
    void        setPixel(uint32_t x, uint32_t y, Colour newPixel) {}
    uint32_t    getWidth() const { return m_width; }
    uint32_t    getHeight() const { return m_height; }

protected:
    uint32_t m_width;
    uint32_t m_height;

    // Pixels are 4 bytes per pixel, ordered RGBARGBA
    std::vector<unsigned char> m_pixels;
};
