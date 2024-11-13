#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>

int main()
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    std::vector<char> buffer(1024);

    // 소켓 생성
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "소켓 생성 실패" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(54321);

    // 서버 주소 설정
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "주소 설정 실패" << std::endl;
        return -1;
    }

    // 서버에 연결
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "서버에 연결 실패" << std::endl;
        return -1;
    }

    std::cout << "서버에 연결 성공" << std::endl;

    // 서버로부터 데이터 읽기
    valread = read(sock, buffer.data(), buffer.size());
    if (valread > 0)
    {
        std::cout << "서버로부터 받은 데이터: ";
        for (int i = 0; i < valread; ++i)
            std::cout << buffer[i];
        std::cout << std::endl;
    }
    else
    {
        std::cerr << "데이터 수신 실패" << std::endl;
    }

    // 소켓 종료
    close(sock);
    return 0;
}

