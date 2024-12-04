#include "StorageManager.h"
#include <iostream>
#include <filesystem>
#include <fstream>

namespace storage{
	
	StorageManager::StorageManager(const std::string& ipAddress) : ip(ipAddress) {
		folderPath = "./" + ip;
		videoFilePath = folderPath + "/video.dat";
		
		// 폴더가 없으면 생성~
		if (!std::filesystem::exists(folderPath))
		{
			std::filesystem::create_directory(folderPath);
		}
		
		// 파일이 없으면 VideoHeader초기화해서 ip별 데이터 담을 준비
		if (!std::filesystem::exists(videoFilePath)
		{
			initVideoHeader();
		}
		else
		{
			// 기존 파일의 크기를 기반으로 VideoHeader를 초기화
			videoHeader.startSize = 0;
			videoHeader.endSize = getFileSize();
			videoHeader.currentSize = videoHeader.endSize;
		}
	}
	
	StorageManager::~StorageManager()
	{
	}
	
	// 초기화 메서드 -> 생성자에 사용
	void StorageManager::initVideoHeader()
	{
		videoHeader.startSize = 0;
		videoHeader.currentSize = 0;
		videoHeader.endSize = 0;
		
		std::cout << "[Init] StartSize = " << videoHeader.startSize
              << ", CurrentSize = " << videoHeader.currentSize
              << ", EndSize = " << videoHeader.endSize << std::endl;
	}
	
	void StorageManager::updateVideoHeader(size_t frameSize)
	{
		videoHeader.currentSize = videoHeader.endSize;			// 현재 크기에 마지막 크기 갱신
		videoHeader.endSize += frameSize;						// 마지막 크기에 프레임 크기 추
		
		 std::cout << "[Update] StartSize = " << videoHeader.startSize
              << ", CurrentSize = " << videoHeader.currentSize
              << ", EndSize = " << videoHeader.endSize << std::endl;
     }
     
     void StorageManager::SaveFrame(const std::string& frameData)
     {
		 // ios::binary -> 줄바꿈 문자 변환 처리 비활성화 후 그대로 이진 데이터로 처리
		 // ios::app -> 기존 파일 덮어쓰지 않고 새로운 데이터 파일 끝에 추가
		 std::ofstream outFile(videoFilePath, std::ios::binary | std::ios::app);
		 
		 // 객체가 열리는거 확인하고 데이터 쓰고 파일 닫아서 저장
		 if (outFile.is_open())
		 {
			 outFile.write(frameData.c_str(), frameData.size());
			 outFile.close();
			 // 프레임 크기로 VideoHeader 갱신
			 updateVideoHeader(frameData.size());
			 
			 std::cout << "Frame Saved Successfully\n";
		 }
		 else
		 {
			 std::cerr << "Failed to Save Frame\n";
		 }
	 }
	 
	 // 기존 파일의 크기를 가져옴
	 uint64_t StorageManager::getFileSize() const
	 {
		 // 파일, 디렉토리 존재부터 확인하고 
		 if (std::filesystem::exists(videoFilePath))
		 {
			 return std::filesystem::file_size(videoFilePath);	// 반환 값으로 파일 크기
		 }
		 return 0;
	 }
	 
	 VideoHeader StorageManager::GetVideoHeader() const
	 {
		 return videoHeader;
	 }
	 
 }	// namespace storage
