#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <netinet/in.h>
#include <vector>

namespace viewer
{
	class Server
	{
		public:
			Server(int port);          // 생성자에서 포트를 매개변수로 받음
			~Server();

			void Start();              // 서버 시작
			void Stop();               // 서버 중단
			void SendData(const std::vector<char>& data);  // GUI 쪽으로 데이터 전송

		private:
			int mPort;                 // 서버 포트 번호
			int mServerFd;             // 서버 소켓 fd
			int mClientFd;             // GUI 클라이언트 소켓 fd
			bool mbIsRunning;           // 서버 실행 여부
			struct sockaddr_in mAddress; // 서버 주소 정보 저장 구조체

			void acceptConnection();    // GUI 연결 수락
	};
}

#endif // SERVER_H

