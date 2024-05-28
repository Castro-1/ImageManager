#ifndef IMAGEHEADER_H
#define IMAGEHEADER_H

#include <cstdint>

struct ImageHeader {
    char formatID[4];      // Identificador del formato del archivo, por ejemplo, "MEDI"
    uint32_t width;        // Ancho de la imagen
    uint32_t height;       // Alto de la imagen
    uint16_t bitDepth;     // Profundidad de bits (por ejemplo, 8, 16, 24, 32)
    uint32_t dataOffset;   // Desplazamiento al inicio de los datos de la imagen
    char compressedData[64]; // Metadatos comprimidos y encriptados
};

#endif // IMAGEHEADER_H
