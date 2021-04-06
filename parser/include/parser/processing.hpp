#pragma once
#include <parser/snapshot.hpp>

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

void processSnapshots(std::vector<std::shared_ptr<Snapshot>> snapshots)
{
    int sum = 0;
    for(int i=0; i<snapshots.size()-1; i++)
    {
        auto diff = snapshots[i+1].get()->memHeapB - snapshots[i].get()->memHeapB;
        sum += diff;
    }
    auto averageDifference = sum/(snapshots.size()-1);

    std::cout << "#OUTLIERS#" << std::endl;
    std::vector<std::shared_ptr<Snapshot>> outliers;
    for(int i=0; i<snapshots.size()-1;i++)
    {
        auto diff = snapshots[i+1].get()->memHeapB - snapshots[i].get()->memHeapB;
        if(diff > averageDifference * 1.25){
            outliers.push_back(snapshots[i+1]);

            std::cout << diff << " bajtova je alocirano" ;
            // provera mape
        }
    }
};

std::map<int, std::vector<std::tuple<std::string, int, int, bool>>> createMap(std::vector<std::shared_ptr<Snapshot>> snapshots)
{
    // bajtovi : [(funkcija, broj poziva, broj snapshota, dealociranje)]
    std::map<int, std::vector<std::tuple<std::string, int, int, bool>>> mapa;

    for(int i=1;i<snapshots.size();i++)
    {
        int diff = snapshots[i].get()->memHeapB - snapshots[i-1].get()->memHeapB;
        //TODO
    }

    return mapa;
};

