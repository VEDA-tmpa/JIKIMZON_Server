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
		BaseServer(int port, std::unique_ptr<cipher::ICiphable> cipherHandler);
		~BaseServer() = default;

		void Start();

	protected:
		virtual void handleData(int socketFd) = 0;

		static logger::Logger logger;

		int mPort;
		int mServerSocketFd;

		std::unique_ptr<cipher::ICiphable> mCipherHandler;
		std::unique_ptr<cipher::TlsServerHandler> mTlsHandler; // TODO: 영향범위 부모클래스에서 처리하도록 수정해야함. 지금은 동작하도록 처리

	private:
		enum { BACK_LOG = 3 };

		void setupServer();
	};
}

#endif /* SERVER_H */