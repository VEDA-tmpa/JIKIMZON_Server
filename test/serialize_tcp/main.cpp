#include <iostream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common/frame/Frame.h" // 직렬화 관련 헤더 포함

#define SERVER_PORT 12345 // 서버 포트
#define BUFFER_SIZE 4096  // 버퍼 크기

void handle_client(int client_sock, frame::Frame frame)
{
	std::cout << "Client connected" << std::endl;

	std::vector<uint8_t> outSerializedFrame;
	frame.Serialize(outSerializedFrame);
	std::cout << "Frame deserialized successfully" << std::endl;

	ssize_t bytesSent = send(client_sock, outSerializedFrame.data(), outSerializedFrame.size(), 0);
	if (bytesSent < 0)
	{
		perror("Send failed");
	}
	else
	{
		std::cout << "Sent " << bytesSent << " bytes to client" << std::endl;
	}

	close(client_sock);
	std::cout << "Client disconnected" << std::endl;
}

int main()
{
	// 1. 소켓 생성
	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	// 2. 서버 주소 설정 및 바인드
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY; // 모든 인터페이스에서 수신
	server_addr.sin_port = htons(SERVER_PORT);

	if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("Bind failed");
		close(server_sock);
		exit(EXIT_FAILURE);
	}

	// 3. 리슨
	if (listen(server_sock, 5) < 0)
	{
		perror("Listen failed");
		close(server_sock);
		exit(EXIT_FAILURE);
	}
	std::cout << "Server listening on port " << SERVER_PORT << std::endl;

	// 4. 클라이언트 연결 대기
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
	if (client_sock < 0)
	{
		perror("Accept failed");
	}


	// Header 객체
	frame::HeaderStruct headerStruct{
		.frameId = 1001,
		.bodySize = 2002,
		.imageWidth = 1280,
		.imageHeight = 720,
		.imageFormat = frame::ImageFormat::RAW
	};
	std::memcpy(headerStruct.timestamp, "20241122_123456.789", 19);
	std::memset(headerStruct.padding1, 0, 3);
	std::memset(headerStruct.padding2, 0, 1);

	frame::Header originHeader;
	originHeader.SetHeader(headerStruct);


	// Body 객체
	std::vector<uint8_t> originImage(headerStruct.bodySize);
	for (size_t i = 0; i < originImage.size(); ++i) 
	{
		originImage[i] = static_cast<uint8_t>(i * 2); // 임의 값: 짝수로 채움
	}
	frame::Body originBody;
	originBody.SetBody(originImage);


	// Frame 객체
	frame::Frame originFrame(originHeader, originBody);


	// 클라이언트 처리
	handle_client(client_sock, originFrame);


	// 5. 서버 소켓 닫기
	close(server_sock);
	return 0;
}
