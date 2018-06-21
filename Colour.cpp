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

std::map<std::string, Colour::BBCColour> Colour::m_colourMapStr = 
{
    {"BLACK",   BBCColour::Black},
    {"RED",     BBCColour::Red},
    {"GREEN",   BBCColour::Green},
    {"YELLOW",  BBCColour::Yellow},
    {"BLUE",    BBCColour::Blue},
    {"MAGENTA", BBCColour::Magenta},
    {"CYAN",    BBCColour::Cyan},
    {"WHITE",   BBCColour::White}
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

Colour::Colour(uint32_t rawRGB)
{
	uint32_t key = (rawRGB << 8) | 0xff;

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


Colour::Colour(const std::string& colourName)
{
    auto it = m_colourMapStr.find(colourName);

    if (it != m_colourMapStr.end())
    {
        m_internalColour = it->second;
    }
    else
    {
        throw std::runtime_error("Invalid BBC Colour");
    }
}

uint32_t Colour::getNumberOfColoursForMode(uint32_t mode)
{
    switch (mode)
    {
        case 0:
        case 4:
            return 2;
            break;

        case 1:
        case 5:
            return 4;
            break;

        case 2:
            return 16;
            break;

        default:
            return 0;
    }
}

uint32_t Colour::getPixelsPerByteForMode(uint32_t mode)
{
    switch (mode)
    {
    case 0:
    case 4:
        return 8;
        break;

    case 1:
    case 5:
        return 4;
        break;

    case 2:
        return 2;
        break;

    default:
        return 0;
    }
}
