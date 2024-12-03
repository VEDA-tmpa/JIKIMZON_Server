#include <vector>
#include <memory>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

#include "common/tcp/client/BaseClient.h"
#include "common/cipher/ChaCha20CipherAdapter.h"
#include "cctv/video/VideoClient.h"
#include "cctv/json/JsonClient.h"
#include "test/fixture/Fixture.h"

// 클라이언트 타입
enum class ClientType { Video, Json };

// CCTV 설정 구조체
struct CCTVConfig {
	std::string ip;
	int port;
	ClientType clientType;
};

// 팩토리 함수
std::unique_ptr<tcp::BaseClient> createClient(const CCTVConfig& config, const std::string& keyPath) 
{
	auto cipherHandler = std::make_unique<cipher::ChaCha20CipherAdapter>(keyPath);

	if (config.clientType == ClientType::Video) 
	{
		return std::make_unique<cctv::VideoClient>(config.ip, config.port, std::move(cipherHandler));
	} 
	else if (config.clientType == ClientType::Json) 
	{
		return std::make_unique<cctv::JsonClient>(config.ip, config.port, std::move(cipherHandler));
	}
	
	return nullptr;
}

int main(void) 
{
	std::vector<CCTVConfig> cctvConfigs = {
		{ fixture::cctv1.ip, fixture::cctv1.videoPort, ClientType::Video },
		{ fixture::cctv2.ip, fixture::cctv2.videoPort, ClientType::Video },
		{ fixture::cctv3.ip, fixture::cctv3.videoPort, ClientType::Video },
		{ fixture::cctv1.ip, fixture::cctv1.jsonPort, ClientType::Json },
		{ fixture::cctv2.ip, fixture::cctv2.jsonPort, ClientType::Json },
		{ fixture::cctv3.ip, fixture::cctv3.jsonPort, ClientType::Json }
	};

	// 각 CCTV를 처리
	for (const auto& config : cctvConfigs) {
		
		pid_t pid = fork();
		if (pid < 0) 
		{
			perror("Fork failed");
			exit(EXIT_FAILURE);
		} 
		else if (pid == 0) // 자식 프로세스
		{ 
			const std::string keyPath = std::string(PROJECT_ROOT) + "/cctv/" + config.ip + "/keyfile.bin";
			auto client = createClient(config, keyPath);
			if (client != nullptr) 
			{
				client->Start();
			}

			exit(EXIT_SUCCESS);
		}
		
	}

	std::cout << "Parent process finished launching CCTV clients." << std::endl;

	return EXIT_SUCCESS;
}