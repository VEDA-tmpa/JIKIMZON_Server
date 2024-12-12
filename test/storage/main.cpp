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
void test_n_json_save(int test_item_count);

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
	storage::JsonItem originItem3(obj3);

	storage.SaveItem(originItem);
	storage.SaveItem(originItem2);
	storage.SaveItem(originItem3);

	// Open the file for reading
	std::ifstream inFile(filePath, std::ios::binary);
	if (!inFile) 
	{
		throw std::runtime_error("Failed to open file for reading: " + filePath);
	}

	// Move the cursor 12 bytes into the file for direct read
	inFile.seekg(sizeof(storage::FileHeaderStruct), std::ios::beg);

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

	storage::JsonItem item;
	item.Deserialize(rawData);




	inFile.seekg(sizeof(storage::FileHeaderStruct) + sizeof(storage::ItemHeaderStruct) + itemHeaderStruct.ItemSize, std::ios::beg);
	std::cout << "expected seekg cursor : " + std::to_string(sizeof(storage::FileHeaderStruct) + sizeof(storage::ItemHeaderStruct) + itemHeaderStruct.ItemSize) << std::endl;

	storage::ItemHeaderStruct itemHeaderStruct2;
	inFile.read(reinterpret_cast<char*>(&itemHeaderStruct2), sizeof(storage::ItemHeaderStruct));
	if (!inFile) 
	{
		throw std::runtime_error("File read failed. 너 맞아?");
	}
	std::cout << "itemHeaderStruct2.ItemSize: " + std::to_string(itemHeaderStruct2.ItemSize) << std::endl;


// const int startOffset = 40; // 탐색 시작 위치
// const int endOffset = 60;  // 탐색 끝 위치
// const size_t itemSize = 36; // JSON 데이터 크기

// for (int i = startOffset; i < endOffset; ++i) {
// 	try {
// 		// 커서를 i 위치로 이동
// 		inFile.seekg(i, std::ios::beg);
// 		if (!inFile) {
// 			throw std::runtime_error("파일 커서 이동 실패");
// 		}

// 		// ItemSize만큼의 데이터를 읽음
// 		std::vector<uint8_t> rawData2(itemSize);
// 		inFile.read(reinterpret_cast<char*>(rawData2.data()), rawData2.size());
// 		if (!inFile) {
// 			throw std::runtime_error("파일 읽기 실패");
// 		}

// 		// JSON 디시리얼라이즈 시도
// 		storage::JsonItem item2;
// 		item2.Deserialize(rawData2);

// 		// 디시리얼라이즈 성공 시 JSON 객체 출력
// 		std::cout << "JSON 디시리얼라이즈 성공! 위치: " << i << std::endl;
// 		std::cout << item2.GetData().dump(4) << std::endl; // JsonItem 객체 출력 (ToString 구현 필요)
// 	} catch (const std::exception& e) {
// 		// 예외 발생 시 메시지 출력 및 다음 i로 이동
// 		std::cerr << "오류 발생 (위치 " << i << "): " << e.what() << std::endl;
// 		continue;
// 	}
// }



	// Read raw data based on ItemSize
	std::vector<uint8_t> rawData2(itemHeaderStruct2.ItemSize);
	inFile.read(reinterpret_cast<char*>(rawData2.data()), rawData2.size());
	if (!inFile) 
	{
		throw std::runtime_error("File read failed.");
	}

	storage::JsonItem item2;
	item2.Deserialize(rawData2);








	inFile.seekg(12 + 39 + 40, std::ios::beg);

	// Read header struct
	// storage::ItemHeaderStruct itemHeaderStruct;
	inFile.read(reinterpret_cast<char*>(&itemHeaderStruct), sizeof(storage::ItemHeaderStruct));
	if (!inFile) 
	{
		throw std::runtime_error("File read failed.");
	}
	std::cout << "itemHeaderStruct.ItemSize: " + std::to_string(itemHeaderStruct.ItemSize) << std::endl;

	// Read raw data based on ItemSize
	std::vector<uint8_t> rawData3(itemHeaderStruct.ItemSize);
	inFile.read(reinterpret_cast<char*>(rawData3.data()), rawData3.size());
	if (!inFile) 
	{
		throw std::runtime_error("File read failed.");
	}
	inFile.close();

	storage::JsonItem item3;
	item3.Deserialize(rawData3);


	assert(originItem.GetData() == item.GetData());
	assert(originItem2.GetData() == item2.GetData());
	assert(originItem3.GetData() == item3.GetData());




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

void test_n_json_save(int test_item_count)
{
	std::string filePath = testDir + "/video.h264";
	std::filesystem::create_directories(std::filesystem::path(filePath).parent_path());

	storage::StorageManager<storage::JsonItem> storage(testIp);

	for (int i = 0; i < test_item_count; ++i) {
		nlohmann::json obj = {
			{"id", i},
			{"name", "user" + std::to_string(i)},
			{"age", 30 + (i % 10)}
		};
		std::cout << "Target JSON " << i << ": " << obj.dump(4) << std::endl;

		storage::JsonItem originItem(obj);
		storage.SaveItem(originItem);





		storage::JsonItem item;
		storage.GetNextItem(item);

		assert(item.GetData() == obj);
		std::cout << "읽은 데이터 : " + item.GetData().dump(4) << std::endl;
	}

	// while (true)
	// {
	// 	storage::JsonItem item;
	// 	storage.GetNextItem(item);
	// 	std::cout << "읽은 데이터 : " + item.GetData().dump(4) << std::endl;
	// }


	// // Read saved items
	// std::ifstream inFile(filePath, std::ios::binary);
	// if (!inFile) 
	// {
	//     throw std::runtime_error("Failed to open file for reading: " + filePath);
	// }


	// inFile.seekg(sizeof(storage::FileHeaderStruct), std::ios::beg);
	

	// for (int i = 0; i < test_item_count; ++i) {
	// 	if (inFile.tellg() == inFile.eof())
	// 	{
	// 		inFile.seekg(sizeof(storage::FileHeaderStruct), std::ios::beg);	
	// 	}

	//     storage::ItemHeaderStruct itemHeaderStruct;
	//     inFile.read(reinterpret_cast<char*>(&itemHeaderStruct), sizeof(storage::ItemHeaderStruct));
	//     if (!inFile) 
	//     {
	//         throw std::runtime_error("File read failed.");
	//     }

	//     std::cout << "itemHeaderStruct.ItemSize: " + std::to_string(itemHeaderStruct.ItemSize) << std::endl;

	//     std::vector<uint8_t> rawData(itemHeaderStruct.ItemSize);
	//     inFile.read(reinterpret_cast<char*>(rawData.data()), rawData.size());
	//     if (!inFile) 
	//     {
	//         throw std::runtime_error("File read failed.");
	//     }

	//     storage::JsonItem item;
	//     item.Deserialize(rawData);

	//     // Verify the saved item
	//     assert(item.GetData() == nlohmann::json({
	//         {"id", i},
	//         {"name", "user" + std::to_string(i)},
	//         {"age", 30 + (i % 10)}
	//     }));
	// }

	// inFile.close();
}


int main()
{
	// CleanUpTestFiles();
	// test_1_json_save();

	
	// CleanUpTestFiles();
	// test_3_json_save();

	CleanUpTestFiles();
	test_n_json_save(24340);

	// CleanUpTestFiles();


	return 0;
}
