#include <iostream>
#include <parser/parser.hpp>


MassifParser::MassifParser(const std::string& path)
    :mDesc(""), mCmd(""), mTimeUnit(""), mFile(path)
{
    if (!mFile.is_open()) {
        status = ParserStatus::ePARSER_FAIL;
        std::cerr << "Fail to open " << path << "\n";
    }
    mContent << mFile.rdbuf();
}

MassifParser::~MassifParser()
{
    if (mFile.is_open()) {
        mFile.close();
    }
}
MassifParser::ParserStatus MassifParser::parse()
{

    std::cout << "Parsirao sam\n";
    return ParserStatus::ePARSER_OK;
}