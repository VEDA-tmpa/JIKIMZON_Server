#include "server.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <vector>

using namespace std;

namespace viewer
{
	/*
	생성자
	서버 소켓 생성, 지정된 포트에서 GUI 쪽 client 연결 대기를 초기화
	*/
	Server::Server(int port)
		: mPort(port), mServerFd(-1), mClientFd(-1), mIsRunning(false)
	{
		mServerFd = socket(AF_INET, SOCK_STREAM, 0);
		if(mServerFd == 0)
		{
			cout << "소켓 생성 실패" << endl;
			exit(EXIT_FAILURE);
		}
		// 서버 주소 정보 set
		mAddress.sin_family = AF_INET;
		mAddress.sin_addr.s_addr = INADDR_ANY;	// 모든 인터페이스에서 연결 허용
		mAddress.sin_port = htons(mPort);		// 포트 번호 설정
	
		// 소켓 바인드
		if(bind(mServerFd, (struct sockaddr *)&mAddress, sizeof(mAddress)) < 0)
		{
			cout << "바인딩 실패" << endl;
			exit(EXIT_FAILURE);
		}

		// listen(최대 3개 허용)
		if(listen(mServerFd, 3) < 0)
		{
			cout << "리스닝 실패" << endl;
			exit(EXIT_FAILURE);
		}

		cout << "서버가 포트 " << mPort << "에서 대기 중" << endl;
	}

	Server::~Server()
	{
		stop();
	}

	void Server::start()
	{
		mIsRunning = true;
		acceptConnection();
	}

	void Server::stop()
	{
		if(mClientFd > 0)
		{
			close(mClientFd);
		}
		if(mServerFd > 0)
		{
			close(mServerFd);
		}
		mIsRunning = false;
		cout << "서버 중단됨" << endl;
	}

	// GUI 연결 대기 및 수락
	void Server::acceptConnection()
	{
		socklen_t addrlen = sizeof(mAddress);

		mClientFd = accept(mServerFd, (struct sockaddr *)&mAddress, &addrlen);
		if(mClientFd < 0)
		{
			cout << "클라이언트 연결 실패" << endl;
			return;
		}

		cout << "GUI 클라이언트 연결 수락" << endl;
	}

	void Server::sendData(const std::vector<char>& data)
	{
		if(mClientFd < 0)
		{
			cerr << "GUI 클라이언트 연결 안됨" << endl;
			return;
		}

		if(send(mClientFd, data.data(), data.size(), 0) < 0)
		{
			cout << "데이터 전송 실패" << endl;
		}
	}
}

