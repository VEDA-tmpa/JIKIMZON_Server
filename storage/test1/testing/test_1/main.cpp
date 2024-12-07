#include "storage/StorageManager.h"
#include "common/frame/Frame.h"
#include "common/frame/Body.h"
#include "common/frame/Header.h"

#include <iostream>
#include <vector>
#include <cstring> // For strncpy

int main()
{
    storage::StorageManager manager("192.168.1.1", "./storage", 1000); // 디스크 크기: 1000바이트

    try {
        // 여러 프레임 생성 및 저장
        for (int i = 1; i <= 10; ++i)
        {
            frame::HeaderStruct headerStruct = {
                static_cast<uint32_t>(i),            // frameId
                static_cast<uint32_t>(100 + i * 50), // bodySize
                640,                                 // imageWidth
                480,                                 // imageHeight
                1,                                   // imageFormat (H264)
                {0},                                 // padding
                "",                                  // timestamp
                {0}                                  // padding
            };

            // 타임스탬프 복사
            std::strncpy(headerStruct.timestamp, "2023-12-07 12:00:00", sizeof(headerStruct.timestamp) - 1);
            headerStruct.timestamp[sizeof(headerStruct.timestamp) - 1] = '\0';


            frame::Header header;
            header.SetHeader(headerStruct);

            std::vector<uint8_t> bodyData(static_cast<uint32_t>(100 + i * 50), static_cast<uint8_t>('A' + i)); // 바디 데이터
            frame::Body body;
            body.SetBody(bodyData);

            frame::Frame frame(header, body);

            // 프레임 저장
            manager.SaveFrame(frame);

            std::cout << "Frame " << i << " saved. Body size: " << bodyData.size() << " bytes." << std::endl;
        }

        std::cout << "All frames saved successfully. Check storage folder." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
