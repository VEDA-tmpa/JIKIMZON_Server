#ifndef STORAGE_FILE_H
#define STORAGE_FILE_H

#include <cstdint>
#include <string>

namespace storage
{
	// constexpr uint64_t MAX_FILE_SIZE = 4L * 1024 * 1024 * 1024; // 4GB
	constexpr uint64_t MAX_FILE_SIZE = 100 * 1024 * 1024; // 100MB

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
	struct HeaderStruct
	{
		uint32_t FirstItemOffset;
		uint32_t LastItemOffset;     

		uint32_t PaddingOffset;
	};

	struct ItemStruct
	{
		std::vector<uint8_t> data;
	}

	class StorageFile
	{
	public:
		StorageFile(std::string filePath);
		~StorageFile() = default;

		void UpdateFileHeader();
		void ReadFileHeader();

		uint32_t GetFirstItemOffset() const;
		uint32_t GetLastItemOffset() const;
		uint32_t GetPaddingOffset() const;

		void SetFirstItemOffset(uint32_t firstItemOffset);
		void SetLastItemOffset(uint32_t lastItemOffset);
		void SetPaddingOffset(uint32_t paddingOffset);
	
	private:
		std::string mStorageFilePath;

		HeaderStruct mHeaderStruct;
	};
}

#endif // STORAGE_FILE_H