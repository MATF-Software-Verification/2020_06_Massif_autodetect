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
            break;
        case CommandLineOpts::CommandLineStatus::eSTATUS_SOURCE:
            manager = new SouceExecutorManager(s);
            break;
        case CommandLineOpts::CommandLineStatus::eSTATUS_EXE:
            manager = new ExecutorManager(cmdLineOpts.getExecFile());
            break;
        default:
            break;
    }
    manager->execOperation();
    
    // MassifParser massParser(cmdLineOpts.getMassifFile());
    // massParser.parse();

    // std::cout << massParser;
    delete manager;
    return 0;
}
