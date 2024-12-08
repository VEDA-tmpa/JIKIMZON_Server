#ifndef SAVE_H
#define SAVE_H

#include <cstring>
#include <string>
#include <fstream>
#include <deque>

class Save
{
public:
    Save(size_t cap);
    
    void SaveJson(std::string& data);
    std::string GetJson();
    std::string ReadJson(size_t size);

private:
    std::fstream mFile;
    size_t mCapacity;
    std::deque<size_t> mItemOffsets;
};

#endif