#include "Server.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <vector>

namespace viewer
{
	Server::Server(int port)
		: mPort(port), mServerFd(-1), mClientFd(-1), mbIsRunning(false)
	{
		mServerFd = socket(AF_INET, SOCK_STREAM, 0);
		if (mServerFd == 0)
		{
			std::cerr << "소켓 생성 실패" << std::endl;
			exit(EXIT_FAILURE);
		}

		mAddress.sin_family = AF_INET;
		mAddress.sin_addr.s_addr = INADDR_ANY;
		mAddress.sin_port = htons(mPort);  // 외부에서 받은 포트 번호로 설정

		if (bind(mServerFd, (struct sockaddr *)&mAddress, sizeof(mAddress)) < 0)
		{
			std::cerr << "바인딩 실패" << std::endl;
			exit(EXIT_FAILURE);
		}

		if (listen(mServerFd, 3) < 0)
		{
			std::cerr << "리스닝 실패" << std::endl;
			exit(EXIT_FAILURE);
		}

		std::cout << "서버가 포트 " << mPort << "에서 대기 중" << std::endl;
	}

	Server::~Server()
	{
		Stop();
	}

	void Server::Start()
	{
		mbIsRunning = true;
		acceptConnection();
	}

	void Server::Stop()
	{
		if (mClientFd > 0)
		{
			close(mClientFd);
		}
		if (mServerFd > 0)
		{
			close(mServerFd);
		}
		mbIsRunning = false;
		std::cout << "서버 중단됨" << std::endl;
	}

	void Server::acceptConnection()
	{
		socklen_t addrlen = sizeof(mAddress);

		mClientFd = accept(mServerFd, (struct sockaddr *)&mAddress, &addrlen);
		if (mClientFd < 0)
		{
			std::cerr << "클라이언트 연결 실패" << std::endl;
			return;
		}

		std::cout << "GUI 클라이언트 연결 수락" << std::endl;
	}

	void Server::SendData(const std::vector<char>& data)
	{
		if (mClientFd < 0)
		{
			std::cerr << "GUI 클라이언트 연결 안됨" << std::endl;
			return;
		}

		if (send(mClientFd, data.data(), data.size(), 0) < 0)
		{
			std::cerr << "데이터 전송 실패" << std::endl;
		}
	}
}

