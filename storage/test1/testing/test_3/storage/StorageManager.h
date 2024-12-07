#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include "common/frame/Frame.h"
#include "common/log/Logger.h"

namespace storage {

class StorageManager {
public:    
    StorageManager(const std::string& clientIp, const std::string& dirPath, size_t diskSize);    
    ~StorageManager();
    
    void SaveFrame(const frame::Frame& frame); // 프레임 데이터를 저장

private:
    
    std::string mClientIp;       // 클라이언트 IP
    std::string mDirPath;        // 디렉토리 경로
    std::string mFilePath;       // 파일 경로
    size_t mDiskSize;            // 디스크 용량
    size_t mCurrentWriteOffset;  // 현재 쓰기 위치
    logger::Logger mLogger;      // 로깅 도구
    
    void initialize();                         // 초기화 (디렉토리/파일 생성)
};

} // namespace storage

#endif // STORAGE_MANAGER_H
