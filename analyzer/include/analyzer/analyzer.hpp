#pragma once
#include <vector>
#include <algorithm>
#include <parser/snapshot.hpp>

class FixifAnalyzer 
{
public:
    FixifAnalyzer(const std::vector<std::shared_ptr<Snapshot>>& snapshots)
        : mSnapshots(snapshots) {
        mPeakSnapshot = *std::find_if(snapshots.cbegin(), snapshots.cend(), [](auto& s) { return s->isPeak; });
    }

    void run();
private:
    void processPeak();
    void processLastSnapshot();
    void processSnapshots();
    void createMap();
    std::shared_ptr<Snapshot> mPeakSnapshot;
    std::vector<std::shared_ptr<Snapshot>> mSnapshots;
};