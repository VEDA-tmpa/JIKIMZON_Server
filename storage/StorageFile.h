#ifndef STORAGE_FILE_H
#define STORAGE_FILE_H

#include <cstdint>
#include <string>

namespace storage
{
    // constexpr uint64_t MAX_FILE_SIZE = 4L * 1024 * 1024 * 1024; // 4GB
    constexpr uint64_t MAX_FILE_SIZE = 100 * 1024 * 1024; // 100MB

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

		uint32_t PaddingOffset;
	};

	class Header
	{
	public:
		Header(std::string filePath);
		~Header();

		void UpdateToFile();
		void LoadFromFile();

		uint32_t GetFirstItemOffset() const;
		uint32_t GetLastItemOffset() const;
		uint32_t GetCurrentItemOffset() const;
		uint32_t GetPaddingOffset() const;

		void SetFirstItemOffset(uint32_t firstItemOffset);
		void SetLastItemOffset(uint32_t lastItemOffset);
		void SetCurrentItemOffset(uint32_t currentItemOffset);
		void SetPaddingOffset(uint32_t paddingOffset);
	
	private:
		std::string mStorageFilePath;

		HeaderStruct mHeaderStruct;
	};
}

#endif // STORAGE_FILE_H