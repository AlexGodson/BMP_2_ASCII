#ifndef _BMP_
#define _BMP_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma pack(push, 1)

struct BMP_HEADER {               // Total: 54 bytes
    uint16_t  type;               // Magic identifier: 0x4d42
    uint32_t  size;               // File size in bytes
    uint16_t  reserved1;          // Not used
    uint16_t  reserved2;          // Not used
    uint32_t  offset;             // Offset to image data in bytes from beginning of file (54 bytes)
    uint32_t  dib_header_size;    // DIB Header size in bytes (40 bytes)
    int32_t   width_px;           // Width of the image
    int32_t   height_px;          // Height of image
    uint16_t  num_planes;         // Number of color planes
    uint16_t  bits_per_pixel;     // Bits per pixel
    uint32_t  compression;        // Compression type
    uint32_t  image_size_bytes;   // Image size in bytes
    int32_t   x_resolution_ppm;   // Pixels per meter
    int32_t   y_resolution_ppm;   // Pixels per meter
    uint32_t  num_colors;         // Number of colors
    uint32_t  important_colors;   // Important colors
};

struct BMP_INFO {
    uint32_t size;
    int32_t width_px;
    int32_t heigth_py;
};

struct PIXEL1 {     //1bpp
    uint8_t B; // COLOUR
};

struct PIXEL2 {     //2bpp
    uint8_t A;      //
    uint8_t B;      //
};

struct PIXEL4 {     // 4bpp
    uint8_t A;      //
    uint8_t B;      //
    uint8_t C;      //
    uint8_t D;      //
};

struct PIXEL8 {     // 8bpp
    uint8_t A;      //
    uint8_t B;      //
    uint8_t C;      //
    uint8_t D;      //
};

struct PIXEL16 {    // 16bpp
    uint8_t A;      //
    uint8_t B;      //
    uint8_t C;      //
    uint8_t D;      //
};

struct PIXEL24 {    // 24bpp
    uint8_t B;      // BLUE
    uint8_t G;      // GREEN
    uint8_t R;      // RED
};

struct PIXEL32 {
    uint8_t A;      //
    uint8_t B;      //
    uint8_t C;      //
    uint8_t D;      //
};

#pragma pack(pop)

// Get some head bitches (and some image data from BMP-HEX stored as single bytes)
uint8_t *get_BMP_data(char *filepath, struct BMP_HEADER *bmp_head);

// reading the data from the raw HEX into the BMP specific pixel format
/*      PIXEL24  --- 24bpp
BLUE    FF 00 00 --- 255  0    0
GREEN   00 FF 00 --- 0    255  0
RED     00 00 FF --- 0    0    255
*/
struct PIXEL24 *HEX_to_PIXEL24(uint8_t *hex_data, struct BMP_INFO bmp_info);



// Prints the image header for debugging
void print_head(struct BMP_HEADER head);

#endif
