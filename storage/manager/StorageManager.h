#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <utility>
#include <unistd.h>

#include "common/frame/Frame.h"
#include "common/log/Logger.h"
#include "storage/manager/database/ItemFactory.h"
#include "storage/manager/database/BaseItem.h"
#include "storage/manager/database/StorageFile.h"

#define OUT

namespace storage
{
	template <typename Data>
	class StorageManager
	{
	public:
		StorageManager(const std::string ip);
		~StorageManager() = default;

		void SaveData(const Data& data);
		void GetNextData(OUT Data& data) const;

	private:
		static logger::Logger logger;

		std::string mId;
		std::string mStorageDirPath;
		std::string mStorageFilePath;

		mutable StorageFile<BaseItem> mStorageFile;
		std::unique_ptr<ItemFactory<Data>> mItemFactory; 

	private:
		mutable uint32_t mCurrentItemOffset;
	};

	template <typename Data>
	logger::Logger StorageManager<Data>::logger("StorageManager");

	template <typename Data>
	StorageManager<Data>::StorageManager(const std::string ip) 
		: mId(ip) 
		, mStorageDirPath(std::string(PROJECT_ROOT) + "/storage/" + ip)
		, mStorageFilePath(mStorageDirPath + "/" + typeid(Data).name() + ".dat")
		, mStorageFile(mStorageFilePath)
		, mItemFactory(std::move(ItemFactory<Data>::CreateFactory()))
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
				mStorageFile.SetFirstItemOffset(INVALID_OFFSET);
				mStorageFile.SetLastItemOffset(INVALID_OFFSET);
				mStorageFile.SetPaddingOffset(INVALID_OFFSET);
			}
		}
		else
		{
			// 1. 파일의 Header 영역에서 값을 읽어와서
			// 2. 메모리에 세팅해준다
			mStorageFile.ReadFileHeader();

		}

		mCurrentItemOffset = mStorageFile.GetLastItemOffset();

		logger.Info("access storagePath: " + mStorageFilePath);
	}
		
	template <typename Data>
	void StorageManager<Data>::SaveData(const Data& data)
	{
		try
		{
			std::unique_ptr<BaseItem> item = mItemFactory->CreateItem(data);
	
			mStorageFile.EnqueueItem(*item);
			logger.Info("Item saved successfully.");
		}
		catch (const std::exception& ex)
		{
			logger.Error(std::string("Error saving data: ") + ex.what());
			throw;
		}
	}

	template <typename Data>
	void StorageManager<Data>::GetNextData(OUT Data& data) const
	{
		try
		{
			logger.Debug("GetNextItem() mCurrentItemOffset: " + std::to_string(mCurrentItemOffset));
			logger.Debug("GetNextItem() mStorageFile.GetLastItemOffset: " + std::to_string(mStorageFile.GetLastItemOffset()));
			if (mCurrentItemOffset != mStorageFile.GetLastItemOffset())
			{
				std::vector<uint8_t> binaryData = mStorageFile.ReadItem(mCurrentItemOffset);
				data = mItemFactory->Deserialize(binaryData);
				logger.Info("Next data retrieved successfully.");
				
				mCurrentItemOffset = mStorageFile.GetNextItemOffset(mCurrentItemOffset);
			}
			else
			{
				logger.Info("Waiting Next Item.");
				usleep(1000000); // 1초
			}
		}
		catch (const std::exception& ex)
		{
			logger.Error(std::string("Error read data: ") + ex.what());
			throw;
		}
	}
}

#endif // STORAGE_MANAGER_H