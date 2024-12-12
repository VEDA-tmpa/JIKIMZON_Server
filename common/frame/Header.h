#ifndef HEADER_H
#define HEADER_H

#include <vector>

#include "common/frame/ISerializable.h"
#include "common/log/Logger.h"

namespace frame
{
	enum class GOP_START_FLAG : uint8_t { FALSE = 0, TRUE = 1 };

	/*
    ----------------------------------------------------------------
    |                      frameId (4 bytes)                       |
    ----------------------------------------------------------------
    |                           bodySize (4)                       |
    ----------------------------------------------------------------
    |          imageWidth (2)       |        imageHeight (2)       |
    ----------------------------------------------------------------
    | imageFormat (1)  |                  Padding (3)              |
    ----------------------------------------------------------------
    |                         timestamp (4)                        |
    ----------------------------------------------------------------
    |                         timestamp (4)                        |
    ----------------------------------------------------------------
    |                         timestamp (4)                        |
    ----------------------------------------------------------------
    |                         timestamp (4)                        |
    ----------------------------------------------------------------
    |                         timestamp (3)       |  padding (1)   |
    ----------------------------------------------------------------
	| GopStartFlag (1) | GopSize (1) |       padding (2)           |
    ----------------------------------------------------------------

    */
    struct HeaderStruct
	{
		uint32_t frameId;		// 4 bytes
		uint32_t bodySize;		// 4 bytes

		uint16_t imageWidth;	// 2 bytes
		uint16_t imageHeight;	// 2 bytes
		uint8_t imageFormat;	// 1 byte
		uint8_t padding1[3];    // 3 bytes (for alignment)

		char timestamp[19];   	// 19 bytes
		uint8_t padding2[1];  	// 1 byte (for alignment)

		GOP_START_FLAG gopStartFlag;   // 1 byte
		uint8_t gopSize;        // 1 byte
		uint8_t padding3[2];    // 2 bytes (for alignment)
	};

	
	class Header : public common::ISerializable
	{
	public:
		static logger::Logger logger;

		void Deserialize(std::vector<uint8_t>& buffer) override;	
		void Serialize(std::vector<uint8_t>& OUT buffer) const override;

		uint32_t GetFrameId() const;
		uint32_t GetBodySize() const;
		uint16_t GetImageWidth() const;
		uint16_t GetImageHeight() const;
		uint8_t GetImageFormat() const;
		std::string GetTimestamp() const;
		GOP_START_FLAG GetGopStartFlag() const;
		uint8_t GetGopSize() const;

		void SetHeader(HeaderStruct& header);

	private:
		HeaderStruct mHeader;
	};
}

#endif // HEADER_H