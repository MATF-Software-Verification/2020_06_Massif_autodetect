#pragma once
#include <optional>
#include <boost/program_options.hpp>


namespace po = boost::program_options;



class CommandLineOpts
{
public:
    CommandLineOpts();
    ~CommandLineOpts() = default;
    enum class CommandLineStatus {
        eSTATUS_OK,
        eSTATUS_HELP,
        eSTATUS_VERSION,
        eSTATUS_FAIL
    };

    CommandLineStatus parse(int argc, char** argv);

private:
    CommandLineOpts(const CommandLineOpts& other);
    CommandLineOpts& operator=(const CommandLineOpts& other);

    /**
     * @brief Boilerplate initialization of boost program options parser
     */    
    void setup();
    
    /** 
     * @brief Method for validation of massif file if its given as argument
     */
    std::optional<std::string> validateMassifFile();

private:
    po::options_description mOptions;
    std::string mMassifFileName;
    std::string mSourceFileName;
};
