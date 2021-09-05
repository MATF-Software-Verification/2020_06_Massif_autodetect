#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <memory>
    
/**
 * @brief Representation of one allocation call 
*/
class Node
{
public:
    Node() = default;
    Node(std::string file, std::string function, int line, std::vector<int> allocation)
    : xFile(file), xFunction(function), xLine(line), xAllocation(allocation), xChildNode(nullptr)
    {}

    friend class XtMemoryAnalyzer;
    friend std::ostream& operator<< (std::ostream &out, const Node &t);

    bool setChild(std::shared_ptr<Node> child);
    bool childEsixtence(){
        return xChildNode != nullptr;
    }
private:
    std::string xFile;  ///< Store corresponding value from xtmemory file field
    std::string xFunction;  ///< Store corresponding value from xtmemory file field
    int xLine;  ///< Store corresponding value from xtmemory file field
    std::vector<int> xAllocation; ///< Store corresponding value from xtmemory file field
    std::shared_ptr<Node> xChildNode; ///< Store corresponding value from xtmemory file field
};    


/**
 * @brief Representation of all allocation calls in program in form of tree
*/
class XTreeMemory
{
public:
    XTreeMemory() = default;

    void addNode(std::shared_ptr<Node> xNode);
    void addTotals(std::vector<int> xTotals);    


    /**
     * @brief Getter for collection of snapshots that is presented in massif file
     * @return Vector of pointers to Snapshot objects
    */
    inline std::vector<std::shared_ptr<Node>> getNodes() const {
        return xNodes;
    }

    /**
     * @brief Getter for info about total memory usage present in xtmemory file
     * @return Vector of ints
    */
    inline std::vector<int> getTotals() const {
        return xTotals;
    }

    friend class XtMemoryAnalyzer;
private:
    std::vector<std::shared_ptr<Node>> xNodes; 
    std::vector<int> xTotals;
};

