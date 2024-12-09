#include <fstream>
#include "StorageFile.h"

namespace storage
{
	Header::Header(std::string filePath)
		: mStorageFilePath(filePath)
	{
	}

	Header::~Header()
	{
	}

	void Header::UpdateToFile()
	{
		std::ofstream file(mStorageFilePath, std::ios::binary | std::ios::in | std::ios::out);
		if (!file)
		{
			throw std::runtime_error("Failed to open file.");
		}
		
		{
			file.seekp(0);
			file.write(reinterpret_cast<const char*>(&mHeaderStruct), sizeof(storage::HeaderStruct));
		}

		file.close();
	}

	void Header::LoadFromFile()
	{
		std::ifstream file(mStorageFilePath, std::ios::binary);
		if (!file) 
		{
			throw std::runtime_error("Failed to open metadata file.");
		}
		
		{
			file.read(reinterpret_cast<char*>(&mHeaderStruct), sizeof(storage::HeaderStruct));
		}

		file.close();
	}

	uint32_t Header::GetFirstItemOffset() const
	{
		return mHeaderStruct.FirstItemOffset;
	}

	uint32_t Header::GetLastItemOffset() const
	{
		return mHeaderStruct.LastItemOffset;
	}

	uint32_t Header::GetCurrentItemOffset() const
	{
		return mHeaderStruct.CurrentItemOffset;
	}

	uint32_t Header::GetPaddingOffset() const
	{
		return mHeaderStruct.PaddingOffset;
	}

	void Header::SetFirstItemOffset(uint32_t firstItemOffset)
	{
		mHeaderStruct.FirstItemOffset = firstItemOffset;
	}

	void Header::SetLastItemOffset(uint32_t lastItemOffset)
	{
		mHeaderStruct.LastItemOffset = lastItemOffset;
	}

	void Header::SetCurrentItemOffset(uint32_t currentItemOffset)
	{
		mHeaderStruct.CurrentItemOffset = currentItemOffset;
	}

	void Header::SetPaddingOffset(uint32_t paddingOffset)
	{
		mHeaderStruct.PaddingOffset = paddingOffset;
	}
}