#include <arpa/inet.h>
#include <cassert>

#include "Header.h"

namespace frame
{
	void Header::Serialize(std::vector<uint8_t>& OUT buffer) const
    {
        buffer.resize(sizeof(HeaderStruct));

        HeaderStruct tempHeader = mHeader;
        tempHeader.frameId = htonl(mHeader.frameId);
        tempHeader.bodySize = htonl(mHeader.bodySize);
        tempHeader.imageWidth = htons(mHeader.imageWidth);
        tempHeader.imageHeight = htons(mHeader.imageHeight);

        std::memcpy(buffer.data(), &tempHeader, sizeof(HeaderStruct));
    }

    void Header::Deserialize(std::vector<uint8_t>& buffer)
    {
        assert(buffer.size() == sizeof(HeaderStruct));

        std::memcpy(&mHeader, buffer.data(), sizeof(HeaderStruct));

        mHeader.frameId = ntohl(mHeader.frameId);
        mHeader.bodySize = ntohl(mHeader.bodySize);
        mHeader.imageWidth = ntohs(mHeader.imageWidth);
        mHeader.imageHeight = ntohs(mHeader.imageHeight);
    }

    uint32_t Header::GetFrameId() const
    {
        return mHeader.frameId;
    }

    uint32_t Header::GetBodySize() const
    {
        return mHeader.bodySize;
    }

	uint16_t Header::GetImageWidth() const
	{
		return mHeader.imageWidth;
	}

	uint16_t Header::GetImageHeight() const
	{
		return mHeader.imageHeight;
	}

	uint8_t Header::GetImageFormat() const
	{
		return mHeader.imageFormat;
	}

	std::string Header::GetTimestamp() const
	{
		return mHeader.timestamp;
	}

	void Header::SetHeader(HeaderStruct& header)
    {
        mHeader = header;
    }
}