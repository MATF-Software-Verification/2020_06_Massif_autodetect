#include <iostream>


#include <parser/parser.hpp>
#include <utils/options_desc.hpp>


int main(int argc, char** argv)
{
    CommandLineOpts cmdLineOpts;
    if (cmdLineOpts.parse(argc, argv) != CommandLineOpts::CommandLineStatus::eSTATUS_OK) {
        return 1;
    }
    
    MassifParser massParser(cmdLineOpts.getMassifFile());
    massParser.parse();

    std::cout << massParser;

    return 0;
}
