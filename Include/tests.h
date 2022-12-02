#ifndef _TESTS_
#define _TESTS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Include/bmp.h"

// Dumps the contents of the image hex to a text file names image_hex.txt
void dump_img_to_txt(struct BMP_HEADER header, char *filename);

// Dumps the read hex data to a text file named produced_hex.txt
void dump_hex_to_txt(struct BMP_HEADER header, uint8_t *hex);

// Dumps the read Pixel data to a textfile named read_pixel_hex.txt
void dump_pixel_to_hex(struct BMP_HEADER header, struct PIXEL24 *pixels);

// For debugging purposes
// Prints all of the meta-data associated with the provided BMP file
void print_head(struct BMP_HEADER image_head);

#endif