
#include <utils/executor_manager.hpp>
#include <cstdlib>
#include <random>
#include <limits>
#include <clang/Driver/Compilation.h>
#include <clang/Driver/Driver.h>
// #include <clang/Frontend/DiagnosticOptions.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/Program.h>
#include <llvm/Support/raw_ostream.h>

void ExecutorManager::execOperation()
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> dis(0, std::numeric_limits<int>::max());
    int pid = dis(generator);
    std::string massifOutFile = "massif.out." + std::to_string(pid);
    std::string xtreeFile = "xtmemory." + std::to_string(pid);
    std::string execCmd = "valgrind --tool=massif --detailed-freq=1 --xtree-memory=full --xtree-memory-file="
                        + xtreeFile
                        + " --massif-out-file="
                        + massifOutFile
                        + " ./" 
                        + mFileName;
    std::cout << execCmd << "\n";
    system(execCmd.c_str());

}


void SouceExecutorManager::execOperation()
{
    auto clangPath = llvm::sys::findProgramByName("clang");    
    // std::cout << clangPath.get() << std::endl;

}