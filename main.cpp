#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <chrono>
#include <regex>

// Build with g++ main.cpp lodepng.cpp -std=c++11
static constexpr char* versionString = "0.1";

void displayUsage()
{
	std::cout << "png2bbc (version " << versionString << ")" << std::endl;
	std::cout << "A utility to create BBC micro sprites from PNG images" << std::endl << std::endl;
	std::cout << "Usage: png2bbc <scriptfile>" << std::endl;
}

void processScript(const std::string& filename)
{
	std::fstream in(filename, std::ios::in);

	if (in.good())
	{
		std::string line;

		while (std::getline(in, line))
		{
			// WITH-IMAGE "image.png" CREATE-SPRITES <filename> <0-7> <x> <y> <w> <h> <num-frames> [row/col pixel-style]
			// std::regex r()
			// if (r.matches()
			//  Image i; etc
		}
	}
}

int main(int argc, char** argv)
{
	// We currently only accept one argument, the script file
	if (argc==2)
	{
		std::string filename	= argv[1];
		auto startTime			= std::chrono::steady_clock::now();

		processScript(filename);
	}
	else
	{
		displayUsage();
	}
  
  return 1;
}
