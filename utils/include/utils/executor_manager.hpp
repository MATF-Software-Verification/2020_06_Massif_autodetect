#pragma once
#include <iostream>

class ExecutorManager
{    
public:
    ExecutorManager(const std::string& fileName)
        : mFileName(fileName) {}
    virtual ~ExecutorManager() {}
    virtual void execOperation();
    int getPid();
    
protected:
    std::string mFileName;
    int mPid;
};
