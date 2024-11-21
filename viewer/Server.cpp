#include <algorithm>
#include <arpa/inet.h>
#include <unistd.h>

#include "viewer/Server.h"
#include "common/frame/Frame.h"
#include "test/fixture/Fixture.h"
#include "common/cipher/ChaCha20.h"

namespace viewer
{
	logger::Logger Server::logger("Server");

	Server::Server(int port)
		: mPort(port)
		, mServerSocketFd(-1)
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

		std::vector<uint8_t> key = cipher::ChaCha20::LoadKeyFromFile( std::string(PROJECT_ROOT) + "/viewer/keyfile.bin");
        std::vector<uint8_t> nonce(12, 0x00); // Replace with secure random nonce
		cipher::ChaCha20 chacha20Handler(key);


		const std::string filePath = std::string(PROJECT_ROOT) + "/storage/" + fixture::cctv1.ip;
		logger.Info(filePath);

		FILE* file = fopen(filePath.c_str(), "rb");
		if (!file) 
		{
			logger.Error("Failed to open input file");
			return;
		}


		char frameBuffer[cctv::Frame::FRAME_SIZE];
		char encrpytFrameBuffer[cctv::Frame::FRAME_SIZE];
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
				while (newBytes >= cctv::Frame::FRAME_SIZE) 
				{
					size_t bytesRead = fread(frameBuffer, sizeof(char), cctv::Frame::FRAME_SIZE, file);
					if (bytesRead == cctv::Frame::FRAME_SIZE) 
					{

						// 1. frameBuffer를 std::vector<uint8_t>로 변환
						std::vector<uint8_t> inputBuffer(frameBuffer, frameBuffer + cctv::Frame::FRAME_SIZE);
						std::vector<uint8_t> outputBuffer(cctv::Frame::FRAME_SIZE);

						// 2. EncryptDecrypt 호출
						chacha20Handler.EncryptDecrypt(nonce, inputBuffer, outputBuffer);

						// 3. 암호화된 데이터를 frameBuffer로 복사
						std::memcpy(encrpytFrameBuffer, outputBuffer.data(), cctv::Frame::FRAME_SIZE);

						int bytesSent = send(socketFd, encrpytFrameBuffer, cctv::Frame::FRAME_SIZE, 0);
						if (bytesSent <= 0) 
						{
							logger.Error("send() failed or client disconnected");
							fclose(file);
							return; // 클라이언트 연결 종료
						}
					} 
					else 
					{
						logger.Error("fread() failed or unexpected EOF");
						break;
					}

					newBytes -= cctv::Frame::FRAME_SIZE; // 읽은 만큼 감소
				}

				// 마지막 읽은 위치 갱신
				lastFileSize = currentFileSize - (newBytes % cctv::Frame::FRAME_SIZE);
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
