#include <cassert>
#include <vector>
#include <cstring>
#include <iostream>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

#include "storage/manager/StorageManager.h"

const std::string testIp = "127.0.1.2";
const std::string testDir = std::string(PROJECT_ROOT) + "/storage/" + testIp;


void CleanUpTestFiles() 
{
	std::filesystem::remove_all(testDir);
}

void SetupTestFiles()
{
	std::string filePath = testDir + "/video.h264";
	std::filesystem::create_directories(std::filesystem::path(filePath).parent_path());
}

void test_concrete_size_json_n_save(int test_item_count)
{
	storage::StorageManager<nlohmann::json> storage(testIp);

	// 현재 타임스탬프 설정 (예제에서는 고정값 사용)
	std::string timestamp = "20241212_213705.707";

	// JSON 객체 생성
	for (int i = 0; i < test_item_count; ++i) 
	{
		nlohmann::json json;
		{
			json["frameId"] = i;
			json["timestamp"] = timestamp;

			nlohmann::json obj;
			obj["className"] = "plastic";
			obj["x"] = 0;
			obj["y"] = 0;
			obj["width"] = 0;
			obj["height"] = 0;

			json["object"].push_back(obj);

			std::cout << json.dump(4) << std::endl;
		}
	
		storage.SaveData(json);
	}
}

void test_variable_size_json_n_save(int test_item_count)
{
	std::string filePath = testDir + "/video.h264";
	std::filesystem::create_directories(std::filesystem::path(filePath).parent_path());

	storage::StorageManager<nlohmann::json> storage(testIp);

	// 현재 타임스탬프 설정 (예제에서는 고정값 사용)
	std::string timestamp = "20241212_213705.707";

	// JSON 객체 생성
	for (int i = 0; i < test_item_count; ++i) 
	{
		nlohmann::json json;
		{
			json["frameId"] = i;
			json["timestamp"] = timestamp;

			nlohmann::json obj;
			obj["className"] = "plastic";
			obj["x"] = i;
			obj["y"] = i;
			obj["width"] = i; 
			obj["height"] = i;

			json["object"].push_back(obj);

			std::cout << json.dump(4) << std::endl;
		}
	
		storage.SaveData(json);
	}
}

int main()
{
	CleanUpTestFiles();
	SetupTestFiles();
	test_concrete_size_json_n_save(400);

	// CleanUpTestFiles();


	return 0;
}
