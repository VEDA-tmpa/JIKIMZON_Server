#include <iostream>
#include <filesystem>
#include <fstream>

#include "StorageManager.h"

namespace storage
{
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
				mFileHeader.Update();
			}
		}
		else
		{
			// 1. 파일의 Header 영역에서 값을 읽어와서
			// 2. 메모리에 세팅해준다
			mFileHeader.Load();
		}
	}

	StorageManager::~StorageManager()
	{
	}
		
	void StorageManager::SaveFrame(const frame::Frame frame)
	{
		std::ofstream storageFile(mStorageFilePath, std::ios::binary | std::ios::app);
		if (storageFile.is_open() == false)
		{
			std::cerr << "Error: Unable to open storage file for writing." << std::endl;
			return;
		}

		
		const uint32_t itemEndOffset = mFileHeader.GetLastItemOffset() + frame.GetSize();

		if (itemEndOffset < storage::MAX_FILE_SIZE) 
		{
			{
				mFileHeader.SetLastItemOffset(itemEndOffset + 1);
			}
			{ // 프레임 데이터 직렬화 및 쓰기
				std::vector<uint8_t> serializedFrame = frame.Serialize();
				storageFile.write(reinterpret_cast<const char*>(serializedFrame.data()), serializedFrame.size());
			}
		}
		else 
		{
			{
				mFileHeader.SetPaddingItemOffset(itemEndOffset + 1);
			}
			{ // 유효한 첫 아이템 찾기
				storageFile.seekg(sizeof(storage::HeaderStruct), std::ios::beg); // 헤더 영역 이후(== 데이터 영역)로 이동
				
				uint32_t firstValidOffset = 0;
				while (storageFile.peek() != EOF)
				{
					std::vector<uint8_t> headerBuffer(sizeof(frame::HeaderStruct));
					storageFile.read(reinterpret_cast<char*>(headerBuffer.data()), headerBuffer.size());

					frame::Header header;
					header.Deserialize(headerBuffer);

					firstValidOffset = static_cast<uint32_t>(storageFile.tellg()) + header.GetBodySize() + 1;
					if (firstValidOffset > frame.GetSize())
					{
						break;
					}

					storageFile.seekg(frame.GetHeader().GetBodySize(), std::ios::cur);
				}

				mFileHeader.SetFirstItemOffset(firstValidOffset);
				mFileHeader.SetLastItemOffset(frame.GetSize());
			}
			
			{ // 프레임 데이터 직렬화 및 쓰기
				storageFile.seekg(sizeof(storage::HeaderStruct), std::ios::beg); // 헤더 영역 이후(== 데이터 영역)로 이동
				
				std::vector<uint8_t> serializedFrame = frame.Serialize();
				storageFile.write(reinterpret_cast<const char*>(serializedFrame.data()), serializedFrame.size());
			}
		}

		mFileHeader.Update();
	}

	void StorageManager::GetNextFrame(OUT frame::frame frame) const
	{
		// 파일 열기 (읽기 전용)
		std::ifstream storageFile(mStorageFilePath, std::ios::binary);
		if (!storageFile.is_open())
		{
			std::cerr << "Error: Unable to open storage file for reading." << std::endl;
			return;
		}


		// 현재 프레임 오프셋 가져오기
		uint32_t currentItemOffset = mFileHeader.GetCurrentItemOffset();
		if (currentItemOffset >= mFileHeader.GetLastItemOffset())
		{
			std::cerr << "Error: No more frames to read." << std::endl;
			return;
		}
		storageFile.seekg(sizeof(storage::HeaderStruct) + currentItemOffset, std::ios::beg); // 파일 포인터를 현재 오프셋으로 이동

		// 프레임 헤더 읽기
		std::vector<uint8_t> headerBuffer(sizeof(frame::HeaderStruct));
		storageFile.read(reinterpret_cast<char*>(headerBuffer.data()), headerBuffer.size());
		if (storageFile.gcount() != headerBuffer.size())
		{
			std::cerr << "Error: Failed to read frame header." << std::endl;
			return;
		}
		frame::Header header;
		header.Deserialize(headerBuffer);

		// 프레임 본문 데이터 읽기
		uint32_t bodySize = header.GetBodySize();
		std::vector<uint8_t> bodyBuffer(bodySize);
		storageFile.read(reinterpret_cast<char*>(bodyBuffer.data()), bodyBuffer.size());
		if (static_cast<uint32_t>(storageFile.gcount()) != bodySize)
		{
			std::cerr << "Error: Failed to read frame body." << std::endl;
			return;
		}
		frame.Deserialize(header, bodyBuffer);


		// 다음 프레임의 오프셋 계산
		uint32_t nextItemOffset = static_cast<uint32_t>(storageFile.tellg());
		if (nextItemOffset == mFileHeader.GetPaddingItemOffset())
		{
			storageFile.seekg(sizeof(storage::HeaderStruct), std::ios::beg); // 헤더 영역 이후(== 데이터 영역) 시작 지점으로 이동
			nextItemOffset = 0;
		}
		mFileHeader.SetCurrentItemOffset(nextItemOffset);


		mFileHeader.Update();
	}
}