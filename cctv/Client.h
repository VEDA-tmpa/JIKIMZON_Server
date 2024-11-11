#ifndef CLIENT_H
#define CLIENT_H

#include <string>

namespace cctv
{
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