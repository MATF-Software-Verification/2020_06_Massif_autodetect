#include <iostream>
#include <parser/parser.hpp>
#include <parser/rules.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/include/std_tuple.hpp>
#include <tuple>

#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/variant.hpp>

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;
namespace fusion = boost::fusion;

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
    using boost::spirit::x3::_attr; 

    auto header = [](auto& ctx){ 
        auto attr = _attr(ctx);
        mDesc = fusion::at_c<0>(attr);
        mCmd = fusion::at_c<1>(attr);
        mTimeUnit time_unit = fusion::at_c<2>(attr);
        std::cout << mDesc << std::endl << mCmd << std::endl << mTimeUnit << std::endl;
    };    

    auto snapshots = [](auto& ctx){
        auto attr = _attr(ctx);
        auto title = fusion::at_c<0>(attr);
        auto time = fusion::at_c<1>(attr);
        auto memHeapB = fusion::at_c<2>(attr);
        auto memHeapExtra = fusion::at_c<3>(attr);
        auto memStacks = fusion::at_c<4>(attr);

        std::cout << "Title: " << title << std::endl
                  << "Time: "  << time  << std::endl
                  << "memHeapB: " << memHeapB << std::endl
                  << "memHeapExtra: " << memHeapExtra << std::endl
                  << "memStacks: " << memStacks << std::endl << std::endl;
        
        //TODO
        auto treeStructure = fusion::at_c<5>(attr); // boost::variant 
    };


    auto const content = MassifParser::mContent.str();
    bool parseStatus = x3::parse(content.begin(),
                                content.end(),
                                massifRules::rHeader [header] >> *(massifRules::rSnapshot) [snapshots]);

    return ParserStatus::ePARSER_OK;
}