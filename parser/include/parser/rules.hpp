#pragma once
#include <boost/spirit/home/x3.hpp>

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;
using x3::lit;
using x3::int_;
using x3::ulong_;


/**
 * @namespace massifRules
 * @brief Contains all grammar rules that are required for parsing massif file 
 * using boost spirit x3 library
*/
namespace massifRules
{
    auto const rDesc = lit("desc:") >> *(x3::print) >> "\n";
    auto const rCmd = lit("cmd:") >> *(x3::print) >> "\n";
    auto const rTimeUnit = lit("time_unit:") >> *(x3::print) >> "\n";
    
    auto const rHeader = rDesc >> rCmd >> rTimeUnit;

    //---------------------------------------------------------------------------------
    
    auto const rDashedLine = lit("#-----------") >> "\n";
    auto const rTitle = rDashedLine >> lit("snapshot=") >> int_ >> "\n" >> rDashedLine; 

    auto const rTime = lit("time=") >> ulong_ >> "\n"; 
    auto const rMemHeapB = lit("mem_heap_B=") >> int_ >> "\n";    
    auto const rMemHeapExtra = lit("mem_heap_extra_B=") >> int_ >> "\n";
    auto const rMemStacks = lit("mem_stacks_B=") >> int_ >> "\n";

    //auto const rEmpty = lit("empty");
    //auto const rDetailed = lit("detailed");
    //auto const rPeak = lit("peak");
    auto const rHeapTree = lit("heap_tree=") >> *(x3::print) >> "\n";

    auto const rSnapshotInfo = rTitle >> rTime >> rMemHeapB >> rMemHeapExtra
                            >> rMemStacks >> rHeapTree;

    auto const rTreeHeader = lit("n") >> int_ >> lit(": ") >> int_ >> *(x3::print) >> "\n";
    auto const rTreeNode =  *x3::lit(" ") >> lit("n") >> int_ >> lit(": ") >> int_ >> 
                            *(x3::print - ":") >> ":" >> *(x3::print - "(") >> 
                            "(" >>  *(x3::print - ":") >> ":" >> int_ >> ")" >> "\n";                             
    auto const rExtraLine =  *x3::lit(" ") >> (lit("n") >> int_ >> lit(": ") >> *(x3::print) >> "\n");

    
} // namespace massifRules

/**
 * @namespace xtmemoryRules
 * @brief Contains all grammar rules that are required for parsing xtreememory file 
 * using boost spirit x3 library
*/
namespace xtmemoryRules
{
    auto const rDesc = lit("# callgrind format") >> "\n"
                    >> lit("version: ") >> int_ >> "\n" 
                    >> lit("creator: ") >> *(x3::print) >> "\n"
                    >> lit("pid: ") >> int_ >> "\n" 
                    >> lit("cmd: ") >> *(x3::print) >> "\n" >> "\n";

    auto const rEvents = lit("positions: ") >> *(x3::print) >> "\n"
                      >> lit("event: curB : currently allocated Bytes\n") 
                      >> lit("event: curBk : currently allocated Blocks\n")
                      >> lit("event: totB : total allocated Bytes\n") 
                      >> lit("event: totBk : total allocated Blocks\n")
                      >> lit("event: totFdB : total Freed Bytes\n")
                      >> lit("event: totFdBk : total Freed Blocks\n")
                      >> lit("events: curB  curBk  totB  totBk  totFdB  totFdBk \n");

    auto const rHeader= rDesc >> rEvents;

    auto const rFile = lit("fl=") >> "(" >> int_ >> ")" >> *(x3::print) >> "\n";
    auto const rFunction = lit("fn=") >> "(" >> int_ >> ")" >> *(x3::print) >> "\n";
    auto const rLineNumber = int_ >> "\n";
    
    auto const rCfi = lit("cfi=") >> "(" >> int_ >> ")" >> *(x3::print) >> "\n";
    auto const rCfn = lit("cfn=") >> "(" >> int_ >> ")" >> *(x3::print) >> "\n";
    auto const rCalls = lit("calls=") >> int_ >> " " >> int_ >> "\n";

    auto const rAllocatedBytesInfo = int_ >> " " >> int_ >> " " >> int_ >> " " >> int_ >> " " >> int_ >> " " >> int_ >> " " >> int_ >> "\n";
    auto const rTotals = "totals: " >> int_ >> " " >>  int_ >> " " >> int_ >> " " >>  int_ >> " "  >> int_ >> " " >>  int_ >>  "\n";
    auto const rDirectAlloc = rFile >> rFunction >> rAllocatedBytesInfo;  
    auto const rSubAlloc = rFile >> rFunction >> rLineNumber >> rCfi >> rCfn >> rCalls >> rAllocatedBytesInfo;
}