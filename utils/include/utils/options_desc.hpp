#pragma once
#include <optional>
#include <boost/program_options.hpp>


namespace po = boost::program_options;


/**
 * @brief Class for interpretation of command line arguments
*/
class CommandLineOpts
{
public:
    CommandLineOpts();
    ~CommandLineOpts() = default;
    /**
     * @enum Indicator of option provided from user
    */
    enum class CommandLineStatus {
        eSTATUS_OK,
        eSTATUS_HELP,
        eSTATUS_VERSION,
        eSTATUS_FAIL,
        eSTATUS_MASSIF,
        eSTATUS_EXE
    };

    /**
     * @brief Parsing command line args function using boost library
     * @param int Number of args
     * @param char** List of args
     * @return Indicator of option
    */
    CommandLineStatus parse(int argc, char** argv);


    inline std::string getMassifFile() const
    {
        return mMassifFile;
    } 

    inline std::string getExecFile() const 
    {
        return mExecFileName;
    }
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
    std::string mMassifFile;
    std::string mExecFileName;
};
