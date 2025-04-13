#ifndef CRC_H
#define CRC_H

#include <cstdint>
#include <cstddef>

uint16_t crc16_ccitt(const uint8_t* data, size_t len, uint16_t start = 0xFFFF);

#endif // CRC_H
