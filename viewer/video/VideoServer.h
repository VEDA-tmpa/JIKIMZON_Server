#ifndef VIDEO_SERVER_H
#define VIDEO_SERVER_H

#include "common/tcp/server/BaseServer.h"
#include "common/log/Logger.h"
#include "common/cipher/ICiphable.h"

#include "storage/manager/database/H264Item.h"
#include "storage/manager/StorageManager.h"

namespace viewer 
{
	class VideoServer : public tcp::BaseServer
	{
	public:
		enum { PORT = 12345 };
		
		VideoServer() = delete;
		VideoServer(int port, std::unique_ptr<cipher::ICiphable> cipherHandler);
		~VideoServer() = default;

	protected:
		void handleData(int socketFd) override;

	private:
		storage::StorageManager<storage::H264Item> mStorageManager;

		void streaming(int socketFd);
	};
}

#endif /* VIDEO_SERVER_H */