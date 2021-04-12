#include <parser/snapshot.hpp>

void Snapshot::addTreeNode(int number, int bytes, std::string location,std::string function, std::string file, int line)
{
    if (tree == nullptr){
        tree = std::make_shared<Tree>(treeHeaderNumber,treeHeaderBytes,"","","",-1,treeHeaderNumber);
    } 

    std::shared_ptr<Tree> newChild = std::make_shared<Tree>(number,bytes,location,function,file,line,number);
    tree->addChild(newChild);
}

void Snapshot::changeHeaderInfo(int number, int bytes, std::string message)
{
    this->treeHeaderNumber = number;
    this->treeHeaderBytes = bytes;
    this->treeHeaderMessage = message;
}


void printPath(std::shared_ptr<Tree> tree) 
{
    if(tree.get()->children.size() == 0)
    {   
        std::cout << std::endl;
        std::cout <<  "    " << tree.get()->bytes << ": ";
        std::cout << tree.get()->function << "(" << tree.get()->file << ":" << tree.get()->line << ")";
    }
    
    for(auto child: tree.get()->children)
    {
        printPath(child);   
        std::cout << "->";
        std::cout << tree.get()->function  << "(" << tree.get()->file << ":" << tree.get()->line << ")";
    }
      
};

std::ostream& operator<< (std::ostream &out, const Snapshot &s)
{
    /*out << "Title: " << s.title << std::endl
                << "Time: " << s.time << std::endl
                << "memHeapB: " << s.memHeapB << std::endl
                << "memHeapExtra: " << s.memHeapExtra << std::endl
                << "memStacks: " << s.memStacks << std::endl;
    out << "treeHeaderNumber: " << s.treeHeaderNumber << std::endl
                << "treeHeaderBytes: " << s.treeHeaderBytes << std::endl
                << "treeHeaderMessage: " << s.treeHeaderMessage << std::endl;        
    if(s.tree!= nullptr)
        out << *s.tree.get();
    out << std::endl;
    */

   std::shared_ptr<Tree> tree = s.tree;

    if (tree != nullptr){
        std::cout << ": " << std::endl;
        for(auto child: tree.get()->children)
        {
            std::cout << "  " << child.get()->bytes 
                      << " bajtova alociranih u" 
                      << child.get()->function 
                      << "(" << child.get()->file << ":" << child.get()->line << "):";
            
            printPath(child);
            std::cout << std::endl;
        }   
    } else {
        std::cout << ". " << std::endl;
    }
    return out;
}

bool Tree::addChild(std::shared_ptr<Tree> child)
{
    bool added;
    for(int i=children.size()-1; i >= 0; i--){
        added = children[i]->addChild(child);
        if (added){
            return true;
        } 
    }

    if (!added && counter > 0){
        counter--;
        children.push_back(child);
        return true;
    }

    return false;
}

std::ostream& Tree::printData(std::ostream &out, int level) const
{
    for(int i = 0; i < level; i++)
        out << " ";
    out << number << " " << bytes << " " << location << " " << function << " " << file << " " << line << std::endl;
    for(auto child: children){
        child->printData(out, level+1);
    }
    
    return out;
}

std::ostream& operator<< (std::ostream &out, const Tree &t)
{
    return t.printData(out, 0);
}

