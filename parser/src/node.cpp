#include "parser/node.hpp"


void XTreeMemory::addNode(std::shared_ptr<Node> node)
{
    this->_nodes.push_back(node);
}


bool Node::setChild(std::shared_ptr<Node> child)
{
    this->_childNode = child;
}

std::ostream& operator<< (std::ostream &out, const Node &t)
{
    out << t._file << ":" << t._function << ":" << t._line << std::endl;
    if (t._childNode != nullptr){
        out << "\t";
        out << *(t._childNode.get());
    }

    if (t._allocation.size() > 0){
        out << "curB:" << t._allocation[0] << " curBk:" << t._allocation[1] 
              << " totB:" << t._allocation[2] << " totBk:" << t._allocation[3] 
              << " totFdB:" << t._allocation[4] << " totFdBk:" << t._allocation[5] << std::endl;
    }
    

    return out;
}
