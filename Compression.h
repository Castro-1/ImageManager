#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <string>
#include <vector>
#include <cstdint>

std::vector<uint8_t> compressLZW(const std::string& input);
std::string decompressLZW(const std::vector<uint8_t>& input);

#endif // COMPRESSION_H
