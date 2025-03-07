	#include "JsonClient.h"

namespace cctv
{
	logger::Logger JsonClient::logger("JsonClient");

	JsonClient::JsonClient(const std::string& host, int port)
		: BaseClient(host, port) 
		, mDataBuffer("")
		, mStorageManager(host)
	{
		logger.Debug("json port: " + std::to_string(port));
	}

	void JsonClient::handleData()
	{
		char buffer[50];
		int received = receiveData(buffer, sizeof(buffer)); 
		if (received > 0)
		{
			try 
			{
				nlohmann::json json = receiveJson(buffer, received);
		
				mStorageManager.SaveData(json);
			}
			catch (const std::exception& e)
			{
				logger.Info("Retry parse JSON data: " + std::string(e.what()));
				// 불완전한 데이터는 로그만 남기고 재시도 루프 유지
			}
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
}
