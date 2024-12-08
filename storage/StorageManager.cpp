#include <iostream>
#include <filesystem>
#include <fstream>
#include <utility>

#include "StorageManager.h"

namespace storage
{
	logger::Logger StorageManager::logger("StorageManager");

	StorageManager::StorageManager(const std::string ip) 
		: mId(ip) 
		, mStorageDirPath(std::string(PROJECT_ROOT) + "/storage/" + ip)
		, mStorageFilePath(mStorageDirPath + "/video.h264")
		, mFileHeader(mStorageFilePath)
	{
		if (std::filesystem::exists(mStorageFilePath) == false)
		{
			{
				std::filesystem::create_directory(mStorageDirPath);
				std::ofstream outVideoFile(mStorageFilePath, std::ios::binary);
			}

			{
				// 1. 메모리에도 세팅해준다
				mFileHeader.SetFirstItemOffset(0);
				mFileHeader.SetLastItemOffset(0);
				mFileHeader.SetCurrentItemOffset(0);
				
				// 2. 파일의 Header 영역을 세팅한다
				mFileHeader.UpdateToFile();
			}
		}
		else
		{
			// 1. 파일의 Header 영역에서 값을 읽어와서
			// 2. 메모리에 세팅해준다
			mFileHeader.LoadFromFile();
		}
	}
		
	void StorageManager::SaveFrame(const frame::Frame& frame)
	{
		std::ofstream storageFile(mStorageFilePath, std::ios::binary | std::ios::app);
		if (storageFile.is_open() == false)
		{
			logger.Error("Unable to open storage file for writing.");
			return;
		}

		// TODO



		mFileHeader.UpdateToFile();
	}



	void StorageManager::GetNextFrame(OUT frame::Frame& frame) const
	{
		std::ifstream storageFile(mStorageFilePath, std::ios::binary);
		if (storageFile.is_open() == false)
		{
			logger.Error("Unable to open storage file for writing.");
			return;
		}

		uint32_t currentItemOffset = mFileHeader.GetCurrentItemOffset();
		if (currentItemOffset == mFileHeader.GetLastItemOffset())
		{
			logger.Debug("No more frames to read");
			return;
		}

		{ // 파일에서 읽기
			seekToOffsetFromHeader(storageFile, currentItemOffset);
			loadFrameFromFile(storageFile, frame);
		}

		{ // 다음 프레임의 오프셋 계산 (== 다음 아이템의 위치 계산)
			uint32_t nextItemOffset = static_cast<uint32_t>(storageFile.tellg());
			if (nextItemOffset == mFileHeader.GetPaddingItemOffset())
			{
				nextItemOffset = 0;
			}
			mFileHeader.SetCurrentItemOffset(nextItemOffset);
		}

		mFileHeader.UpdateToFile();
	}

	void StorageManager::seekToOffsetFromHeader(std::ifstream &storageFile, uint32_t offset)
	{
		storageFile.seekg(sizeof(storage::HeaderStruct) + offset, std::ios::beg);
	}

	void StorageManager::loadFrameFromFile(std::ifstream &storageFile, OUT frame::Frame& frame)
	{
		frame::Header header;
		{
			std::vector<uint8_t> headerBuffer(sizeof(frame::HeaderStruct));
			storageFile.read(reinterpret_cast<char*>(headerBuffer.data()), headerBuffer.size());
			
			header.Deserialize(headerBuffer);
		}

		frame::Body body;
		{
			uint32_t bodySize = header.GetBodySize();
			std::vector<uint8_t> bodyBuffer(bodySize);
			storageFile.read(reinterpret_cast<char*>(bodyBuffer.data()), bodyBuffer.size());

			body.Deserialize(bodyBuffer);
		}

		frame::Frame tempFrame(header, body);
		frame = std::move(tempFrame);
	}
}