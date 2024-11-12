#include <cstdlib>
#include <iostream>
#include "server.h"

int main()
{
    int port = 54321;  // GUI 클라이언트가 연결할 포트 번호
    viewer::Server server(port);

    // 서버 시작
    server.start();
    std::cout << "서버가 포트 " << port << "에서 실행 중입니다." << std::endl;

    // 간단한 대기 루프 (데이터 전송 테스트)
    std::cout << "서버가 클라이언트의 연결을 대기 중입니다..." << std::endl;

    // 테스트로 GUI 쪽으로 데이터 보내기
    std::vector<char> sampleData = { 'H', 'e', 'l', 'l', 'o', ' ', 'G', 'U', 'I' };
    server.sendData(sampleData);  // GUI 클라이언트가 연결되면 데이터 전송

    // 서버 중단
    server.stop();
    std::cout << "서버가 중단되었습니다." << std::endl;

    return EXIT_SUCCESS;
}

