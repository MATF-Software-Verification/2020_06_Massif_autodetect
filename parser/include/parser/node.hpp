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
    : _file(file), _function(function), _line(line), _allocation(allocation), _childNode(nullptr)
    {}

    bool setChild(std::shared_ptr<Node> child);
    std::ostream& printData(std::ostream &out, int level) const;
    friend std::ostream& operator<< (std::ostream &out, const Node &t);
    
    std::string _file;
    std::string _function;
    int _line;
    std::vector<int> _allocation;

    std::shared_ptr<Node> _childNode;
};    

class XTreeMemory
{
public:
    XTreeMemory() = default;

    void addNode(std::shared_ptr<Node> node);

    std::vector<std::shared_ptr<Node>> _nodes; 
    std::vector<int> _totals;
};

