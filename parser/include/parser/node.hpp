#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <memory>

class Node
{
public:
    Node() = default;
    Node(std::string file, std::string function, int line, std::vector<int> allocation)
    : xFile(file), xFunction(function), xLine(line), xAllocation(allocation), xChildNode(nullptr)
    {}


    friend class XtMemoryAnalyzer;

    bool setChild(std::shared_ptr<Node> child);
    std::ostream& printData(std::ostream &out, int level) const;
    friend std::ostream& operator<< (std::ostream &out, const Node &t);
    
private:
    std::string xFile;
    std::string xFunction;
    int xLine;
    std::vector<int> xAllocation;
    std::shared_ptr<Node> xChildNode;
};    

class XTreeMemory
{
public:
    XTreeMemory() = default;
    void addNode(std::shared_ptr<Node> xNode);
    inline std::vector<std::shared_ptr<Node>> getNodes() const {
        return xNodes;
    }

    inline auto getTotals() const {
        return xTotals;
    }

    friend class XtMemoryAnalyzer;
private:
    std::vector<std::shared_ptr<Node>> xNodes; 
    std::vector<int> xTotals;

};

