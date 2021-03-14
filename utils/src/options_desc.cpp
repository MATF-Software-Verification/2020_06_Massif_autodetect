#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;


bool checkArgs(int ac, char** av)
{
    std::cout <<"FIXIF v" << FIXIF_VERSION << "\n";
    try {
        po::options_description desc("Possible opts");
        desc.add_options()
            ("help", "Help information")
            ;
        po::variables_map vm;
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 1;
        }
    } catch (std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return false;
    }

    return true;
}