#ifndef SAVE_H
#define SAVE_H

#include <cstring>
#include <string>
#include <fstream>
#include <queue>

class Save
{
public:
    Save(size_t cap);

    void SaveJson(std::string& data);
    std::string ReadJson(size_t size);
private:
    std::fstream mFile;

    size_t mCapacity;

    std::queue<size_t> mItemOffsets;
};

#endif