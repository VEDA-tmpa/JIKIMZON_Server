#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>

#include "common/cipher/ChaCha20.h"

#define SERVER_PORT 12345

int main(void)
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
	std::cout << "connect client " << std::endl;


// ------------------------------------------------------------

	std::vector<uint8_t> key(32, 0x02);
	// key 값 출력
	std::cout << "Key (as bytes): ";
    for (uint8_t c : key) {
        std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(c) << " ";
    }
    std::cout << std::dec << std::endl;

	std::vector<uint8_t> nonce(12, 0x00);
	std::string timestamp = "2024112691234569789";

	
	// timestamp의 마지막 12문자를 uint8_t로 변환 후 nonce에 복사
	std::transform(timestamp.end() - 12, timestamp.end(), nonce.begin(),
				[](char c) { return static_cast<uint8_t>(c); });

	// nonce 값 출력
    std::cout << "Nonce: ";
    for (uint8_t c : nonce) {
        std::cout << static_cast<char>(c);
    }
    std::cout << std::endl;
	std::cout << "Nonce (as bytes): ";
    for (uint8_t c : nonce) {
        std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(c) << " ";
    }
    std::cout << std::dec << std::endl;


	cipher::ChaCha20 chacha20Handler(key);


	std::vector<uint8_t> plaintext(100, 65);
	std::cout << "plaintext: ";
	for (uint8_t c : plaintext) 
	{
		std::cout << static_cast<char>(c);
	}
	std::cout << std::endl;



	// Encrypt the plaintext
	std::vector<uint8_t> ciphertext;
	chacha20Handler.EncryptDecrypt(nonce, plaintext, ciphertext);
	std::cout << "Ciphertext: ";
	for (uint8_t c : ciphertext) 
	{
		std::cout << std::hex << static_cast<int>(c) << " ";
	}
	std::cout << std::dec << std::endl;

	// Decrypt the ciphertext
	std::vector<uint8_t> decrypted;
	chacha20Handler.EncryptDecrypt(nonce, ciphertext, decrypted);
	std::cout << "Decrypted: ";
	for (uint8_t c : decrypted) 
	{
		std::cout << static_cast<char>(c);
	}
	std::cout << std::endl;



// ------------------------------------------------------------



	// 5. send to client
	ssize_t bytesSent = send(client_sock, ciphertext.data(), ciphertext.size(), 0);
	if (bytesSent < 0)
	{
		perror("Send failed");
	}
	else
	{
		std::cout << "Sent " << bytesSent << " bytes to client" << std::endl;
	}

	close(client_sock);
    std::cout << "close Client" << std::endl;



// ------------------------------------------------------------

	// 6. 서버 소켓 닫기
	close(server_sock);

	return 0;
}