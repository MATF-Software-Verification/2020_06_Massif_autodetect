#pragma once
#include <boost/spirit/home/x3.hpp>

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;

namespace massifRules
{
    using x3::lit;
    using x3::int_;
    using x3::ulong_;
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

    auto const rEmpty = lit("empty");
    auto const rDetailed = lit("detailed");
    auto const rPeak = lit("peak");
    auto const rHeapTree = lit("heap_tree=") >> (rEmpty | rDetailed | rPeak) >> "\n";

    auto const rSnapshotInfo = rTitle >> rTime >> rMemHeapB >> rMemHeapExtra
                            >> rMemStacks >> rHeapTree;

    auto const rTreeHeader = lit("n") >> int_ >> lit(": ") >> int_ >> *(x3::print) >> "\n";
    auto const rTreeNode =  *x3::lit(" ") >> lit("n") >> int_ >> lit(": ") >> int_ >> 
                            *(x3::print - ":") >> ":" >> *(x3::print - "(") >> 
                            "(" >>  *(x3::print - ":") >> ":" >> int_ >> ")" >> "\n";                             
    auto const rExtraLine =  *x3::lit(" ") >> (lit("n") >> int_ >> lit(": ") >> *(x3::print) >> "\n");

    
} // namespace massifRules
