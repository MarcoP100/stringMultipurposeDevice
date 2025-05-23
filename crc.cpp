#include "crc.h"

uint16_t crc16_ccitt(const uint8_t* data, size_t len, uint16_t start) {
    uint16_t crc = start;
    for (size_t i = 0; i < len; ++i) {
        crc ^= (uint16_t)data[i] << 8;
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    return crc;
}
