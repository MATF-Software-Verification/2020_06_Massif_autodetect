#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <parser/snapshot.hpp>

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

    std::string mDesc;
    std::string mCmd;
    std::string mTimeUnit;
    std::vector<std::shared_ptr<Snapshot>> mSnapshots;
    std::shared_ptr<Snapshot> mPeakSnapshot;
    
    std::ifstream mFile;
    std::stringstream mContent;
    ParserStatus status = ParserStatus::ePARSER_OK;
};