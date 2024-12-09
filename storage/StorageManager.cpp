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
				std::ofstream storageFile(mStorageFilePath, std::ios::binary);
				{ // 더미값으로 채워두기 (정적 배열을 기반으로한 링-큐에서 착안)
					if (storageFile.is_open() == false)
					{
						logger.Error("Unable to open storage file for writing.");
						return;
					}
					const uint8_t dummyValue = 0xFF;  // 더미 데이터 값
					
					uint64_t remainingSize = storage::MAX_FILE_SIZE;
					while (remainingSize > 0)
					{
						// 한 번에 쓸 수 있는 데이터 크기 (예: 1MB씩 쓰기)
						size_t chunkSize = std::min(remainingSize, static_cast<uint64_t>(1024 * 1024));

						// 더미 데이터를 쓴다
						std::vector<uint8_t> buffer(chunkSize, dummyValue);
						storageFile.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());

						remainingSize -= chunkSize;
					}
				}
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

		logger.Debug("[StorageManager::StorageManager(const std::string ip)] mFileHeader.GetFirstItemOffset() : " + std::to_string(mFileHeader.GetFirstItemOffset()));
		logger.Debug("[StorageManager::StorageManager(const std::string ip)] mFileHeader.GetLastItemOffset() : " + std::to_string(mFileHeader.GetLastItemOffset()));
		logger.Debug("[StorageManager::StorageManager(const std::string ip)] mFileHeader.GetCurrentItemOffset() : " + std::to_string(mFileHeader.GetCurrentItemOffset()));
		logger.Debug("[StorageManager::StorageManager(const std::string ip)] mFileHeader.GetPaddingOffset() : " + std::to_string(mFileHeader.GetPaddingOffset()));
	}
		
	void StorageManager::SaveFrame(const frame::Frame& frame)
	{
    	std::fstream storageFile(mStorageFilePath, std::ios::in | std::ios::out | std::ios::binary);
		if (storageFile.is_open() == false)
		{
			logger.Error("Unable to open storage file for writing.");
			return;
		}

		seekToOffsetFromHeader(storageFile, mFileHeader.GetLastItemOffset());
		seekFrameEnd(storageFile, static_cast<uint32_t>(storageFile.tellp()));

		uint32_t nextItemOffset = static_cast<uint32_t>(storageFile.tellp());

		if (nextItemOffset + frame.GetSize() > storage::MAX_FILE_SIZE)
		{
			mFileHeader.SetPaddingOffset(nextItemOffset);

			nextItemOffset = 0;
			seekToOffsetFromHeader(storageFile, nextItemOffset);
		}

		if (nextItemOffset <= mFileHeader.GetFirstItemOffset())
		{
			uint32_t validFirstOffset = mFileHeader.GetFirstItemOffset();
			while (nextItemOffset + frame.GetSize() > validFirstOffset)
			{
				seekFrameEnd(storageFile, validFirstOffset);
				validFirstOffset = static_cast<uint32_t>(storageFile.tellp());
			}

			mFileHeader.SetFirstItemOffset(validFirstOffset);
		}
		
		mFileHeader.SetLastItemOffset(nextItemOffset);
		mFileHeader.SetCurrentItemOffset(mFileHeader.GetLastItemOffset()); // for streaming mode
	
		mFileHeader.UpdateToFile();

		logger.Debug("[void StorageManager::SaveFrame(const frame::Frame& frame)] mFileHeader.GetFirstItemOffset() : " + std::to_string(mFileHeader.GetFirstItemOffset()));
		logger.Debug("[void StorageManager::SaveFrame(const frame::Frame& frame)] mFileHeader.GetLastItemOffset() : " + std::to_string(mFileHeader.GetLastItemOffset()));
		logger.Debug("[void StorageManager::SaveFrame(const frame::Frame& frame)] mFileHeader.GetCurrentItemOffset() : " + std::to_string(mFileHeader.GetCurrentItemOffset()));
		logger.Debug("[void StorageManager::SaveFrame(const frame::Frame& frame)] mFileHeader.GetPaddingOffset() : " + std::to_string(mFileHeader.GetPaddingOffset()));
	}

	void StorageManager::writeToFile(std::ofstream& storageFile, frame::Frame& frame)
	{
		std::vector<uint8_t> outSerializeFrame;
		frame.Serialize(outSerializeFrame);
		storageFile.write(reinterpret_cast<const char*>(outSerializeFrame.data()), outSerializeFrame.size());
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
		logger.Debug("[void StorageManager::GetNextFrame(OUT frame::Frame& frame) const] currentItemOffset : " + std::to_string(currentItemOffset));

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
			if (nextItemOffset == mFileHeader.GetPaddingOffset())
			{
				nextItemOffset = 0;
			}
			mFileHeader.SetCurrentItemOffset(nextItemOffset);
		}

		mFileHeader.UpdateToFile();

		logger.Debug("[void StorageManager::GetNextFrame(OUT frame::Frame& frame) const] mFileHeader.GetFirstItemOffset() : " + std::to_string(mFileHeader.GetFirstItemOffset()));
		logger.Debug("[void StorageManager::GetNextFrame(OUT frame::Frame& frame) const] mFileHeader.GetLastItemOffset() : " + std::to_string(mFileHeader.GetLastItemOffset()));
		logger.Debug("[void StorageManager::GetNextFrame(OUT frame::Frame& frame) const] mFileHeader.GetCurrentItemOffset() : " + std::to_string(mFileHeader.GetCurrentItemOffset()));
		logger.Debug("[void StorageManager::GetNextFrame(OUT frame::Frame& frame) const] mFileHeader.GetPaddingOffset() : " + std::to_string(mFileHeader.GetPaddingOffset()));
	}

	void StorageManager::seekToOffsetFromHeader(std::ifstream& storageFile, uint32_t offset)
	{
		storageFile.seekg(sizeof(storage::HeaderStruct) + offset, std::ios::beg);
	}

	void StorageManager::seekToOffsetFromHeader(std::ofstream& storageFile, uint32_t offset)
	{
		storageFile.seekp(sizeof(storage::HeaderStruct) + offset, std::ios::beg);
	}

	void StorageManager::seekToOffsetFromHeader(std::fstream& storageFile, uint32_t offset)
	{
		storageFile.seekp(sizeof(storage::HeaderStruct) + offset, std::ios::beg);
		storageFile.seekg(sizeof(storage::HeaderStruct) + offset, std::ios::beg);
	}

	void StorageManager::seekFrameEnd(std::fstream& storageFile, uint32_t offset)
	{
		seekToOffsetFromHeader(storageFile, offset);

		frame::Header header;
		{
			std::vector<uint8_t> headerBuffer(sizeof(frame::HeaderStruct));			
			storageFile.read(reinterpret_cast<char*>(headerBuffer.data()), headerBuffer.size());
			
			header.Deserialize(headerBuffer);
		}

		{
			uint32_t bodySize = header.GetBodySize();
	
			storageFile.seekp(bodySize, std::ios::cur); // 현재 위치에서 bodySize만큼 이동
		}
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