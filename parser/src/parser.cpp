#include <iostream>
#include <parser/parser.hpp>
#include <parser/rules.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/include/std_tuple.hpp>

#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/include/at.hpp>

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;
namespace fusion = boost::fusion;

MassifParser::MassifParser(const std::string& path)
    :mDesc(""), mCmd(""), mTimeUnit(""), mPeakSnapshot(nullptr) ,mFile(path)
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


std::ostream& operator<< (std::ostream &out, const MassifParser &mp){
    for(auto snapshot: mp.mSnapshots){
        out << *snapshot.get();
    }

    return out;
}

MassifParser::ParserStatus MassifParser::parse()
{
    using boost::spirit::x3::_attr;
    std::shared_ptr<Snapshot> currentSnapshot;

    auto snapshotInfo = [&](auto& ctx){
        auto attr = _attr(ctx);
        auto title = fusion::at_c<0>(attr);
        auto time = fusion::at_c<1>(attr);
        auto memHeapB = fusion::at_c<2>(attr);
        auto memHeapExtra = fusion::at_c<3>(attr);
        auto memStacks = fusion::at_c<4>(attr);
        std::string snapshotType = fusion::at_c<5>(attr);

        currentSnapshot = std::make_shared<Snapshot>(title, time, memHeapB, memHeapExtra, memStacks);
        if(!snapshotType.compare("peak")){
            this->mPeakSnapshot = currentSnapshot;
        }
        else{ 
            this->mSnapshots.push_back(currentSnapshot);
        }
    };

    auto treeHeader = [&](auto& ctx) {
        auto attr = _attr(ctx);
        auto number = fusion::at_c<0>(attr);
        auto bytes = fusion::at_c<1>(attr);
        auto message = fusion::at_c<2>(attr);

        currentSnapshot->changeHeaderInfo(number, bytes, message);
    };

    auto treeNode = [&](auto& ctx) {
        auto attr = _attr(ctx);

        auto number = fusion::at_c<0>(attr);
        auto bytes = fusion::at_c<1>(attr);
        auto location = fusion::at_c<2>(attr);
        auto function = fusion::at_c<3>(attr);
        auto file = fusion::at_c<4>(attr);
        auto line = fusion::at_c<5>(attr);

        currentSnapshot->addTreeNode(number,bytes,location,function,file,line);
    };

    auto extraLine = [](auto& ctx) {
        auto attr = _attr(ctx);

        auto number = fusion::at_c<0>(attr);
        auto message = fusion::at_c<1>(attr);

        // std::cout << "ExtraLineNumber: " << number << std::endl 
        //           << "ExtraLineMessage: " << message << std::endl << std::endl; ; 
    };

    auto header = [&](auto& ctx){ 
        auto attr = _attr(ctx);
        this->mDesc = fusion::at_c<0>(attr);
        this->mCmd = fusion::at_c<1>(attr);
        this->mTimeUnit = fusion::at_c<2>(attr);
    };     

    auto const content = MassifParser::mContent.str();
    bool parseStatus = x3::parse(content.begin(),
                                content.end(),
                                massifRules::rHeader [header] >> 
                                *(massifRules::rSnapshotInfo [snapshotInfo] >> 
                                ((massifRules::rTreeHeader [treeHeader]
                                    >> *(massifRules::rTreeNode [treeNode] | massifRules::rExtraLine [extraLine])) | "")));

    // rule = header >> *(snapshot)
    // snapshot = snapshotInfo >> optional(tree)
    // tree = treeHeader >> *(treeNode | extraLine)

    return ParserStatus::ePARSER_OK;
};

