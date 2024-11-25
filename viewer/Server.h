#ifndef SERVER_H
#define SERVER_H

#include "common/log/Logger.h"
#include "common/cipher/ICiphable.h"

namespace viewer 
{
	class Server
	{
	public:
		enum { PORT = 12345 };
		
		Server() = delete;
		Server(int port, std::unique_ptr<cipher::ICiphable> cipherHandler);
		~Server() = default;

		void Start();

	private:
		enum { BACK_LOG = 3 };
		static logger::Logger logger;

		int mPort;
		int mServerSocketFd;

		std::unique_ptr<cipher::ICiphable> mCipherHandler;

		void setupServer();
		void streaming(int socketFd);
	};
}

#endif /* SERVER_H */