#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <parser/snapshot.hpp>
#include <parser/node.hpp>
#include <boost/algorithm/string.hpp>

class MassifParser
{
public:
    MassifParser() = default;
    MassifParser(const std::string& path);
    ~MassifParser();
    enum class ParserStatus {
        ePARSER_OK,
        ePARSER_FAIL
    };
    
    ParserStatus parse();
    friend std::ostream& operator<< (std::ostream &out, const MassifParser &mp);
    inline const std::vector<std::shared_ptr<Snapshot>>& getSnapshots() {
        if (status == ParserStatus::ePARSER_OK) {
            return mSnapshots;
        } else {
            static decltype(mSnapshots) result{};
            return result;
        }
    }

    inline const std::shared_ptr<Snapshot>& getPeakSnapshot() {
        if (status == ParserStatus::ePARSER_OK) {
            return mPeakSnapshot;
        } else {
            static decltype(mPeakSnapshot) result{};
            return result;
        }
    }

    void run();
private:
    std::string mDesc;
    std::string mCmd;
    std::string mTimeUnit;
    std::vector<std::shared_ptr<Snapshot>> mSnapshots;
    std::shared_ptr<Snapshot> mPeakSnapshot;
    
    std::ifstream mFile;
    std::stringstream mContent;
    ParserStatus status = ParserStatus::ePARSER_OK;
};

class XtmemoryParser
{
public:
    XtmemoryParser() = default;
    XtmemoryParser(const std::string& path);
    ~XtmemoryParser();

    enum class ParserStatus {
        ePARSER_OK,
        ePARSER_FAIL
    };
    
    ParserStatus parse();
    friend std::ostream& operator<< (std::ostream &out, const XtmemoryParser &mp);
    inline const XTreeMemory& getTree() {
        if (status == ParserStatus::ePARSER_OK) {
            return xTree;
        } else {
            static decltype(xTree) result{};
            return result;
        }
    }

    void run();
    
    XTreeMemory xTree;

    std::ifstream xFile;
    std::stringstream xContent;
    ParserStatus status = ParserStatus::ePARSER_OK;
};