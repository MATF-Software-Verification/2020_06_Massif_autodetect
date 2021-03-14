#include <iostream>


#include <parser/parser.hpp>
#include <utils/options_desc.hpp>


int main(int argc, char** argv)
{
    if (!checkArgs(argc, argv)) {
        return 1;
    }
    return 0;
}
