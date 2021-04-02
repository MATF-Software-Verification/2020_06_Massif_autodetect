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

  //  using massifHeader = std::tuple<std::string, std::string, std::string>; //rHeader
  //  using treeHeader = std::tuple<int,int,std::string>; //rTreeHeader
  //  using node = std::tuple<int, int, std::string, std::string, std::string, int>; //rTreeNode
  //  using tree = x3::optional<std::tuple<treeHeader, std::vector<node>>>; //-(rTreeStructure)
  //  using snapshot = std::tuple<int, ulong, int, int, int, tree>; //rSnapshot

    //using tree = std::optional<std::tuple<int,int,std::string, std::vector<std::tuple<int,int,std::string, std::string, std::string, int>>>>;
    //using result = std::tuple<std::string, std::string, std::string,  //rHeader
                            //std::vector<std::tuple<int,ulong,int,int,int, tree>>>; //rSnapshots
    
    // bez optional
    using result = std::tuple<std::string, std::string, std::string,  //rHeader
                             std::vector<std::tuple<int,ulong,int,int,int,
                             int,int,std::string, std::vector<std::tuple<int,int,std::string, std::string, std::string, int>>>>>; //rSnapshots
    
    result res;

    auto const content = std::string("desc: (none) \
    cmd: ./a.out \
    time_unit: i \
    #----------- \
    snapshot=0 \
    #----------- \
    time=0 \
    mem_heap_B=0 \
    mem_heap_extra_B=0 \
    mem_stacks_B=0 \
    heap_tree=empty\
    #----------- \
    snapshot=1 \
    #----------- \
    time=139703 \
    mem_heap_B=1000 \
    mem_heap_extra_B=16 \
    mem_stacks_B=0 \
    heap_tree=empty ");
    
    bool parseStatus = x3::parse(content.begin(),
                                content.end(),
                                massifRules::rHeader >> *(massifRules::rSnapshot),
                                res);
/*
    std::string first;
    std::string second;
    std::string third;
    std::vector<std::tuple<int,ulong,int,int,int,tree>> fourth;
    std::tie (first,second,third,fourth) = res;   // unpacking tuple into variables
*/
    
    return ParserStatus::ePARSER_OK;
}