#ifndef _BMP_
#define _BMP_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma pack(push, 1)

// struct that holds all of the meta-data for the .bmp file provided
// Total: 54 bytes
struct BMP_HEADER {
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


// TODO ALOT OF THESE PROPERLY
// TODO Implement properly some more bmp format functions
// Right now only 24BPP images work...... I think
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

/*      PIXEL24  --- 24bpp
Blue    FF 00 00 --- 255  0    0
Green   00 FF 00 --- 0    255  0
Red     00 00 FF --- 0    0    255
*/
struct PIXEL24 {
    uint8_t B;      // BLUE
    uint8_t G;      // GREEN
    uint8_t R;      // RED
};

/*      PIXEL32 --- 32bpp
NULL    11100000 00000000 00000000 00000000
Alpha   00011111 00000000 00000000 00000000
Blue    00000000 11111110 00000000 00000000
Green   00000000 00000001 11111110 00000000
Red     00000000 00000000 00000001 11111111
*/
struct PIXEL32 {
    uint8_t A;      //
    uint8_t B;      //
    uint8_t C;      //
    uint8_t D;      //
};

#pragma pack(pop)

// Get some head, bitches (and some image data from BMP-HEX stored as single bytes)
uint8_t *get_BMP_data(char *filepath, struct BMP_HEADER *bmp_head);


// reading the data from the raw HEX into the BMP specific pixel format
/*      PIXEL24  --- 24bpp
Blue -  FF 00 00 --- 255  0    0
Green - 00 FF 00 --- 0    255  0
Red -   00 00 FF --- 0    0    255
....    00 00 00 --- Buffer to bring the total bytes per row to a multiple of 4
*/
struct PIXEL24 *HEX_to_PIXEL24(uint8_t *hex_data, struct BMP_HEADER bmp_info);


/*      PIXEL32 --- 32bpp
NULL    11100000 00000000 00000000 00000000
Alpha   00011111 00000000 00000000 00000000
Blue    00000000 11111110 00000000 00000000
Green   00000000 00000001 11111110 00000000
Red     00000000 00000000 00000001 11111111
....    00 00 00 00--- then buffer to bring the total bytes per row to a multiple of 4
*/
struct PIXEL32 *HEX_to_PIXEL32(uint8_t *hex_data, struct BMP_HEADER bmp_info);


// averages the pixels to single values to represent their brightness directly based on brightness
// B G R -> 87 201 18 -> 102
uint8_t *normalise_pixels_linear24(struct BMP_HEADER head, struct PIXEL24 *pixels);


// converts the pixel RGB values to a brightness value
// higher R/G/B values are weighted to increase the brightness
uint8_t *normalise_pixels_weighted24(struct BMP_HEADER head, struct PIXEL24 *pixels);


// averages the pixels to single values to represent their brightness directly based on brightness
// Ignores Alpha values
// A B G R -> 87 201 18 -> 102
uint8_t *normalise_pixels_linear32(struct BMP_HEADER head, struct PIXEL32 *pixels);


// converts the pixel RGB values to a brightness value
// Ignoring alpha values
// higher R/G/B values are weighted to increase the brightness
uint8_t *normalise_pixels_weighted32(struct BMP_HEADER head, struct PIXEL32 *pixels);


// Compresses the image to the correct width and height to print to ascii
uint8_t *compress_image(struct BMP_HEADER head, uint8_t *pixel_norm, int asc_w, int asc_h);


// converst normalised pixel values to an array of ascii characters provided
uint8_t *asciify(uint8_t *values, int asc_w, int asc_h, uint8_t *ascii_table, int table_sz);


// Prints the ascii text array to the terminal
void print_ascii(uint8_t *ascii_array, int asc_w, int asc_h);


// Saves the ascii image to ascii.txt file
void save_ascii(uint8_t *ascii_array, int asc_w, int asc_h);

#endif
