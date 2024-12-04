#ifndef VIDEOHEADER_H
#define VIDEOHEADER_H

#include <cstdint>

struct VideoHeader
{
    uint64_t startSize;    // 시작 데이터 크기 (바이트)
    uint64_t currentSize;  // 현재 데이터 크기 (바이트)
    uint64_t endSize;      // 총 데이터 크기 (바이트)
};

#endif // VIDEOHEADER_H
