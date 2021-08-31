#include <parser/node.hpp>


void XTreeMemory::addNode(std::shared_ptr<Node> node)
{
    this->xNodes.push_back(node);
}


bool Node::setChild(std::shared_ptr<Node> child)
{
    this->xChildNode = child;
    return true;
}

std::ostream& operator<< (std::ostream &out, const Node &t)
{
    if (t.xAllocation.size() > 0){
        out << "curB:" << t.xAllocation[0] << " curBk:" << t.xAllocation[1] 
              << " totB:" << t.xAllocation[2] << " totBk:" << t.xAllocation[3] 
              << " totFdB:" << t.xAllocation[4] << " totFdBk:" << t.xAllocation[5] << std::endl;
        if (t.xAllocation[3] >= 3) {
            out << "NOTICE THIS* YOU PROBABLY ALLOCATED CHUNK OF MEM IN LOOP TRY TO ISOLATE MEMORY ALLOCATION" << std::endl;
        }
    }

    out << t.xFile << ":" << t.xFunction << ":" << t.xLine << std::endl;
    if (t.xChildNode != nullptr){
        out << "\t";
        out << *(t.xChildNode.get());
    }

    return out;
}
