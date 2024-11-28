#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>

#include "common/log/Logger.h"
#include "common/cipher/ICiphable.h"

namespace tcp 
{
	class BaseServer
	{
	public:		
		BaseServer() = delete;
		BaseServer(int port, std::unique_ptr<cipher::ICiphable> cipherHandler);
		~BaseServer() = default;

		void Start();

	protected:
		virtual void handleData(int socketFd) = 0;

		static logger::Logger logger;

		int mPort;
		int mServerSocketFd;

		std::unique_ptr<cipher::ICiphable> mCipherHandler;

	private:
		enum { BACK_LOG = 3 };

		void setupServer();
	};
}

#endif /* SERVER_H */