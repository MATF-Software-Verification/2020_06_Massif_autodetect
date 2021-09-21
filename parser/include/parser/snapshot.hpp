#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <memory>


/**
 * @brief Representation of tree contained in one snapshot
*/
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
    
    int number; ///< Store corresponding value from massif file field
    int bytes; ///< Store corresponding value from massif file field
    int line; ///< Store corresponding value from massif file field
    int counter; ///< Store corresponding value from massif file field

    std::string location; ///< Store corresponding value from massif file field
    std::string function; ///< Store corresponding value from massif file field
    std::string file; ///< Store corresponding value from massif file field
    std::vector<std::shared_ptr<Tree>> children; ///< Store snapshots from massif file
};    


/**
 * @brief Representation of one snapshot 
*/
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

    int title; ///< Store corresponding value from massif file field
    int time; ///< Store corresponding value from massif file field
    int memHeapB; ///< Store corresponding value from massif file field
    int memHeapExtra; ///< Store corresponding value from massif file field
    int memStacks; ///< Store corresponding value from massif file field

    int treeHeaderNumber; ///< Store corresponding value from massif file field
    int treeHeaderBytes; ///< Store corresponding value from massif file field
    std::string treeHeaderMessage; ///< Store corresponding value from massif file field
    std::shared_ptr<Tree> tree; ///< Store corresponding value from massif file field
};

