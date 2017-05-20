#include <iostream>
#include <vector>
#include <thread>
#include <memory>
#include "lodepng.h"

// Build with g++ main.cpp lodepng.cpp -std=c++11

class RGBA // TODO: Support BBC micro colours (i.e red/white/black/cyan/magenta etc) as constants
{
public:
  enum class BBCColour : uint32_t
  {
    Black = 0x00,
    White = 0xffffff
  };

  RGBA() {}
  RGBA(const BBCColour&) {}
  RGBA(uint32_t rawRGBA) {}
  RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a=0xff) {}
  ~RGBA() {}

public:
  bool isValidBBCColour() const { return true; }
};

class Image
{
public:
  Image(const char* filename) : m_width(0), m_height(0), m_errorCode(-1)
  {
    m_errorCode = lodepng::decode(m_pixels, m_width, m_height, filename);
  }
  Image() = delete;
  ~Image() {}

public:
  bool isValid() const { return (m_errorCode==0); }
  RGBA getPixel(uint32_t x, uint32_t y) const {RGBA thisPixel; return thisPixel;}
  void setPixel(uint32_t x, uint32_t y, RGBA newPixel) {}
  uint32_t getWidth() const { return m_width; }
  uint32_t getHeight() const { return m_height; }

protected:
  uint32_t m_width;
  uint32_t m_height;
  uint32_t m_errorCode;

  // Pixels are 4 bytes per pixel, ordered RGBARGBA
  std::vector<unsigned char> m_pixels;
};


int main(int argc, char** argv)
{
  Image theImage("file.png");

  if (theImage.isValid())
    {
      std::cout << "Dimensions: " << theImage.getWidth() << "," << theImage.getHeight() << std::endl;
    }
  else
    {
      std::cout << "Failed to load png" << std::endl;
    }
  
  return 1;
}
