#include <iostream>
#include <parser/parser.hpp>
#include <parser/rules.hpp>
#include <boost/spirit/home/x3.hpp>


namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;

MassifParser::MassifParser(const std::string& path)
    :mDesc(""), mCmd(""), mTimeUnit(""), mFile(path)
{
    if (!mFile.is_open()) {
        status = ParserStatus::ePARSER_FAIL;
        std::cerr << "Fail to open " << path << "\n";
    }
    mContent << mFile.rdbuf();
}

MassifParser::~MassifParser()
{
    if (mFile.is_open()) {
        mFile.close();
    }
}

MassifParser::ParserStatus MassifParser::parse()
{
    
    typedef std::tuple<int, int, std::string> header; //TreeHeader
    typedef std::tuple<int, int, std::string, std::string, std::string, int> node;
    typedef std::tuple<header, boost::optional<std::vector<node>>> tree;
    typedef std::tuple<ulong, int, int, int, tree> snapshot;

    std::pair<std::string, std::vector<snapshot>> result;
    bool parseStatus = x3::parse(mContent.str().begin(),
                                 mContent.str().end(),
                                 massifRules::rHeader
                                 >> 
                                 +(massifRules::rSnapshot)
                                
    , result);

    return ParserStatus::ePARSER_OK;
}