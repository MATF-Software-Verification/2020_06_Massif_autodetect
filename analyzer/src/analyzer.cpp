#include <analyzer/analyzer.hpp>
#include <numeric>
#include <map>


void FixifAnalyzer::processPeak()
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


void FixifAnalyzer::processLastSnapshot()
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


void FixifAnalyzer::processSnapshots() 
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

void FixifAnalyzer::run()
{
    processPeak();
    std::cout << "-----------------------------------------------------------------------------" << std::endl;
    processLastSnapshot();
    std::cout << "-----------------------------------------------------------------------------" << std::endl;
    processSnapshots();
    std::cout << "-----------------------------------------------------------------------------" << std::endl;

    return;
}


void Analyzer::run()
{
    //std::cout << "Analyze xtmemory.kcg.pid file: " << std::endl;  
    for(auto const& node : mNodes)
    {
        std::cout << *node.get() << std::endl;
    }
}