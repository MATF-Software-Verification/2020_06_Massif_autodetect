
#include <utils/executor_manager.hpp>
#include <cstdlib>
#include <random>
#include <limits>

void ExecutorManager::execOperation()
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> dis(0, std::numeric_limits<int>::max());
    mPid = dis(generator);
    std::string massifOutFile = "massif.out." + std::to_string(mPid);
    std::string xtreeFile = "xtmemory.kcg." + std::to_string(mPid);
    std::string execCmd = "valgrind --tool=massif --detailed-freq=1 --xtree-memory=full --xtree-memory-file="
                        + xtreeFile
                        + " --massif-out-file="
                        + massifOutFile
                        + " ./" 
                        + mFileName;
    //std::cout << execCmd << "\n";
    system(execCmd.c_str());
    std::cout << "-----------------------------------------------------------------------------" << std::endl;
}

int ExecutorManager::getPid()
{
    return mPid;
}