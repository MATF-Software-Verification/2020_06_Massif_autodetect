#include <iostream>
#include <parser/parser.hpp>
#include <parser/rules.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/tuple/tuple.hpp>
#include <optional>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/include/std_tuple.hpp>

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
    using x3::lit;
    using x3::int_;
    using x3::ulong_;

    using massifHeader = std::tuple<std::string, std::string, std::string>; //rHeader
    using treeHeader = std::tuple<int, int, std::string>; //rTreeHeader
    using node = std::tuple<int, int, std::string, std::string, std::string, int>; //rTreeNode
    using tree = std::optional<std::tuple<treeHeader, std::vector<node>>>; //-(rTreeStructure)
    using snapshot = std::tuple<int, ulong, int, int, int, tree>; //rSnapshot
    // std::tuple<massifHeader, std::vector<snapshot>> result;
    // std::tuple<int, int> result;
    std::tuple<treeHeader, int> result;
    bool parseStatus = x3::parse(mContent.str().begin(),
                                 mContent.str().end(),
                                 (massifRules::rTreeHeader) >> int_
    , result);

    return ParserStatus::ePARSER_OK;
}