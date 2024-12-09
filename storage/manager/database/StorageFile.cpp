#include <fstream>
#include "StorageFile.h"

namespace storage
{
	StorageFile::StorageFile(std::string filePath)
		: mStorageFilePath(filePath)
	{
	}

	void StorageFile::UpdateFileHeader()
	{
		std::ofstream file(mStorageFilePath, std::ios::binary);
		if (!file) 
		{
			throw std::runtime_error("Failed to open file for writing.");
		}

		file.write(reinterpret_cast<const char*>(&mHeaderStruct), sizeof(storage::HeaderStruct));
		if (file.fail()) 
		{
			throw std::runtime_error("Failed to write header to file.");
		}
	}

	void StorageFile::ReadFileHeader()
	{
		std::ifstream file(mStorageFilePath, std::ios::binary);
		if (!file) 
		{
			throw std::runtime_error("Failed to open file for reading.");
		}

		file.read(reinterpret_cast<char*>(&mHeaderStruct), sizeof(storage::HeaderStruct));
		if (file.fail()) 
		{
			throw std::runtime_error("Failed to read header from file.");
		}
	}

	uint32_t StorageFile::GetFirstItemOffset() const
	{
		return mHeaderStruct.FirstItemOffset;
	}

	uint32_t StorageFile::GetLastItemOffset() const
	{
		return mHeaderStruct.LastItemOffset;
	}

	uint32_t StorageFile::GetCurrentItemOffset() const
	{
		return mHeaderStruct.CurrentItemOffset;
	}

	uint32_t StorageFile::GetPaddingOffset() const
	{
		return mHeaderStruct.PaddingOffset;
	}

	void StorageFile::SetFirstItemOffset(uint32_t firstItemOffset)
	{
		mHeaderStruct.FirstItemOffset = firstItemOffset;
	}

	void StorageFile::SetLastItemOffset(uint32_t lastItemOffset)
	{
		mHeaderStruct.LastItemOffset = lastItemOffset;
	}

	void StorageFile::SetCurrentItemOffset(uint32_t currentItemOffset)
	{
		mHeaderStruct.CurrentItemOffset = currentItemOffset;
	}

	void StorageFile::SetPaddingOffset(uint32_t paddingOffset)
	{
		mHeaderStruct.PaddingOffset = paddingOffset;
	}
}