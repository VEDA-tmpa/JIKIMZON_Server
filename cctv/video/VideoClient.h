#ifndef VIDEO_CLIENT_H
#define VIDEO_CLIENT_H

#define OUT

#include <cstdint>
#include <cstdio>
#include <vector>

#include "common/tcp/client/BaseClient.h"
#include "common/frame/Frame.h"
#include "storage/Storage.h"

namespace cctv
{
    class VideoClient : public tcp::BaseClient
    {
    public:
        VideoClient(const std::string& host, int port, 
					std::unique_ptr<cipher::ICiphable> cipherHandler,
					video::Storage storage);
        ~VideoClient();

    protected:
        void handleData() override;

    private:
		void saveFrame(frame::Frame frame);
		
		frame::Frame receiveFrame();

		void decryptBody(const std::vector<uint8_t>& data, const std::string& timestamp, std::vector<uint8_t>& OUT decrypted);

		video::Storage mStorage;
    };
}
#endif /* VIDEO_CLIENT_H */
