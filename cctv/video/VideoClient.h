#ifndef VIDEO_CLIENT_H
#define VIDEO_CLIENT_H

#define OUT

#include <cstdint>
#include <cstdio>
#include <vector>

#include "common/tcp/client/BaseClient.h"
#include "common/frame/Frame.h"

namespace cctv
{
	typedef void (*SaveHandler)(FILE* file, const char* data, size_t dataSize);

    class VideoClient : public tcp::BaseClient
    {
    public:
        VideoClient(const std::string& host, int port, std::unique_ptr<cipher::ICiphable> cipherHandler);
        ~VideoClient();

    protected:
        void handleData(const void* buffer, size_t size) override;

    private:
		void saveFrame(frame::Frame frame);
		void saveToFile(FILE* file, const char* data, size_t size); // TODO : storage 에서 제공
		
		frame::Frame receiveFrame();

		void decryptBody(const std::vector<uint8_t>& data, const std::string& timestamp, std::vector<uint8_t>& OUT decrypted);
    };
}
#endif /* VIDEO_CLIENT_H */
