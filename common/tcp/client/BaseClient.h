#ifndef BASE_CLIENT_H
#define BASE_CLIENT_H

#include <string>
#include <memory>

#include "common/log/Logger.h"
#include "common/cipher/ICiphable.h"
#include "common/storage/StorageUtil.h"

#include "common/cipher/TlsClientHandler.h"

namespace tcp
{
	class BaseClient 
	{
	public:
		BaseClient(const std::string& host, int port);
		virtual ~BaseClient();

		void Start();
		void Close();

	protected:
		virtual void handleData() = 0;
		
		int receiveData(void* buffer, size_t size);

		static logger::Logger logger;

		std::string mHost;
		int mPort;
		int mSocketFd;
		bool mbClosed;

	private:
		void connectToServer();

		std::unique_ptr<cipher::TlsClientHandler> mTlsHandler;
	};
}
#endif /* BASE_CLIENT_H */
