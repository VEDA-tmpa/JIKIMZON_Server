#ifndef STORAGE_FILE_H
#define STORAGE_FILE_H

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
	|      PaddingOffset   (4)     |
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

		uint32_t PaddingOffset;
	};

	/*
	------------ Header ------------
	|         ItemSize    (4)     |
	------------- Data -------------
	|              ...             |
	--------------------------------
	*/
	struct ItemHeaderStruct
	{
		uint32_t ItemSize;
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
		uint32_t GetPaddingOffset() const;

		void SetFirstItemOffset(uint32_t firstItemOffset);
		void SetLastItemOffset(uint32_t lastItemOffset);
		void SetPaddingOffset(uint32_t paddingOffset);
	
		uint32_t GetNextItemOffset(uint32_t itemOffset) const;

		void AppendItem(const ITEM& item);
		std::vector<uint8_t> ReadItem(uint32_t itemOffset) const;

	private:
		static logger::Logger logger;

		std::string mStorageFilePath;

		mutable FileHeaderStruct mFileHeaderStruct;
	};

	template <typename ITEM>
	logger::Logger StorageFile<ITEM>::logger("StorageFile");


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
		logger.Debug("ReadFileHeader() mFileHeaderStruct.PaddingOffset: " + std::to_string(mFileHeaderStruct.PaddingOffset));
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
	uint32_t StorageFile<ITEM>::GetPaddingOffset() const
	{
		std::ifstream file(mStorageFilePath, std::ios::binary | std::ios::in | std::ios::out);
		file.read(reinterpret_cast<char*>(&mFileHeaderStruct), sizeof(FileHeaderStruct));

		return mFileHeaderStruct.PaddingOffset;
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
	}	

	template <typename ITEM>
	void StorageFile<ITEM>::SetPaddingOffset(uint32_t paddingOffset)
	{
		std::fstream file(mStorageFilePath, std::ios::binary | std::ios::in | std::ios::out);
		if (!file)
		{
			throw std::runtime_error("Failed to open file: " + mStorageFilePath);
		}

		{
			size_t paddingOffsetFilePosition = offsetof(FileHeaderStruct, PaddingOffset);
			file.seekp(paddingOffsetFilePosition, std::ios::beg);
			if (!file)
			{
				throw std::runtime_error("Failed to seek to PaddingOffset in file");
			}
			file.write(reinterpret_cast<const char*>(&paddingOffset), sizeof(paddingOffset));
			if (!file)
			{
				throw std::runtime_error("Failed to write PaddingOffset to file");
			}
		}

		mFileHeaderStruct.PaddingOffset = paddingOffset;
	}

	template <typename ITEM>
	uint32_t StorageFile<ITEM>::GetNextItemOffset(uint32_t itemOffset) const
	{
	// 	// Open the file in binary mode
	// 	std::ifstream file(mStorageFilePath, std::ios::binary | std::ios::in | std::ios::out);
	// 	if (!file)
	// 	{
	// 		throw std::runtime_error("Failed to open file: " + mStorageFilePath);
	// 	}

	// 	// Check the file size to ensure sufficient data is available
	// 	file.seekg(0, std::ios::end);
	// 	size_t fileSize = file.tellg();
	// 	logger.Debug("File size: " + std::to_string(fileSize));

	// 	// Calculate the position to seek to
	// 	size_t seekPosition = sizeof(FileHeaderStruct) + itemOffset;
	// 	logger.Debug("Seeking to position: " + std::to_string(seekPosition));

	// 	if (seekPosition >= fileSize)
	// 	{
	// 		throw std::runtime_error("Seek position is beyond the file size." + mStorageFilePath + "; Position: " + std::to_string(seekPosition));
	// 	}

	// 	// Seek to the desired position in the file
	// 	file.seekg(seekPosition, std::ios::beg);
	// 	if (!file.good())
	// 	{
	// 		throw std::runtime_error("Failed to seek to the correct position in file.");
	// 	}

	// 	// Read the item header
	// 	ItemHeaderStruct itemHeaderStruct;
	// 	logger.Debug("Reading ItemHeaderStruct at position: " + std::to_string(file.tellg()));

	// 	file.read(reinterpret_cast<char*>(&itemHeaderStruct), sizeof(ItemHeaderStruct));
	// 	if (!file)
	// 	{
	// 		throw std::runtime_error("Failed to read item header. Current position: " + std::to_string(file.tellg()));
	// 	}
	// 	logger.Debug("ItemHeaderStruct.ItemSize: " + std::to_string(itemHeaderStruct.ItemSize));

	// 	// Ensure the next item offset does not exceed the file size
	// 	uint32_t nextItemOffset = (seekPosition - sizeof(FileHeaderStruct)) + (sizeof(ItemHeaderStruct) + itemHeaderStruct.ItemSize);
	// 	logger.Debug("Calculated next item offset: " + std::to_string(nextItemOffset));

	// 	if (nextItemOffset > storage::MAX_DATA_OFFSET)
	// 	{
	// 		throw std::runtime_error("Next item offset exceeds file size. Offset: " + std::to_string(nextItemOffset));
	// 	}

	// 	return nextItemOffset;
	// }

	template <typename ITEM>
	void StorageFile<ITEM>::AppendItem(const ITEM& item)
	{
	// 	std::vector<uint8_t> serializedData = item.Serialize();
	// 	logger.Debug("Serialized data size: " + std::to_string(serializedData.size()));

	// 	std::ofstream file(mStorageFilePath, std::ios::binary | std::ios::in | std::ios::out);
	// 	if (!file)
	// 	{
	// 		throw std::runtime_error("Failed to open file for appending: " + mStorageFilePath);
	// 	}
	// 	logger.Debug("Opened file for appending: " + mStorageFilePath);

	// 	uint32_t itemInsertOffset = 0;

	// 	if (mFileHeaderStruct.LastItemOffset == INVALID_OFFSET && mFileHeaderStruct.FirstItemOffset == INVALID_OFFSET) // 비어있던 경우
	// 	{
	// 		itemInsertOffset = 0;
	// 		logger.Debug("File is empty. Setting first and last item offsets to 0.");

	// 		mFileHeaderStruct.FirstItemOffset = itemInsertOffset;
	// 		mFileHeaderStruct.LastItemOffset = itemInsertOffset;

	// 		logger.Debug("FirstItemOffset set to: " + std::to_string(mFileHeaderStruct.FirstItemOffset));
	// 		logger.Debug("LastItemOffset set to: " + std::to_string(mFileHeaderStruct.LastItemOffset));
	// 	}
	// 	else if (mFileHeaderStruct.LastItemOffset == 0 && mFileHeaderStruct.FirstItemOffset == 0) // 아이템이 하나있던 경우
	// 	{   
	// 		itemInsertOffset = GetNextItemOffset(mFileHeaderStruct.LastItemOffset);
	// 		logger.Debug("One item exists. Next item offset: " + std::to_string(itemInsertOffset));

	// 		mFileHeaderStruct.LastItemOffset = itemInsertOffset;
	// 		logger.Debug("LastItemOffset updated to: " + std::to_string(mFileHeaderStruct.LastItemOffset));
	// 	}
	// 	else // 두번째 이후 아이템 추가
	// 	{
	// 		logger.Debug("aaaaaaaa FirstItemOffset: " + std::to_string(mFileHeaderStruct.FirstItemOffset));
	// 		logger.Debug("aaaaaaaa LastItemOffset: " + std::to_string(mFileHeaderStruct.LastItemOffset));
	// 		logger.Debug("aaaaaaaa PaddingOffset: " + std::to_string(mFileHeaderStruct.PaddingOffset));

	// 		itemInsertOffset = GetNextItemOffset(mFileHeaderStruct.LastItemOffset);
	// 		logger.Debug("aaaaaaaa after GetNextItemOffset    itemInsertOffset: " + std::to_string(itemInsertOffset));

	// 		logger.Debug("Next item offset: " + std::to_string(itemInsertOffset));

	// 		logger.Debug("aaaaaaaa itemInsertOffset + item.Size(): " + std::to_string(itemInsertOffset + item.Size()));
	// 		logger.Debug("aaaaaaaa storage::MAX_DATA_OFFSET: " + std::to_string(storage::MAX_DATA_OFFSET));

	// 		// uint32_t oldPaddingOffset = mFileHeaderStruct.PaddingOffset;
	// 		// uint32_t paddingOffset = -1;
	// 		if (itemInsertOffset + item.Size() >= storage::MAX_DATA_OFFSET)
	// 		{
	// 			mFileHeaderStruct.PaddingOffset = itemInsertOffset;
	// 			// paddingOffset = itemInsertOffset;
	// 			logger.Debug("File exceeds MAX_FILE_SIZE. Padding offset set to: " + std::to_string(mFileHeaderStruct.PaddingOffset));

	// 			itemInsertOffset = 0;

	// 			file.seekp(sizeof(FileHeaderStruct) + itemInsertOffset, std::ios::beg);
	// 			logger.Debug("Seek position after padding: " + std::to_string(file.tellp()));
	// 		}

	// 		logger.Debug("aaaaaaaa itemInsertOffset: " + std::to_string(itemInsertOffset));
	// 		logger.Debug("aaaaaaaa FirstItemOffset: " + std::to_string(mFileHeaderStruct.FirstItemOffset));
	// 		if (itemInsertOffset <= mFileHeaderStruct.FirstItemOffset)
	// 		{
	// 			uint32_t itemOffset = mFileHeaderStruct.FirstItemOffset;
	// 			logger.Debug("Item insert offset is less than or equal to first item offset. Searching for valid offset.");





	// 			if (itemOffset == mFileHeaderStruct.PaddingOffset)
	// 			{
	// 				itemOffset = 0;
	// 			}
	// 			while ((itemInsertOffset + item.Size() > itemOffset))
	// 			{
					
	// 				itemOffset = GetNextItemOffset(itemOffset);
					

	// 				logger.Debug("New item offset: " + std::to_string(itemOffset));
	// 			}

	// 			mFileHeaderStruct.FirstItemOffset = itemOffset;
	// 			logger.Debug("FirstItemOffset updated to: " + std::to_string(mFileHeaderStruct.FirstItemOffset));
	// 		}

	// 		// if (paddingOffset != -1)
	// 		// {
	// 		// 	mFileHeaderStruct.PaddingOffset = paddingOffset;
	// 		// }
	// 		mFileHeaderStruct.LastItemOffset = itemInsertOffset;
	// 		logger.Debug("LastItemOffset updated to: " + std::to_string(mFileHeaderStruct.LastItemOffset));
	// 	}

	// 	// fileWrite
	// 	file.seekp(sizeof(FileHeaderStruct) + itemInsertOffset, std::ios::beg);

	// 	logger.Debug("Seek position before writing target itemInsertOffset: " + std::to_string(itemInsertOffset));
	// 	logger.Debug("expected Seek position before writing: " + std::to_string(sizeof(FileHeaderStruct) + itemInsertOffset));
	// 	logger.Debug("Seek position before writing: " + std::to_string(file.tellp()));

	// 	file.write(reinterpret_cast<const char*>(serializedData.data()), serializedData.size());
	// 	logger.Debug("target size of serialized data : " + std::to_string(serializedData.size()));
	// 	logger.Debug("cursor tellp()  ; After file write: " + std::to_string(file.tellp()));
	// 	if (file.fail())
	// 	{
	// 		throw std::runtime_error("Failed to append item to file: " + mStorageFilePath);
	// 	}
	// 	logger.Debug("Successfully appended item at offset: " + std::to_string(itemInsertOffset));

	// 	UpdateFileHeader();
	// 	logger.Debug("File header updated.");
	}


	template <typename ITEM>
	std::vector<uint8_t> StorageFile<ITEM>::ReadItem(uint32_t itemOffset) const
	{
		// std::vector<uint8_t> out; // 반환할 데이터를 담을 벡터

		// // 파일 열기
		// std::ifstream file(mStorageFilePath, std::ios::binary | std::ios::in);
		// if (!file)
		// {
		// 	throw std::runtime_error("Failed to open file for reading.");
		// }

		// // 로그 출력
		// logger.Debug("ReadItem init itemOffset: " + std::to_string(itemOffset));
		// logger.Debug("ReadItem init mFileHeaderStruct.PaddingOffset: " + std::to_string(mFileHeaderStruct.PaddingOffset));

		// // PaddingOffset 처리
		// if (itemOffset == mFileHeaderStruct.PaddingOffset)
		// {
		// 	itemOffset = 0;
		// }

		// // 파일 포인터 이동
		// file.seekg(sizeof(FileHeaderStruct) + itemOffset, std::ios::beg);
		// logger.Debug("itemOffset: " + std::to_string(itemOffset));
		// logger.Debug("sizeof(FileHeaderStruct) + itemOffset: " + std::to_string(sizeof(FileHeaderStruct) + itemOffset));
		// logger.Debug("file.tellg(): " + std::to_string(file.tellg()));

		// // 헤더 읽기
		// ItemHeaderStruct itemHeaderStruct;
		// file.read(reinterpret_cast<char*>(&itemHeaderStruct), sizeof(ItemHeaderStruct));
		// if (file.fail())
		// {
		// 	throw std::runtime_error("Failed to read item header.");
		// }

		// logger.Debug("itemHeaderStruct.ItemSize: " + std::to_string(itemHeaderStruct.ItemSize));

		// // 데이터를 읽어 벡터에 저장
		// out.resize(itemHeaderStruct.ItemSize);
		// file.read(reinterpret_cast<char*>(out.data()), out.size());
		// if (file.fail())
		// {
		// 	throw std::runtime_error("Failed to read item data.");
		// }

		// // 반환
		// return out;
	}

}

#endif // STORAGE_FILE_H