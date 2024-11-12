#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <netinet/in.h>		// sockaddr_in 구조체 사용
#include <vector>

namespace viewer
{
	/*
	GUI 담당자에게 데이터를 전달하기 위한 TCP 서버 클래스
	'Server' 클래스는 'cctv::Client'에서 수신한 데이터를
	TCP를 통해 GUI 담당자에게 전달하는 역할 수행
	*/

	class Server
	{
		public:
			Server(int port);
			~Server();

			void start();					// 서버 시작
			void stop();					// 서버 중단
			// GUI 쪽으로 데이터 전송
			void sendData(const std::vector<char>& Data);

		private:
			int mPort;						// 서버 포트 번호
			int mServerFd;					// 서버 소켓 fd
			int mClientFd;					// GUI 클라이언트 소켓 fd
			bool mIsRunning;				// 서버 실행 여부
			struct sockaddr_in mAddress;	// 서버 주소 정보 저장 구조체

			void acceptConnection();		// GUI연결 수락
	};
}

#endif	// SERVER_H
