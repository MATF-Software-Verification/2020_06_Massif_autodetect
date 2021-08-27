#include <iostream>


#include <parser/parser.hpp>
#include <utils/options_desc.hpp>
#include <utils/executor_manager.hpp>


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
            massParser.parse();
            std::cout << massParser;
            
            break;
        }
        case CommandLineOpts::CommandLineStatus::eSTATUS_EXE:
        {  
            manager = new ExecutorManager(cmdLineOpts.getExecFile());
            manager->execOperation();
            
            auto pid = manager->getPid();
            MassifParser massParser("massif.out." + std::to_string(pid));
            massParser.parse();
            std::cout << massParser;

            delete manager;
            break;
        }
        default:
            break;
    }


    return 0;
}
