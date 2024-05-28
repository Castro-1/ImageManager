#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <vector>
#include <cstdint>

void xorEncryptDecrypt(std::vector<uint8_t>& data, char key);

#endif // ENCRYPTION_H
