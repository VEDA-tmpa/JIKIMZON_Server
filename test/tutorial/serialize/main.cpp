#include <iostream>
#include <string>
#include <cstring>
#include <cassert>

#include "common/frame/Frame.h"

void test_header_serialization_deserialization();
void test_body_serialization_deserialization();
void test_frame_composited_header_body();
void test_frame_serialization_deserialization();

int main()
{
	test_header_serialization_deserialization();
	test_body_serialization_deserialization();
	test_frame_composited_header_body();
	test_frame_serialization_deserialization();
	
	return 0;
}

void test_header_serialization_deserialization()
{
	frame::HeaderStruct headerStruct {
		.frameId = 1001,
		.bodySize = 2002,
		.imageWidth = 1280,
		.imageHeight = 720,
		.imageFormat = frame::ImageFormat::RAW
	};
	std::memcpy(headerStruct.timestamp, "20241122_123456.789", 19);
	std::memset(headerStruct.padding1, 0, 3);
	std::memset(headerStruct.padding2, 0, 1);

	// Header 객체
	frame::Header originHeader;
	originHeader.SetHeader(headerStruct);


	// Header 직렬화
	std::vector<uint8_t> outSerializedHeader;
	originHeader.Serialize(outSerializedHeader);

	// Header 역직렬화
	frame::Header header;
	header.Deserialize(outSerializedHeader);


	// assertion
	std::cout << "frameId: " << static_cast<int>(header.GetFrameId()) << std::endl;
	assert(header.GetFrameId() == headerStruct.frameId);

	std::cout << "bodySize: " << static_cast<int>(header.GetBodySize()) << std::endl;
	assert(header.GetBodySize() == headerStruct.bodySize);

	std::cout << "imageWidth: " << static_cast<int>(header.GetImageWidth()) << std::endl;
	assert(header.GetImageWidth() == headerStruct.imageWidth);

	std::cout << "imageHeight: " << static_cast<int>(header.GetImageHeight()) << std::endl;
	assert(header.GetImageHeight() == headerStruct.imageHeight);

	std::cout << "imageFormat: " << static_cast<int>(header.GetImageFormat()) << std::endl;
	assert(header.GetImageFormat() == headerStruct.imageFormat);

	std::cout << "timestamp: " << header.GetTimestamp() << std::endl;
	assert(std::string(header.GetTimestamp()) == std::string(headerStruct.timestamp));


	std::cout << "passed: test_header_serialization_deserialization" << std::endl;
}

void test_body_serialization_deserialization()
{
	frame::HeaderStruct headerStruct {
		.frameId = 1001,
		.bodySize = 2002,
		.imageWidth = 1280,
		.imageHeight = 720,
		.imageFormat = frame::ImageFormat::RAW
	};
	std::memcpy(headerStruct.timestamp, "20241122_123456.789", 19);
	std::memset(headerStruct.padding1, 0, 3);
	std::memset(headerStruct.padding2, 0, 1);



	// Body 객체
	std::vector<uint8_t> originImage(headerStruct.bodySize);
	for (size_t i = 0; i < originImage.size(); ++i) 
	{
		originImage[i] = static_cast<uint8_t>(i * 2); // 임의 값: 짝수로 채움
	}
	frame::Body originBody;
	originBody.SetBody(originImage);

	// Body 직렬화
	std::vector<uint8_t> outSerializedBody;
	originBody.Serialize(outSerializedBody);

	// Body 역직렬화
	frame::Body body;
	body.Deserialize(outSerializedBody);


	// assertion
	const std::vector<uint8_t>& image = body.GetImage();
	assert(headerStruct.bodySize == image.size());

	for (size_t i = 0; i < headerStruct.bodySize; ++i) 
	{
		assert(image[i] == originImage[i]); // 값 비교
	}

	std::cout << "passed: test_body_serialization_deserialization" << std::endl;
}

void test_frame_composited_header_body()
{
	// Header 객체
	frame::HeaderStruct headerStruct{
		.frameId = 1001,
		.bodySize = 2002,
		.imageWidth = 1280,
		.imageHeight = 720,
		.imageFormat = frame::ImageFormat::RAW
	};
	std::memcpy(headerStruct.timestamp, "20241122_123456.789", 19);
	std::memset(headerStruct.padding1, 0, 3);
	std::memset(headerStruct.padding2, 0, 1);

	frame::Header originHeader;
	originHeader.SetHeader(headerStruct);

	// Header 직렬화
	std::vector<uint8_t> outSerializedHeader;
	originHeader.Serialize(outSerializedHeader);

	// Header 역직렬화
	frame::Header header;
	header.Deserialize(outSerializedHeader);


	// Body 객체
	std::vector<uint8_t> originImage(headerStruct.bodySize);
	for (size_t i = 0; i < originImage.size(); ++i) 
	{
		originImage[i] = static_cast<uint8_t>(i * 2); // 임의 값: 짝수로 채움
	}
	frame::Body originBody;
	originBody.SetBody(originImage);

	// Body 직렬화
	std::vector<uint8_t> outSerializedBody;
	originBody.Serialize(outSerializedBody);

	// Body 역직렬화
	frame::Body body;
	body.Deserialize(outSerializedBody);


	// Frame 객체
	frame::Frame frame(header, body);


	// assertion
	{ // header
		frame::Header deserializedHeader = frame.GetHeader();
		assert(deserializedHeader.GetFrameId() == headerStruct.frameId);
		assert(deserializedHeader.GetBodySize() == headerStruct.bodySize);
		assert(deserializedHeader.GetImageWidth() == headerStruct.imageWidth);
		assert(deserializedHeader.GetImageHeight() == headerStruct.imageHeight);
		assert(deserializedHeader.GetImageFormat() == headerStruct.imageFormat);
		assert(std::string(deserializedHeader.GetTimestamp()) == std::string(headerStruct.timestamp));
	}
	{ // body
		const std::vector<uint8_t>& deserializedImage = frame.GetBody().GetImage();
		assert(deserializedImage.size() == headerStruct.bodySize);
		for (size_t i = 0; i < headerStruct.bodySize; ++i)
		{
			assert(deserializedImage[i] == originImage[i]);
		}
	}

	std::cout << "passed: test_frame_composited_header_body" << std::endl;
}


void test_frame_serialization_deserialization()
{
	// Header 객체
	frame::HeaderStruct headerStruct{
		.frameId = 1001,
		.bodySize = 2002,
		.imageWidth = 1280,
		.imageHeight = 720,
		.imageFormat = frame::ImageFormat::RAW
	};
	std::memcpy(headerStruct.timestamp, "20241122_123456.789", 19);
	std::memset(headerStruct.padding1, 0, 3);
	std::memset(headerStruct.padding2, 0, 1);

	frame::Header originHeader;
	originHeader.SetHeader(headerStruct);

	// Header 직렬화
	std::vector<uint8_t> outSerializedHeader;
	originHeader.Serialize(outSerializedHeader);

	// Header 역직렬화
	frame::Header header;
	header.Deserialize(outSerializedHeader);


	// Body 객체
	std::vector<uint8_t> originImage(headerStruct.bodySize);
	for (size_t i = 0; i < originImage.size(); ++i) 
	{
		originImage[i] = static_cast<uint8_t>(i * 2); // 임의 값: 짝수로 채움
	}
	frame::Body originBody;
	originBody.SetBody(originImage);

	// Body 직렬화
	std::vector<uint8_t> outSerializedBody;
	originBody.Serialize(outSerializedBody);

	// Body 역직렬화
	frame::Body body;
	body.Deserialize(outSerializedBody);


	// Frame 객체
	frame::Frame originFrame(header, body);

	// Frame 직렬화
	std::vector<uint8_t> outSerializedFrame;
	originFrame.Serialize(outSerializedFrame);

	// Frame 역직렬화
	frame::Frame frame;
	frame.Deserialize(outSerializedFrame);


	// assertion
	{ // header
		frame::Header deserializedHeader = frame.GetHeader();
		assert(deserializedHeader.GetFrameId() == headerStruct.frameId);
		assert(deserializedHeader.GetBodySize() == headerStruct.bodySize);
		assert(deserializedHeader.GetImageWidth() == headerStruct.imageWidth);
		assert(deserializedHeader.GetImageHeight() == headerStruct.imageHeight);
		assert(deserializedHeader.GetImageFormat() == headerStruct.imageFormat);
		assert(std::string(deserializedHeader.GetTimestamp()) == std::string(headerStruct.timestamp));
	}
	{ // body
		const std::vector<uint8_t>& deserializedImage = frame.GetBody().GetImage();
		assert(deserializedImage.size() == headerStruct.bodySize);
		for (size_t i = 0; i < headerStruct.bodySize; ++i)
		{
			assert(deserializedImage[i] == originImage[i]);
		}
	}

	std::cout << "passed: test_frame_serialization_deserialization" << std::endl;
}
