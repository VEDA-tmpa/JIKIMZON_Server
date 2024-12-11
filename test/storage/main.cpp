#include <cassert>
#include <vector>
#include <cstring>
#include <iostream>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

#include "storage/manager/database/JsonItem.h"
#include "storage/manager/StorageManager.h"


const std::string testIp = "127.0.1.2";
const std::string testDir = std::string(PROJECT_ROOT) + "/storage/" + testIp;


void CleanUpTestFiles() 
{
	std::filesystem::remove_all(testDir);
}

void test_1_json_save();
void test_3_json_save();

void test_1_json_save()
{
	std::string filePath = testDir + "/video.h264";
	std::filesystem::create_directories(std::filesystem::path(filePath).parent_path());

	storage::StorageManager<storage::JsonItem> storage(testIp);

	nlohmann::json obj = {
		{"id", 1},
		{"name", "snowdeer"},
		{"age", 45}
	};
	std::cout << "Target JSON: " << obj.dump(4) << std::endl;

	storage::JsonItem originItem(obj);

	storage.SaveItem(originItem);


	// // Open the file for reading
	// std::ifstream inFile(filePath, std::ios::binary);
	// if (!inFile) 
	// {
	// 	throw std::runtime_error("Failed to open file for reading: " + filePath);
	// }

	// // Move the cursor 12 bytes into the file for direct read
	// inFile.seekg(12, std::ios::beg);

	// // Read header struct
	// storage::ItemHeaderStruct itemHeaderStruct;
	// inFile.read(reinterpret_cast<char*>(&itemHeaderStruct), sizeof(storage::ItemHeaderStruct));
	// if (!inFile) 
	// {
	// 	throw std::runtime_error("File read failed.");
	// }
	// std::cout << "itemHeaderStruct.ItemSize: " + std::to_string(itemHeaderStruct.ItemSize) << std::endl;

	// // Read raw data based on ItemSize
	// std::vector<uint8_t> rawData(itemHeaderStruct.ItemSize);
	// inFile.read(reinterpret_cast<char*>(rawData.data()), rawData.size());
	// if (!inFile) 
	// {
	// 	throw std::runtime_error("File read failed.");
	// }
	// inFile.close();

	// storage::JsonItem item;
	// item.Deserialize(rawData);

	storage::JsonItem outItem;
	storage.GetNextItem(outItem);

	assert(originItem.GetData() == outItem.GetData());
}

void test_3_json_save()
{
	std::string filePath = testDir + "/video.h264";
	std::filesystem::create_directories(std::filesystem::path(filePath).parent_path());

	storage::StorageManager<storage::JsonItem> storage(testIp);

	nlohmann::json obj = {
		{"id", 1},
		{"name", "snowdeer"},
		{"age", 45}
	};
	std::cout << "Target JSON: " << obj.dump(4) << std::endl;
	nlohmann::json obj2 = {
		{"id", 2},
		{"name", "jigiseong"},
		{"age", 30}
	};
	std::cout << "Target JSON2: " << obj2.dump(4) << std::endl;
	nlohmann::json obj3 = {
		{"id", 3},
		{"name", "jiggyjiggy"},
		{"age", 30}
	};
	std::cout << "Target JSON3: " << obj3.dump(4) << std::endl;


	storage::JsonItem originItem(obj);
	storage::JsonItem originItem2(obj2);
	// storage::JsonItem originItem3(obj3);

	storage.SaveItem(originItem);
	storage.SaveItem(originItem2);
	// storage.SaveItem(originItem3);

	// Open the file for reading
	std::ifstream inFile(filePath, std::ios::binary);
	if (!inFile) 
	{
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








	inFile.seekg(12 + 39, std::ios::beg);

	// Read header struct
	// storage::ItemHeaderStruct itemHeaderStruct;
	inFile.read(reinterpret_cast<char*>(&itemHeaderStruct), sizeof(storage::ItemHeaderStruct));
	if (!inFile) 
	{
		throw std::runtime_error("File read failed.");
	}
	std::cout << "itemHeaderStruct.ItemSize: " + std::to_string(itemHeaderStruct.ItemSize) << std::endl;

	// Read raw data based on ItemSize
	std::vector<uint8_t> rawData2(itemHeaderStruct.ItemSize);
	inFile.read(reinterpret_cast<char*>(rawData2.data()), rawData2.size());
	if (!inFile) 
	{
		throw std::runtime_error("File read failed.");
	}
	inFile.close();

	storage::JsonItem item2;
	item2.Deserialize(rawData2);








	// inFile.seekg(12 + 39 + 41, std::ios::beg);

	// // Read header struct
	// // storage::ItemHeaderStruct itemHeaderStruct;
	// inFile.read(reinterpret_cast<char*>(&itemHeaderStruct), sizeof(storage::ItemHeaderStruct));
	// if (!inFile) 
	// {
	// 	throw std::runtime_error("File read failed.");
	// }
	// std::cout << "itemHeaderStruct.ItemSize: " + std::to_string(itemHeaderStruct.ItemSize) << std::endl;

	// // Read raw data based on ItemSize
	// std::vector<uint8_t> rawData3(itemHeaderStruct.ItemSize);
	// inFile.read(reinterpret_cast<char*>(rawData3.data()), rawData3.size());
	// if (!inFile) 
	// {
	// 	throw std::runtime_error("File read failed.");
	// }
	// inFile.close();

	// storage::JsonItem item3;
	// item3.Deserialize(rawData3);


	assert(originItem.GetData() == item.GetData());
	assert(originItem2.GetData() == item2.GetData());
	// assert(originItem3.GetData() == item3.GetData());




	// storage::JsonItem outItem;
	// storage.GetNextItem(outItem);

	// assert(originItem.GetData() == outItem.GetData());

	// storage::JsonItem outItem2;
	// storage.GetNextItem(outItem2);

	// assert(originItem2.GetData() == outItem2.GetData());

	// storage::JsonItem outItem3;
	// storage.GetNextItem(outItem3);

	// assert(originItem3.GetData() == outItem3.GetData());
}

int main()
{
	// CleanUpTestFiles();
	// test_1_json_save();

	
	CleanUpTestFiles();
	test_3_json_save();



	CleanUpTestFiles();


	return 0;
}
