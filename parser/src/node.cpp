#include <parser/node.hpp>
#include <boost/algorithm/string.hpp>
#include <utils/colors.hpp>

void XTreeMemory::addNode(std::shared_ptr<Node> node)
{
    this->xNodes.push_back(node);
}

void XTreeMemory::addTotals(std::vector<int> xTotals)
{
    this->xTotals = xTotals;
}


bool Node::setChild(std::shared_ptr<Node> child)
{
    this->xChildNode = child;
    return true;
}

std::ostream& operator<< (std::ostream &out, const Node &t)
{
    if (t.xAllocation.size() > 0){
        if (t.xAllocation[3] >= 3) {
            red();
            out << "NOTICE THIS* You probably allocated chunk of mem in loop; try yo isolate memory allocation." << std::endl;
            reset();
        }
        
        if(t.xAllocation[0] > 0){
            red();
            out << "The memory allocated here is not freed." << std::endl;
            reset();
        }
        
        blue();
        out << "current " << t.xAllocation[0] << "B (" << t.xAllocation[1] << " block(s));" 
              << " total " << t.xAllocation[2] << "B (" << t.xAllocation[3] << " block(s));" 
              << " freed " << t.xAllocation[4] << "B (" << t.xAllocation[5] << " block(s))" << std::endl;
        reset();
    }

    
    std::vector<std::string> splits;
    boost::algorithm::split(splits, t.xFile, boost::is_any_of("/"));
    out << t.xFunction << "(" << *(splits.rbegin()) << ":" << t.xLine << ")";

    //out << t.xFunction << "(" << t.xFile << ":" << t.xLine << ")";
    if (t.xChildNode != nullptr){
        out << " => ";
        out << *(t.xChildNode.get());
    } else {
       out << std::endl;
    }


    return out;
}
