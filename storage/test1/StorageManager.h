#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#include <cstddef>
#include "VideoHeader.h"
#include "common/frame/Frame.h"

namespace storage
{
    class StorageManager
    {
    public:
        
        StorageManager(const std::string& ip);
        ~StorageManager();

        void SaveFrame(const frame::Frame& frame);
        void GetNextFrame(frame::Frame& frame);

    private:
        void initializeStorage();                        // 스토리지 초기화
        void updateHeader();                             // 헤더 파일 업데이트
        void loadHeader();                               // 헤더 파일 읽기
        void addPadding(size_t start, size_t end);       // 특정 영역 패딩 추가
        size_t getRemainSpace() const;                   // 남은 공간 계산
        size_t getFrameSizeAt(size_t index) const;       // 특정 위치에서 프레임 크기 가져오기

        std::string mId;                                 // Storage ID
        std::filesystem::path mStoragePath;              // Storage 경로
        std::filesystem::path mVideoPath;                // 비디오 파일 경로
        size_t mMaximumSize;                             // 버퍼 최대 크기

        video::HeaderStruct mHeader;       // 순환 버퍼 헤더
        std::vector<uint8_t> buffer;       // 순환 버퍼 데이터 저장
    };

} // namespace storage

#endif // STORAGE_MANAGER_H
