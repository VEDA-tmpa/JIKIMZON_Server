#include "Frame.h"

#include <arpa/inet.h>
#include <iostream>
#include <cstring>

namespace frame
{
	void Frame::Serialize(std::vector<uint8_t>& OUT frameBuffer)
    {
        // setup buffer size
        frameBuffer.clear();
        frameBuffer.resize(sizeof(Header) + mHeader.bodySize);

        // serialize header
        {
            mHeader.frameId = htonl(mHeader.frameId);
            mHeader.bodySize = htonl(mHeader.bodySize);
            mHeader.imageWidth = htons(mHeader.imageWidth);
            mHeader.imageHeight = htons(mHeader.imageHeight);
    
            std::memcpy(frameBuffer.data(), reinterpret_cast<void*>(&mHeader), sizeof(Header));
        }

        // serialize body
        {
            std::memcpy(frameBuffer.data() + sizeof(Header), mBody.image.data(), mBody.image.size());
        }
    }

    void Frame::Deserialize(std::vector<uint8_t>& OUT frameBuffer)
    {
        if (frameBuffer.size() < sizeof(Header))
        {
            throw std::runtime_error("Buffer size is too small for a valid Frame.");
        }

        {
            std::memcpy(&mHeader, frameBuffer.data(), sizeof(Header));

            mHeader.frameId = ntohl(mHeader.frameId);
            mHeader.bodySize = ntohl(mHeader.bodySize);
            mHeader.imageWidth = ntohs(mHeader.imageWidth);
            mHeader.imageHeight = ntohs(mHeader.imageHeight);
        }

        {
            size_t bodySize = frameBuffer.size() - sizeof(Header);
            mBody.image.resize(bodySize);
            std::memcpy(mBody.image.data(), frameBuffer.data() + sizeof(Header), bodySize);
        }
    }

	const Header& Frame::GetHeader() const
	{
		return mHeader;
	}
	const Body& Frame::GetBody() const
	{
		return mBody;
	}

    void Frame::SetHeader(struct Header& header)
    {
        mHeader = header;
    }
}