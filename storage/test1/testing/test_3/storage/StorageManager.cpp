#include "StorageManager.h"
#include <fstream>
#include <vector>
#include <cstring>

namespace storage {

StorageManager::StorageManager(const std::string& clientIp, const std::string& dirPath, size_t diskSize)
    : mClientIp(clientIp),
      mDirPath(dirPath),
      mFilePath(dirPath + "/video.bin"),
      mDiskSize(diskSize),
      mCurrentWriteOffset(0),
      mLogger("StorageManager")
{
    initialize();
}

StorageManager::~StorageManager()
{
}

void StorageManager::initialize()
{
    namespace fs = std::filesystem;

    if (!fs::exists(mDirPath))
    {
        fs::create_directories(mDirPath);
    }

    if (!fs::exists(mFilePath))
    {
        std::ofstream outFile(mFilePath, std::ios::binary);
        if (!outFile)
        {
            throw std::runtime_error("Failed to create file: " + mFilePath);
        }
        outFile.close();
    }
}

void StorageManager::SaveFrame(const frame::Frame& frame)
{
    std::vector<uint8_t> frameBuffer;
    frame.Serialize(frameBuffer);
    size_t frameSize = frameBuffer.size();

    if (frameSize > mDiskSize)
    {
        mLogger.Error("Frame size exceeds total disk size!");
        throw std::runtime_error("Frame size exceeds total disk size!");
    }

    std::fstream outFile(mFilePath, std::ios::binary | std::ios::in | std::ios::out);
    if (!outFile)
    {
        throw std::runtime_error("Failed to open file: " + mFilePath);
    }

    // 새 프레임이 현재 위치에서 디스크를 초과할 경우, 앞부분으로 이동
    if (mCurrentWriteOffset + frameSize > mDiskSize)
    {
        mLogger.Info("Disk full, wrapping around to overwrite from the beginning.");
        mCurrentWriteOffset = 0;
    }

    // 현재 위치에서 프레임 저장
    outFile.seekp(mCurrentWriteOffset, std::ios::beg);
    outFile.write(reinterpret_cast<const char*>(frameBuffer.data()), frameSize);
    mCurrentWriteOffset += frameSize;

    // 패딩 처리: 파일 크기가 고정된 경우
    if (mCurrentWriteOffset < mDiskSize)
    {
        size_t paddingSize = mDiskSize - mCurrentWriteOffset;
        std::vector<uint8_t> padding(paddingSize, 0x00);
        outFile.write(reinterpret_cast<const char*>(padding.data()), paddingSize);
        mCurrentWriteOffset += paddingSize;
    }

    outFile.close();
    mLogger.Info("Frame saved successfully.");
}

} // namespace storage
