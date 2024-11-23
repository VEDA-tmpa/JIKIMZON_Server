#ifndef FRAME_H
#define FRAME_H

#define OUT

#include <cstdint>
#include <vector>

#include "Header.h"
#include "Body.h"

namespace frame
{
    enum ImageFormat : uint8_t
    {
        RAW = 0,
        H264 = 1
    };

    class Frame
	{
	public:
		Frame() = default;
		Frame(Header header, Body body);
		~Frame() = default;

		void Serialize(std::vector<uint8_t>& OUT buffer);
		void Deserialize(std::vector<uint8_t>& buffer);

		size_t GetSize() const;
		const Header& GetHeader() const; 
        const Body& GetBody() const;

	private:
		Header mHeader;
		Body mBody;
	};
}

#endif // FRAME_H