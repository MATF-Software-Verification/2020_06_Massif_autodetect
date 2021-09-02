#include <parser/node.hpp>
#include <boost/algorithm/string.hpp>

void XTreeMemory::addNode(std::shared_ptr<Node> node)
{
    this->xNodes.push_back(node);
}


bool Node::setChild(std::shared_ptr<Node> child)
{
    this->xChildNode = child;
    return true;
}

void red () {
  printf("\033[1;31m");
}

void reset () {
  printf("\033[0m");
}

std::ostream& operator<< (std::ostream &out, const Node &t)
{
    if (t.xAllocation.size() > 0){
        if (t.xAllocation[3] >= 3) {
            red();
            out << "NOTICE THIS* YOU PROBABLY ALLOCATED CHUNK OF MEM IN LOOP TRY TO ISOLATE MEMORY ALLOCATION" << std::endl;
            reset();
        }
        
        if(t.xAllocation[0] > 0){
            red();
            out << "THE MEMORY ALLOCATED HERE IS NOT FREED" << std::endl;
            reset();
        }

        out << "curB:" << t.xAllocation[0] << " curBk:" << t.xAllocation[1] 
              << " totB:" << t.xAllocation[2] << " totBk:" << t.xAllocation[3] 
              << " totFdB:" << t.xAllocation[4] << " totFdBk:" << t.xAllocation[5] << std::endl;
    }

    std::vector<std::string> splits;
    boost::algorithm::split(splits, t.xFile, boost::is_any_of("/"));

    out << t.xFunction << "(" << *(splits.rbegin()) << ":" << t.xLine << ")";
    if (t.xChildNode != nullptr){
        out << " => ";
        out << *(t.xChildNode.get());
    } else {
       out << std::endl;
    }


    return out;
}
