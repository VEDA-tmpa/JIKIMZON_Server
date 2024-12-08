#ifndef STORAGE_FILE_H
#define STORAGE_FILE_H

#include <cstdint>
#include <string>

namespace storage
{
    constexpr uint32_t MAX_FILE_SIZE = 4L * 1024 * 1024 * 1024; // 4GB

	/*
	max file size = 10GB
	------------ Header ------------
	|     FirstItemOffset   (4)    |
	|     LastItemOffset    (4)    |
	|    CurrentItemOffset  (4)    |
	|    PaddingItemOffset  (4)    |
	------------- Data -------------
	|                              |
	|                              |
	|                              |
	|                              |
	|                              |
	|                              |
	|                              |
	--------------------------------
	*/
	struct HeaderStruct
	{
		uint32_t FirstItemOffset;
		uint32_t LastItemOffset;     

		uint32_t CurrentItemOffset; 

		uint32_t PaddingItemOffset;
	};

	class Header
	{
	public:
		Header(std::string filePath);
		~Header();

		void Update();
		void Load();

		uint32_t GetFirstItemOffset() const;
		uint32_t GetLastItemOffset() const;
		uint32_t GetCurrentItemOffset() const;
		uint32_t GetPaddingItemOffset() const;

		void SetFirstItemOffset(uint32_t firstItemOffset);
		void SetLastItemOffset(uint32_t lastItemOffset);
		void SetCurrentItemOffset(uint32_t currentItemOffset);
		void SetPaddingItemOffset(uint32_t paddingItemOffset);
	
	private:
		std::string mStorageFilePath;

		HeaderStruct mHeaderStruct;
	}
}

#endif // STORAGE_FILE_H