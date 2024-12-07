#include "storage/StorageManager.h"
#include "common/frame/Frame.h"
#include "common/frame/Body.h"
#include "common/frame/Header.h"

#include <iostream>
#include <vector>
#include <cstring> // For strncpy

int main()
{
    // StorageManager 생성: 디스크 크기 1000바이트
    storage::StorageManager manager("192.168.1.1", "./storage", 1000); 

    try {
        // 테스트 1: 여러 프레임 생성 및 저장
        std::cout << "=== Test 1: Multiple Frames Save ===" << std::endl;
        for (int i = 1; i <= 10; ++i)
        {
            // 헤더 생성
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

            // 타임스탬프 설정                          YYYYMMDD_HHMMSS.sss    
            std::strncpy(headerStruct.timestamp, "20241207_134200.001", sizeof(headerStruct.timestamp) - 1);
            headerStruct.timestamp[sizeof(headerStruct.timestamp) - 1] = '\0';

            // Header 객체 생성 및 설정
            frame::Header header;
            header.SetHeader(headerStruct);

            // 바디 데이터 생성 (바디 크기와 알파벳 데이터로 채움)
            std::vector<uint8_t> bodyData(static_cast<uint32_t>(100 + i * 50), static_cast<uint8_t>('A' + i)); 
            frame::Body body;
            body.SetBody(bodyData);

            // 프레임 객체 생성
            frame::Frame frame(header, body);

            // 프레임 저장
            manager.SaveFrame(frame);

            // 저장 결과 출력
            std::cout << "Frame " << i << " saved. Body size: " << bodyData.size() << " bytes." << std::endl;
        }

        std::cout << "=== Test 1 Completed: Check storage folder for results ===" << std::endl;

        // 테스트 2: 디스크 크기 고정 여부 확인
        std::cout << "\n=== Test 2: Disk Size Check ===" << std::endl;
        std::ifstream inFile("./storage/video.bin", std::ios::binary);
        if (!inFile)
        {
            throw std::runtime_error("Failed to open video.bin for verification");
        }

        // 파일 크기 확인
        inFile.seekg(0, std::ios::end);
        size_t fileSize = inFile.tellg();
        inFile.close();

        std::cout << "File size after saving frames: " << fileSize << " bytes." << std::endl;
        if (fileSize == 1000)
        {
            std::cout << "PASS: File size matches the fixed disk size (1000 bytes)." << std::endl;
        }
        else
        {
            std::cout << "FAIL: File size does not match the fixed disk size." << std::endl;
        }

        // 테스트 완료 메시지
        std::cout << "\n=== All tests completed successfully ===" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
