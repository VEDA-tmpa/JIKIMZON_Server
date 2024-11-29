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

	void JsonClient::handleData()
	{
		char buffer[50]; // TODO: 버퍼에 쌓여있고, 늦게들어온다면, 버퍼 속 구분자(|)를 확인하는 로직 추가필요
		int received = receiveData(buffer, sizeof(buffer)); 
		if (received > 0)
		{
			nlohmann::json json = receiveJson(buffer, received);
			saveJson(json);
		}
	}

	nlohmann::json JsonClient::receiveJson(const void* buffer, size_t size)
	{
		// 수신된 데이터를 버퍼에 추가
		mDataBuffer.append(static_cast<const char*>(buffer), size);
		logger.Debug("json buffer: " + mDataBuffer);

		// 구분자를 찾는다
		size_t delimiterPos = mDataBuffer.find(JSON_DELIMITER);
		if (delimiterPos == std::string::npos)
		{
			// 구분자가 없다면 아직 JSON이 완전하지 않음
			throw std::runtime_error("Incomplete JSON data. Waiting for more data.");
		}

		// 구분자 이전까지의 JSON 문자열 추출
		std::string jsonString = mDataBuffer.substr(0, delimiterPos);

		// 남은 데이터를 mDataBuffer에 유지
		mDataBuffer.erase(0, delimiterPos + 1);

		// JSON 파싱 시도
		try
		{
			nlohmann::json json = nlohmann::json::parse(jsonString);
			logger.Debug("Received JSON: \n" + json.dump(4));
			return json;
		}
		catch (const nlohmann::json::parse_error& e)
		{
			logger.Warning("JSON parse error: " + std::string(e.what()));
			throw std::runtime_error("Failed to parse JSON data.");
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
