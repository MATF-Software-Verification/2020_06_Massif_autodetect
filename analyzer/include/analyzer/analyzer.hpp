#pragma once
#include <vector>
#include <algorithm>
#include <map>

#include <parser/snapshot.hpp>
#include <parser/node.hpp>

class FixifAnalyzer
{
public:
    FixifAnalyzer() {}
    virtual ~FixifAnalyzer() {}
    virtual void run() = 0;
};

class MassifAnalyzer : public FixifAnalyzer
{
public:
    MassifAnalyzer(const std::vector<std::shared_ptr<Snapshot>> &snapshots, const std::shared_ptr<Snapshot> &peakSnapshot)
        : mSnapshots(snapshots), mPeakSnapshot(peakSnapshot)
    {
    }

    void run() override;

private:
    void processPeak();
    void processLastSnapshot();
    void processSnapshots();

    std::shared_ptr<Snapshot> mPeakSnapshot;
    std::vector<std::shared_ptr<Snapshot>> mSnapshots;
};

class XtMemoryAnalyzer : public FixifAnalyzer
{
public:
    XtMemoryAnalyzer(const std::shared_ptr<XTreeMemory> tree, const std::map<int, std::string>& fileMap)
        : xTree(tree), mFileMap(fileMap)
    {}
    void run() override;

private:
    std::shared_ptr<XTreeMemory> xTree;
    std::map<int, std::string> mFileMap;
    bool appendToSource();
};