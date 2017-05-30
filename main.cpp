#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <chrono>
#include <regex>

#include "Image.h"

// Build on g++ with -std=c++11
static constexpr char versionString[] = "0.1";

void displayUsage()
{
	std::cout << "png2bbc (version " << versionString << ")" << std::endl;
	std::cout << "A utility to create BBC micro sprites from PNG images" << std::endl << std::endl;
	std::cout << "Usage: png2bbc <scriptfile>" << std::endl;
}

void processItem(const std::string& imageFile, uint32_t mode, const std::string& binFile, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t numFrames)
{
	Image theImage(imageFile);
}

bool processScript(const std::string& filename)
{
	bool r(false);
	std::fstream in;
	in.exceptions(std::fstream::failbit | std::fstream::badbit);

	try
	{
		in.open(filename, std::ios::in);

		std::string currentLine;

		// WITH-IMAGE <pngfile> USING-MODE <0-7> CREATE-FILE <filename> FROM-DATA <x> <y> <w> <h> <num-frames> [row/col pixel-style]
		std::regex r(R"([[:space:]]*WITH-IMAGE[[:space:]]+([^[:space:]]+)[[:space:]]+USING-MODE[[:space:]]+([0-7])[[:space:]]+CREATE-FILE[[:space:]]+([^[:space:]]+)[[:space:]]+FROM-DATA.*)");
		std::smatch m;

		while (!in.eof() && std::getline(in, currentLine))
		{
			if (std::regex_match(currentLine,m,r))
			{
				auto imageFile	= m[1].str();
				auto mode		= m[2].str();
				auto binFile	= m[3].str();
				uint32_t x = 0;
				uint32_t y = 0;
				uint32_t w = 0;
				uint32_t h = 0;
				uint32_t numFrames = 0;
				processItem(imageFile, 2, binFile, x, y, w, h, numFrames);
			}
		}

		in.close();
	}
	catch (const std::exception&)
	{
		r = false;
	}

	return r;
}

int main(int argc, char** argv)
{
	// We currently only accept one argument; the script file
	if (argc==2)
	{
		std::string filename	= argv[1];
		auto startTime			= std::chrono::steady_clock::now();

		if (processScript(filename))
		{
			auto msTaken = std::chrono::duration_cast<std::chrono::milliseconds>(startTime - std::chrono::steady_clock::now()).count();
			std::cout << "Finished in " << msTaken << "ms";
		}
		else
		{
			std::cout << "Error processing" << std::endl;
		}
	}
	else
	{
		displayUsage();
	}
  
  return 1;
}
