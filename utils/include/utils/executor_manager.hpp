#pragma once
#include <iostream>

class ExecutorManager
{    
public:
    ExecutorManager(const std::string& fileName)
        : mFileName(fileName) {}
    virtual ~ExecutorManager() {}
    virtual void execOperation();
protected:
    std::string mFileName;
};



class SouceExecutorManager : public ExecutorManager
{
public:
    SouceExecutorManager(const std::string& fileName)
        : ExecutorManager(fileName) {}
    ~SouceExecutorManager() {}
    void execOperation() override;
};



