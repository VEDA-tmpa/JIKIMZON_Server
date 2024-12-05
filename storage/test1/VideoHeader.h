#ifndef VIDEOHEADER_H
#define VIDEOHEADER_H

#include <cstdint>

namespace video
{
   
    struct HeaderStruct
    {
        uint32_t startIndex;    // 4 bytes 버퍼의 시작 인덱스
        uint32_t endIndex;      // 4 bytes 버퍼의 끝 인덱스 
        uint32_t currentIndex;  // 4 bytes 현재 읽기 위치
    };
    
}

#endif // VIDEOHEADER_H
