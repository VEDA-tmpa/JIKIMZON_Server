#include "save.hpp"

#include <iostream>
#include <cstdlib>

Save::Save(size_t cap) : mFile("json.dat", std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc), mCapacity(cap)
{
    if (!mFile)
    {
        throw std::runtime_error("Failed to open file");
    }

    mItemOffsets.emplace_back(1);
    
    std::vector<char> padding(mCapacity + 2, '\0');
    padding[0] = '[';
    padding[mCapacity - 1] = ']';
    mFile.seekp(0);
    mFile.write(padding.data(), mCapacity);
}

std::string Save::GetJson()
{
    auto end = mItemOffsets.back();
    auto pre = --(--mItemOffsets.end());
    size_t size = end - *pre - 1;

    std::vector<char> buffer(size);
    mFile.seekg(*pre);
    mFile.read(buffer.data(), size);

    return std::string(buffer.begin(), buffer.end());
}

void Save::SaveJson(std::string &data) {
    data.push_back(',');

    // 남은 공간 확인
    if (data.size() > mCapacity)
    {
        throw std::runtime_error("Data size exceeds queue capacity");
    }

    // queue 끝에 다다랐을 때
    if (mCapacity - mItemOffsets.back() < data.size())
    {
        size_t size = mCapacity - mItemOffsets.back();
        std::vector<char> padding(size, '\0');
        
        mFile.seekp(mItemOffsets.back());
        mFile.write(padding.data(), size);

        mItemOffsets.emplace_back(1);

        while (mItemOffsets.front() < mItemOffsets.back() + data.size())
        {
            mItemOffsets.pop_front();
        }
    } 

    mFile.seekp(mItemOffsets.back());
    mFile.write(data.data(), data.size());
    mItemOffsets.emplace_back(mItemOffsets.back() + data.size());
    
    int size = mItemOffsets.front() - mItemOffsets.back();
    size = abs(size);

    std::vector<char> padding(size, '\0');
    
    char existing[1];
    mFile.seekg(mItemOffsets.back());
    mFile.read(existing, 1);

    if (existing[0] != '\0')
    {
        mFile.seekp(mItemOffsets.back());
        mFile.write(padding.data(), size);
    }

    // 파일 동기화
    mFile.flush();
}

void Save::GetJson()
{
    auto end = --mItemOffsets.end();
    auto pre = --end;
    size_t size = *end - *pre;
    std::vector<char> buffer(size);
    mFile.seekg(0);
    mFile.read(buffer.data(), size);
}

std::string Save::ReadJson(size_t size)
{
    if (size > mCapacity)
    {
        throw std::runtime_error("Read size exceeds queue capacity");
    }

    std::vector<char> buffer(size);
    mFile.seekg(0);
    mFile.read(buffer.data(), size);

    // 패딩 제거
    buffer.erase(std::remove(buffer.begin(), buffer.end(), '\0'), buffer.end());

    return std::string(buffer.begin(), buffer.end());
}