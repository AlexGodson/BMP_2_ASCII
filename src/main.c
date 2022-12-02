#include <stdio.h>
#include <stdlib.h>

#include "../Include/bmp.h"
#include "../Include/tests.h" // Remove  when the project is done

#define table_sz 92

uint8_t ASCII_TABLE[table_sz] = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ&@";
// uint8_t ASCII_TABLE[table_sz] = ".-+/=#@";

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("ERROR: incorrect usage\n");
        printf("Asciify <filepath>\n");
        exit(-1);
    }
    printf("\nFILE: %s\n", argv[1]);

    struct BMP_HEADER bmp_head;
    uint8_t *HEX_DATA = get_BMP_data(*(argv + 1), &bmp_head);

    if (HEX_DATA == NULL) {
        printf("ERROR: Image data not loaded correctly\n");
        printf("Exiting Program\n");
        return -1;
    }

    struct PIXEL24 *Pixel_Data = HEX_to_PIXEL24(HEX_DATA, bmp_head);

    if (Pixel_Data == NULL) {
        printf("ERROR: Pixel data empty, Exiting the program\n");
        return -1;
    }

    // Normalised pixel data stored as 8 bit ints
    uint8_t *Normal_Pixels = normalise_pixels_linear(bmp_head, Pixel_Data);

    if (Normal_Pixels == NULL) {
        printf("ERROR: normalised pixels not initialised\n");
        exit(-1);
    }

    int asc_w = bmp_head.width_px;
    int asc_h = bmp_head.height_px;

    // converting normalised pixels to corresponding ascii array;
    uint8_t *ascii = asciify(Normal_Pixels, asc_w, asc_h, ASCII_TABLE, table_sz);

    if (ascii == NULL) {
        printf("ERROR: No ascii image array found\n");
        exit(-1);
    }

    print_ascii(ascii, asc_w, asc_h);
    save_ascii(ascii, asc_w, asc_h);
    
    print_head(bmp_head);

    // Strictly for Debugging, Get rid of soon
    // print_head(bmp_head);

    free(HEX_DATA);
    return 0;
}