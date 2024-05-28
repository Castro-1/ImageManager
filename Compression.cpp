#include "Compression.h"
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <bitset>

std::vector<uint8_t> compressLZW(const std::string& input) {
    std::unordered_map<std::string, uint16_t> dictionary;
    for (int i = 0; i < 256; i++) {
        dictionary[std::string(1, i)] = i;
    }

    std::string w;
    std::vector<uint8_t> result;
    uint16_t code = 256;
    for (char c : input) {
        std::string wc = w + c;
        if (dictionary.count(wc)) {
            w = wc;
        } else {
            result.push_back(dictionary[w] >> 8);
            result.push_back(dictionary[w] & 0xFF);
            if (code < 4096) {
                dictionary[wc] = code++;
            }
            w = std::string(1, c);
        }
    }

    if (!w.empty()) {
        result.push_back(dictionary[w] >> 8);
        result.push_back(dictionary[w] & 0xFF);
    }

    std::cout << "Compressed data: ";
    for (auto byte : result) {
        std::cout << std::bitset<8>(byte) << " ";
    }
    std::cout << std::endl;

    return result;
}

std::string decompressLZW(const std::vector<uint8_t>& input) {
    if (input.size() < 2) {
        throw std::runtime_error("Invalid input size for decompression");
    }

    std::vector<std::string> dictionary(256);
    for (int i = 0; i < 256; i++) {
        dictionary[i] = std::string(1, i);
    }

    uint16_t code = 256;
    std::string w(1, input[1]); // Adjust to read correctly from input
    std::string result = w;

    uint16_t k;
    for (size_t i = 2; i < input.size(); i += 2) {
        k = (input[i] << 8) | input[i + 1];
        std::string entry;
        if (k < dictionary.size()) {
            entry = dictionary[k];
        } else if (k == code) {
            entry = w + w[0];
        } else {
            throw std::runtime_error("Invalid compressed code");
        }

        result += entry;

        if (code < 4096) {
            dictionary.push_back(w + entry[0]);
            code++;
        }

        w = entry;
    }

    std::cout << "Decompressed data: " << result << std::endl;
    return result;
}
