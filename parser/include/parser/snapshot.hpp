#pragma once
#include <string>
#include <vector>
#include <iostream>
#include<memory>

class Tree
{
public:
    Tree() = default;
    Tree(int number, int bytes, std::string location,std::string function, std::string file, int line, int counter)
        : number(number), bytes(bytes), location(location), function(function), file(file), line(line), counter(counter)
    {}

    bool addChild(std::shared_ptr<Tree> child);
    std::ostream& printData(std::ostream &out, int level) const;
    friend std::ostream& operator<< (std::ostream &out, const Tree &t);
    
    int number;
    int bytes;
    int line; 
    int counter;

    std::string location;
    std::string function;
    std::string file;
    std::vector<std::shared_ptr<Tree>> children;
};    

class Snapshot
{
public:
    Snapshot() = default;
    Snapshot(int title, int time, int memHeapB, int memHeapExtra, int memStacks)
        :title(title), time(time), memHeapB(memHeapB), memHeapExtra(memHeapExtra), memStacks(memStacks)
    {
        tree = nullptr;
        treeHeaderNumber = -1;
        treeHeaderBytes = -1;
        treeHeaderMessage = "Tree doesn't exist";
    }

    void addTreeNode(int number, int bytes, std::string location,std::string function, std::string file, int line);
    void changeHeaderInfo(int number, int bytes, std::string message);

    friend std::ostream& operator<< (std::ostream &out, const Snapshot &s);

    int title;
    int time;
    int memHeapB;
    int memHeapExtra;
    int memStacks;

    int treeHeaderNumber;
    int treeHeaderBytes;
    std::string treeHeaderMessage;

    std::shared_ptr<Tree> tree; 
};

