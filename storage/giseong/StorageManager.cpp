#include <iostream>
#include <filesystem>
#include <fstream>

#include "StorageManager.h"

namespace storage
{
	StorageManager::StorageManager(const std::string& ip) 
		: mId(ip) 
	{
		mStoragePath = std::string(PROJECT_ROOT) + "/storage/" + mId;
		mVideoPath = mStoragePath + "/video.h264";
		
		if (std::filesystem::exists(mVideoPath) == false)
		{
			// 디렉토리 및 파일 생성
			std::filesystem::create_directory(mStoragePath);
			std::ofstream outVideoFile("video.h264");

			// 메모리에도 세팅해준다
			mHeader.startIndex = 0;			
			mHeader.endIndex = 0;			
			mHeader.currentIndex = 0;

			// 파일의 Header 영역을 세팅한다
			updateHeader();
		}

		else // 있네?
		{
			// 1. 파일의 Header 영역에서 값을 읽어와서
			// 2. 메모리에 세팅해준다
			loadHeader();
		}
	}

	StorageManager::~StorageManager()
	{
	}
	     
	void StorageManager::SaveFrame(const frame::Frame frame)
	{
		std::ofstream outFile(mVideoPath, std::ios::binary | std::ios::app);
		if (outFile.is_open() == false)
		{
			return;
		}

		{
			video::storage::maximumSize;

			if (mHeader.endIndex + frame.GetHeader().GetBodySize < video::storage::maximumSize) 
			{
				outFile.write(frame.Serialize());
			}
			else 
			{
				paddingIndex = mHeader.endIndex + frame.GetHeader().GetBodySize + 1;



				frame.GetSize();

				frame::Frame outFrame;
				// 1. 헤더구조체 크기만큼 읽는다 ()
				// 2. 객체화(deserialize) 하고 
				// 3. 거기서 바디만
				



				endIndex = 0;
			}
		}

		updateHeader();
	}

	void StorageManager::updateHeader()
	{
		std::ofstream file(mVideoPath, std::ios::binary | std::ios::in | std::ios::out);
		if (!file)
		{
			throw std::runtime_error("Failed to open metadata file.");
		}
		file.seekp(0); // Move to the beginning
		file.write(reinterpret_cast<const char*>(&mHeader), sizeof(video::HeaderStruct));
		file.close();
	}

	void StorageManager::loadHeader()
	{
		std::ifstream file(mVideoPath, std::ios::binary);
		if (!file) 
		{
			throw std::runtime_error("Failed to open metadata file.");
		}
		file.read(reinterpret_cast<char*>(&mHeader), sizeof(video::HeaderStruct));
		file.close();
	}
}
