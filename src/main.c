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
    get_settings(&BMP);

    // transforms the Hex data to a pixel array using the respective bits per pixel format
    struct PIXEL *pixels = BMP.transform_hex(BMP, HEX_DATA);

    // normalises the pixel data into an array scaled from 0 255 brightness
    uint8_t *pixels_normalised = BMP.normalise_pixels(BMP, pixels);

    BMP.asc_h = BMP.height_px;
    BMP.asc_w = BMP.width_px;
    uint8_t *ascii = asciify(pixels_normalised, BMP, ASCII_TABLE, table_sz);

    print_ascii(ascii, BMP);
    save_ascii(ascii, BMP);

    print_head(BMP_HEAD);

    return 0;
}