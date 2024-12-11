#include <cassert>
#include <vector>
#include <cstring>
#include <iostream>
#include <filesystem>

#include <nlohmann/json.hpp>

#include "storage/manager/database/JsonItem.h"
#include "storage/manager/database/StorageFile.h"

void CleanUpTestFiles() 
{
	std::filesystem::remove_all(std::string(PROJECT_ROOT) + "/storage/test");
}

void test_direct_file_manipulate_json_save_load();

void test_direct_file_manipulate_json_save_load()
{
	std::string filePath = std::string(PROJECT_ROOT) + "/storage/test/file_direct_write_read";
	std::filesystem::create_directories(std::filesystem::path(filePath).parent_path());

	nlohmann::json obj = {
		{"id", 1},
		{"name", "snowdeer"},
		{"age", 45}
	};
	std::cout << "Target JSON: " << obj.dump(4) << std::endl;

	storage::JsonItem originItem(obj);
	std::vector<uint8_t> serializedData = originItem.Serialize();


	// Open file for writing
	std::ofstream outFile(filePath, std::ios::binary);
	if (!outFile) 
	{
		throw std::runtime_error("Failed to open file for writing: " + filePath);
	}

	// Move the cursor 12 bytes into the file for direct write
	outFile.seekp(12, std::ios::beg);
	outFile.write(reinterpret_cast<const char*>(serializedData.data()), serializedData.size());
	if (!outFile) 
	{
		throw std::runtime_error("File write failed.");
	}
	outFile.close();



	// Open file for reading
	std::ifstream inFile(filePath, std::ios::binary);
	if (!inFile) {
		throw std::runtime_error("Failed to open file for reading: " + filePath);
	}

	// Move the cursor 12 bytes into the file for direct read
	inFile.seekg(12, std::ios::beg);

	// Read header struct
	storage::ItemHeaderStruct itemHeaderStruct;
	inFile.read(reinterpret_cast<char*>(&itemHeaderStruct), sizeof(storage::ItemHeaderStruct));
	if (!inFile) 
	{
		throw std::runtime_error("File read failed.");
	}
	std::cout << "itemHeaderStruct.ItemSize: " + std::to_string(itemHeaderStruct.ItemSize) << std::endl;

	// Read raw data based on ItemSize
	std::vector<uint8_t> rawData(itemHeaderStruct.ItemSize);
	inFile.read(reinterpret_cast<char*>(rawData.data()), rawData.size());
	if (!inFile) 
	{
		throw std::runtime_error("File read failed.");
	}
	inFile.close();


	storage::JsonItem item;
	item.Deserialize(rawData);

	assert(originItem.GetData() == item.GetData());
}

int main()
{
	CleanUpTestFiles();
	test_direct_file_manipulate_json_save_load();

	return 0;
}
