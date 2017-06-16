#include "Image.h"

Colour Image::getPixel(uint32_t x, uint32_t y) const
{
    auto i = (y*m_width*4) + (x*4);

    if (i > m_pixels.size())
    {
        throw std::runtime_error("Bad pixel co-ords");
    }
    else
    {
        Colour r(m_pixels[i], m_pixels[i + 1], m_pixels[i + 2]);

        return r;
    }
}
