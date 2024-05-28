#include <iostream>
#include <fstream>
#include <cstring>
#include <bitset>
#include "ImageManager.h"
#include "Compression.h"
#include "Encryption.h"

// Function to create a new image file with a header and metadata
void createImageFile(const std::string& filename, uint32_t width, uint32_t height, uint16_t bitDepth, 
                     const std::string& patientName, const std::string& patientID, const std::string& imageType) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error creating file: " << filename << std::endl;
        return;
    }

    ImageHeader header;
    std::memset(&header, 0, sizeof(header)); // Limpiar la cabecera
    std::memcpy(header.formatID, "MEDI", 4); // Establecer el identificador de formato
    header.width = width;
    header.height = height;
    header.bitDepth = bitDepth;
    header.dataOffset = sizeof(ImageHeader);

    // Prepare metadata
    std::string metadata = patientName + "|" + patientID + "|" + imageType;
    std::cout << "Original metadata: " << metadata << std::endl;

    // Compress metadata using LZW
    std::vector<uint8_t> compressedData = compressLZW(metadata);
    std::cout << "Compressed data size: " << compressedData.size() << std::endl;

    // Check if compressed data fits in the header
    if (compressedData.size() > sizeof(header.compressedData)) {
        std::cerr << "Compressed data is too large to fit in the header" << std::endl;
        return;
    }

    // Encrypt compressed data using XOR
    xorEncryptDecrypt(compressedData, 0xAA);
    std::cout << "Encrypted compressed data: ";
    for (auto byte : compressedData) {
        std::cout << std::bitset<8>(byte) << " ";
    }
    std::cout << std::endl;

    // Copy compressed and encrypted data to header
    std::memset(header.compressedData, 0, sizeof(header.compressedData));
    std::memcpy(header.compressedData, compressedData.data(), compressedData.size());

    file.write(reinterpret_cast<const char*>(&header), sizeof(header));

    file.close();
    std::cout << "Image file created: " << filename << std::endl;
}

// Function to read the header and display the metadata of an existing image file
void readImageFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    ImageHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (std::strncmp(header.formatID, "MEDI", 4) != 0) {
        std::cerr << "Invalid file format" << std::endl;
        file.close();
        return;
    }

    // Determine the actual size of the compressed data
    size_t dataSize = sizeof(header.compressedData);
    while (dataSize > 0 && header.compressedData[dataSize - 1] == 0) {
        --dataSize;
    }

    // Decrypt compressed data using XOR
    std::vector<uint8_t> compressedData(header.compressedData, header.compressedData + dataSize);
    std::cout << "Encrypted compressed data read: ";
    for (auto byte : compressedData) {
        std::cout << std::bitset<8>(byte) << " ";
    }
    std::cout << std::endl;

    xorEncryptDecrypt(compressedData, 0xAA);
    std::cout << "Decrypted compressed data: ";
    for (auto byte : compressedData) {
        std::cout << std::bitset<8>(byte) << " ";
    }
    std::cout << std::endl;

    // Decompress metadata using LZW
    std::string metadata = decompressLZW(compressedData);
    std::cout << "Decompressed metadata: " << metadata << std::endl;

    // Extract patient information
    std::string patientName, patientID, imageType;
    size_t pos1 = metadata.find('|');
    size_t pos2 = metadata.find('|', pos1 + 1);
    patientName = metadata.substr(0, pos1);
    patientID = metadata.substr(pos1 + 1, pos2 - pos1 - 1);
    imageType = metadata.substr(pos2 + 1);

    std::cout << "File: " << filename << std::endl;
    std::cout << "Patient Name: " << patientName << std::endl;
    std::cout << "Patient ID: " << patientID << std::endl;
    std::cout << "Image Type: " << imageType << std::endl;
    std::cout << "Width: " << header.width << std::endl;
    std::cout << "Height: " << header.height << std::endl;
    std::cout << "Bit Depth: " << header.bitDepth << std::endl;

    file.close();
}

// Main function
int main() {
    int choice;
    std::string filename;
    std::string patientName;
    std::string patientID;
    std::string imageType;
    uint32_t width, height;
    uint16_t bitDepth;

    while (true) {
        std::cout << "Menu:" << std::endl;
        std::cout << "1. Create new image file" << std::endl;
        std::cout << "2. Read existing image file" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Enter new image filename: ";
                std::cin >> filename;
                std::cout << "Enter width: ";
                std::cin >> width;
                std::cout << "Enter height: ";
                std::cin >> height;
                std::cout << "Enter bit depth: ";
                std::cin >> bitDepth;
                std::cout << "Enter patient name: ";
                std::cin.ignore();
                std::getline(std::cin, patientName);
                std::cout << "Enter patient ID: ";
                std::cin >> patientID;
                std::cout << "Enter image type (e.g., RX): ";
                std::cin >> imageType;
                createImageFile(filename, width, height, bitDepth, patientName, patientID, imageType);
                break;
            case 2:
                std::cout << "Enter image filename: ";
                std::cin >> filename;
                readImageFile(filename);
                break;
            case 3:
                return 0;
            default:
                std::cerr << "Invalid choice. Please try again." << std::endl;
        }
    }
    return 0;
}
