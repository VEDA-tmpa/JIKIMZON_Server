#ifndef FRAME_H
#define FRAME_H

#define OUT

#include <cstdint>
#include <vector>

namespace frame
{
    /*
    -------------------------------------
    |         frameId (4 bytes)         |
    -------------------------------------
    |         bodySize (4)              |
    -------------------------------------
    | imageWidth (2)  | imageHeight (2) |
    -------------------------------------
    | imageFormat (1) | Padding (3)     |
    -------------------------------------
    |  timestamp (4)                    |
    -------------------------------------
    |  timestamp (4)                    |
    -------------------------------------
    |  timestamp (4)                    |
    -------------------------------------
    |  timestamp (4)                    |
    -------------------------------------
    | timestamp (3)   | Padding (1)     |
    -------------------------------------
    */
    struct Header
    {
        uint32_t frameId;   // 4 bytes
        uint32_t bodySize;  // 4 bytes

        uint16_t imageWidth;   // 2 bytes
        uint16_t imageHeight;  // 2 bytes
        uint8_t imageFormat;   // 1 byte
        uint8_t padding1[3];   // 3 bytes (for alignment)

        char timestamp[19];   // 19 bytes
        uint8_t padding2[1];  // 1 byte (for alignment)
    };

    enum ImageFormat : uint8_t
    {
        RAW = 0,
        H264 = 1
    };

    struct Body
	{
		std::vector<uint8_t> image;
	};

    class Frame
	{
	public:
        void Serialize(std::vector<uint8_t>& OUT buffer);
		void Deserialize(std::vector<uint8_t>& OUT buffer);

		const Header& GetHeader() const; 
        const Body& GetBody() const;

        void SetHeader(struct Header& header);

	private:
		Header mHeader;
		Body mBody;
	};
}

#endif // FRAME_H