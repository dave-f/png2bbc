#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <chrono>
#include <regex>

#include "Image.h"
#include "ScreenByte.h"

// Build on g++ with -std=c++11
static constexpr char versionString[] = "1.0";

void displayTitle()
{
	std::cout << "png2bbc (version " << versionString << ")" << std::endl << std::endl;
}

void displayUsage()
{
	std::cout << "A utility to create BBC micro sprites from PNG images" << std::endl;
	std::cout << "Usage: png2bbc <scriptfile>" << std::endl;
}

void processItem(const std::shared_ptr<Image> theImage, uint32_t mode, std::shared_ptr<std::vector<Colour>> theColours, const std::string& binFile, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t numFrames)
{
	std::fstream outFile(binFile, std::ios::out | std::ios::binary);
	outFile.exceptions(std::fstream::failbit | std::fstream::badbit);

	ScreenByte currentByte(mode);

	if (w % currentByte.getPixelsPerByte() !=0)
	{
		throw std::runtime_error("Sprite width not a multiple of pixels per byte");
	}

	for (uint32_t k = 0; k<numFrames; ++k)
	{
		for (uint32_t j = y; j < y + h; ++j)
		{
			for (uint32_t i = x; i < x + w; ++i)
			{
				auto thisPixel = theImage->getPixel(i, j);

				// The colour must be expected
				auto it = std::find(theColours->begin(), theColours->end(), thisPixel);
				ptrdiff_t s;

				if (it != theColours->end())
				{
					s = std::distance(theColours->begin(), it);
				}
				else
				{
					throw std::runtime_error("Unsupported colour");
				}

				if (currentByte.addPixel(s))
				{
					auto theByte = currentByte.readByte();

					outFile.write(reinterpret_cast<const char*>(&theByte), 1);
				}
			}
		}

		x += w;
	}

	outFile.close();
}

bool processScript(const std::string& filename)
{
	bool r(false);
	uint32_t currentLineNumber = 0;
	std::fstream in;
	in.exceptions(std::fstream::badbit);

	try
	{
		in.open(filename, std::ios::in);

		if (!in.good())
		{
			throw std::runtime_error("Cannot open script file");
		}

		std::string currentLine;
		std::shared_ptr<Image> currentImage;
		int8_t currentMode(-1);
		std::shared_ptr<std::vector<Colour>> currentColours = std::make_shared<std::vector<Colour>>();

		// MODE <0-5>
		std::regex rxModeCommand(R"([[:space:]]*MODE[[:space:]]+([0-5]).*)");
		// COLOURS <colour>[,...]
		std::regex rxColoursCommand(R"([[:space:]]*COLOURS[[:space:]]+(.*))");
		// IMAGE <filename>
		std::regex rxImageCommand(R"([[:space:]]*IMAGE[[:space:]]+([^[:space:]]+).*)");
		// CREATE-FILE <filename> FROM-DATA <x> <y> <w> <h> <num-frames> [row/col pixel-style]
		std::regex rxCreateCommand(R"([[:space:]]*CREATE-FILE[[:space:]]+([^[:space:]]+)[[:space:]]+FROM-DATA[[:space:]]+([0-9]+)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+).*)");

		while (!in.eof() && std::getline(in, currentLine))
		{
			++currentLineNumber;
			std::smatch m;

			if (std::regex_match(currentLine,m,rxModeCommand))
			{
				currentMode = std::stoi(m[1].str());
			}
			else if (std::regex_match(currentLine,m,rxColoursCommand))
			{
				std::regex words_regex("[^ ,]+");
				auto colourList = m[1].str();
				std::sregex_iterator s(colourList.begin(),colourList.end(), words_regex);
				auto se = std::sregex_iterator();

				for (auto i = s; i !=se; ++i)
				{
					auto upperString = i->str();

					for (auto & c : upperString)
					{
						c = toupper(c);
					}

					currentColours->push_back(Colour(upperString));
				}
			}
			else if (std::regex_match(currentLine,m,rxImageCommand))
			{
				currentImage = std::make_shared<Image>(m[1].str());
			}
			else if (std::regex_match(currentLine,m,rxCreateCommand))
			{
				if (currentMode == -1)
				{
					throw std::runtime_error("Bad mode");
				}

				if (!currentColours)
				{
					throw std::runtime_error("No palette set");
				}

				if (!currentImage)
				{
					throw std::runtime_error("No image");
				}

				if (currentColours->size() > Colour::getNumberOfColoursForMode(currentMode))
				{
					throw std::runtime_error("Too many colours for this mode");
				}

				auto outputFile	= m[1].str();
				uint32_t x		= std::stoi(m[2].str());
				uint32_t y		= std::stoi(m[3].str());
				uint32_t w		= std::stoi(m[4].str());
				uint32_t h		= std::stoi(m[5].str());
				uint32_t frames = std::stoi(m[6].str());

				processItem(currentImage, currentMode, currentColours, outputFile, x, y, w, h, frames);
				std::cout << "Built " << outputFile << std::endl;
			}
		}

		in.close();
		r = true;
	}
	catch (const std::exception& e)
	{
		if (currentLineNumber)
		{
			std::cout << filename << ":" << currentLineNumber << ": " << e.what() << std::endl;
		}
		else
		{
			std::cout << "Error: " << e.what() << std::endl;
		}

		r = false;
	}

	return r;
}

int main(int argc, char** argv)
{
	displayTitle();

	// We currently only accept one argument; the script file
	if (argc==2)
	{
		std::string filename	= argv[1];
		auto startTime			= std::chrono::steady_clock::now();

		if (processScript(filename))
		{
			auto msTaken = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
			std::cout << "Finished in " << msTaken << "ms" << std::endl;

			return 0;
		}
	}
	else
	{
		displayUsage();
	}
  
  return 1;
}
