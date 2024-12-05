#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
#include <iomanip>

#include "common/cipher/ChaCha20.h"

#define SERVER_IP "192.168.10.99" // 서버 IP
// #define SERVER_IP "127.0.0.1" // 서버 IP
#define SERVER_PORT 12345     // 서버 포트 (서버와 동일하게 설정)

// 클라이언트 메인 함수
int main()
{
    // 1. 소켓 생성
    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "socket" << std::endl;


    // 2. 서버 주소 설정
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
    {
        perror("Invalid server address");
        close(client_sock);
        exit(EXIT_FAILURE);
    }
    std::cout << "setup" << std::endl;


    // 3. 서버에 연결
    if (connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        close(client_sock);
        exit(EXIT_FAILURE);
    }
    std::cout << "Connected to server" << std::endl;





// ------------------------------------------------------------



    // 4. 암호화된 데이터 수신
    std::vector<uint8_t> ciphertext(100); // 예상 크기: 서버에서 암호화한 데이터 크기
    ssize_t bytesReceived = recv(client_sock, ciphertext.data(), ciphertext.size(), 0);
    if (bytesReceived < 0)
    {
        perror("Receive failed");
        close(client_sock);
        exit(EXIT_FAILURE);
    }
    else if (bytesReceived == 0)
    {
        std::cerr << "Connection closed by server" << std::endl;
        close(client_sock);
        exit(EXIT_FAILURE);
    }
    std::cout << "Received " << bytesReceived << " bytes from server" << std::endl;
	
	// 받은 데이터 출력해보기
	std::cout << "Received ciphertext: ";
	for (uint8_t c : ciphertext) 
	{
		std::cout << std::hex << static_cast<int>(c) << " ";
	}
	std::cout << std::dec << std::endl;



// ------------------------------------------------------------



    // 5. 복호화 준비
    std::vector<uint8_t> key(32, 0x02);
	// key 값 출력
	std::cout << "Key (as bytes): ";
    for (uint8_t c : key) {
        std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(c) << " ";
    }
    std::cout << std::dec << std::endl;

	std::vector<uint8_t> nonce(12, 0x00);
	std::string timestamp = "20241126_123456.789";

	// timestamp의 마지막 12문자를 uint8_t로 변환 후 nonce에 복사
	for (size_t i = 0; i < 12; ++i) 
	{
		nonce[i] = static_cast<uint8_t>(timestamp[timestamp.size() - 12 + i]);
	}

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


	// std::vector<uint8_t> nonce = { 0x36, 0x5f, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x2e, 0x37, 0x38, 0x39 };
	// std::cout << "nonce size: " << nonce.size() << std::endl;

	// // nonce 값 출력
    // std::cout << "Nonce: ";
    // for (uint8_t c : nonce) {
    //     std::cout << static_cast<char>(c);
    // }
    // std::cout << std::endl;
	// std::cout << "Nonce (as bytes): ";
    // for (uint8_t c : nonce) {
    //     std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(c) << " ";
    // }
    // std::cout << std::dec << std::endl;


	cipher::ChaCha20 chacha20Handler(key);

    // 6. 복호화 수행
    std::vector<uint8_t> decrypted;
    ciphertext.resize(bytesReceived); // 받은 데이터 크기에 맞게 조정
    chacha20Handler.EncryptDecrypt(nonce, ciphertext, decrypted);

    // 7. 복호화된 데이터 출력
    std::cout << "Decrypted plaintext: ";
    for (uint8_t c : decrypted)
    {
        std::cout << static_cast<char>(c);
    }
    std::cout << std::endl;




	// [디버깅] 다시 암호화
	std::vector<uint8_t> debugCiphertext;
	chacha20Handler.EncryptDecrypt(nonce, decrypted, debugCiphertext);
	std::cout << "debugCiphertext: ";
	for (uint8_t c : debugCiphertext) 
	{
		std::cout << std::hex << static_cast<int>(c) << " ";
	}
	std::cout << std::dec << std::endl;


// ------------------------------------------------------------







    // 8. 소켓 닫기
    close(client_sock);
    std::cout << "Disconnected from server" << std::endl;

    return 0;
}
