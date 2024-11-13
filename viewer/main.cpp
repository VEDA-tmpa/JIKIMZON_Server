#include <cstdlib>
#include <iostream>
#include "server.h"

int main()
{
    int port;
    std::cout << "서버 포트를 입력하세요: ";
    std::cin >> port;  // 사용자로부터 포트 입력 받기

    viewer::Server server(port);

    server.start();
    std::cout << "서버가 포트 " << port << "에서 실행 중입니다." << std::endl;

    std::vector<char> sampleData = { 'H', 'e', 'l', 'l', 'o', ' ', 'G', 'U', 'I' };
    server.sendData(sampleData);  

    server.stop();
    std::cout << "서버가 중단되었습니다." << std::endl;

    return EXIT_SUCCESS;
}

