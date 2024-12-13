#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>

#include "common/log/Logger.h"
#include "common/cipher/ICiphable.h"

#include "common/cipher/TlsServerHandler.h"

namespace tcp 
{
	class BaseServer
	{
	public:		
		BaseServer() = delete;
		BaseServer(int port);
		~BaseServer() = default;

		void Start();

	protected:
		virtual void handleData(int socketFd) = 0;

		static logger::Logger logger;

		int mPort;
		int mServerSocketFd;

		std::unique_ptr<cipher::TlsServerHandler> mTlsHandler;

	private:
		enum { BACK_LOG = 3 };

		void setupServer();
	};
}

#endif /* SERVER_H */