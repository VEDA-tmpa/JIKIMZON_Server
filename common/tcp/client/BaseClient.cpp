#include "BaseClient.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace tcp
{
	void SaveToFile(FILE* file, const char* data, size_t size)
	{
		fwrite(data, sizeof(char), size, file);
	}

	logger::Logger BaseClient::logger("BaseClient");

	BaseClient::BaseClient(const std::string& host, int port, std::unique_ptr<cipher::ICiphable> cipherHandler)
		: mHost(host)
		, mPort(port)
		, mSocketFd(-1)
		, mbClosed(true) 
		, mCipherHandler(std::move(cipherHandler))
	{
	}

	BaseClient::~BaseClient()
	{
		Close();
	}

	void BaseClient::Start()
	{
		connectToServer();

		char buffer[4096];
		while (mbClosed == false)
		{
			int received = receiveData(buffer, sizeof(buffer));
			if (received > 0)
			{
				handleData(buffer, received);
			}
		}
	}

	void BaseClient::Close()
	{
		if (mbClosed == false)
		{
			close(mSocketFd);
			mbClosed = true;
		}
	}

	void BaseClient::connectToServer()
	{
		logger.Info("connectToServer() start");
		
		// 소켓 생성
		mSocketFd = socket(AF_INET, SOCK_STREAM, 0);
		if (mSocketFd < 0)
		{
			logger.Error("socket() fail");
			return;
		}
		logger.Info("socket() success");

		// 서버 주소 설정
		sockaddr_in serverAddr{};
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(mPort);
		inet_pton(AF_INET, mHost.c_str(), &serverAddr.sin_addr);
		logger.Info("sockaddr_in structure setting success");

		// 서버에 연결
		if (connect(mSocketFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
		{
			logger.Error("connect() fail");
			return;
		}
		mbClosed = false;
		logger.Info("connect() success");
	}

	int BaseClient::receiveData(void* buffer, size_t size)
	{
		int totalBytesReceived = 0;

		while (totalBytesReceived < size)
		{
			int bytesReceived = recv(mSocketFd,
									 reinterpret_cast<char*>(buffer) + totalBytesReceived,
									 size - totalBytesReceived,
									 0);

			if (bytesReceived < 0)  // 에러 발생
			{
				if (errno == EINTR)
				{
					continue; // 인터럽트 발생 시 재시도
				}
				logger.Error("recv() failed while receiving data");
				return -1;
			}
			else if (bytesReceived == 0) // 서버가 연결을 종료함
			{
				logger.Info("Connection closed by server");
				return 0;
			}

			totalBytesReceived += bytesReceived;
		}

		return totalBytesReceived;
	}
}
