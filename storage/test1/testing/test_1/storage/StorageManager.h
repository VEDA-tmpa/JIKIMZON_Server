#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <string>
#include <fstream>
#include <vector>

#include "common/frame/Frame.h"
#include "common/log/Logger.h"

namespace storage {

class StorageManager {
public:
    // 생성자
    StorageManager(const std::string& clientIp, const std::string& dirPath, size_t diskSize);

    // 소멸자
    ~StorageManager();

    // Public 메서드
    void SaveFrame(const frame::Frame& frame); // 프레임 데이터를 저장

private:
    // Private 멤버 변수
    std::string mClientIp;       // 클라이언트 IP
    std::string mDirPath;        // 디렉토리 경로
    std::string mFilePath;       // 파일 경로
    size_t mDiskSize;            // 디스크 용량

    logger::Logger mLogger;      // 로깅 객체

    // Private 메서드
    void initialize();           // 초기화 (디렉토리/파일 생성)
    void adjustDiskSpace(size_t newFrameSize); // 공간 확보 (오래된 데이터 삭제)
};

} // namespace storage

#endif // STORAGE_MANAGER_H
