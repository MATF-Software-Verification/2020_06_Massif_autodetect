#include <iostream>

#include <parser/parser.hpp>

#include <utils/options_desc.hpp>
#include <analyzer/analyzer.hpp>

int main(int argc, char** argv)
{
    CommandLineOpts cmdLineOpts;
    if (cmdLineOpts.parse(argc, argv) != CommandLineOpts::CommandLineStatus::eSTATUS_OK) {
        return 1;
    }

    
    MassifParser massParser(cmdLineOpts.getMassifFile());
    if (MassifParser::ParserStatus::ePARSER_OK != massParser.parse()) {
        std::cerr << "Failed parsing " << cmdLineOpts.getMassifFile() << std::endl;
        return 1; 
    }
    
    /*
    XtmemoryParser xtParser(cmdLineOpts.getMassifFile());
    if (XtmemoryParser::ParserStatus::ePARSER_OK != xtParser.parse()) {
        std::cerr << "Failed parsing " << cmdLineOpts.getMassifFile() << std::endl;
        return 1; 
    }
    */

    /*
    Analyzer analyzer(std::move(xtParser.getTree()._nodes));
    analyzer.run();
    */
    
    FixifAnalyzer analyzer(std::move(massParser.getSnapshots()), std::move(massParser.getPeakSnapshot()));
    analyzer.run();
    
    
    return 0;
}
