#ifndef JSONFILE_H
#define JSONFILE_H

#include <cstring>
#include <string>
#include <fstream>
#include <deque>

class JsonFile
{
public:
    JsonFile(size_t cap);
    
    void SaveJson(std::string& data);
    std::string GetJson();

private:
    std::fstream mFile;
    size_t mCapacity;
    std::deque<size_t> mItemOffsets;
};

#endif