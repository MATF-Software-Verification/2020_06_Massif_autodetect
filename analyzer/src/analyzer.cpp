#include <analyzer/analyzer.hpp>
#include <numeric>
#include <map>


void FixifAnalyzer::processPeak()
{
    std::cout << "#PEAK#" << std::endl;
    if(mPeakSnapshot == nullptr){
        std::cout << "Peak ne postoji." << std::endl;
        return;
    }
    
    std::cout << mPeakSnapshot->memHeapB << " bajtova je alocirano" ;
    std::cout << *mPeakSnapshot;
}


void FixifAnalyzer::processLastSnapshot()
{
    auto snapshot = mSnapshots.back();
    std::cout << "#STILL ALLOCATED#" << std::endl;
    auto stillAllocated = snapshot->memHeapB;
    if(stillAllocated == 0 )
        return;
    
    std::cout << stillAllocated << " bajtova nije oslobodjeno";
    std::cout << *snapshot;
}


std::vector<std::pair<std::string, int>> getPathOfBytes(std::shared_ptr<Tree> tree) {
    if (tree->children.size() == 0) {
        std::string path = tree->function + "(" + tree->file + ":" + std::to_string(tree->line) + ")";
        return {std::make_pair(path, tree->bytes)};
    }

    std::vector<std::pair<std::string, int>> returnValue;
    for (auto& child: tree->children) {
        std::vector<std::pair<std::string, int>> pairs = getPathOfBytes(child);
        for (auto pair: pairs) {
            std::string path = pair.first + " ->" + tree->function + "(" + tree->file + ":" + std::to_string(tree->line) + ")";
            returnValue.push_back(std::make_pair(path, pair.second));
        }
    }

    return returnValue;
}



std::map<std::string, int> getMapByTree(std::shared_ptr<Tree> tree) 
{

    std::map<std::string, int> mapByTree;
    for (auto& child: tree->children) {
        std::vector<std::pair<std::string, int>> pathsOfBytes = getPathOfBytes(child);
        for(auto x: pathsOfBytes) {
            mapByTree[x.first] = x.second;
        }
    }

    return mapByTree;
}


void FixifAnalyzer::processSnapshots() 
{
    auto counter = 0, sum = 0;
    for(int i = 0; i < mSnapshots.size() - 1; i++)
    {
        auto diff = mSnapshots[i+1]->memHeapB - mSnapshots[i]->memHeapB;
        if (diff > 0) {
            sum += diff;
            counter++;
        }
    }
    auto averageDifference = sum/counter;

    std::cout << "#OUTLIERS#" << std::endl;
    std::cout << averageDifference << std::endl;
    
    std::vector<std::shared_ptr<Snapshot>> outliers;
    for(int i = 0; i< mSnapshots.size() - 1; i++)
    {
        auto diff = mSnapshots[i+1]->memHeapB - mSnapshots[i]->memHeapB;
        if(diff > averageDifference * 1.5){
            outliers.push_back(mSnapshots[i+1]);

            std::cout << diff << " bajtova je alocirano:" << std::endl; 
            if (mSnapshots[i+1]->tree != nullptr && mSnapshots[i]->tree != nullptr) {
                std::map<std::string, int> beforeInfo = getMapByTree(mSnapshots[i]->tree);
                std::map<std::string, int> currentInfo = getMapByTree(mSnapshots[i+1]->tree);
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
}

void FixifAnalyzer::createMap()
{
    std::map<std::string, int> map;
    
    auto firstDetailedSnapshot = *std::find_if(mSnapshots.cbegin(), mSnapshots.cend(), [](auto& s) { return s->tree; });
    auto lastDetailedSnapshot = *std::find_if(mSnapshots.crbegin(), mSnapshots.crend(), [](auto& s) { return s->tree; });

    std::map<std::string, int> dealokations;
    if (!firstDetailedSnapshot) return;
    for (int i = firstDetailedSnapshot->title; i <= lastDetailedSnapshot->title; i++) {
        if (!mSnapshots[i]->tree)
            continue;
        std::map<std::string, int> newMap = getMapByTree(mSnapshots[i]->tree);
        for (auto& x: map) {
            if (newMap.find(x.first) == newMap.end()) {
                dealokations[x.first] = x.second;
            }
        }
    }
}

void FixifAnalyzer::run()
{
    processPeak();
    std::cout << "=========================================" << std::endl;
    processLastSnapshot();
    std::cout << "=========================================" << std::endl;
    processSnapshots();
    std::cout << "=========================================" << std::endl;
    createMap();
    return;
}