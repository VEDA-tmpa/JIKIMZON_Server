#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include "Client.h"
#include "common/frame/Frame.h"
#include "common/cipher/ICiphable.h"

namespace cctv
{
	logger::Logger Client::logger("Client");

	Client::Client(std::string& host, int port, std::unique_ptr<cipher::ICiphable> cipherHandler)
		: mHost(host)
		, mPort(port)
		, mbClosed(false)
		, mSocketFd(-1)
		, mCipherHandler(std::move(cipherHandler))
	{
	}

	Client::~Client()
	{
		Close();
	}

	void Client::Start()
	{
		connectToServer();

		receiveFrames(saveFrameToFile);

	}

	void Client::saveFrameToFile(FILE* file, const char* frameData, size_t frameSize)
	{
		fwrite(frameData, sizeof(char), frameSize, file);
		logger.Info("Frame saved to file");
	}

	/*
	- inet_pton: 문자열 형식의 IP 주소를 네트워크 형식으로 변환하는 함수.
	- htons: 호스트 바이트 순서를 네트워크 바이트 순서로 변환.
	- sockaddr_in: IPv4 주소 정보를 저장하는 구조체.
	- AF_INET: IPv4 인터넷 프로토콜을 의미하는 주소 패밀리.
	*/
	void Client::connectToServer()
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
		sockaddr_in server_addr = { 0, };
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(mPort);
		inet_pton(AF_INET, mHost.c_str(), &server_addr.sin_addr);
		logger.Info("sockaddr_in structure setting success");

		// 서버에 연결
		if (connect(mSocketFd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
		{
			logger.Error("connect() fail");
			return;
		}
		logger.Info("connect() success");
	}

	void Client::Close()
	{
		if (mbClosed)
		{
			return;
		}

		mbClosed = true;
		close(mSocketFd);
		logger.Info("close(socketFd)");
	}

	int Client::receiveData(void* buffer, size_t size)
	{
		int totalBytesReceived = 0;
		while (totalBytesReceived < size)
		{
			int bytesReceived = recv(mSocketFd,
									 reinterpret_cast<char*>(buffer) + totalBytesReceived,
									 size - totalBytesReceived,
									 0);
			if (bytesReceived < 0)
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

	void Client::receiveFrames(SaveFrameHandler saveFrameHandler)
	{
		std::string filePath = std::string(PROJECT_ROOT) + "/storage/" + mHost + ".h264";
		logger.Info("filePath: " + filePath);

		FILE* file = fopen(filePath.c_str(), "ab");
		if (!file)
		{
			logger.Error("Failed to open output file");
			return;
		}

		while (true)
		{
			// 1. Header 수신
			logger.Info("Header receive");
			std::vector<uint8_t> headerBuffer(sizeof(frame::HeaderStruct));
			int headerResult = receiveData(headerBuffer.data(), sizeof(frame::HeaderStruct));
			logger.Debug("Header receive size");
			if (headerResult <= 0)
			{
				goto end;
			}

			// 2. Header 역직렬화
			logger.Info("Header deserialize");
			frame::Header header;
			header.Deserialize(headerBuffer);
			logger.Debug("Header received. FrameId: " + std::to_string(header.GetFrameId()) +
						 ", BodySize: " + std::to_string(header.GetBodySize()));

			// 3. Body 수신
			logger.Info("Body receive");
			std::vector<uint8_t> bodyBuffer(header.GetBodySize());
			int bodyResult = 0;
			if (header.GetBodySize() > 0)
			{
				bodyResult = receiveData(bodyBuffer.data(), header.GetBodySize());
			}

			if (bodyResult < 0)
			{
				goto end;
			}

			// Body 복호화
			std::vector<uint8_t> decrypted;
			std::string timestamp = header.GetTimestamp();
			std::vector<uint8_t> nonce(12, 0x00);
			std::copy(timestamp.end() - 12, timestamp.end(), nonce.begin());
			mCipherHandler->Decrypt(bodyBuffer, decrypted, nonce);

			// 4. Body 역직렬화 (Body가 0일 경우 처리하지 않음)
			logger.Info("Body deserialize");
			frame::Body body;
			if (bodyResult > 0)
			{
				body.Deserialize(bodyBuffer);
			}

			// 5. Frame 객체 생성
			logger.Info("Frame init");
			frame::Frame frame(header, body);
			std::vector<uint8_t> frameBuffer;
			frame.Serialize(frameBuffer);
			
			// 6. 저장
			saveFrameHandler(file, reinterpret_cast<const char*>(frameBuffer.data()), frameBuffer.size());
		}
end:
		fclose(file);
	}
}