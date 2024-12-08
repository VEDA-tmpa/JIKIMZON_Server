#include <cassert>
#include <vector>
#include <cstring>
#include <iostream>

#include "storage/StorageManager.h"
#include "common/frame/Frame.h"

void test_1_save_load();
void test_n_save_load(int numFrames);

void test_1_save_load()
{
	storage::StorageManager storage("127.0.0.1");

	frame::HeaderStruct headerStruct{
		.frameId = 1001,
		.bodySize = 2002,
		.imageWidth = 1280,
		.imageHeight = 720,
		.imageFormat = frame::ImageFormat::H264
	};
	std::memcpy(headerStruct.timestamp, "20241122_123456.789", 19);
	std::memset(headerStruct.padding1, 0, 3);
	std::memset(headerStruct.padding2, 0, 1);

	frame::Header originHeader;
	originHeader.SetHeader(headerStruct);

	std::vector<uint8_t> originImage(headerStruct.bodySize);
	
	for (size_t i = 0; i < originImage.size(); ++i)
	{
		originImage[i] = static_cast<uint8_t>(i * 2);
	}
	
	frame::Body originBody;
	originBody.SetBody(originImage);

	frame::Frame originFrame(originHeader, originBody);

// ------------------------------------

	storage.SaveFrame(originFrame);

// ------------------------------------

	frame::Frame outFrame;
	storage.GetNextFrame(outFrame);

// ------------------------------------
	
	frame::Header outHeader = outFrame.GetHeader();

	std::cout << outHeader.GetFrameId() << std::endl;
	std::cout << outHeader.GetImageFormat() << std::endl;
	std::cout << outHeader.GetBodySize() << std::endl;
	std::cout << outHeader.GetTimestamp() << std::endl;
	std::cout << outHeader.GetImageHeight() << std::endl;
	std::cout << outHeader.GetImageWidth() << std::endl;
	assert(outHeader.GetFrameId() == originHeader.GetFrameId());
	assert(outHeader.GetImageFormat() == originHeader.GetImageFormat());
	assert(outHeader.GetBodySize() == originHeader.GetBodySize());
	assert(outHeader.GetTimestamp() == originHeader.GetTimestamp());
	assert(outHeader.GetImageHeight() == originHeader.GetImageHeight());
	assert(outHeader.GetImageWidth() == originHeader.GetImageWidth());
}

void test_n_save_load(int numFrames)
{
	storage::StorageManager storage("127.0.0.1");

	for (int i = 0; i < numFrames; ++i)
	{
		frame::HeaderStruct headerStruct{
			.frameId = static_cast<uint32_t>(1001 + i),  // 각 프레임마다 frameId를 다르게 설정
			.bodySize = 2002,
			.imageWidth = 1280,
			.imageHeight = 720,
			.imageFormat = frame::ImageFormat::H264
		};
		std::memcpy(headerStruct.timestamp, "20241122_123456.789", 19);
		std::memset(headerStruct.padding1, 0, 3);
		std::memset(headerStruct.padding2, 0, 1);

		frame::Header originHeader;
		originHeader.SetHeader(headerStruct);

		std::vector<uint8_t> originImage(headerStruct.bodySize);
		
		for (size_t i = 0; i < originImage.size(); ++i)
		{
			originImage[i] = static_cast<uint8_t>(i * 2);
		}
		
		frame::Body originBody;
		originBody.SetBody(originImage);

		frame::Frame originFrame(originHeader, originBody);

		storage.SaveFrame(originFrame);
	}

	// ------------------------------------

	// 검증
	for (int i = 0; i < numFrames; ++i)
	{
		frame::Frame outFrame;
		storage.GetNextFrame(outFrame);

		frame::Header outHeader = outFrame.GetHeader();

		assert(outHeader.GetFrameId() == (1001 + i));
		assert(outHeader.GetImageFormat() == frame::ImageFormat::H264);
		assert(outHeader.GetBodySize() == 2002);
		assert(outHeader.GetTimestamp() == "20241122_123456.789");
		assert(outHeader.GetImageHeight() == 720);
		assert(outHeader.GetImageWidth() == 1280);
	}
}

int main()
{
	test_1_save_load();
	// test_n_save_load(100);

	return 0;
}
