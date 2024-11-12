#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main()
{
    // 소켓 생성
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "소켓 생성 실패\n";
        return -1;
    }

    // 서버 주소 설정
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(54321);  // 서버 포트 번호
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        std::cerr << "유효하지 않은 주소\n";
        return -1;
    }

    // 서버에 연결
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "서버 연결 실패\n";
        return -1;
    }

    std::cout << "서버에 연결됨\n";

    // 서버로부터 데이터 수신
    char buffer[1024] = {0};
    int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        std::cout << "서버로부터 받은 데이터: " << buffer << "\n";
    } else {
        std::cerr << "데이터 수신 실패 또는 서버가 연결을 닫음\n";
    }

    // 소켓 닫기
    close(sock);
    std::cout << "클라이언트 종료\n";

    return 0;
}

