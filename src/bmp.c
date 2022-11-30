#include <stdio.h>
#include <stdlib.h>

#include "../Include/bmp.h"

// Get some head bitches (and some image data from BMP-HEX stored as single bytes)
uint8_t *get_BMP_data(char *filepath, struct BMP_HEADER *bmp_head) {
    FILE *fptr = fopen(filepath, "rb");

    if (fptr == NULL) {
        printf("Error opening file: %s\n", filepath);
        return NULL;
    }

    fread(bmp_head, sizeof(*bmp_head), 1, fptr);

    if (bmp_head == NULL) {
        printf("ERROR: File header information could not be loaded\n");
        return NULL;
    }
    if (bmp_head->type != 0x4D42) {
        printf("ERROR: Invalid file, not a BMP\n");
        return NULL;
    }

    // Allocating memory to store the Hex data of the Image as single bytes
    uint8_t *HEX_DATA = (uint8_t*)malloc(bmp_head->image_size_bytes);

    if (HEX_DATA == NULL) {
        printf("ERROR: Memory could not be allocated for image hex data\n");
        return NULL;
    }

    // Moving the file data pointer to the start of the image Hex data
    // Image data starts at bmp_head->offset
    int found_seek = fseek(fptr, bmp_head->offset, SEEK_SET);

    if (found_seek == -1) {
        printf("ERROR: Could not find bmp image offset in file\n");
        return NULL;
    }

    // Reading the full BMP image HEX data into memory and closing the image file
    fread(HEX_DATA, bmp_head->image_size_bytes, 1, fptr);
    fclose(fptr);

    // If the size of the read data is NULL or not equal to what it stated in the header
    // the image was not read correctly and an error is thrown
    if (HEX_DATA == NULL) {
        printf("ERROR: Could not load full image file\n");
        return NULL;
    }

    return HEX_DATA;
}


// reading the data from the raw HEX into the BMP specific pixel format
/*      PIXEL24  --- 24bpp
BLUE    FF 00 00 --- 255  0    0
GREEN   00 FF 00 --- 0    255  0
RED     00 00 FF --- 0    0    255
*/
struct PIXEL24 *HEX_to_PIXEL24(uint8_t *hex_data, struct BMP_INFO bmp_info) {
    int padding = 0;
    struct PIXEL24 IMAGE[bmp_info.heigth_py][bmp_info.width_px];
    return NULL;
}


// For debugging purposes
void print_head(struct BMP_HEADER image_head) {
    printf("Image: type: %X\n", image_head.type);
    printf("Image: size: %d\n", image_head.size);
    printf("Image: reserved1: %d\n", image_head.reserved1);
    printf("Image: reserved2: %d\n", image_head.reserved2);
    printf("Image: offset: %d\n", image_head.offset);
    printf("Image: offset: %x\n", image_head.offset);
    printf("Image: dib_header_size: %d\n", image_head.dib_header_size);
    printf("Image: width_px: %d\n", image_head.width_px);
    printf("Image: height_px: %d\n", image_head.height_px);
    printf("Image: num_planes: %d\n", image_head.num_planes);
    printf("Image: bits_per_pixel: %d\n", image_head.bits_per_pixel);
    printf("Image: compression: %d\n", image_head.compression);
    printf("Image: image_size_bytes: %d\n", image_head.image_size_bytes);
    printf("Image: x_resolution_ppm: %d\n", image_head.x_resolution_ppm);
    printf("Image: y_resolution_ppm: %d\n", image_head.y_resolution_ppm);
    printf("Image: num_colors: %d\n", image_head.num_colors);
    printf("Image: important_colors: %d\n", image_head.important_colors);
}