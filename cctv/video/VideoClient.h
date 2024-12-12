#ifndef VIDEO_CLIENT_H
#define VIDEO_CLIENT_H

#define OUT

#include <cstdint>
#include <cstdio>
#include <vector>

#include "common/tcp/client/BaseClient.h"
#include "common/frame/Frame.h"
#include "storage/manager/StorageManager.h"
#include "storage/manager/database/H264Item.h"

namespace cctv
{
    class VideoClient : public tcp::BaseClient
    {
    public:
        VideoClient(const std::string& host, int port, std::unique_ptr<cipher::ICiphable> cipherHandler);
        ~VideoClient() = default;

    protected:
        void handleData() override;

    private:
		storage::StorageManager<storage::H264Item> mStorageManager;
		
		frame::Frame receiveFrame();

		void decryptBody(const std::vector<uint8_t>& data, const std::string& timestamp, std::vector<uint8_t>& OUT decrypted);
    };
}
#endif /* VIDEO_CLIENT_H */
