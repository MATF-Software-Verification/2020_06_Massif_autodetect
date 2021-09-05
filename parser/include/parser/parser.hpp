#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <parser/snapshot.hpp>
#include <parser/node.hpp>
#include <boost/algorithm/string.hpp>
/**
 * @brief Parser interface
*/
class IParser
{
public:
    IParser() = default;
    
    enum class ParserStatus {
        ePARSER_OK,
        ePARSER_FAIL
    };

    /**
     * @brief Function which performs parsing process
     * @return Indicator of success
    */
    virtual ParserStatus parse() = 0;
};


/**
 * @brief Implementation of parser for massif.out.* files
*/
class MassifParser : public IParser
{
public:
    MassifParser() = default;
    MassifParser(const std::string& path);
    ~MassifParser();
    
    ParserStatus parse() override;

public:
    friend std::ostream& operator<< (std::ostream &out, const MassifParser &mp);
    
    /**
     * @brief Getter for collection of snapshots that is presented in massif file
     * @return Vector of pointers to Snapshot objects
    */
    inline const std::vector<std::shared_ptr<Snapshot>>& getSnapshots() {
        if (status == ParserStatus::ePARSER_OK) {
            return mSnapshots;
        } else {
            static decltype(mSnapshots) result{};
            return result;
        }
    }
    /**
     * @brief Getter for snapshot annotated as peak in massif file
     * @return Pointer to Snapshot object
    */
    inline const std::shared_ptr<Snapshot>& getPeakSnapshot() {
        if (status == ParserStatus::ePARSER_OK) {
            return mPeakSnapshot;
        } else {
            static decltype(mPeakSnapshot) result{};
            return result;
        }
    }


private:
    std::string mDesc; ///< Represents corresponding field from massif file
    std::string mCmd; ///< Represents corresponding field from massif file
    std::string mTimeUnit; ///< Represents corresponding field from massif file
    std::vector<std::shared_ptr<Snapshot>> mSnapshots; ///< Represents corresponding field from massif file
    std::shared_ptr<Snapshot> mPeakSnapshot; ///< Represents corresponding field from massif file
    
    std::ifstream mFile; ///< Represents massif.out
    std::stringstream mContent; 
    ParserStatus status = ParserStatus::ePARSER_OK;
};



/**
 * @brief Implementation of parser for xtmemory.kcg.* files
*/
class XtmemoryParser : public IParser
{
public:
    XtmemoryParser(){
        xTree = std::make_shared<XTreeMemory>();
    }
    XtmemoryParser(const std::string& path);
    ~XtmemoryParser();
    
    ParserStatus parse() override;

public:
    friend std::ostream& operator<< (std::ostream &out, const XtmemoryParser &mp);
  
    /**
     * @brief Getter for tree structure containing info from xtmemory.kcg. file
     * @return Shared pointer to XTreeMemory object
    */
    inline const std::shared_ptr<XTreeMemory>& getTree() {
        if (status == ParserStatus::ePARSER_OK) {
            return xTree;
        } else {
            static decltype(xTree) result{};
            return result;
        }
    }

    /**
     * @brief Getter for map containing integer mapped to file name 
    */
    inline std::map<int, std::string> getFileMap() const {
        return fileNameMap;
    }
    
private:
    std::shared_ptr<XTreeMemory> xTree; ///< Represents structure containing info about allocations
    std::ifstream xFile; ///< Represents xtmemory.kcg file
    std::stringstream xContent;
    ParserStatus status = ParserStatus::ePARSER_OK;

    std::map<int, std::string> fileNameMap; ///< Mapping integer to file name
    std::map<int, std::string> functionNameMap; ///< Mapping integer to function name
};