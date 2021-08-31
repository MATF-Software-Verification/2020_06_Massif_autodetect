#include <iostream>
#include <utils/options_desc.hpp>
#include <utils/executor_manager.hpp>
#include <parser/parser.hpp>
#include <analyzer/analyzer.hpp>

int main(int argc, char** argv)
{
    CommandLineOpts cmdLineOpts;
    CommandLineOpts::CommandLineStatus cmdLineStatus;
    if ((cmdLineStatus = cmdLineOpts.parse(argc, argv)) == CommandLineOpts::CommandLineStatus::eSTATUS_FAIL) {
        return 1;
    }


    ExecutorManager* manager;
    std::string s("");
    switch (cmdLineStatus) {
        case CommandLineOpts::CommandLineStatus::eSTATUS_MASSIF:
        {
            MassifParser massParser(cmdLineOpts.getMassifFile());
            //massParser.parse();
            //std::cout << massParser;
            
            if (MassifParser::ParserStatus::ePARSER_OK != massParser.parse()) {
                std::cerr << "Failed parsing " << cmdLineOpts.getMassifFile() << std::endl;
                return 1;
            }
    
            FixifAnalyzer* analyzer = new MassifAnalyzer(std::move(massParser.getSnapshots()), std::move(massParser.getPeakSnapshot()));
            analyzer->run();
            delete analyzer;
            break;
        }
        case CommandLineOpts::CommandLineStatus::eSTATUS_EXE:
        {  
            manager = new ExecutorManager(cmdLineOpts.getExecFile());
            manager->execOperation();
            auto pid = manager->getPid();
        
            MassifParser massParser("massif.out." + std::to_string(pid));
            if (MassifParser::ParserStatus::ePARSER_OK != massParser.parse()) {
                std::cerr << "Failed parsing " << "massif.out." + std::to_string(pid) << std::endl;
                return 1;
            } else {
                FixifAnalyzer*  massifAnalyzer = new MassifAnalyzer( std::move(massParser.getSnapshots()), std::move(massParser.getPeakSnapshot()));
                massifAnalyzer->run();
                delete massifAnalyzer;
            }

            XtmemoryParser xtParser("xtmemory.kcg." + std::to_string(pid));
            if (XtmemoryParser::ParserStatus::ePARSER_OK != xtParser.parse()) {
                std::cerr << "Failed parsing " << "xtmemory.kcg." + std::to_string(pid) << std::endl;
                return 1; 
            } else {
                FixifAnalyzer* xtmemoryAnalyzer = new XtMemoryAnalyzer(std::move(xtParser.getTree().xNodes));
                xtmemoryAnalyzer->run();
                delete xtmemoryAnalyzer;
            }    

            delete manager;
            break;
        }
        default:
            break;
    }


    return 0;
}
