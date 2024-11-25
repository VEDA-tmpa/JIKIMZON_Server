#include <algorithm>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

#include "viewer/Server.h"
#include "common/frame/Frame.h"
#include "test/fixture/Fixture.h"
#include "common/cipher/ChaCha20.h"

namespace viewer
{
	logger::Logger Server::logger("Server");

	Server::Server(int port, std::unique_ptr<cipher::ICiphable> cipherHandler)
		: mPort(port)
		, mServerSocketFd(-1)
		, mCipherHandler(std::move(cipherHandler))
	{
	}

	void Server::Start() 
	{
		setupServer();
		
		while (true)
		{
			sockaddr_in clientAddr{};
			socklen_t clientAddrLen = sizeof(clientAddr);
			logger.Info("Waiting for client connection...");

			int clientSocketFd = accept(mServerSocketFd, (struct sockaddr*)&clientAddr, &clientAddrLen);
			if (clientSocketFd == -1) {
				logger.Error("accept() failed");
				continue;
			}

			char clientIP[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
			logger.Info("Client connected from IP: " + std::string(clientIP));


			streaming(clientSocketFd);


			close(clientSocketFd); // 클라이언트 소켓 닫기
			logger.Info("Client disconnected");
		}
	}

	void Server::setupServer()
	{
		// server socket
		mServerSocketFd = socket(AF_INET, SOCK_STREAM, 0);
		if (mServerSocketFd == -1) {
			logger.Error("socket() fail");
		}
		logger.Info("socket() success");

		sockaddr_in serverAddr{};
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = INADDR_ANY;
		serverAddr.sin_port = htons(mPort);
		logger.Info("server sockaddr_in structure setting success");

		// bind
		if (bind(mServerSocketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) 
		{
			logger.Error("bind() fail");
			close(mServerSocketFd);
		}
		logger.Info("bind() success");

		// listen
		if (listen(mServerSocketFd, Server::BACK_LOG) == -1) 
		{
			logger.Error("listen() fail");
			close(mServerSocketFd);
		}
		logger.Info("listen() success");

		logger.Info("listening port: " + std::to_string(mPort));
	}

	void Server::streaming(int socketFd)
	{
		logger.Info("streaming start");

		const std::string filePath = std::string(PROJECT_ROOT) + "/storage/" + fixture::cctv1.ip + ".h264";
		logger.Info(filePath);

		FILE* file = fopen(filePath.c_str(), "rb");
		if (!file) 
		{
			logger.Error("Failed to open input file");
			return;
		}

		std::vector<uint8_t> headerBuffer(sizeof(frame::HeaderStruct));
		long lastFileSize = 0; // 마지막 읽은 위치 추적 변수

		while (true)
		{
			if (fseek(file, 0, SEEK_END) != 0) // 파일 포인터를 파일의 끝으로 이동
			{
				logger.Error("fseek() failed");
				return;
			} 
			long currentFileSize = ftell(file); // 파일 포인터의 위치
			if (currentFileSize == -1) 
			{
				logger.Error("ftell() failed");
				return;
			}

			if (currentFileSize > lastFileSize) 
			{
				long newBytes = currentFileSize - lastFileSize;

				if (fseek(file, lastFileSize, SEEK_SET) != 0) 
				{
					logger.Error("fseek() to lastFileSize failed");
					break;
				}

				// 새로운 데이터를 프레임 단위로 읽고 전송
				while (newBytes >= sizeof(frame::HeaderStruct)) 
				{
					// 헤더 읽기
					frame::Header header;
					if (fread(headerBuffer.data(), sizeof(uint8_t), sizeof(frame::HeaderStruct), file) != sizeof(frame::HeaderStruct))
					{
						logger.Error("Failed to read header");
						break;
					}
					header.Deserialize(headerBuffer);

					logger.Debug("header: frameId: " + std::to_string(header.GetFrameId()));
					logger.Debug("header: imageHeight: " + std::to_string(header.GetImageHeight()));
					logger.Debug("header: imageWidth: " + std::to_string(header.GetImageWidth()));
					logger.Debug("header: bodySize: " + std::to_string(header.GetBodySize()));
					logger.Debug("header: timestamp: " + header.GetTimestamp());

					uint32_t bodySize = header.GetBodySize();
					if (newBytes < sizeof(frame::HeaderStruct) + bodySize)
					{
						logger.Info("Incomplete frame detected. Waiting for more data.");
						break;
					}

					// Body 읽기
					std::vector<uint8_t> bodyBuffer(bodySize);
					if (fread(bodyBuffer.data(), sizeof(uint8_t), bodySize, file) != bodySize)
					{
						logger.Error("Failed to read body");
						break;
					}
					frame::Body body;

					// Body 암호화
					std::vector<uint8_t> encryptedBody;
					std::vector<uint8_t> nonce(12, 0x00);
					mCipherHandler->Encrypt(bodyBuffer, encryptedBody, nonce);

					body.SetBody(encryptedBody);


					// Frame 직렬화 및 전송
					frame::Frame frame(header, body);
					std::vector<uint8_t> frameBuffer;
					frame.Serialize(frameBuffer);

					if (send(socketFd, frameBuffer.data(), frameBuffer.size(), 0) <= 0)
					{
						logger.Error("send() failed or client disconnected");
						fclose(file);
						return; // 클라이언트 연결 종료
					}

					newBytes -= (sizeof(frame::HeaderStruct) + bodySize);
				}

				// 마지막 읽은 위치 갱신
				lastFileSize = currentFileSize - newBytes;
			} 
			else 
			{
				usleep(100000); // 0.1초 대기
			}
			
		}

		fclose(file);
		logger.Info("streaming end");
	}
}
