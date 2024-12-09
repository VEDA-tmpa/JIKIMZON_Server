#include "Frame.h"

#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <utility>
#include <cassert>

namespace frame
{
	Frame::Frame(Header serializedHeader, Body serializedBody)
        : mHeader(serializedHeader)
        , mBody(serializedBody)
    {
    }

    size_t Frame::GetSize() const
    {
        return sizeof(HeaderStruct) + mHeader.GetBodySize();
    }

    const Header& Frame::GetHeader() const
    {
        return mHeader;
    }

    const Body& Frame::GetBody() const
    {
        return mBody;
    }

    void Frame::Serialize(std::vector<uint8_t>& OUT buffer) const
    {
        std::vector<uint8_t> headerBuffer;
        std::vector<uint8_t> bodyBuffer;

        mHeader.Serialize(headerBuffer);
        mBody.Serialize(bodyBuffer);

        buffer.clear();
        buffer.insert(buffer.end(), headerBuffer.begin(), headerBuffer.end());
        buffer.insert(buffer.end(), bodyBuffer.begin(), bodyBuffer.end());
    }

	void Frame::Deserialize(std::vector<uint8_t>& buffer)
	{
		if (buffer.size() < sizeof(HeaderStruct))
		{
			std::cerr << "Buffer size is too small to deserialize HeaderStruct" << std::endl;
			return;
		}

		
		std::vector<uint8_t> headerBuffer(buffer.begin(), buffer.begin() + sizeof(HeaderStruct));
		mHeader.Deserialize(headerBuffer);
		
		
		size_t bodyStartIndex = sizeof(HeaderStruct);
		size_t bodySize = mHeader.GetBodySize();
	
		if (buffer.size() < bodyStartIndex + bodySize)
		{
			std::cerr << "Buffer size is too small to deserialize Body" << std::endl;
			return;
		}
		
		std::vector<uint8_t> bodyBuffer(buffer.begin() + bodyStartIndex, buffer.begin() + bodyStartIndex + bodySize);
		mBody.Deserialize(bodyBuffer);
	}

	Frame& Frame::operator=(Frame&& other)
    {
        if (this != &other)
        {
            mHeader = std::move(other.mHeader);
            mBody = std::move(other.mBody);
        }
        return *this;
    }
}
