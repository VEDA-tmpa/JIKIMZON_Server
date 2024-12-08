#include "StorageManager.h"
#include <stdexcept>
#include <vector>
#include <filesystem>

namespace storage {
    
    namespace fs = std::filesystem;

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
    // 디렉토리 생성
    if (!fs::exists(mDirPath))
    {
        fs::create_directories(mDirPath);
    }

    // 파일 생성
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
        // 오래된 프레임 읽기
        std::vector<uint8_t> frameBuffer(sizeof(frame::HeaderStruct));
        inFile.read(reinterpret_cast<char*>(frameBuffer.data()), sizeof(frame::HeaderStruct));

        frame::Header header;
        header.Deserialize(frameBuffer);

        size_t frameSize = sizeof(frame::HeaderStruct) + header.GetBodySize();
        inFile.seekg(header.GetBodySize(), std::ios::cur);

        remainingSize -= frameSize;
    }

    // 나머지 데이터 읽기
    buffer.resize(remainingSize);
    inFile.read(reinterpret_cast<char*>(buffer.data()), remainingSize);
    inFile.close();

    // 파일 새로 작성
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

    outFile.write(reinterpret_cast<const char*>(frameBuffer.data()), frameSize);
    outFile.close();

    mLogger.Info("Frame saved successfully.");
}

} // namespace storage
