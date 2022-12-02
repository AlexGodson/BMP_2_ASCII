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

    // Reading 54 bytes from the bmp-header data to the BMP_HEADER struct once
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
    uint8_t *HEX_DATA = (uint8_t *)malloc(bmp_head->image_size_bytes);

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
Blue -  FF 00 00 --- 255  0    0
Green - 00 FF 00 --- 0    255  0
Red -   00 00 FF --- 0    0    255
*/
struct PIXEL24 *HEX_to_PIXEL24(uint8_t *hex_data, struct BMP_HEADER bmp_info) {
    struct PIXEL24 *Pixel_Data = (struct PIXEL24 *)malloc(sizeof(struct PIXEL24) * bmp_info.height_px * bmp_info.width_px);

    if (Pixel_Data == NULL) {
        printf("ERROR: error allocating enough space for the Pixel data\n");
        return NULL;
    }

    int padding = 4 - (bmp_info.width_px % 4);
    int hex_pos = 0;
    for (int hp = 0; hp < bmp_info.height_px; ++hp) {
        for (int wp = 0; wp < bmp_info.width_px; ++wp) {
            Pixel_Data[(hp * bmp_info.width_px) + wp].B = *(hex_data + hex_pos);
            Pixel_Data[(hp * bmp_info.width_px) + wp].G = *(hex_data + hex_pos + 1);
            Pixel_Data[(hp * bmp_info.width_px) + wp].R = *(hex_data + hex_pos + 2);
            hex_pos += 3;
        }
        // shifting the bits to the end of the row to make it a multiple of 4
        if (padding < 4) {
            hex_pos += (padding);
        }
    }

    return Pixel_Data;
}


// averages the pixels to single values to represent their brightness directly based on brightness
// B G R -> 87 201 18 -> 102
uint8_t *normalise_pixels_linear(struct BMP_HEADER head, struct PIXEL24 *pixels) {
    uint8_t *normal_pixels = (uint8_t*)malloc(sizeof(uint8_t) * head.width_px * head.height_px);

    if (normal_pixels == NULL) {
        printf("ERROR: Couldn't allocate memory for the normalised pixels\n");
        return NULL;
    }

    // setting the value at pos to the average pixel value of the pixel at pos
    uint8_t avg;
    for (int pos = 0; pos < head.width_px * head.height_px; ++pos) {
        avg = (pixels[pos].B + pixels[pos].G + pixels[pos].R) / 3;
        *(normal_pixels + pos) = avg;
    }

    // Freeing the pixel data as it is not needed anymore
    free(pixels);
    return (normal_pixels);
}


// converts the pixel RGB values to a brightness value
// higher R/G/B values are weighted to increase the brightness
uint8_t *normalise_pixels_weighted(struct BMP_HEADER head, struct PIXEL24 *pixels) {
    uint8_t *normal_pixels = (uint8_t*)malloc(sizeof(uint8_t) * head.width_px * head.height_px);

    for (int pos = 0; pos < head.width_px * head.height_px; ++pos) {
        // TODO
    }

    free(pixels);
    return normal_pixels;
}


// Compresses the image to the correct width and height to print to ascii
uint8_t *compress_image(struct BMP_HEADER head, uint8_t *pixel_norm, int asc_w, int asc_h) {
    uint8_t *normal_pixels = (uint8_t*)malloc( sizeof(uint8_t) * asc_w * asc_h );
    
    // TODO

    return normal_pixels;
}


// converts normalised pixel values to an array of ascii characters provided
uint8_t *asciify(uint8_t *values, int asc_w, int asc_h, uint8_t *ascii_table, int table_sz) {
    uint8_t *ascii = (uint8_t*)malloc(sizeof(uint8_t) * asc_w * asc_h);

    if (ascii == NULL) {
        printf("ERROR: ascii image array not initialised\n");
        return NULL;
    }

    int ascii_char;
    
    for (int i = 0; i < asc_w * asc_h; ++i) {
        ascii_char = *(values + i) * (table_sz-1);
        ascii_char = ascii_char / 255;
        *(ascii + i) = *(ascii_table + ascii_char);
    }

    free(values);

    return ascii;
}


// Prints the ascii text array to the terminal
void print_ascii(uint8_t *ascii_array, int asc_w, int asc_h) {
    // BMP image 0,0 is at the botton left corner
    // it must print upside down
    int pos;
    int row = asc_h-1;
    for (int hp = 0; hp < asc_h; ++hp) {
        for (int wp = 0; wp < asc_w; ++wp) {
            pos = (row * asc_w + wp);
            printf("%c%c", *(ascii_array + pos), *(ascii_array + pos));
        }
        row--;
        printf("\n");
    }

    printf("\n\n");
}


// Saves the ascii image to ascii.txt file
void save_ascii(uint8_t *ascii_array, int asc_w, int asc_h) {
    FILE *fptr = fopen("/home/alexg/Code/BMP/ASCII.txt", "w");

    if (fptr == NULL) {
        printf("ERROR: failed to open Ascii output text file\n");
        return;
    }

    printf("Saving Ascii image to text file named \"ASCII.txt\"\n");
    // BMP image 0,0 is at the botton left corner
    // it must print upside down
    int pos;
    int row = asc_h-1;
    for (int hp = 0; hp < asc_h; ++hp) {
        for (int wp = 0; wp < asc_w; ++wp) {
            pos = (row * asc_w + wp);
            fprintf(fptr, "%c%c", *(ascii_array + pos), *(ascii_array + pos));
        }
        row--;
        fprintf(fptr, "\n");
    }

    fprintf(fptr, "\n\n");
    fclose(fptr);
}
