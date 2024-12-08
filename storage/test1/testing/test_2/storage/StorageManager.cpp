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

void StorageManager::adjustDiskSpace(size_t newFrameSize)
{
    mLogger.Info("Adjusting disk space...");
    std::ifstream inFile(mFilePath, std::ios::binary);
    if (!inFile)
    {
        throw std::runtime_error("Failed to open file: " + mFilePath);
    }

    inFile.seekg(0, std::ios::end);
    size_t fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    if (fileSize + newFrameSize <= mDiskSize)
    {
        inFile.close();
        mLogger.Info("No need to adjust disk space.");
        return;
    }

    std::vector<uint8_t> buffer;
    size_t remainingSize = fileSize;

    while (remainingSize + newFrameSize > mDiskSize)
    {
        std::vector<uint8_t> headerBuffer(sizeof(frame::HeaderStruct));
        inFile.read(reinterpret_cast<char*>(headerBuffer.data()), sizeof(frame::HeaderStruct));

        frame::Header header;
        header.Deserialize(headerBuffer);

        size_t frameSize = sizeof(frame::HeaderStruct) + header.GetBodySize();
        inFile.seekg(header.GetBodySize(), std::ios::cur);
        remainingSize -= frameSize;
    }

    buffer.resize(remainingSize);
    inFile.read(reinterpret_cast<char*>(buffer.data()), remainingSize);
    inFile.close();

    std::ofstream outFile(mFilePath, std::ios::binary | std::ios::trunc);
    if (!outFile)
    {
        throw std::runtime_error("Failed to open file for writing: " + mFilePath);
    }

    outFile.write(reinterpret_cast<const char*>(buffer.data()), remainingSize);
    outFile.close();

    mLogger.Info("Disk space adjusted.");
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

    adjustDiskSpace(frameSize);

    std::ofstream outFile(mFilePath, std::ios::binary | std::ios::app);
    if (!outFile)
    {
        throw std::runtime_error("Failed to open file: " + mFilePath);
    }

    outFile.write(reinterpret_cast<const char*>(frameBuffer.data()), frameBuffer.size());
    outFile.close();

    fixDiskSizeWithPadding();

    mLogger.Info("Frame saved successfully.");
}

void StorageManager::fixDiskSizeWithPadding()
{
    std::ofstream outFile(mFilePath, std::ios::binary | std::ios::app);
    if (!outFile)
    {
        throw std::runtime_error("Failed to open file: " + mFilePath);
    }

    outFile.seekp(0, std::ios::end);
    size_t fileSize = outFile.tellp();

    if (fileSize < mDiskSize)
    {
        size_t paddingSize = mDiskSize - fileSize;
        std::vector<uint8_t> padding(paddingSize, 0x00);
        outFile.write(reinterpret_cast<const char*>(padding.data()), paddingSize);
    }

    outFile.close();
    mLogger.Info("Padding added to maintain fixed disk size.");
}

} // namespace storage
