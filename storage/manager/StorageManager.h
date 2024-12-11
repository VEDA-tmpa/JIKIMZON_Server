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
#include "storage/manager/database/BaseItem.h"
#include "storage/manager/database/StorageFile.h"

#define OUT

namespace storage
{
	template <typename ITEM>
	class StorageManager
	{
	public:
		StorageManager(const std::string ip);
		~StorageManager() = default;

		void SaveItem(const ITEM& item);
		void GetNextItem(OUT ITEM& item) const;

	private:
		static logger::Logger logger;

		std::string mId;
		std::string mStorageDirPath;
		std::string mStorageFilePath;

		mutable storage::StorageFile<ITEM> mStorageFile;

	private:
		mutable uint32_t mCurrentItemOffset;
		mutable uint32_t mNextItemOffset;

		mutable uint32_t mNewItemOffset;
	};

	template <typename ITEM>
	logger::Logger StorageManager<ITEM>::logger("StorageManager");

	template <typename ITEM>
	StorageManager<ITEM>::StorageManager(const std::string ip) 
		: mId(ip) 
		, mStorageDirPath(std::string(PROJECT_ROOT) + "/storage/" + ip)
		, mStorageFilePath(mStorageDirPath + "/" + typeid(ITEM).name() + ".dat")
		, mStorageFile(mStorageFilePath)
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
				mStorageFile.SetFirstItemOffset(INVALID_OFFSET);
				mStorageFile.SetLastItemOffset(INVALID_OFFSET);
				mStorageFile.SetPaddingOffset(INVALID_OFFSET);
				
				// 2. 파일의 Header 영역을 세팅한다
				mStorageFile.UpdateFileHeader();
			}
		}
		else
		{
			// 1. 파일의 Header 영역에서 값을 읽어와서
			// 2. 메모리에 세팅해준다
			mStorageFile.ReadFileHeader();
		}

		// mCurrentItemOffset = mStorageFile.mFileHeaderStruct.LastItemOffset; 
		// mNewItemOffset = GetNewItemOffset();

		mNextItemOffset = 0;
	}
		
	template <typename ITEM>
	void StorageManager<ITEM>::SaveItem(const ITEM& item)
	{
		try
		{
			mStorageFile.AppendItem(item);
			logger.Info("Item saved successfully.");
		}
		catch (const std::exception& ex)
		{
			logger.Error(std::string("Error saving item: ") + ex.what());
			throw;
		}
	}

	template <typename ITEM>
	void StorageManager<ITEM>::GetNextItem(OUT ITEM& item) const
	{
		try
		{
			if (mNextItemOffset <= mStorageFile.GetLastItemOffset())
			{
				item = mStorageFile.ReadItem(mNextItemOffset);
				mNextItemOffset = mStorageFile.GetNextItemOffset(mNextItemOffset);
				logger.Info("Next item retrieved successfully.");
			}
			else
			{
				logger.Info("Waiting Next Item.");
				usleep(100000);
			}
		}
		catch (const std::exception& ex)
		{
			logger.Error(std::string("Error read item: ") + ex.what());
			throw;
		}
	}
}

#endif // STORAGE_MANAGER_H