#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <iostream>

#include "common/log/Logger.h"

namespace cctv
{
	typedef struct host {
		std::string ip;
		int port;
	} host_t;

	typedef void (*SaveFrameHandler)(FILE*, const char*, size_t);

	class Client
	{
	public:
		Client(std::string& host, int port);
		~Client();
		
		void Start();
		void Close();

	private:
		static logger::Logger logger;

		std::string& mHost;
		int mPort;
		int mSocketFd;
		bool mbClosed;

		void connectToServer();

		int receiveData(void* buffer, size_t size);
		void receiveFrames(SaveFrameHandler SaveFrameHandler);
		static void saveFrameToFile(FILE* file, const char* frameData, size_t frameSize);
	};
}
#endif /* CLIENT_H */