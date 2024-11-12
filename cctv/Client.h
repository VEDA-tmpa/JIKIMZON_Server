#ifndef CLIENT_H
#define CLIENT_H

#include <string>

namespace cctv
{
	typedef struct host {
		std::string ip;
		int port;
	} host_t;

	class Client
	{
	public:
		Client(std::string& host, int port);
		~Client();
		
		void Start();
		void Close();

	private:
		std::string& mHost;
		int mPort;
		int mSocketFd;
		bool mbClosed;

		void connectToServer();

		void receiveFrameAndSaveToFile();
	};
}
#endif /* CLIENT_H */