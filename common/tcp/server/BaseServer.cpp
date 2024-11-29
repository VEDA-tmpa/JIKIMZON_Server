#include <algorithm>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

#include "common/tcp/server/BaseServer.h"
#include "common/frame/Frame.h"
#include "test/fixture/Fixture.h"
#include "common/cipher/ChaCha20.h"

namespace tcp
{
	logger::Logger BaseServer::logger("BaseServer");

	BaseServer::BaseServer(int port, std::unique_ptr<cipher::ICiphable> cipherHandler)
		: mPort(port)
		, mServerSocketFd(-1)
		, mCipherHandler(std::move(cipherHandler))
	{
	}

	void BaseServer::Start() 
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


			handleData(clientSocketFd);


			close(clientSocketFd); // 클라이언트 소켓 닫기
			logger.Info("Client disconnected");
		}
	}

	void BaseServer::setupServer()
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
		if (listen(mServerSocketFd, BaseServer::BACK_LOG) == -1) 
		{
			logger.Error("listen() fail");
			close(mServerSocketFd);
		}
		logger.Info("listen() success");

		logger.Info("listening port: " + std::to_string(mPort));
	}
}
