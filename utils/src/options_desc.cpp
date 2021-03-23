#include <iostream>
#include <utils/options_desc.hpp>
#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/home/x3.hpp>

#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS

CommandLineOpts::CommandLineOpts()
    :mOptions(""), mMassifFile(""), mSourceFileName("")
{
      setup();
}

void CommandLineOpts::setup()
{
    po::options_description options("Program Options");
    options.add_options()
    ("help,h", "Display help menu")
    ("version,v", "Display program version")
    ("massif,m", po::value<std::string>(&mMassifFile), "Path to massif file")
    ("source,s", po::value<std::string>(&mSourceFileName), "Path to source C/C++ file")
    ;
    mOptions.add(options);
}

CommandLineOpts::CommandLineStatus CommandLineOpts::parse(int argc, char** argv)
{
    CommandLineStatus st = CommandLineStatus::eSTATUS_OK;
    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, mOptions), vm);
        po::notify(vm);

        if (vm.count("help") or vm.size() == 0) {
            std::cout << mOptions << "\n";
            return CommandLineStatus::eSTATUS_HELP;
        }

        if (vm.count("version")) {
            std::cout <<"FIXIF v" << FIXIF_VERSION << "\n";
            return CommandLineStatus::eSTATUS_VERSION;
        }

        if (vm.count("massif")) {
            auto errMsg = validateMassifFile();
            if (errMsg.has_value()) {
                std::cerr << "Error: " << errMsg.value() << '\n';
                return CommandLineStatus::eSTATUS_FAIL;  
            }
        } else if (vm.count("source")) {
            //TODO Validation of source file
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: parsing error " << e.what() << '\n';
        return CommandLineStatus::eSTATUS_FAIL;
    }
    catch (...) {
        std::cerr << "Error: parsing error unknown " << '\n';
        return CommandLineStatus::eSTATUS_FAIL;
    }
    
    return st;
}


std::optional<std::string> CommandLineOpts::validateMassifFile()
{
    if (!boost::filesystem::exists(mMassifFile)) return std::optional<std::string>{mMassifFile + " doesn't exist"};
    using boost::spirit::x3::phrase_parse;
    using boost::spirit::x3::digit;
    using boost::spirit::x3::lit;
    using boost::spirit::x3::ascii::space;
    boost::filesystem::path p(mMassifFile);
    auto massifFileName = p.filename().string();
    bool r = phrase_parse(std::begin(massifFileName), std::end(massifFileName),
        "massif.out." >> +(digit),  
        space
    );   
    if (!r) return std::optional<std::string>{"Massif file must be in format massif.out.*"};
    return std::nullopt;
}