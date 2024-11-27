#include "JsonClient.h"

namespace cctv
{
    JsonClient::JsonClient(const std::string& host, int port, std::unique_ptr<cipher::ICiphable> cipherHandler)
        : BaseClient(host, port, std::move(cipherHandler)) 
		, mDataBuffer("")
	{
	}

    JsonClient::~JsonClient() 
	{
	}

    void JsonClient::handleData(const void* buffer, size_t size)
    {
		// 수신된 데이터를 버퍼에 추가
		mDataBuffer.append(static_cast<const char*>(buffer), size);

		// JSON 데이터가 완전한지 확인
		if (!nlohmann::json::accept(mDataBuffer))
		{
			logger.Warn("Incomplete JSON data, waiting for more data.");
			return;
		}

		try
		{
			// JSON 파싱 시도
			auto json = nlohmann::json::parse(mDataBuffer);

			// JSON 처리
			saveJson(json);

			// 파싱 성공 시, 데이터 버퍼 초기화
			mDataBuffer.clear();
		}
		catch (const nlohmann::json::parse_error& e)
		{
			logger.Warn("JSON parse error: " + std::string(e.what()));
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
			SaveToFile(file, jsonString.c_str(), jsonString.size());
		}

		fclose(file);
	}
}
