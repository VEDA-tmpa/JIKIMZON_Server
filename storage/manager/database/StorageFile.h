#ifndef STORAGE_FILE_H
#define STORAGE_FILE_H

#include <cassert>
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <cstddef>

#include "common/log/Logger.h"

namespace storage
{
	// constexpr uint64_t MAX_FILE_SIZE = 4L * 1024 * 1024 * 1024; // 4GB
	// constexpr uint64_t MAX_FILE_SIZE = 100 * 1024 * 1024; // 100MB
	// constexpr uint64_t MAX_FILE_SIZE = 10 * 1024 * 1024; // 10MB
	// constexpr uint32_t MAX_FILE_SIZE = 1 * 1024 * 1024; // 1MB
	// constexpr uint32_t MAX_FILE_SIZE = 0.1L * 1024 * 1024; // 0.1MB
	constexpr uint32_t MAX_FILE_SIZE = 0.01L * 1024 * 1024; // 0.01MB

	/*
	------------ Header ------------
	|      FirstItemOffset (4)     |
	|      LastItemOffset  (4)     |
	------------- Data -------------
	| item1 |      item2      | .. |
	--------------------------------
	|              ...             |
	--------------------------------
	|              ...             |
	--------------------------------
	|              ...             |
	--------------------------------
	|              ...             |
	--------------------------------
	*/
	struct FileHeaderStruct
	{
		uint32_t FirstItemOffset;
		uint32_t LastItemOffset;     
	};

	/*
	------------ Header ------------
	|          DataSize    (4)     |
	------------- Data -------------
	|              ...             |
	--------------------------------
	*/
	struct ItemHeaderStruct
	{
		uint32_t DataSize;
	};
	constexpr uint32_t MAX_DATA_OFFSET = MAX_FILE_SIZE - sizeof(FileHeaderStruct);
	constexpr uint32_t INVALID_OFFSET = MAX_DATA_OFFSET + 1;

	struct ItemStruct
	{
		ItemHeaderStruct HeaderStruct;
		std::vector<uint8_t> Data;
	};

	template <typename ITEM>
	class StorageFile
	{
	public:
		StorageFile(const std::string filePath);
		~StorageFile() = default;

		void ReadFileHeader();
		
		uint32_t GetFirstItemOffset() const;
		uint32_t GetLastItemOffset() const;

		void SetFirstItemOffset(uint32_t firstItemOffset);
		void SetLastItemOffset(uint32_t lastItemOffset);
	
		uint32_t GetNextItemOffset(uint32_t itemOffset) const;

		void EnqueueItem(const ITEM& item);
		std::vector<uint8_t> ReadItem(uint32_t itemOffset) const;

		std::ofstream openOutFile();
		std::ifstream openInFile();

	private:
		uint32_t GetNextValidOffset(uint32_t offset, uint32_t size);
		uint32_t GetItemSize(uint32_t offset);


		uint64_t getFileSize();


	private:
		static logger::Logger logger;

		std::string mStorageFilePath;

		mutable FileHeaderStruct mFileHeaderStruct;
	};

	template <typename ITEM>
	logger::Logger StorageFile<ITEM>::logger("StorageFile");

	template <typename ITEM>
	uint64_t StorageFile<ITEM>::getFileSize()
	{
		ifstream file = openInFile();

		file.seekg(0, std::ios::end);
		size_t fileSize = file.tellg();

		logger.Debug("actual File size: " + std::to_string(fileSize));
		logger.Debug("expected File size: " + std::to_string(MAX_FILE_SIZE));
		assert(fileSize == MAX_FILE_SIZE);

		return fileSize;
	}



	// void readWithWrapAround(
	// 	std::ifstream& file,
	// 	char* destination,
	// 	uint32_t offset,
	// 	uint32_t size,
	// 	uint32_t maxOffset)
	// {
	// 	// 현재 오프셋이 MAX_DATA_OFFSET을 넘어가는지 확인
	// 	if (offset + size > maxOffset) 
	// 	{
	// 		uint32_t firstPartSize = maxOffset - offset;

	// 		// 첫 번째 부분 읽기
	// 		file.seekg(offset, std::ios::beg);
	// 		file.read(destination, firstPartSize);

	// 		// 두 번째 부분 읽기 (wrap-around)
	// 		file.seekg(0, std::ios::beg);
	// 		file.read(destination + firstPartSize, size - firstPartSize);
	// 	} 
	// 	else 
	// 	{
	// 		// Wrap-around가 필요 없는 경우
	// 		file.seekg(offset, std::ios::beg);
	// 		file.read(destination, size);
	// 	}
	// }



	// void writeWithWrapAround(std::ofstream& file,
	// 						 const char* source,
	// 						 uint32_t offset,
	// 						 uint32_t size,
	// 						 uint32_t maxOffset)
	// {
	// 	// 현재 오프셋이 MAX_DATA_OFFSET을 넘어가는지 확인
	// 	if (offset + size > maxOffset) 
	// 	{
	// 		uint32_t firstPartSize = maxOffset - offset;

	// 		// 첫 번째 부분 쓰기
	// 		file.seekp(offset, std::ios::beg);
	// 		file.write(source, firstPartSize);

	// 		// 두 번째 부분 쓰기 (wrap-around)
	// 		file.seekp(0, std::ios::beg);
	// 		file.write(source + firstPartSize, size - firstPartSize);
	// 	} 
	// 	else 
	// 	{
	// 		// Wrap-around가 필요 없는 경우
	// 		file.seekp(offset, std::ios::beg);
	// 		file.write(source, size);
	// 	}
	// }






	template <typename ITEM>
	StorageFile<ITEM>::StorageFile(const std::string filePath)
		: mStorageFilePath(filePath)
	{
	}

	template <typename ITEM>
	void StorageFile<ITEM>::ReadFileHeader()
	{
		std::ifstream file(mStorageFilePath, std::ios::binary | std::ios::in | std::ios::out);
		if (!file)
		{
			throw std::runtime_error("Failed to open file for reading.");
		}

		file.read(reinterpret_cast<char*>(&mFileHeaderStruct), sizeof(FileHeaderStruct));
		if (file.fail())
		{
			throw std::runtime_error("Failed to read file header.");
		}
		logger.Debug("ReadFileHeader() mFileHeaderStruct.FirstItemOffset: " + std::to_string(mFileHeaderStruct.FirstItemOffset));
		logger.Debug("ReadFileHeader() mFileHeaderStruct.LastItemOffset: " + std::to_string(mFileHeaderStruct.LastItemOffset));
	}

	template <typename ITEM>
	uint32_t StorageFile<ITEM>::GetFirstItemOffset() const
	{
		std::ifstream file(mStorageFilePath, std::ios::binary | std::ios::in | std::ios::out);
		file.read(reinterpret_cast<char*>(&mFileHeaderStruct), sizeof(FileHeaderStruct));

		return mFileHeaderStruct.FirstItemOffset;
	}

	template <typename ITEM>
	uint32_t StorageFile<ITEM>::GetLastItemOffset() const
	{
		std::ifstream file(mStorageFilePath, std::ios::binary | std::ios::in | std::ios::out);
		file.read(reinterpret_cast<char*>(&mFileHeaderStruct), sizeof(FileHeaderStruct));

		return mFileHeaderStruct.LastItemOffset;
	}

	template <typename ITEM>
	void StorageFile<ITEM>::SetFirstItemOffset(uint32_t firstItemOffset)
	{
		std::fstream file(mStorageFilePath, std::ios::binary | std::ios::in | std::ios::out);
		if (!file)
		{
			throw std::runtime_error("Failed to open file: " + mStorageFilePath);
		}

		{
			size_t firstItemOffsetFilePosition = offsetof(FileHeaderStruct, FirstItemOffset);
			file.seekp(firstItemOffsetFilePosition, std::ios::beg);
			if (!file)
			{
				throw std::runtime_error("Failed to seek to FirstItemOffset in file");
			}
			file.write(reinterpret_cast<const char*>(&firstItemOffset), sizeof(firstItemOffset));
			if (!file)
			{
				throw std::runtime_error("Failed to write FirstItemOffset to file");
			}
		}
		
		mFileHeaderStruct.FirstItemOffset = firstItemOffset;

		logger.Debug("SetFirstItemOffset: " + std::to_string(firstItemOffset));
	}

	template <typename ITEM>
	void StorageFile<ITEM>::SetLastItemOffset(uint32_t lastItemOffset)
	{
		std::fstream file(mStorageFilePath, std::ios::binary | std::ios::in | std::ios::out);
		if (!file)
		{
			throw std::runtime_error("Failed to open file: " + mStorageFilePath);
		}

		{
			size_t lastItemOffsetFilePosition = offsetof(FileHeaderStruct, LastItemOffset);
			file.seekp(lastItemOffsetFilePosition, std::ios::beg);
			if (!file)
			{
				throw std::runtime_error("Failed to seek to LastItemOffset in file");
			}
			file.write(reinterpret_cast<const char*>(&lastItemOffset), sizeof(lastItemOffset));
			if (!file)
			{
				throw std::runtime_error("Failed to write LastItemOffset to file");
			}
		}

		mFileHeaderStruct.LastItemOffset = lastItemOffset;

		logger.Debug("SetLastItemOffset: " + std::to_string(lastItemOffset));
	}	

	template <typename ITEM>
	uint32_t StorageFile<ITEM>::GetNextItemOffset(uint32_t itemOffset) const
	{
		assert(itemOffset <= MAX_DATA_OFFSET);

		std::ifstream file = openInFile();

		ItemHeaderStruct headerStruct;
		file.seekg(sizeof(FileHeaderStruct) + itemOffset | std::ios::beg);
		if (itemOffset + sizeof(ItemHeaderStruct) > MAX_DATA_OFFSET) 
		{
			uint32_t firstPartSize = MAX_DATA_OFFSET - itemOffset;
			file.read(reinterpret_cast<char*>(&headerStruct), firstPartSize);

			file.seekg(sizeof(FileHeaderStruct) + 0 | std::ios::beg);
			file.read(reinterpret_cast<char*>(&headerStruct) + firstPartSize, sizeof(ItemHeaderStruct) - firstPartSize);
		} 
		else 
		{
			file.read(reinterpret_cast<char*>(&headerStruct), sizeof(ItemHeaderStruct));
		}

		uint32_t nextItemOffset = itemOffset + sizeof(ItemHeaderStruct) + headerStruct.DataSize;
		assert(nextItemOffset <= MAX_DATA_OFFSET);

		return nextItemOffset;
	}


	template <typename ITEM>
	void StorageFile<ITEM>::EnqueueItem(const ITEM& item)
	{
		std::ofstream file = openOutFile();
		std::vector<uint8_t> serializedItem = item.Serialize();
		uint32_t itemSize = serializedItem.size();

		uint32_t writeOffset = (GetLastItemOffset() == INVALID_OFFSET) ? 0 : GetLastItemOffset();

		if (writeOffset + itemSize > MAX_DATA_OFFSET) 
		{
			uint32_t firstPartSize = MAX_DATA_OFFSET - writeOffset;

			file.seekp(sizeof(FileHeaderStruct) + writeOffset, std::ios::beg);
			file.write(reinterpret_cast<const char*>(serializedItem.data()), firstPartSize);

			file.seekp(sizeof(FileHeaderStruct), std::ios::beg);
			file.write(reinterpret_cast<const char*>(serializedItem.data()) + firstPartSize, itemSize - firstPartSize);
		}
		else 
		{
			file.seekp(sizeof(FileHeaderStruct) + writeOffset, std::ios::beg);
			file.write(reinterpret_cast<const char*>(serializedItem.data()), itemSize);
		}

		// 다음 아이템 위치로 writeOffset 갱신
		writeOffset = (writeOffset + itemSize) % MAX_DATA_OFFSET;
		SetLastItemOffset(writeOffset);
	}

	template <typename ITEM>
	std::vector<uint8_t> StorageFile<ITEM>::ReadItem(uint32_t itemOffset) const
	{
		assert(itemOffset <= MAX_DATA_OFFSET);

		std::ifstream file = openInFile();

		ItemHeaderStruct headerStruct;
		file.seekg(sizeof(FileHeaderStruct) + itemOffset | std::ios::beg);
		if (itemOffset + sizeof(ItemHeaderStruct) > MAX_DATA_OFFSET) 
		{
			uint32_t firstPartSize = MAX_DATA_OFFSET - itemOffset;
			file.read(reinterpret_cast<char*>(&headerStruct), firstPartSize);

			file.seekg(sizeof(FileHeaderStruct) + 0 | std::ios::beg);
			file.read(reinterpret_cast<char*>(&headerStruct) + firstPartSize, sizeof(ItemHeaderStruct) - firstPartSize);
		} 
		else 
		{
			file.read(reinterpret_cast<char*>(&headerStruct), sizeof(ItemHeaderStruct));
		}


		std::vector<uint8_t> binaryItem;
		binaryItem.reserve(sizeof(ItemHeaderStruct) + headerStruct.DataSize);

		std::memcpy(binaryItem.data(), &headerStruct, sizeof(ItemHeaderStruct));


		uint32_t dataOffset = itemOffset + sizeof(ItemHeaderStruct);
		if (dataOffset + headerStruct.DataSize > MAX_DATA_OFFSET) 
		{
			uint32_t firstPartSize = MAX_DATA_OFFSET - dataOffset;
			file.read(reinterpret_cast<char*>(binaryItem.data() + sizeof(ItemHeaderStruct)), firstPartSize);

			file.seekg(sizeof(FileHeaderStruct) + 0 | std::ios::beg);
			file.read(reinterpret_cast<char*>(binaryItem.data() + sizeof(ItemHeaderStruct) + firstPartSize), headerStruct.DataSize - firstPartSize);
		} 
		else 
		{
			file.seekg(sizeof(FileHeaderStruct) + dataOffset | std::ios::beg);
			file.read(reinterpret_cast<char*>(binaryItem.data() + sizeof(ItemHeaderStruct)), headerStruct.DataSize);
		}

		return binaryItem;
	}

	template <typename ITEM>
	std::ofstream StorageFile<ITEM>::openOutFile()
	{
		std::ofstream file(mStorageFilePath, std::ios::binary | std::ios::in | std::ios::out);
		if (!file)
		{
			throw std::runtime_error("Failed to open file for appending: " + mStorageFilePath);
		}
		logger.Debug("Opened file for appending: " + mStorageFilePath);

		return file;
	}

	template <typename ITEM>
	std::ifstream StorageFile<ITEM>::openInFile()
	{
		std::ifstream file(mStorageFilePath, std::ios::binary | std::ios::in);
		if (!file)
		{
			throw std::runtime_error("Failed to open file for reading." + mStorageFilePath);
		}
		logger.Debug("Opened file for reading: " + mStorageFilePath);

		return file;
	}
}

#endif // STORAGE_FILE_H