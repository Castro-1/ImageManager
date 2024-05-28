#include "Encryption.h"

void xorEncryptDecrypt(std::vector<uint8_t>& data, char key) {
    for (auto& byte : data) {
        byte ^= key;
    }
}
