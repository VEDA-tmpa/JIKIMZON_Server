#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <cstring>

// WinSock2 라이브러리 헤더 파일
#include <winsock2.h>
#include <ws2tcpip.h>

// Windows 라이브러리 헤더 파일 - Sleep 함수 사용
#include <windows.h>

// WinSock2.h 다음에 windows.h를 include해야 함

#include "body.hpp"
#include "header.hpp"
#include "frame.hpp"

#pragma comment(lib, "ws2_32.lib") // WinSock2 라이브러리 링크

#define PORT 12345

// g++ -o main.exe .\main.cpp .\body.cpp .\frame.cpp .\header.cpp -I 'C:\mingw64\x86_64-w64-mingw32\include\' -lws2_32
// include path는 mingw64 / clang / msvc 등 컴파일러에 따라 다르게

void SendData(SOCKET mClientSock, std::vector<uint8_t>& data)
{
    if (mClientSock < 0)
    {
        std::cerr << "not valid client sock" << std::endl;
        exit(EXIT_FAILURE);
    }

    // winsock의 send는 const char* 형식임
    if (send(mClientSock, reinterpret_cast<const char*>(data.data()), data.size(), 0) < 0)
    {
        std::cerr << "Send failed with error: " << WSAGetLastError() << std::endl;
    }
}

int main(void) {

    WSADATA wsaData;                        // WinSock 데이터 구조체
    SOCKET mSockfd = INVALID_SOCKET;        // SOCKET 타입 사용
    SOCKET mClientSock = INVALID_SOCKET;

    struct sockaddr_in mServerAddr;
    struct sockaddr_in mClientAddr;
    socklen_t mClientSockLen = sizeof(mClientAddr);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    mSockfd = socket(AF_INET, SOCK_STREAM, 0);
  	if (mSockfd == INVALID_SOCKET)
	{
		std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return EXIT_FAILURE;
	}

	mServerAddr.sin_family = AF_INET;
	mServerAddr.sin_addr.s_addr = INADDR_ANY;
	mServerAddr.sin_port = htons(PORT);

	if (bind(mSockfd, (struct sockaddr*)&mServerAddr, sizeof(mServerAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(mSockfd);
        WSACleanup();
        return EXIT_FAILURE;
    }

	// Listen for incoming connections
	if (listen(mSockfd, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(mSockfd);
        WSACleanup();
        return EXIT_FAILURE;
    }
	printf("Waiting for a client to connect...\n");

	// Accept a connection from client
	mClientSock = accept(mSockfd, (struct sockaddr *)&mClientAddr, &mClientSockLen);
	if (mClientSock == INVALID_SOCKET)
    {
        std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
        closesocket(mSockfd);
        WSACleanup();
        return EXIT_FAILURE;
    }
	printf("Client connected!\n");

    uint32_t frameId = 0;

    //
    while (true)
    {
        std::vector<uint8_t> buffer;
        std::vector<uint8_t> encryptedFrame(100, 5);
        std::string timestamp = "20241125_123456.789";  // YYYYMMDD_HHMMSS.sss

        // frame header 설정
        frame::HeaderStruct headerStruct{
            .frameId = static_cast<uint32_t>(frameId),
            .bodySize = static_cast<uint32_t>(encryptedFrame.size()),
            .imageWidth = static_cast<uint16_t>(1280),
            .imageHeight = static_cast<uint16_t>(720),
            .imageFormat = frame::ImageFormat::H264,
        };
        std::strcpy(headerStruct.timestamp, timestamp.c_str());

        frame::Header header(headerStruct);

        // frame body 설정
        frame::Body body(encryptedFrame);

        // Frame 객체 생성
        frame::Frame frame(header, body);

        // Frame 객체 Serialize 후 전송
        frame.Serialize(buffer);
        SendData(mClientSock, buffer);

        frameId += 1;
        Sleep(100);
    }

    return 0;
}