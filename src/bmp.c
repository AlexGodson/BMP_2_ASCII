#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Include/bmp.h"

// Get some head bitches (and some image data from BMP-HEX stored as single bytes)
uint8_t *get_BMP_data(struct BMP_HEADER *bmp_head, char *filepath) {
    FILE *fptr = fopen(filepath, "rb");

    if (fptr == NULL) {
        printf("Error opening file: %s\n", filepath);
        return NULL;
    }

    // Reading 54 bytes from the bmp-header data to the BMP_HEADER struct once
    fread(bmp_head, sizeof(struct BMP_HEADER), 1, fptr);

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


// For debugging purposes
// Prints all of the meta-data associated with the provided BMP file
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


// turns the header from the read in file to the useful one to be passed to functions
struct BMP_INFO transform_header(struct BMP_HEADER header) {
    // initialising an bmp_info with useful data
    struct BMP_INFO bmp_info = {
        header.size,
        header.offset,
        header.width_px,
        header.height_px,
        header.bits_per_pixel,
        header.image_size_bytes,
    };
    
    switch (header.bits_per_pixel) {
    /*  Not Implemented yet
    case 1:
        bmp_info.transform_hex = HEX_to_PIXEL1;
        bmp_info.normalise_pixels = normalise_pixels1;
        break;
    case 2:
        bmp_info.transform_hex = HEX_to_PIXEL2;
        bmp_info.normalise_pixels = normalise_pixels2;
        break;
    case 4:
        bmp_info.transform_hex = HEX_to_PIXEL4;
        bmp_info.normalise_pixels = normalise_pixels4;
        break;
    case 8:
        bmp_info.transform_hex = HEX_to_PIXEL8;
        bmp_info.normalise_pixels = normalise_pixels8;
        break;
    case 16:
        bmp_info.transform_hex = HEX_to_PIXEL16;
        bmp_info.normalise_pixels = normalise_pixels16;
        break;
    */
    case 24:
        bmp_info.transform_hex = HEX_to_PIXEL24;
        bmp_info.normalise_pixels = normalise_pixels24;
        break;
    case 32:
        bmp_info.transform_hex = HEX_to_PIXEL32;
        bmp_info.normalise_pixels = normalise_pixels32;
        break;
    }

    return bmp_info;
}


// Retrieves the settings stated in the Settings.txt file and puts them in the BMP_INFO struct
void get_settings(struct BMP_INFO *bmp_info) {
    FILE *fptr = fopen("Settings.txt", "r");
    if (fptr == NULL) {
        printf("ERROR: Error opening settings file\n");
        exit(-1);
    }

    int settings[SETTINGS_NUM] = {0};
    char digits[MAX_DIGITS] = {0};
    int setting_num = 0, digits_place = 0;
    char cur = fgetc(fptr);
    while (cur != EOF) {
        if (0 <= (cur-'0') && (cur-'0') <= 9) {
            digits[digits_place] = cur;
            digits_place++;
        }
        if (cur == ',') {
            settings[setting_num] = atoi(digits);
            setting_num++;
            digits_place = 0;
            for (int i = 0; i < MAX_DIGITS; ++i) {
                digits[i] = 0;
            }
        }
        if (setting_num == SETTINGS_NUM) {
            break;
        }
        cur = fgetc(fptr);
    }
    fclose(fptr);

    bmp_info->ascii_scaling_factor = settings[0];
    bmp_info->cut_off = settings[1];
    bmp_info->mark_down = settings[2];

    // Don't judge me, I know this is silly but the functionality isn't implemented yet
    // Just to be on the safe side we will leave this here so you dont mess with anything
    bmp_info->ascii_scaling_factor = 1; // TODO GET RID OF MEEEEE
    bmp_info->asc_w = bmp_info->width_px/bmp_info->ascii_scaling_factor;
    bmp_info->asc_h = bmp_info->height_px/bmp_info->ascii_scaling_factor;
}


// reading the data from the raw HEX into the BMP specific pixel format
/*      24bpp
Blue -  FF 00 00 --- 255  0    0
Green - 00 FF 00 --- 0    255  0
Red -   00 00 FF --- 0    0    255
....    00 00 00 --- Buffer to bring the total bytes per row to a multiple of 4
*/
struct PIXEL *HEX_to_PIXEL24(struct BMP_INFO bmp_info, uint8_t *hex_data) {
    struct PIXEL *pixel_data = (struct PIXEL*)malloc(sizeof(struct PIXEL) * bmp_info.height_px * bmp_info.width_px);

    if (pixel_data == NULL) {
        printf("ERROR: error allocating enough space for the Pixel data\n");
        return NULL;
    }

    int padding = 4 - (bmp_info.width_px % 4);
    int hex_pos = 0;
    for (int hp = 0; hp < bmp_info.height_px; ++hp) {
        for (int wp = 0; wp < bmp_info.width_px; ++wp) {
            pixel_data[(hp * bmp_info.width_px) + wp].A = *(hex_data + hex_pos);
            pixel_data[(hp * bmp_info.width_px) + wp].B = *(hex_data + hex_pos + 1);
            pixel_data[(hp * bmp_info.width_px) + wp].C = *(hex_data + hex_pos + 2);
            hex_pos += 3;
        }
        // shifting the bits to the end of the row to make it a multiple of 4
        if (padding < 4) {
            hex_pos += (padding);
        }
    }

    free(hex_data);
    return pixel_data;
}


/*      32bpp
Blue -  FF 00 00 00 --- 255  0    0    0
Green - 00 FF 00 00 --- 0    255  0    0
Red -   00 00 FF 00 --- 0    0    255  0
Alpha - 00 00 00 FF --- 0    0    0    255 
....    00 00 00 00--- then buffer to bring the total bytes per row to a multiple of 4 (Not Necessary here)
*/
struct PIXEL *HEX_to_PIXEL32(struct BMP_INFO bmp_info, uint8_t *hex_data) {
    struct PIXEL *pixel_data = (struct PIXEL*)malloc(sizeof(struct PIXEL) * bmp_info.height_px * bmp_info.width_px);

    if (pixel_data == NULL) {
        printf("ERROR: error allocating enough space for the Pixel data\n");
        return NULL;
    }

    int hex_pos = 0;
    for (int hp = 0; hp < bmp_info.height_px; ++hp) {
        for (int wp = 0; wp < bmp_info.width_px; ++wp) {
            pixel_data[(hp * bmp_info.width_px) + wp].A = *(hex_data + hex_pos);
            pixel_data[(hp * bmp_info.width_px) + wp].B = *(hex_data + hex_pos + 1);
            pixel_data[(hp * bmp_info.width_px) + wp].C = *(hex_data + hex_pos + 2);
            pixel_data[(hp * bmp_info.width_px) + wp].D = *(hex_data + hex_pos + 3);
            hex_pos += 4;
        }
    }

    free(hex_data);
    return pixel_data;
}


/*      24bpp
Blue -  FF 00 00 --- 255  0    0
Green - 00 FF 00 --- 0    255  0
Red -   00 00 FF --- 0    0    255
....    00 00 00 --- Buffer to bring the total bytes per row to a multiple of 4
*/
uint8_t *normalise_pixels24(struct BMP_INFO head, struct PIXEL *pixels) {
    // allocating enough memory to hold the normalised pixel values
    uint8_t *normal_pixels = (uint8_t*)malloc(sizeof(uint8_t) * head.width_px * head.height_px);

    if (normal_pixels == NULL) {
        printf("ERROR: Couldn't allocate memory for the normalised pixels\n");
        return NULL;
    }

    // The average value of the pixels RGB values to be calculated in the following switch statement
    uint8_t avg;
    for (int pos = 0; pos < head.width_px * head.height_px; ++pos) {
        avg = (pixels[pos].A + pixels[pos].B + pixels[pos].C) / 3;
        *(normal_pixels + pos) = avg;
    }

    // Freeing the pixel data as it is not needed anymore
    free(pixels);
    return (normal_pixels);
}


// /*      32bpp
// Blue    11111111 10000000 00000000 00000000
// Green   00000000 01111111 10000000 00000000
// Red     00000000 00000000 01111111 00000000
// Alpha   00000000 00000000 00000000 11111000
// NULL    00000000 00000000 00000000 00000111
// ....    00 00 00 00--- then buffer to bring the total bytes per row to a multiple of 4
// */
uint8_t *normalise_pixels32(struct BMP_INFO head, struct PIXEL *pixels) {
    // allocating enough memory to hold the normalised pixel values
    uint8_t *normal_pixels = (uint8_t*)malloc(sizeof(uint8_t) * head.width_px * head.height_px);

    if (normal_pixels == NULL) {
        printf("ERROR: Couldn't allocate memory for the normalised pixels\n");
        return NULL;
    }

    // The average value of the pixels RGB values to be calculated in the following switch statement
    uint8_t avg;
    for (int pos = 0; pos < head.width_px * head.height_px; ++pos) {
        avg = (pixels[pos].A + pixels[pos].B + pixels[pos].C) / 3;
        *(normal_pixels + pos) = avg;
    }

    // Freeing the pixel data as it is not needed anymore
    free(pixels);
    return (normal_pixels);
}


// Compresses the image to the correct width and height to print to ascii
// TODO
uint8_t *compress_image(struct BMP_INFO bmp_info, uint8_t *pixel_norm, int asc_w, int asc_h) {
    uint8_t *asc_BLOC = (uint8_t*)malloc( sizeof(uint8_t) * asc_w * asc_h );
    
    // if (asc_BLOC == NULL) {
    //     printf("ERROR: Ascii image memory not initialised\n");
    //     return NULL;
    // }

    // int width_cutoff = (bmp_info.width_px % bmp_info.ascii_scaling_factor) / 2;
    // int height_cutoff = (bmp_info.height_px & bmp_info.ascii_scaling_factor) / 2;
    // int start_pix = (height_cutoff * bmp_info.width_px) + width_cutoff;

    // int useful_pixels = bmp_info.ascii_scaling_factor * bmp_info.ascii_scaling_factor * bmp_info.asc_h * bmp_info.asc_w;

    // int avg;
    // for(int px = start_pix; px < useful_pixels; ++px) {

    // }
    

    return asc_BLOC;
}


// converts normalised pixel values to an array of ascii characters provided
uint8_t *asciify(struct BMP_INFO bmp_info, uint8_t *values, uint8_t *ascii_table, int table_sz) {
    uint8_t *ascii = (uint8_t*)malloc(sizeof(uint8_t) * bmp_info.asc_w * bmp_info.asc_h);

    if (ascii == NULL) {
        printf("ERROR: ascii image array not initialised\n");
        return NULL;
    }

    int ascii_char;
    printf("mark_down: %d", bmp_info.mark_down);
    for (int i = 0; i < bmp_info.asc_w * bmp_info.asc_h; ++i) {
        ascii_char = *(values + i) * (table_sz-1);
        ascii_char = (ascii_char / 255) - bmp_info.mark_down;
        if (ascii_char < 0 || ascii_char > bmp_info.cut_off) {
            ascii_char = 0;
        }
        *(ascii + i) = *(ascii_table + ascii_char);
    }

    free(values);
    return ascii;
}


// Prints the ascii text array to the terminal
void print_ascii(struct BMP_INFO bmp_info, uint8_t *ascii_array) {
    // BMP image 0,0 is at the botton left corner
    // it must print upside down
    int pos;
    int row = bmp_info.asc_h-1;
    for (int hp = 0; hp < bmp_info.asc_h; ++hp) {
        for (int wp = 0; wp < bmp_info.asc_w; ++wp) {
            pos = (row * bmp_info.asc_w + wp);
            printf("%c%c", *(ascii_array + pos), *(ascii_array + pos));
        }
        row--;
        printf("\n");
    }

    printf("\n\n");
}


// Saves the ascii image to ascii.txt file
void save_ascii(struct BMP_INFO bmp_info, uint8_t *ascii_array) {
    FILE *fptr = fopen("ASCII.txt", "w");

    if (fptr == NULL) {
        printf("ERROR: failed to open Ascii output text file\n");
        return;
    }

    printf("Saving Ascii image to text file named \"ASCII.txt\"\n");
    // BMP image 0,0 is at the botton left corner
    // it must print upside down
    int pos;
    int row = bmp_info.asc_h-1;
    for (int hp = 0; hp < bmp_info.asc_h; ++hp) {
        for (int wp = 0; wp < bmp_info.asc_w; ++wp) {
            pos = (row * bmp_info.asc_w + wp);
            fprintf(fptr, "%c%c", *(ascii_array + pos), *(ascii_array + pos));
        }
        row--;
        fprintf(fptr, "\n");
    }

    fprintf(fptr, "\n\n");
    fclose(fptr);
}
