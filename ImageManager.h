#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <string>
#include "ImageHeader.h"

void createImageFile(const std::string& filename, uint32_t width, uint32_t height, uint16_t bitDepth, 
                     const std::string& patientName, const std::string& patientID, const std::string& imageType);

void readImageFile(const std::string& filename);

#endif // IMAGEMANAGER_H
