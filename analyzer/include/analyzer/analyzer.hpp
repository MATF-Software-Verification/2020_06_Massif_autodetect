#pragma once
#include <vector>
#include <algorithm>
#include <parser/snapshot.hpp>
#include <parser/node.hpp>

class FixifAnalyzer 
{
public:
    FixifAnalyzer(const std::vector<std::shared_ptr<Snapshot>>& snapshots, const std::shared_ptr<Snapshot>& peakSnapshot)
        : mSnapshots(snapshots), mPeakSnapshot(peakSnapshot)
    {}

    void run();

private:
    void processPeak();
    void processLastSnapshot();
    void processSnapshots();
    
    std::shared_ptr<Snapshot> mPeakSnapshot;
    std::vector<std::shared_ptr<Snapshot>> mSnapshots;
};


class Analyzer 
{
public:
    Analyzer(const std::vector<std::shared_ptr<Node>>& nodes)
        : _nodes(nodes)
    {}

    void run();
    
    std::vector<std::shared_ptr<Node>> _nodes;
};