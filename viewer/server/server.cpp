#include "server.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>

namespace viewer
{
    Server::Server(int port)
        : mPort(port), mServerFd(-1), mClientFd(-1), mIsRunning(false)
    {
        mServerFd = socket(AF_INET, SOCK_STREAM, 0);
        if (mServerFd < 0) {
            std::cerr << "소켓 생성 실패" << std::endl;
            exit(EXIT_FAILURE);
        }

        // 서버 주소 설정
        mAddress.sin_family = AF_INET;
        mAddress.sin_addr.s_addr = INADDR_ANY;
        mAddress.sin_port = htons(mPort);

        // 바인드
        if (bind(mServerFd, (struct sockaddr *)&mAddress, sizeof(mAddress)) < 0) {
            std::cerr << "바인딩 실패" << std::endl;
            exit(EXIT_FAILURE);
        }

        // 수신 대기 상태 설정
        if (listen(mServerFd, 3) < 0) {
            std::cerr << "리스닝 실패" << std::endl;
            exit(EXIT_FAILURE);
        }
        
        std::cout << "서버가 포트 " << mPort << "에서 대기 중입니다." << std::endl;
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
        if (mClientFd > 0) close(mClientFd);
        if (mServerFd > 0) close(mServerFd);
        mIsRunning = false;
        std::cout << "서버 중단됨" << std::endl;
    }

    void Server::acceptConnection()
    {
        socklen_t addrlen = sizeof(mAddress);
        mClientFd = accept(mServerFd, (struct sockaddr *)&mAddress, &addrlen);
        if (mClientFd < 0) {
            std::cerr << "클라이언트 연결 실패" << std::endl;
            return;
        }
        std::cout << "GUI 클라이언트 연결 수락" << std::endl;
    }

    void Server::sendData(const std::vector<char>& data)
    {
        if (mClientFd < 0) {
            std::cerr << "GUI 클라이언트 연결 안됨" << std::endl;
            return;
        }
        if (send(mClientFd, data.data(), data.size(), 0) < 0) {
            std::cout << "데이터 전송 실패" << std::endl;
        } else {
            std::cout << "데이터 전송 성공" << std::endl;
        }
    }
}

