#include <analyzer/analyzer.hpp>
#include <numeric>
#include <fstream>
#include <istream>

void MassifAnalyzer::processPeak()
{
    std::cout << "#PEAK#" << std::endl;

    /* ako peak ne postoji znaci da memorija nigde nije oslobadjana,
       svakako bi trebealo korisnika obavestiti o tome */
    
    if(mPeakSnapshot == nullptr){
        std::cout << "Peak doesn't exist." << std::endl;
        return;
    }
    
    std::cout << mPeakSnapshot->memHeapB << " bytes are allocated" ;
    std::cout << *mPeakSnapshot;
}


void MassifAnalyzer::processLastSnapshot()
{
    std::cout << "#STILL ALLOCATED#" << std::endl;

    auto snapshot = mSnapshots.back();
    auto stillAllocated = snapshot->memHeapB;
    
    if(stillAllocated == 0 )
        return;
    
    std::cout << stillAllocated << " bytes not freed";
    std::cout << *snapshot;
}


/* pomocna funkcija (izdvojena zbog primene rekurzije) */
std::vector<std::pair<std::string, int>> getPath(std::shared_ptr<Tree> tree) 
{
    /* kreiraju se putanje koje sadrze funkcije i pozicije u fajlu koje alociraju odredjeni blok memorije */
    /* putanja u obliku funkcija1 (datoteka.c: broj linije) -> ... -> funkcijaN (datoteka.c:broj linije) */

    /* ukoliko podstablo ne sadrzi decu, direktno se kreira par (putanja, broj bajtova) */
    if (tree->children.size() == 0) 
    {
        std::string path = tree->function + "(" + tree->file + ":" + std::to_string(tree->line) + ")";
        return {std::make_pair(path, tree->bytes)};
    }

    std::vector<std::pair<std::string, int>> pathToBytes;

    for (auto& child: tree->children) 
    {
        std::vector<std::pair<std::string, int>> pairs = getPath(child);

        for (auto pair: pairs) 
        {
            std::string path = pair.first + " => " + tree->function + "(" + tree->file + ":" + std::to_string(tree->line) + ")";
            pathToBytes.push_back(std::make_pair(path, pair.second));
        }
    }

    return pathToBytes;
}


std::map<std::string, int> getMapByTree(std::shared_ptr<Tree> tree) 
{

    std::map<std::string, int> mapByTree;

    /* za svako dete korenog cvora odrediti putanju */
    for (auto& child: tree->children) 
    {
        std::vector<std::pair<std::string, int>> pathsOfBytes = getPath(child);
        
        /* svaku putanju smestiti u mapu u kojoj je kljuc  putanja, a vrednost kolicina alociranih bajtova na toj putanji  */
        for(auto path: pathsOfBytes) 
        {
            mapByTree[path.first] = path.second;
        }
    }

    return mapByTree;
}


void MassifAnalyzer::processSnapshots() 
{
    auto counter = 0, sum = 0;

    for(int i = 0; i < mSnapshots.size() - 1; i++)
    {
        /* koliko je u svakom koraku alocirano bajtova */
        auto diff = mSnapshots[i+1]->memHeapB - mSnapshots[i]->memHeapB;
    
        /* ukoliko je vrseno alociranje nove memorije, a ne oslobadjanje */
        if (diff > 0) 
        {
            sum += diff;
            counter += 1;
        }
    }

    auto averageDifference = sum/counter;

    std::cout << "#OUTLIERS#" << std::endl;
    std::cout << "Average value of allocated bytes: " << averageDifference << " bytes " << std::endl;
    
    std::vector<std::shared_ptr<Snapshot>> outliers;

    for(int i = 0; i < mSnapshots.size() - 1; i++)
    {
        auto diff = mSnapshots[i+1]->memHeapB - mSnapshots[i]->memHeapB;
        
        /* ako je razlika znacajna tako da predstavlja autlajer */
        if(diff > averageDifference * 1.5)
        {
            outliers.push_back(mSnapshots[i+1]);

            std::cout << diff << " bytes allocated:" <<  std::endl; 

            /* ako su dva uzastopna snimka detaljna */
            if (mSnapshots[i+1]->tree != nullptr && mSnapshots[i]->tree != nullptr) 
            {
                std::map<std::string, int> previousInfo = getMapByTree(mSnapshots[i]->tree);
                std::map<std::string, int> currentInfo = getMapByTree(mSnapshots[i+1]->tree);

                /* za svaku putanju u drugom detaljnom stablu */
                for (auto xCurrent: currentInfo) 
                {
                    /* pronaci istu putanju u prethodnom, prvom detaljnom stablu */
                    auto xPrevious = previousInfo.find(xCurrent.first);
                    
                    /* ukoliko takvo nesto ne postoji, pronasli smo mesto nove alokacije */
                    if (xPrevious == previousInfo.end()) 
                    {
                        std::cout << "  " << xCurrent.first << std::endl;
                        break;
                    }

                    /* u suprotnom, ako postoji i razlika u bajtovim je jednaka upravo trazenoj, onda smo pronasli mesto nove alokacije  */
                    if (xCurrent.second - (*xPrevious).second == diff) 
                    {
                        std::cout << "  " << xCurrent.first << std::endl;
                        break;
                    }
                }
            }
        }
    }
}

void MassifAnalyzer::run()
{
    processPeak();
    std::cout << "-----------------------------------------------------------------------------" << std::endl;
    processLastSnapshot();
    std::cout << "-----------------------------------------------------------------------------" << std::endl;
    processSnapshots();
    std::cout << "-----------------------------------------------------------------------------" << std::endl;

    return;
}


void XtMemoryAnalyzer::run()
{
    //std::cout << "Analyze xtmemory.kcg.pid file: " << std::endl;  
    for(auto const& node : xTree->getNodes())
    {
        std::cout << *node.get() << std::endl;
    }

    appendToSource();

    /*std::cout <<  "Program stotals: " << std::endl;
    std::cout << "curB:" << t->xTotals[0] << " curBk:" << t->xTotals[1] 
              << " totB:" << t->xTotals[2] << " totBk:" << t->xTotals[3] 
              << " totFdB:" << t->xTotals[4] << " totFdBk:" << t->xTotals[5] << std::endl;*/
}

bool XtMemoryAnalyzer::appendToSource()
{

    std::map<std::string, std::pair<std::ifstream, std::ofstream>> files;
    std::transform(mFileMap.begin(), mFileMap.end(), std::inserter(files, files.end()),
                [](auto const& arg) -> std::pair<std::string, std::pair<std::ifstream, std::ofstream>> {
                    std::string path(arg.second);
                    std::ifstream iStream(path);
                    std::string path_of_copy;

                    if (iStream.is_open()) {
                        auto idx = path.rfind('.');
                        if (idx != std::string::npos) {
                            path_of_copy = path.substr(0, idx) + ".fixif" + path.substr(idx);
                        }
                        std::cout << path_of_copy << "\t\t\t FIXIF" << std::endl;
                     
                    }

                    return std::make_pair(
                        path, 
                        std::make_pair(std::move(iStream), std::ofstream(path_of_copy))
                    );   

                    
                    
                });
    for (auto& [path, streams]: files)
    {
        auto isOOpen = streams.second.is_open();
        auto str = path;
        if (!streams.first.is_open()) continue;
        std::vector<std::shared_ptr<Node>> filtered_nodes;
        auto it = xTree->getNodes().begin() + 1;
        std::copy_if(it, xTree->getNodes().end(),
                        std::back_inserter(filtered_nodes),
                        [&](const auto node) {
                            
                             return str == node->xFile;   
                        });

        std::string line;
        for (int i = 1; std::getline(streams.first, line); i++)
        {
            uint32_t curB = 0, curBk = 0, totB = 0, totBk = 0, totFdB = 0, totFdBk = 0;
            bool hasData = false;
            for (const auto& node: filtered_nodes) 
            {
                if (node->xLine == i) {
                    hasData = true;
                    curB += node->xAllocation[0];
                    curBk += node->xAllocation[1];
                    totB += node->xAllocation[2];
                    totBk += node->xAllocation[3];
                    totFdB += node->xAllocation[4];
                    totFdBk += node->xAllocation[5];
                }
            }
            (isOOpen ? streams.second : std::cout) << line;
            if (hasData) {
                (isOOpen ? streams.second : std::cout) << "\t// "
                    << "curB:" << curB << " curBk:" << curBk 
                    << " totB:" << totB << " totBk:" << totBk 
                    << " totFdB:" << totFdB << " totFdBk:" << totFdBk 
                    << std::endl;
            }
            else {
                (isOOpen ? streams.second : std::cout) << std::endl;
            }

        }
    
        streams.first.close();
        if (isOOpen)  streams.second.close();
    }

    
    return true;
}