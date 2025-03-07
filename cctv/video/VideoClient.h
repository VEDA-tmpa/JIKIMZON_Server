#ifndef VIDEO_CLIENT_H
#define VIDEO_CLIENT_H

#define OUT

#include <cstdint>
#include <cstdio>
#include <vector>

#include "common/frame/GOP.h"

#include "common/tcp/client/BaseClient.h"
#include "common/frame/Frame.h"
#include "storage/manager/StorageManager.h"

namespace cctv
{
	class VideoClient : public tcp::BaseClient
	{
	public:
		VideoClient(const std::string& host, int port);
		~VideoClient() = default;

	protected:
		void handleData() override;

	private:
		storage::StorageManager<frame::H264::GOP> mStorageManager;
		
		frame::Frame receiveFrame();
	};
}
#endif /* VIDEO_CLIENT_H */
