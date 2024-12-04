#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include <string>
#include <filesystem>
#include <fstream>
#include "VideoHeader.h"

namespace storage{
	
class StorageManager
{
public:
	StorageManager(const std::string& ipAddress);
	~StorageManager();
	void SaveFrame(const std::string& frameData);	// 프레임 저장
	VideoHeader GetVideoHeader() const;				// VideoHeader 반환
	
private:
	std::string ip;					// 카메라 객체 역할
	std::string folderPath;			// IP 이름으로 된 폴더 경로
	std::string videoFilePath		// 저장할 영상 파일 경로
	VideoHeader videoHeader;		// 영상 정보 구조체
	
	void initVideoHeader();			// VideoHeader 초기화
	void updateVideoHeader();		// VideoHeader 갱신
	uint64_t getFileSize() const;	// 파일 크기 계산
};

}	// namespace storage
	
#endif	// STORAGEMANAGER_H
