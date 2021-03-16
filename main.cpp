#include <iostream>


#include <parser/parser.hpp>
#include <utils/options_desc.hpp>


int main(int argc, char** argv)
{
    CommandLineOpts cmdLineOpts;
    cmdLineOpts.parse(argc, argv);
    return 0;
}
