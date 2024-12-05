#include "StorageManager.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

namespace storage
{
    StorageManager::StorageManager(const std::string& ip)
        : mId(ip), mMaximumSize(3 * 1280 * 720), buffer(3 * 1280 * 720, 0)
    {
        initializeStorage();
    }

    StorageManager::~StorageManager() {}

    void StorageManager::initializeStorage()
    {
        mStoragePath = std::filesystem::path("./storage") / mId;
        mVideoPath = mStoragePath / "video.h264";

        if (!std::filesystem::exists(mStoragePath))
        {
            std::filesystem::create_directory(mStoragePath);
        }

        if (!std::filesystem::exists(mVideoPath))
        {
            std::ofstream outFile(mVideoPath, std::ios::binary);
            if (!outFile)
            {
                throw std::runtime_error("Failed to create video file: " + mVideoPath.string());
            }
            mHeader = {0, 0, 0};
            updateHeader();
        }
        else
        {
            loadHeader();
        }
    }
    
    void StorageManager::updateHeader()
    {
        std::ofstream file(mVideoPath, std::ios::binary | std::ios::trunc);
        if (!file)
        {
            throw std::runtime_error("Failed to open metadata file for updating: " + mVideoPath.string());
        }

        video::HeaderStruct header = {0, 0, 0};
        file.write(reinterpret_cast<const char*>(&header), sizeof(video::HeaderStruct));
        file.close();

    }
    
    void StorageManager::loadHeader()
    {
        std::ifstream file(mVideoPath, std::ios::binary);
        if (!file)
        {
            throw std::runtime_error("Failed to open metadata file: " + mVideoPath.string());
        }

        std::vector<uint8_t> buffer(sizeof(video::HeaderStruct));
        file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

        if (file.gcount() != sizeof(video::HeaderStruct))
        {
            throw std::runtime_error("Failed to read complete header: expected " +
                                    std::to_string(sizeof(video::HeaderStruct)) +
                                    ", got " + std::to_string(file.gcount()));
        }

        frame::Header header;
        header.Deserialize(buffer);

        file.close();
    }
    
    void StorageManager::SaveFrame(const frame::Frame& frame)
    {
        // 프레임 직렬화
        std::vector<uint8_t> serializedFrame;
        frame.Serialize(serializedFrame);
        size_t frameSize = serializedFrame.size();

        if (frameSize > mMaximumSize)
        {
            throw std::runtime_error("Frame size exceeds maximum storage size");
        }

        // 현재 endIndex부터 버퍼 끝까지 남은 공간 계산
        size_t spaceAtEnd = mMaximumSize - mHeader.endIndex;

        // 프레임을 저장할 공간 확보
        if (spaceAtEnd < frameSize) // 버퍼 끝까지 공간 부족
        {
            // 시작 부분에서 연속적으로 저장 가능한지 확인
            size_t availableSpace = getRemainSpace();
            if (availableSpace < frameSize) // 총 공간 부족 시 오래된 프레임 삭제
            {
                while (getRemainSpace() < frameSize)
                {
                    size_t frameSizeToRemove = getFrameSizeAt(mHeader.startIndex);
                    mHeader.startIndex = (mHeader.startIndex + frameSizeToRemove) % mMaximumSize;
                }
            }

            // 버퍼의 시작으로 돌아가 패딩 처리
            addPadding(mHeader.endIndex, mMaximumSize);
            mHeader.endIndex = 0; // 시작 위치로 이동
        }

        // 연속적으로 저장 가능한지 확인
        if (mHeader.endIndex + frameSize > mMaximumSize)
        {
            throw std::runtime_error("Unexpected error: Insufficient space after cleanup.");
        }

        // 데이터를 버퍼에 저장
        std::copy(serializedFrame.begin(), serializedFrame.end(), buffer.begin() + mHeader.endIndex);

        // 헤더 업데이트
        mHeader.currentIndex = mHeader.endIndex;
        mHeader.endIndex = (mHeader.endIndex + frameSize) % mMaximumSize;

        updateHeader();
    }

    void StorageManager::GetNextFrame(frame::Frame& frame)
    {
        if (mHeader.currentIndex == mHeader.endIndex)
        {
            throw std::runtime_error("No more frames available to read.");
        }

        size_t frameSize = getFrameSizeAt(mHeader.currentIndex);

        // 현재 위치에서 프레임이 연속적으로 저장되어 있는지 확인
        if (mHeader.currentIndex + frameSize > mMaximumSize)
        {
            throw std::runtime_error("Unexpected error: Frame spans buffer boundary. This should not happen.");
        }

        // 데이터를 연속적으로 읽기
        std::vector<uint8_t> serializedFrame(frameSize);
        std::copy(buffer.begin() + mHeader.currentIndex,
                buffer.begin() + mHeader.currentIndex + frameSize,
                serializedFrame.begin());

        // 프레임 복원
        frame.Deserialize(serializedFrame);

        // 읽기 위치 업데이트
        mHeader.currentIndex = (mHeader.currentIndex + frameSize) % mMaximumSize;
    }    
    
    void StorageManager::addPadding(size_t start, size_t end)
    {
        if (end > start)
        {
            std::fill(buffer.begin() + start, buffer.begin() + end, 0);
        }
        else
        {
            std::fill(buffer.begin() + start, buffer.end(), 0);
            std::fill(buffer.begin(), buffer.begin() + end, 0);
        }
    }

    size_t StorageManager::getRemainSpace() const
    {
        if (mHeader.startIndex == mHeader.endIndex)
        {
            // 버퍼가 비어 있는 경우
            return mMaximumSize;
        }
        else if (mHeader.endIndex > mHeader.startIndex)
        {
            // 데이터가 연속적으로 저장된 경우
            return mMaximumSize - (mHeader.endIndex - mHeader.startIndex);
        }
        else
        {
            // 데이터가 순환된 경우 (논리적으로 발생하지 않음)
            throw std::logic_error("Invalid buffer state: data should always be stored contiguously.");
        }
    }

    size_t StorageManager::getFrameSizeAt(size_t index) const
    {
        frame::Header header;
        std::vector<uint8_t> headerBuffer(sizeof(video::HeaderStruct));
        size_t realIndex = index % mMaximumSize;

        std::copy(buffer.begin() + realIndex, buffer.begin() + realIndex + headerBuffer.size(), headerBuffer.begin());
        header.Deserialize(headerBuffer);

        return header.GetBodySize() + sizeof(video::HeaderStruct);
    }     
}
