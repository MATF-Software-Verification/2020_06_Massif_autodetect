#pragma once
#include <parser/snapshot.hpp>
#include <string>
#include <vector>

void processLastSnapshot(std::shared_ptr<Snapshot> snapshot)
{
    std::cout << "#STILL ALLOCATED#" << std::endl;
    auto stillAllocated = snapshot.get()->memHeapB;
    if(stillAllocated == 0 )
        return;
    
    std::cout << stillAllocated << " bajtova nije oslobodjeno";
    std::cout << *snapshot.get();
};


void processPeak(std::shared_ptr<Snapshot> peakSnapshot)
{
    std::cout << "#PEAK#" << std::endl;
    if(peakSnapshot == nullptr){
        std::cout << "Peak ne postoji." << std::endl;
        return;
    }
    
    std::cout << peakSnapshot.get()->memHeapB << " bajtova je alocirano" ;
    std::cout << *peakSnapshot.get();
};

std::vector<std::pair<std::string, int>> getPathOfBytes(std::shared_ptr<Tree> tree) {
    if (tree.get()->children.size() == 0) {
        std::string path = tree.get()->function + "(" + tree.get()->file + ":" + std::to_string(tree.get()->line) + ")";
        return {std::make_pair(path, tree.get()->bytes)};
    }

    std::vector<std::pair<std::string, int>> returnValue;
    for (auto child: tree.get()->children) {
        std::vector<std::pair<std::string, int>> pairs = getPathOfBytes(child);
        for (auto pair: pairs) {
            std::string path = pair.first + " ->" + tree.get()->function + "(" + tree.get()->file + ":" + std::to_string(tree.get()->line) + ")";
            returnValue.push_back(std::make_pair(path, pair.second));
        }
    }

    return returnValue;
};

std::map<std::string, int> getMapByTree(std::shared_ptr<Tree> tree) 
{

    std::map<std::string, int> mapByTree;
    for (auto child: tree.get()->children) {
        std::vector<std::pair<std::string, int>> pathsOfBytes = getPathOfBytes(child);
        for(auto x: pathsOfBytes) {
            mapByTree[x.first] = x.second;
        }
    }

    return mapByTree;
};

void processSnapshots(std::vector<std::shared_ptr<Snapshot>> snapshots)
{
    int sum = 0;
    int counter = 0;
    for(int i=0; i<snapshots.size()-1; i++)
    {
        auto diff = snapshots[i+1].get()->memHeapB - snapshots[i].get()->memHeapB;
        if (diff > 0) {
            sum += diff;
            counter++;
        }
    }
    auto averageDifference = sum/counter;

    std::cout << "#OUTLIERS#" << std::endl;
    std::cout << averageDifference << std::endl;
    
    std::vector<std::shared_ptr<Snapshot>> outliers;
    for(int i=0; i<snapshots.size()-1;i++)
    {
        auto diff = snapshots[i+1].get()->memHeapB - snapshots[i].get()->memHeapB;
        if(diff > averageDifference * 1.5){
            outliers.push_back(snapshots[i+1]);

            std::cout << diff << " bajtova je alocirano:" << std::endl; 
            if (snapshots[i+1].get()->tree != nullptr && snapshots[i].get()->tree != nullptr) {
                std::map<std::string, int> beforeInfo = getMapByTree(snapshots[i].get()->tree);
                std::map<std::string, int> currentInfo = getMapByTree(snapshots[i+1].get()->tree);
                for (auto xCurrent: currentInfo) {
                    auto xBefore = beforeInfo.find(xCurrent.first);
                    if (xBefore == beforeInfo.end()) {
                        std::cout << "  " << xCurrent.first << std::endl;
                        break;
                    }
                    if (xCurrent.second - (*xBefore).second == diff) {
                        std::cout << "  " << xCurrent.first << std::endl;
                        break;
                    }
                }
            }
        }
    }
};

std::shared_ptr<Snapshot> getFirstDetailedSnapshot(std::vector<std::shared_ptr<Snapshot>> snapshots)
{
    for(auto snapshot: snapshots) {
        if (snapshot.get()->tree != nullptr) {
            return snapshot;
        }
    }

    return nullptr;
};

std::shared_ptr<Snapshot> getLastDetailedSnapshot(std::vector<std::shared_ptr<Snapshot>> snapshots) 
{
    for (auto snapshot = snapshots.rbegin(); snapshot != snapshots.rend(); snapshot++) {
        if ((*snapshot).get()->tree != nullptr) {
            return *snapshot;
        }
    }

    return nullptr;
};

void createMap(std::vector<std::shared_ptr<Snapshot>> snapshots)
{
    std::map<std::string, int> map;

    std::shared_ptr<Snapshot> firstDetailedSnapshot = getFirstDetailedSnapshot(snapshots);
    std::shared_ptr<Snapshot> lastDetailedSnapshot = getLastDetailedSnapshot(snapshots);

    std::map<std::string, int> dealokations;

    for (int i = firstDetailedSnapshot.get()->title; i <= lastDetailedSnapshot.get()->title; i++) {
        if (snapshots[i].get()->tree == nullptr)
            continue;

        std::map<std::string, int> newMap = getMapByTree(snapshots[i].get()->tree);
        for (auto x: map) {
            if (newMap.find(x.first) == newMap.end()) {
                dealokations[x.first] = x.second;
            }
        }
    }

    


};