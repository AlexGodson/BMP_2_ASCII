#include <stdio.h>
#include <stdlib.h>

#include "../Include/bmp.h"

#define table_sz 92

uint8_t ASCII_TABLE[table_sz] = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ&@";


int main(int argc, char **argv) {
    if (argc != 2) {
        printf("ERROR: incorrect usage\n");
        printf("Asciify <filepath>\n");
        exit(-1);
    }
    printf("\nFILE: %s\n", argv[1]);


    struct BMP_HEADER BMP_HEAD;
    uint8_t *HEX_DATA = get_BMP_data(&BMP_HEAD, *(argv + 1));

    if (HEX_DATA == NULL) {
        printf("ERROR: Image data not loaded correctly\n");
        printf("Exiting Program\n");
        return -1;
    }

    // struct to hold important data and the pixel transform function prototype
    struct BMP_INFO BMP = transform_header(BMP_HEAD);

    // sets the settings of the ascii image generator based on the Settings.txt file
    printf("Getting settings\n");
    get_settings(&BMP);

    // transforms the Hex data to a pixel array using the respective bits per pixel format
    struct PIXEL *pixels = BMP.transform_hex(BMP, HEX_DATA);

    // normalises the pixel data into an array scaled from 0 255 brightness
    uint8_t *pixels_normalised = BMP.normalise_pixels(BMP, pixels);


    // Turns the pixel data into the ascii character at the respective brightness level
    // Ascii array position == normal_vals/255 * Ascii_array_length
    uint8_t *ascii = asciify(BMP, pixels_normalised, ASCII_TABLE, table_sz);

    // printing the ascii image to console and 
    print_ascii(BMP, ascii);
    save_ascii(BMP, ascii);

    print_head(BMP_HEAD);

    free(ascii);

    printf("Settings 1: %d\nSettings 2: %d\n", BMP.asc_w, BMP.asc_h);
    printf("Settings 3: %d\nSettings 4: %d\n", BMP.cut_off, BMP.mark_down);

    return 0;
}