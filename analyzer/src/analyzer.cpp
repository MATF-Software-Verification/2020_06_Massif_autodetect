#include <analyzer/analyzer.hpp>
#include <numeric>
#include <map>


void MassifAnalyzer::processPeak()
{
    printf("\033[1;31m");
    std::cout << "PEAK" << std::endl;
    printf("\033[0m");

    /* ako peak ne postoji znaci da memorija nigde nije oslobadjana,
       svakako bi trebealo korisnika obavestiti o tome */
    
    if(mPeakSnapshot == nullptr){
        std::cout << "Peak doesn't exist (memory is not freed at any point)." << std::endl;
        return;
    }
    
    std::cout << mPeakSnapshot->memHeapB << " bytes are allocated" ;
    std::cout << *mPeakSnapshot;
}


void MassifAnalyzer::processLastSnapshot()
{
    printf("\033[1;31m");
    std::cout << "STILL ALLOCATED" << std::endl;
    printf("\033[0m");

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

    std::map<std::string, int> timeMap;
    std::map<std::string, int> outliers;

    auto timeOfExecution = (*mSnapshots.rbegin())->time;

    printf("\033[1;31m");;
    std::cout << "OUTLIERS (time)" << std::endl;
    std::cout << "Allocation exist more than third of time of program execution." << std::endl;
    printf("\033[0m");

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

        /* Desila se nova alokacija*/
        if (diff > 0){

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
        } 
        /* Desila se dealokacija */
        else if (diff < 0){
            
            /* za svaku putanju u prvom (i) stablu */
            for (auto xPrevious: previousInfo) 
            {
                /* pronaci istu putanju u sledecem, drugom detaljnom stablu (i+1)*/
                auto xCurrent = currentInfo.find(xPrevious.first);
                
                /* ukoliko takvo nesto ne postoji, pronasli smo mesto dealokacije */
                if (xCurrent == currentInfo.end()) 
                {
                    auto timeDiff =  mSnapshots[i+1]->time; - timeMap[xPrevious.first];
                    if (timeDiff > timeOfExecution/3.0){
                        /* alokacija postoji vise od trecine vremena izvrsavanja programa, proveriti da li je to u redu */
                        std::cout << xPrevious.first << " (time diff: " << timeDiff  << ")" << std::endl;
                    }
                    break;
                }
            }
        }   /* Inace nista se nije desilo, snapshot je ponovljen */   

    }

    /*  slucaj kada ne postoji dealokacija, memorija je zadrzana do kraja*/
    std::map<std::string,int> lastSnapshot = getMapByTree((*mSnapshots.rbegin())->tree);
    for (auto snap: lastSnapshot){
        auto timeDiff = timeOfExecution - timeMap[snap.first];

        if(timeDiff > timeOfExecution/3.0){
            printf("\033[1;31m");
            printf("\033[0m");
            std::cout << snap.first << " (time diff: " << timeDiff  << ")" <<  std::endl;
        }
    }

    printf("\033[1;31m");;
    std::cout << std::endl << "OUTLIERS (bytes)" << std::endl;
    std::cout << "Allocated bytes overcome 1.5 times more of average program alloction size." << std::endl;
    printf("\033[0m");

    std::cout << "Average value of allocated bytes: " << averageDifference << " bytes " << std::endl;
    
    for(auto outlier: outliers){
        std::cout << outlier.second << " bytes allocated:" <<  std::endl; 
        std::cout << "  " << outlier.first << std::endl;
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

    auto totals = xTree->getTotals();
    printf("\033[1;31m");
    std::cout <<  "PROGRAM TOTALS" << std::endl;
    printf("\033[0m");
    std::cout << "curB:" << totals[0] << " curBk:" << totals[1] 
              << " totB:" << totals[2] << " totBk:" << totals[3] 
              << " totFdB:" << totals[4] << " totFdBk:" << totals[5] << std::endl;
}
