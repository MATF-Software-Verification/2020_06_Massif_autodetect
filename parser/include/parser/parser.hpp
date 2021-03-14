#pragma once
#include <string>
#include <vector>
#include <memory>

#include <parser/snapshot.hpp>


class MassifParser
{
public:
    MassifParser() = default;

private:
    std::string mDesc;
    std::string mCmd;
    std::string mTimeUnit;
    std::vector<std::shared_ptr<Snapshot>> mSnapshots;
};