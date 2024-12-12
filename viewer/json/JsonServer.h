#ifndef VIDEO_SERVER_H
#define VIDEO_SERVER_H

#include <nlohmann/json.hpp>

#include "common/tcp/server/BaseServer.h"
#include "common/log/Logger.h"
#include "common/cipher/ICiphable.h"

#include "storage/manager/StorageManager.h"

namespace viewer 
{
	class JsonServer : public tcp::BaseServer
	{
	public:
		enum { PORT = 54321 };
		
		JsonServer() = delete;
		JsonServer(int port, std::unique_ptr<cipher::ICiphable> cipherHandler);
		~JsonServer() = default;

	protected:
		void handleData(int socketFd) override;

		
	private:
		storage::StorageManager<nlohmann::json> mStorageManager;

		void streaming(int socketFd);
	};
}

#endif /* VIDEO_SERVER_H */