#include <cstddef>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <chrono>
#include <regex>
#include <set>

#include "Image.h"
#include "ScreenByte.h"

// Build on g++ with -std=c++11
static constexpr char versionString[] = "1.8";

enum class PixelOrder : uint8_t
{
    Line = 1,
    PreshiftedLine,
    Block
};

void displayTitle()
{
    std::cout << "png2bbc (version " << versionString << ")" << std::endl << std::endl;
}

void displayUsage()
{
    std::cout << "A utility to create BBC micro sprites from PNG images" << std::endl << std::endl;
    std::cout << "Usage: " << std::endl;
    std::cout << "        png2bbc [-l] <scriptfile>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "        -l : List outputs from scriptfile, but do not generate them" << std::endl;
}

// Produce a block of data in character row format, useful for tiles
void processBlock(const std::shared_ptr<Image> theImage, uint32_t mode, std::shared_ptr<std::vector<Colour>> theColours, std::shared_ptr<std::map<uint32_t, uint8_t>> customColours, const std::string& binFile, bool appendMode, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t numBlocks)
{
    std::fstream outFile;
    ScreenByte currentByte(mode);
    
    outFile.exceptions(std::fstream::failbit | std::fstream::badbit);
    outFile.open(binFile, appendMode ? (std::ios::app | std::ios::binary) : (std::ios::out | std::ios::binary));

    auto ppb = currentByte.getPixelsPerByte();

    if ( w % ppb != 0 )
    {
        throw std::runtime_error("Block width not a multiple of pixels per byte");
    }

    if ( h % 8 != 0 )
    {
        throw std::runtime_error("Block height not a multiple of eight");
    }

    auto blocksX  = w / ppb;
    auto blocksY  = h / 8;

    for (uint32_t k=0; k<numBlocks; ++k)
    {
        for (uint32_t j = 0; j<blocksY; ++j)
        {
            for (uint32_t i = 0; i<blocksX; ++i)
            {
                for (uint32_t n = 0; n<8; ++n)
                {
                    for (uint32_t m = 0; m<ppb; ++m)
                    {
                        auto thisPixel = theImage->getPixel(x + (i*ppb)+m, y + (j*8)+n);
                        bool usingCustomColour = false;
                        ptrdiff_t s;

                        // Custom colours take precedence so look at these first
                        auto customIt = customColours->find(thisPixel.getRawRGB());

                        if (customIt != customColours->end())
                        {
                            s = customIt->second;
                            usingCustomColour = true;
                        }

                        if (!usingCustomColour)
                        {
                            auto it = std::find(theColours->begin(), theColours->end(), thisPixel);

                            if (it != theColours->end())
                            {
                                s = std::distance(theColours->begin(), it);
                            }
                            else
                            {
                                throw std::runtime_error("Unsupported colour");
                            }
                        }

                        if (currentByte.addPixel(s))
                        {
                            auto theByte = currentByte.readByte();

                            outFile.write(reinterpret_cast<const char*>(&theByte), 1);
                        }
                    }
                }
            }
        }
        x += w;
    }
}

// Produce a block of data in line format, useful for sprites
void processSprite(const std::shared_ptr<Image> theImage, uint32_t mode, std::shared_ptr<std::vector<Colour>> theColours, std::shared_ptr<std::map<uint32_t,uint8_t>> customColours, const std::string& binFile, bool appendMode, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t numFrames, uint32_t numShifts)
{
    std::fstream outFile;
    ScreenByte currentByte(mode);

    outFile.exceptions(std::fstream::failbit | std::fstream::badbit);
    outFile.open(binFile, appendMode ? (std::ios::app | std::ios::binary) : (std::ios::out | std::ios::binary));

    if ( (w % currentByte.getPixelsPerByte()) != 0 )
    {
        throw std::runtime_error("Sprite width not a multiple of pixels per byte");
    }

    for (uint32_t k = 0; k<numFrames; ++k)
    {
        for (uint32_t j = y; j < y + h; ++j)
        {
            // If we are shifting, write extra black pixels into each row
            ptrdiff_t blackPixelValue;

            if (numShifts>0)
            {
                auto it = std::find(theColours->begin(), theColours->end(), Colour::BBCColour::Black);

                if (it == theColours->end())
                {
                    throw std::runtime_error("No black colour found (needed for shifting)");
                }
                else
                {
                    blackPixelValue = std::distance(theColours->begin(), it);

                    for (uint32_t i=0; i<numShifts; ++i)
                    {
                        currentByte.addPixel(blackPixelValue);
                    }
                }
            }

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

            // Check if theres any residual pixels to write
            if ((numShifts > 0) && !currentByte.isEmpty())
            {
                while (!currentByte.addPixel(blackPixelValue))
                    ;
                
                auto theByte = currentByte.readByte();
                outFile.write(reinterpret_cast<const char*>(&theByte),1);
            }
        }
        x += w;
    }

    outFile.close();
}

bool processScript(const std::string& filename, std::set<std::string>& outputs, bool buildFiles)
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
        std::shared_ptr<std::map<uint32_t,uint8_t>> customColours = std::make_shared<std::map<uint32_t, uint8_t>>();
        PixelOrder currentPixelOrder;
        std::string currentPixelOrderStr;

        // COMMENT or BLANK LINE
        std::regex rxIgnore(R"(([[:space:]]*;.*|[[:space:]]*))");
        // MODE <GRAPHICS MODE>
        std::regex rxModeCommand(R"([[:space:]]*MODE[[:space:]]+([01245]).*)",std::regex_constants::icase);
        // COLOURS <colour>[,...]
        std::regex rxColoursCommand(R"([[:space:]]*COLOURS[[:space:]]+(.*))",std::regex_constants::icase);
        // IMAGE <filename>
        std::regex rxImageCommand(R"([[:space:]]*IMAGE[[:space:]]+([^[:space:]]+).*)",std::regex_constants::icase);
        // CREATE-FILE / APPEND-FILE <filename> FROM-DATA <x> <y> <w> <h> <num-frames> [DATA-ORDER <BLOCK | LINE | PRESHIFTED>]
        std::regex rxCreateCommand(R"([[:space:]]*(CREATE-FILE|APPEND-FILE)[[:space:]]+([^[:space:]]+)[[:space:]]+FROM-DATA[[:space:]]+([0-9]+)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+)([[:space:]]+DATA-ORDER[[:space:]]+(BLOCK|LINE|PRESHIFTED))?.*)",std::regex_constants::icase);
        // CUSTOM-COLOUR <hex-colour> <colour number>
        std::regex rxCustomColourCommand(R"([[:space:]]*CUSTOM-COLOUR[[:space:]]+([[:xdigit:]]{6})[[:space:]]+([0-9]{1,2}).*)");

        while (!in.eof() && std::getline(in, currentLine))
        {
            ++currentLineNumber;
            std::smatch m;

            if (std::regex_match(currentLine,rxIgnore))
            {
                // Just a comment or an empty line; ignore
            }
            else if (std::regex_match(currentLine,m,rxModeCommand))
            {
                currentMode = std::stoi(m[1].str());
            }
            else if (std::regex_match(currentLine,m,rxColoursCommand))
            {
                std::regex words_regex("[^ ,]+");
                auto colourList = m[1].str();
                std::sregex_iterator s(colourList.begin(),colourList.end(), words_regex);
                auto se = std::sregex_iterator();

                currentColours->clear();
                customColours->clear();

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
            else if (std::regex_match(currentLine,m,rxCustomColourCommand))
            {
                uint32_t colour;
                auto value = std::stoi(m[2].str());

                std::stringstream ss;
                ss << std::hex << m[1].str();
                ss >> colour;

                customColours->insert_or_assign(colour,value);
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

                if (currentColours->empty())
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

                std::string createModeStr = m[1].str();

                for (auto& c : createModeStr)
                {
                    c = toupper(c);
                }

                bool appendMode   = (createModeStr == "APPEND-FILE");
                auto outputFile   = m[2].str();
                uint32_t x        = std::stoi(m[3].str());
                uint32_t y        = std::stoi(m[4].str());
                uint32_t w        = std::stoi(m[5].str());
                uint32_t h        = std::stoi(m[6].str());
                uint32_t frames   = std::stoi(m[7].str());

                if (m[8].str().find("BLOCK") != std::string::npos)
                {
                    currentPixelOrder = PixelOrder::Block;
                    currentPixelOrderStr = "Block";
                }
                else if (m[8].str().find("PRESHIFTED") != std::string::npos)
                {
                    currentPixelOrder = PixelOrder::PreshiftedLine;
                    currentPixelOrderStr = "Preshifted";
                }
                else
                {
                    currentPixelOrder = PixelOrder::Line;
                    currentPixelOrderStr = "Line";
                }

                if (currentPixelOrder == PixelOrder::Block)
                {
                    outputs.insert(outputFile);

                    if (buildFiles)
                    {
                        processBlock(currentImage, currentMode, currentColours, customColours, outputFile, appendMode, x, y, w, h, frames);
                    }
                }
                else if (currentPixelOrder == PixelOrder::PreshiftedLine)
                {
                    auto ppb = Colour::getPixelsPerByteForMode(currentMode);

                    for (uint32_t i=0; i<ppb; ++i)
                    {
                        std::string thisOutputFile = outputFile + "_" + std::to_string(i);
                        outputs.insert(thisOutputFile);

                        if (buildFiles)
                        {
                            processSprite(currentImage, currentMode, currentColours, customColours, thisOutputFile, appendMode, x, y, w, h, frames, i);
                        }
                    }
                }
                else
                {
                    outputs.insert(outputFile);

                    if (buildFiles)
                    {
                        processSprite(currentImage, currentMode, currentColours, customColours, outputFile, appendMode, x, y, w, h, frames, 0);
                    }
                }

                if (buildFiles)
                {
                    std::cout << "Wrote " << frames << " sprite(s) (" << currentPixelOrderStr << " format) to " << outputFile << std::endl;
                }
            }
            else
            {
                throw std::runtime_error("Unrecognised command");
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
    // With one argument, this must be the script file
    if (argc==2)
    {
        displayTitle();

        std::string filename = argv[1];
        auto startTime = std::chrono::steady_clock::now();
        std::set<std::string> outputs;

        if (processScript(filename,outputs,true))
        {
            auto msTaken = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
            std::cout << outputs.size() << " file(s) created in " << msTaken << "ms" << std::endl;

            return 0;
        }
    }
    // With two arguments, we should be being asked to list outputs
    else if (argc == 3)
    {
        std::string filename;
        std::string argOne = argv[1];
        std::string argTwo = argv[2];
        bool argOK = false;

        if (argOne=="-l" || argOne=="-L")
        {
            filename = argTwo;
            argOK = true;
        }
        else if (argTwo == "-l" || argTwo=="-L")
        {
            filename = argOne;
            argOK = true;
        }

        if (argOK)
        {
            std::set<std::string> outputs;

            if (processScript(filename, outputs, false))
            {
                for (const auto& i : outputs)
                {
                    std::cout << " " << i;
                }
            }
            std::cout << std::endl;
        }
        else
        {
            displayTitle();
            displayUsage();
        }
    }
    else
    {
        displayTitle();
        displayUsage();
    }

    return 1;
}
