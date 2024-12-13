#ifndef VIDEO_SERVER_H
#define VIDEO_SERVER_H

#include "common/frame/GOP.h"

#include "common/tcp/server/BaseServer.h"
#include "common/log/Logger.h"
#include "common/cipher/ICiphable.h"

#include "storage/manager/StorageManager.h"

namespace viewer 
{
	class VideoServer : public tcp::BaseServer
	{
	public:
		enum { PORT = 12345 };
		
		VideoServer() = delete;
		VideoServer(int port);
		~VideoServer() = default;

	protected:
		void handleData(int socketFd) override;

	private:
		storage::StorageManager<frame::H264::GOP> mStorageManager;

		void streaming(int socketFd);
	};
}

#endif /* VIDEO_SERVER_H */