#include "server_v1.h"
#include "Frame.h"   // Frame 상수 사용
#include <iostream>
#include <fstream>    // 파일 입출력
#include <unistd.h>
#include <sys/socket.h>
#include <vector>

namespace viewer
{
    Server::Server(int port)
        : mPort(port), mServerFd(-1), mClientFd(-1), mIsRunning(false)
    {
        mServerFd = socket(AF_INET, SOCK_STREAM, 0);
        if(mServerFd == 0)
        {
            std::cerr << "소켓 생성 실패" << std::endl;
            exit(EXIT_FAILURE);
        }
        mAddress.sin_family = AF_INET;
        mAddress.sin_addr.s_addr = INADDR_ANY;
        mAddress.sin_port = htons(mPort);
        
        if(bind(mServerFd, (struct sockaddr *)&mAddress, sizeof(mAddress)) < 0)
        {
            std::cerr << "바인딩 실패" << std::endl;
            exit(EXIT_FAILURE);
        }

        if(listen(mServerFd, 3) < 0)
        {
            std::cerr << "리스닝 실패" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::cout << "서버가 포트 " << mPort << "에서 대기 중" << std::endl;
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
        std::cout << "서버 중단됨" << std::endl;
    }

    void Server::acceptConnection()
    {
        socklen_t addrlen = sizeof(mAddress);
        mClientFd = accept(mServerFd, (struct sockaddr *)&mAddress, &addrlen);
        if(mClientFd < 0)
        {
            std::cerr << "클라이언트 연결 실패" << std::endl;
            return;
        }
        std::cout << "GUI 클라이언트 연결 수락" << std::endl;
    }

    std::vector<char> Server::readFrameFromFile()
    {
        std::vector<char> frameData(cctv::Frame::FRAME_SIZE); // Frame 크기를 기준으로 버퍼 생성
        std::ifstream file("storage/frame_data.bin", std::ios::binary);
        
        if (file.is_open())
        {
            file.read(frameData.data(), cctv::Frame::FRAME_SIZE);
            if (!file)
            {
                std::cerr << "파일에서 프레임 데이터를 읽는 중 오류 발생" << std::endl;
            }
            file.close();
        }
        else
        {
            std::cerr << "파일을 열 수 없음" << std::endl;
        }

        return frameData;
    }

    void Server::sendData()
    {
        if(mClientFd < 0)
        {
            std::cerr << "GUI 클라이언트 연결 안됨" << std::endl;
            return;
        }

        std::vector<char> frameData = readFrameFromFile();
        
        if(send(mClientFd, frameData.data(), frameData.size(), 0) < 0)
        {
            std::cerr << "데이터 전송 실패" << std::endl;
        }
    }
}

