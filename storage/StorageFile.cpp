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

	void Header::Update()
	{
		std::ofstream file(mStorageFilePath, std::ios::binary | std::ios::in | std::ios::out);
		if (!file)
		{
			throw std::runtime_error("Failed to open file.");
		}
		
		{
			file.seekp(0);
			file.write(reinterpret_cast<const char*>(&mHeaderStruct), sizeof(video::HeaderStruct));
		}

		file.close();
	}

	void Header::Load()
	{
		std::ifstream file(mStorageFilePath, std::ios::binary);
		if (!file) 
		{
			throw std::runtime_error("Failed to open metadata file.");
		}
		
		{
			file.read(reinterpret_cast<char*>(&mHeaderStruct), sizeof(video::HeaderStruct));
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

	uint32_t Header::GetPaddingItemOffset() const
	{
		return mHeaderStruct.PaddingItemOffset;
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

	void Header::SetPaddingItemOffset(uint32_t paddingItemOffset)
	{
		mHeaderStruct.PaddingItemOffset = paddingItemOffset;
	}
}