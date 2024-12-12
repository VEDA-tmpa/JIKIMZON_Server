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
		std::vector<uint8_t> ReadData(uint32_t itemOffset) const;

		std::ofstream openOutFile();
		std::ifstream openInFile() const ;

	private:
		uint64_t getFileSize();

		void readWithWrapAround(std::ifstream& file, char* destination, uint32_t offset, uint32_t size, uint32_t maxOffset) const;
    	void writeWithWrapAround(std::ofstream& file, const char* source, uint32_t offset, uint32_t size, uint32_t maxOffset);


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
	void StorageFile<ITEM>::readWithWrapAround(std::ifstream& file, char* destination, uint32_t offset, uint32_t size, uint32_t maxOffset) const
	{
		if (offset + size > maxOffset)
		{
			uint32_t firstPartSize = maxOffset - offset;
			file.seekg(offset, std::ios::beg);
			file.read(destination, firstPartSize);

			file.seekg(0, std::ios::beg);
			file.read(destination + firstPartSize, size - firstPartSize);
		}
		else
		{
			file.seekg(offset, std::ios::beg);
			file.read(destination, size);
		}
	}

	template <typename ITEM>
	void StorageFile<ITEM>::writeWithWrapAround(std::ofstream& file, const char* source, uint32_t offset, uint32_t size, uint32_t maxOffset)
	{
		if (offset + size > maxOffset)
		{
			uint32_t firstPartSize = maxOffset - offset;
			file.seekp(offset, std::ios::beg);
			file.write(source, firstPartSize);

			file.seekp(0, std::ios::beg);
			file.write(source + firstPartSize, size - firstPartSize);
		}
		else
		{
			file.seekp(offset, std::ios::beg);
			file.write(source, size);
		}
	}

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
		{
			file.seekg(0, std::ios::beg);
			file.read(reinterpret_cast<char*>(&mFileHeaderStruct), sizeof(FileHeaderStruct));
			if (!file)
			{
				throw std::runtime_error("Failed to read FileHeaderStruct from file");
			}
		}

		logger.Debug("GetFirstItemOffset() mFileHeaderStruct.FirstItemOffset: " + std::to_string(mFileHeaderStruct.FirstItemOffset));

		return mFileHeaderStruct.FirstItemOffset;
	}

	template <typename ITEM>
	uint32_t StorageFile<ITEM>::GetLastItemOffset() const
	{
		std::ifstream file(mStorageFilePath, std::ios::binary | std::ios::in | std::ios::out);
		{
			file.seekg(0, std::ios::beg);
			file.read(reinterpret_cast<char*>(&mFileHeaderStruct), sizeof(FileHeaderStruct));
			if (!file)
			{
				throw std::runtime_error("Failed to read FileHeaderStruct from file");
			}
		}
			
		logger.Debug("GetLastItemOffset() mFileHeaderStruct.LastItemOffset: " + std::to_string(mFileHeaderStruct.LastItemOffset));

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

		// 처음에 첫 번째 아이템과 마지막 아이템 오프셋을 처리
		uint32_t writeOffset = 0;
		if (GetFirstItemOffset() == INVALID_OFFSET && GetLastItemOffset() == INVALID_OFFSET) 
		{
			// 첫 번째 아이템과 마지막 아이템 오프셋이 INVALID_OFFSET일 때 초기화
			SetFirstItemOffset(0);
			SetLastItemOffset(0);
			std::cout << "First and Last item offsets are INVALID. Initializing to 0." << std::endl;
		}
		else if (GetFirstItemOffset() == 0 && GetLastItemOffset() == 0)
		{
			// 첫 번째 아이템과 마지막 아이템이 0일 때, 다음 아이템 오프셋 계산
			writeOffset = GetNextItemOffset(GetLastItemOffset());
			std::cout << "First and Last item offsets are 0. Calculating next write offset: " << writeOffset << std::endl;
		}
		else
		{
			// 첫 번째와 마지막 아이템이 존재하는 경우, writeOffset을 lastItemOffset 이후로 설정
			writeOffset = GetNextItemOffset(GetLastItemOffset());
		}

		uint32_t nextItemOffset = writeOffset;

		// 덮어쓰기를 계속해서 확인
		while (nextItemOffset + itemSize > MAX_DATA_OFFSET)
		{
			uint32_t firstPartSize = MAX_DATA_OFFSET - nextItemOffset;
			std::cout << "Writing in two parts. First part size: " << firstPartSize << std::endl;

			file.seekp(sizeof(FileHeaderStruct) + nextItemOffset, std::ios::beg);
			file.write(reinterpret_cast<const char*>(serializedItem.data()), firstPartSize);
			std::cout << "First part written to offset: " << nextItemOffset << std::endl;

			file.seekp(sizeof(FileHeaderStruct), std::ios::beg);
			file.write(reinterpret_cast<const char*>(serializedItem.data()) + firstPartSize, itemSize - firstPartSize);
			std::cout << "Second part written starting from offset: " << 0 << std::endl;

			nextItemOffset = (nextItemOffset + itemSize) % MAX_DATA_OFFSET;
			std::cout << "Next item offset after write: " << nextItemOffset << std::endl;
		}

		// 일반적인 데이터 쓰기
		if (nextItemOffset + itemSize <= MAX_DATA_OFFSET)
		{
			file.seekp(sizeof(FileHeaderStruct) + nextItemOffset, std::ios::beg);
			file.write(reinterpret_cast<const char*>(serializedItem.data()), itemSize);
			nextItemOffset = (nextItemOffset + itemSize) % MAX_DATA_OFFSET;
			std::cout << "Written data in a single block to offset: " << nextItemOffset << std::endl;
		}

		// 마지막 아이템의 오프셋 갱신
		SetLastItemOffset(nextItemOffset);
		std::cout << "Updated last item offset to: " << nextItemOffset << std::endl;

		// 첫 번째 아이템 오프셋 갱신
		if (GetFirstItemOffset() == INVALID_OFFSET) 
		{
			// 첫 번째 아이템 오프셋이 INVALID_OFFSET이라면, 현재 아이템의 오프셋을 첫 번째 아이템으로 설정
			SetFirstItemOffset(nextItemOffset);
			std::cout << "First item offset was INVALID. Setting it to next item offset: " << nextItemOffset << std::endl;
		}
		else if (nextItemOffset == writeOffset)
		{
			// 덮어쓰기 되는 경우, 첫 번째 아이템의 오프셋을 다시 설정
			SetFirstItemOffset(nextItemOffset);
			std::cout << "Overwriting occurred. First item offset updated to: " << nextItemOffset << std::endl;
		}

		// 첫 번째 아이템의 오프셋 확인
		uint32_t firstItemOffset = GetFirstItemOffset();
		std::cout << "First item offset: " << firstItemOffset << std::endl;
	}



// template <typename ITEM>
// void StorageFile<ITEM>::EnqueueItem(const ITEM& item)
// {
// 	std::cout << "init " << std::endl;
// 	GetFirstItemOffset();
// 	GetLastItemOffset();

//     std::ofstream file = openOutFile();
//     std::vector<uint8_t> serializedItem = item.Serialize();
//     uint32_t itemSize = serializedItem.size();

//     uint32_t writeOffset = 0;
// 	uint32_t itemOffset  = 0;
//     // 첫 번째 아이템 오프셋과 마지막 아이템 오프셋을 확인
//     if (GetFirstItemOffset() == INVALID_OFFSET && GetLastItemOffset() == INVALID_OFFSET) 
//     {
//         SetLastItemOffset(0);
//         SetFirstItemOffset(0);
//         std::cout << "First and Last item offsets are INVALID. Initializing to 0." << std::endl;
// 		// goto aa;
//     }
//     else if (GetFirstItemOffset() == 0 && GetLastItemOffset() == 0)
//     {
//         writeOffset = GetNextItemOffset(GetLastItemOffset());
//         std::cout << "First and Last item offsets are 0. Calculating next write offset: " << writeOffset << std::endl;
// 		// goto aa;

//     }
// 	else {
    
// 		nextItemOffset = writeOffset;

// 		// 덮어쓰기를 계속해서 확인
// 		while (nextItemOffset + itemSize > MAX_DATA_OFFSET)
// 		{
// 			uint32_t firstPartSize = MAX_DATA_OFFSET - nextItemOffset;
// 			std::cout << "Writing in two parts. First part size: " << firstPartSize << std::endl;

// 			file.seekp(sizeof(FileHeaderStruct) + nextItemOffset, std::ios::beg);
// 			file.write(reinterpret_cast<const char*>(serializedItem.data()), firstPartSize);
// 			std::cout << "First part written to offset: " << nextItemOffset << std::endl;

// 			file.seekp(sizeof(FileHeaderStruct), std::ios::beg);
// 			file.write(reinterpret_cast<const char*>(serializedItem.data()) + firstPartSize, itemSize - firstPartSize);
// 			std::cout << "Second part written starting from offset: " << 0 << std::endl;

// 			nextItemOffset = (nextItemOffset + itemSize) % MAX_DATA_OFFSET;
// 			std::cout << "Next item offset after write: " << nextItemOffset << std::endl;
// 		}
// 	}

//     // 일반적인 데이터 쓰기
//     if (writeOffset + itemSize <= MAX_DATA_OFFSET)
//     {
//         file.seekp(sizeof(FileHeaderStruct) + writeOffset, std::ios::beg);
//         file.write(reinterpret_cast<const char*>(serializedItem.data()), itemSize);
//         nextItemOffset = (writeOffset + itemSize) % MAX_DATA_OFFSET;
//         std::cout << "Written data in a single block to offset: " << nextItemOffset << std::endl;
//     }

//     // 마지막 아이템의 오프셋 갱신
//     SetLastItemOffset(writeOffset);
//     std::cout << "Updated last item offset to: " << writeOffset << std::endl;

//     // 첫 번째 아이템의 오프셋 갱신
//     if (nextItemOffset == writeOffset)
//     {
//         SetFirstItemOffset(nextItemOffset);
//         std::cout << "Overwriting occurred. First item offset updated to: " << nextItemOffset << std::endl;
//     }

//     GetFirstItemOffset();
//     GetLastItemOffset();

// }



// template <typename ITEM>
// void StorageFile<ITEM>::EnqueueItem(const ITEM& item)
// {
//     std::ofstream file = openOutFile();
//     std::vector<uint8_t> serializedItem = item.Serialize();
//     uint32_t itemSize = serializedItem.size();

//     uint32_t writeOffset = (GetLastItemOffset() == INVALID_OFFSET) ? 0 : GetLastItemOffset();
//     uint32_t firstItemOffset = GetFirstItemOffset(); // 첫 번째 아이템의 오프셋
//     uint32_t nextItemOffset = writeOffset; // 다음 아이템의 오프셋

//     // 덮어쓰기를 계속해서 확인
//     while (nextItemOffset + itemSize > MAX_DATA_OFFSET)
//     {
//         // 덮어쓰는 영역을 확인하고, 다음 아이템으로 넘어가기
//         uint32_t firstPartSize = MAX_DATA_OFFSET - nextItemOffset;
//         file.seekp(sizeof(FileHeaderStruct) + nextItemOffset, std::ios::beg);
//         file.write(reinterpret_cast<const char*>(serializedItem.data()), firstPartSize);

//         file.seekp(sizeof(FileHeaderStruct), std::ios::beg);
//         file.write(reinterpret_cast<const char*>(serializedItem.data()) + firstPartSize, itemSize - firstPartSize);

//         nextItemOffset = (nextItemOffset + itemSize) % MAX_DATA_OFFSET;
//     }

//     // 일반적인 데이터 쓰기
//     if (nextItemOffset + itemSize <= MAX_DATA_OFFSET)
//     {
//         file.seekp(sizeof(FileHeaderStruct) + nextItemOffset, std::ios::beg);
//         file.write(reinterpret_cast<const char*>(serializedItem.data()), itemSize);
//         nextItemOffset = (nextItemOffset + itemSize) % MAX_DATA_OFFSET;
//     }

//     // 마지막 아이템의 오프셋 갱신
//     SetLastItemOffset(nextItemOffset);

//     // 덮어쓴 후 첫 번째 아이템 갱신
//     if (nextItemOffset >= firstItemOffset)
//     {
//         // 덮어썼다면, 첫 번째 아이템의 오프셋을 갱신
//         SetFirstItemOffset(nextItemOffset);
//     }

// 	GetFirstItemOffset();
// }

	template <typename ITEM>
	std::vector<uint8_t> StorageFile<ITEM>::ReadData(uint32_t itemOffset) const
	{
		std::cout << "itemOffset: " << itemOffset << " MAX_DATA_OFFSET: " << MAX_DATA_OFFSET << std::endl;
		assert(itemOffset <= MAX_DATA_OFFSET);

		std::ifstream file = openInFile();

		ItemHeaderStruct headerStruct;
		file.seekg(sizeof(FileHeaderStruct) + itemOffset, std::ios::beg);
		if (itemOffset + sizeof(ItemHeaderStruct) > MAX_DATA_OFFSET) 
		{
			uint32_t firstPartSize = MAX_DATA_OFFSET - itemOffset;
			file.read(reinterpret_cast<char*>(&headerStruct), firstPartSize);

			file.seekg(sizeof(FileHeaderStruct) + 0, std::ios::beg);
			file.read(reinterpret_cast<char*>(&headerStruct) + firstPartSize, sizeof(ItemHeaderStruct) - firstPartSize);
		} 
		else 
		{
			file.read(reinterpret_cast<char*>(&headerStruct), sizeof(ItemHeaderStruct));
		}

		// 데이터 크기만큼 공간을 할당
		std::vector<uint8_t> data(headerStruct.DataSize);
		std::cout << "data.size() " << data.size() << std::endl;

		uint32_t dataOffset = itemOffset + sizeof(ItemHeaderStruct);
		if (dataOffset + headerStruct.DataSize > MAX_DATA_OFFSET) 
		{
			uint32_t firstPartSize = MAX_DATA_OFFSET - dataOffset;
			file.read(reinterpret_cast<char*>(data.data()), firstPartSize);

			file.seekg(sizeof(FileHeaderStruct) + 0, std::ios::beg);
			file.read(reinterpret_cast<char*>(data.data()) + firstPartSize, headerStruct.DataSize - firstPartSize);
		} 
		else 
		{
			file.seekg(sizeof(FileHeaderStruct) + dataOffset, std::ios::beg);
			file.read(reinterpret_cast<char*>(data.data()), headerStruct.DataSize);
		}

		return data;
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
	std::ifstream StorageFile<ITEM>::openInFile() const
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