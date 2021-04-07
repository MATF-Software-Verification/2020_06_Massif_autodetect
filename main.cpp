#include <iostream>

#include <parser/parser.hpp>
#include <utils/options_desc.hpp>
#include <parser/processing.hpp>

int main(int argc, char** argv)
{
    CommandLineOpts cmdLineOpts;
    if (cmdLineOpts.parse(argc, argv) != CommandLineOpts::CommandLineStatus::eSTATUS_OK) {
        return 1;
    }
    
    MassifParser massParser(cmdLineOpts.getMassifFile());
    massParser.parse();

    processPeak(massParser.mPeakSnapshot);
    std::cout << "---------------------------------------------------------------------" << std::endl;
    processLastSnapshot(massParser.mSnapshots.back());
    std::cout << "---------------------------------------------------------------------" << std::endl;
    processSnapshots(massParser.mSnapshots);
    
    return 0;
}
