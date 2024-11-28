	#include "JsonClient.h"

namespace cctv
{
	logger::Logger JsonClient::logger("JsonClient");

	JsonClient::JsonClient(const std::string& host, int port, std::unique_ptr<cipher::ICiphable> cipherHandler)
		: BaseClient(host, port, std::move(cipherHandler)) 
		, mDataBuffer("")
	{
		logger.Debug("json port: " + std::to_string(port));
	}

	JsonClient::~JsonClient() 
	{
	}

	void JsonClient::handleData(const void* buffer, size_t size)
	{
		// 수신된 데이터를 버퍼에 추가
		mDataBuffer.append(static_cast<const char*>(buffer), size);
		logger.Debug("json buffer: " + mDataBuffer);

		size_t delimiterPos = 0;
		while ((delimiterPos = mDataBuffer.find(JSON_DELIMITER)) != std::string::npos)
		{
			// 구분자 이전까지의 데이터 추출
			std::string jsonString = mDataBuffer.substr(0, delimiterPos);

			try
			{
				// JSON 파싱 시도
				auto json = nlohmann::json::parse(jsonString);
				logger.Debug("Received JSON: \n" + json.dump(4));

				// JSON 처리
				saveJson(json);
			}
			catch (const nlohmann::json::parse_error& e)
			{
				logger.Warning("JSON parse error: " + std::string(e.what()));
			}

			// 처리된 데이터 제거 (구분자 포함)
			mDataBuffer.erase(0, delimiterPos + 1);
		}
	}

	void JsonClient::saveJson(nlohmann::json json)
	{
		logger.Debug("Received JSON: " + json.dump(4));


		std::string filePath = std::string(PROJECT_ROOT) + "/storage/" + mHost + ".json";
		logger.Info("filePath: " + filePath);

		FILE* file = fopen(filePath.c_str(), "ab");
		if (!file)
		{
			logger.Error("Failed to open output file");
			return;
		}

		{
			std::string jsonString = json.dump();
			storage::SaveToFile(file, jsonString.c_str(), jsonString.size());
		}

		fclose(file);
	}
}
