#ifndef _BMP_
#define _BMP_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_DIGITS 8
#define SETTINGS_NUM 3

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

#pragma pack(pop)

// More useful struct for passing bmp-data throughout the project for various functionality
struct BMP_INFO {
    uint32_t  size; 
    uint32_t  offset; 
    int32_t   width_px;
    int32_t   height_px;
    uint16_t  bits_per_pixel;
    uint32_t  image_size_bytes;
    struct PIXEL *(*transform_hex)(struct BMP_INFO bmp_info, uint8_t *hex_data);
    uint8_t *(*normalise_pixels)(struct BMP_INFO head, struct PIXEL *pixels);
    int cut_off, mark_down;
    int ascii_scaling_factor;
    int asc_w, asc_h
;
};

// Generic Pixel type used by all the different formats
// BMP files have a max 4 bytes per pixel.
struct PIXEL {
    uint8_t A;
    uint8_t B;
    uint8_t C;
    uint8_t D;
};


// Get some head, bitches (and some image data from BMP-HEX data)
uint8_t *get_BMP_data(struct BMP_HEADER *bmp_head, char *filepath);


// For debugging purposes
// Prints all of the meta-data associated with the provided BMP file
void print_head(struct BMP_HEADER image_head);


// turns the header from the BMP image data to a more useful one to be passed to functions
struct BMP_INFO transform_header(struct BMP_HEADER header);


// Retrieves the settings stated in the Settings.txt file and puts them in the BMP_INFO struct
void get_settings(struct BMP_INFO *bmp_info);


// reading the data from the raw HEX into the BMP specific pixel format
// functions below called by a function pointer stored in BMP_INFO
/*      1bpp
    // TODO
*/
struct PIXEL *HEX_to_PIXEL1(struct BMP_INFO bmp_info, uint8_t *hex_data);


/*      2bpp
    // TODO
*/
struct PIXEL *HEX_to_PIXEL2(struct BMP_INFO bmp_info, uint8_t *hex_data);


/*      4bpp
    // TODO
*/
struct PIXEL *HEX_to_PIXEL4(struct BMP_INFO bmp_info, uint8_t *hex_data);


/*      8bpp
    // TODO
*/
struct PIXEL *HEX_to_PIXEL8(struct BMP_INFO bmp_info, uint8_t *hex_data);


/*      16bpp
    // TODO
*/
struct PIXEL *HEX_to_PIXEL16(struct BMP_INFO bmp_info, uint8_t *hex_data);

// reading the data from the raw HEX into the BMP specific pixel format
/*      24bpp
Blue -  FF 00 00 --- 255  0    0
Green - 00 FF 00 --- 0    255  0
Red -   00 00 FF --- 0    0    255
....    00 00 00 --- Buffer to bring the total bytes per row to a multiple of 4
*/
struct PIXEL *HEX_to_PIXEL24(struct BMP_INFO bmp_info, uint8_t *hex_data);


/*      32bpp
Blue -  FF 00 00 00 --- 255  0    0    0
Green - 00 FF 00 00 --- 0    255  0    0
Red -   00 00 FF 00 --- 0    0    255  0
Alpha - 00 00 00 FF --- 0    0    0    255 
....    00 00 00 00--- then buffer to bring the total bytes per row to a multiple of 4
*/
struct PIXEL *HEX_to_PIXEL32(struct BMP_INFO bmp_info, uint8_t *hex_data);


// TO COMMENT
/*

*/
uint8_t *normalise_pixels1(struct BMP_INFO head, struct PIXEL *pixels);


/*

*/
uint8_t *normalise_pixels2(struct BMP_INFO head, struct PIXEL *pixels);


/*

*/
uint8_t *normalise_pixels4(struct BMP_INFO head, struct PIXEL *pixels);


/*

*/
uint8_t *normalise_pixels8(struct BMP_INFO head, struct PIXEL *pixels);


/*

*/
uint8_t *normalise_pixels16(struct BMP_INFO head, struct PIXEL *pixels);


/*      24bpp
Blue -  FF 00 00 --- 255  0    0
Green - 00 FF 00 --- 0    255  0
Red -   00 00 FF --- 0    0    255
....    00 00 00 --- Buffer to bring the total bytes per row to a multiple of 4
*/
uint8_t *normalise_pixels24(struct BMP_INFO head, struct PIXEL *pixels);


/*      32bpp
Blue -  FF 00 00 00 --- 255  0    0    0
Green - 00 FF 00 00 --- 0    255  0    0
Red -   00 00 FF 00 --- 0    0    255  0
Alpha - 00 00 00 FF --- 0    0    0    255 
....    00 00 00 00--- then buffer to bring the total bytes per row to a multiple of 4
*/
uint8_t *normalise_pixels32(struct BMP_INFO head, struct PIXEL *pixels);


// Compresses the image to the correct width and height to print to ascii
uint8_t *compress_image(struct BMP_INFO bmp_info, uint8_t *pixel_norm, int asc_w, int asc_h);


// converst normalised pixel values to an array of ascii characters provided
uint8_t *asciify(struct BMP_INFO bmp_info, uint8_t *values, uint8_t *ascii_table, int table_sz);


// Prints the ascii text array to the terminal
void print_ascii(struct BMP_INFO bmp_info, uint8_t *ascii_array);


// Saves the ascii image to ascii.txt file
void save_ascii(struct BMP_INFO bmp_info, uint8_t *ascii_array);

#endif
