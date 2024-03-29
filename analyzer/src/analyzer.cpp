#include <analyzer/analyzer.hpp>
#include <numeric>
#include <fstream>
#include <istream>
#include <utils/colors.hpp>

void MassifAnalyzer::processPeak()
{
    bold_red();
    std::cout << "PEAK" << std::endl;
    reset();

    /* ako peak ne postoji znaci da memorija nigde nije oslobadjana,
       svakako bi trebealo korisnika obavestiti o tome */
    
    if(mPeakSnapshot == nullptr){
        blue();
        std::cout << "Peak doesn't exist in massif report (memory is not freed at any point)." << std::endl;
        reset();
    } else {
        blue();
        std::cout << "Peak from massif report " << "(" << mPeakSnapshot->memHeapB << " bytes allocated)";
        reset();
        std::cout << *mPeakSnapshot;
    }
    
    if(realPeakSnapshot != nullptr && mPeakSnapshot->memHeapB < realPeakSnapshot->memHeapB){
        blue();
        std::cout << "Peak is not precisely determined by massif. Real peak (" << realPeakSnapshot->memHeapB << " bytes allocated)";
        reset();
        std::cout << *realPeakSnapshot;
    }
}


void MassifAnalyzer::processLastSnapshot()
{
    bold_red();
    std::cout << "STILL ALLOCATED" << std::endl;
    reset();

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
        
        /* svaku putanju smestiti u mapu u kojoj je kljuc putanja, a vrednost kolicina alociranih bajtova na toj putanji  */
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
    realPeakSnapshot = mSnapshots[0];

    for(int i = 0; i < mSnapshots.size() - 1; i++)
    {
        if (mSnapshots[i+1]->memHeapB > realPeakSnapshot->memHeapB)
            realPeakSnapshot = mSnapshots[i+1];

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

    std::map<std::string, int> timeMap;
    std::map<std::string, int> outliers;

    auto timeOfExecution = (*mSnapshots.rbegin())->time;

    bold_red();
    std::cout << "OUTLIERS (time)" << std::endl;
    red();
    std::cout << "Allocation exist more than third of time of program execution ";
    reset();
    std::cout << "(Time of execution is " << timeOfExecution << " ms):" << std::endl;


    for(int i = 0; i < mSnapshots.size() - 1; i++)
    {
        auto diff = mSnapshots[i+1]->memHeapB - mSnapshots[i]->memHeapB;

        std::map<std::string, int> previousInfo;
        std::map<std::string, int> currentInfo;

        if (mSnapshots[i]->tree != nullptr ) {
            previousInfo = getMapByTree(mSnapshots[i]->tree);
        } 
        
        if(mSnapshots[i+1]->tree != nullptr){
            currentInfo = getMapByTree(mSnapshots[i+1]->tree);
        }


        /* za svaku putanju u drugom stablu */
        for (auto xCurrent: currentInfo) 
        {
            /* pronaci istu putanju u prethodnom, prvom detaljnom stablu */
            auto xPrevious = previousInfo.find(xCurrent.first);
            
            /* ukoliko takvo nesto ne postoji, pronasli smo mesto nove alokacije */
            if (xPrevious == previousInfo.end()) 
            {
                timeMap[xCurrent.first] = mSnapshots[i+1]->time;
                if(diff > averageDifference * 1.5){
                    outliers[xCurrent.first] = diff;
                }
                break;
            } 
            else if(xCurrent.second - (*xPrevious).second == diff && diff > averageDifference*1.5) 
            {
                outliers[xCurrent.first] = diff;
                break;
            }
        }
        
        /* za svaku putanju u prvom (i) stablu */
        for (auto xPrevious: previousInfo) 
        {
            /* pronaci istu putanju u sledecem, drugom detaljnom stablu (i+1)*/
            auto xCurrent = currentInfo.find(xPrevious.first);
            
            /* ukoliko takvo nesto ne postoji, pronasli smo mesto dealokacije */
            if (xCurrent == currentInfo.end()) 
            {
                auto timeDiff =  mSnapshots[i+1]->time - timeMap[xPrevious.first];
                if (timeDiff > timeOfExecution/3.0){
                    /* alokacija postoji vise od trecine vremena izvrsavanja programa, proveriti da li je to u redu */
                    std::cout << xPrevious.first << " (time diff: " << timeDiff  << ")" << std::endl;
                }
                break;
            }
        }
    } 

    /*  slucaj kada ne postoji dealokacija, memorija je zadrzana do kraja*/
    std::map<std::string,int> lastSnapshot = getMapByTree((*mSnapshots.rbegin())->tree);
    for (auto snap: lastSnapshot){
        auto timeDiff = timeOfExecution - timeMap[snap.first];

        if(timeDiff > timeOfExecution/3.0){
            std::cout << snap.first << " (time diff: " << timeDiff  << ")" <<  std::endl;
        }
    }

    bold_red();
    std::cout << std::endl << "OUTLIERS (bytes)" << std::endl;
    red();
    std::cout << "Allocated bytes overcome one and a half times more than average program allocation size ";
    reset();

    std::cout << "(Average value of allocation is " << averageDifference << " bytes): " << std::endl;
    
    for(auto outlier: outliers){
        std::cout << outlier.second << " bytes allocated:" <<  std::endl; 
        std::cout << "  " << outlier.first << std::endl;
    }
}

void MassifAnalyzer::run()
{
    processLastSnapshot();
    std::cout << "-----------------------------------------------------------------------------" << std::endl;
    processSnapshots();
    std::cout << "-----------------------------------------------------------------------------" << std::endl;
    processPeak();
    std::cout << "-----------------------------------------------------------------------------" << std::endl;

    return;
}



void XtMemoryAnalyzer::run()
{
    auto write = true;
    //std::cout << "Analyze xtmemory.kcg.pid file: " << std::endl;  
    for(auto const& node : xTree->getNodes())
    {
        if(write){
            std::cout << *node.get() << std::endl;
        }

        if (node->childEsixtence()){
            // postoji dete => preskociti ga u ispisivanju
            write = false;
        } else {
            write = true;
        }
    }

    auto totals = xTree->getTotals();
    bold_red();
    std::cout <<  "PROGRAM TOTALS" << std::endl;
    reset();
    std::cout << "current " << totals[0] << "B (" << totals[1]  << "block(s));"
              << " total " << totals[2] << "B (" << totals[3] << " block(s));"
              << " freed " << totals[4] << "B (" << totals[5] << " block(s))" << std::endl;

    appendToSource();
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
                        green();
                        std::cout << std::endl << "Generated file: " << path_of_copy << std::endl;
                        reset();
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
        for (auto& node: xTree->getNodes())
            if (str == node->xFile)
                    filtered_nodes.push_back(node);

        
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
                    << "current " << curB << "B" << " (" << curBk << " block(s));" 
                    << " total " << totB << "B (" << totBk << " block(s));"  
                    << " freed " << totFdB << "B (" << totFdBk  << " block(s));" 
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