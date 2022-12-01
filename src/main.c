#include <stdio.h>
#include <stdlib.h>

#include "../Include/bmp.h"

char ASCII_TABLE[92] = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ&@";

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

    print_head(bmp_head);

    struct PIXEL24 *Pixel_Data = HEX_to_PIXEL24(HEX_DATA, bmp_head);

    if (Pixel_Data == NULL) {
        printf("ERROR: Pixel data empty, Exiting the program\n");
        return -1;
    }

    int last = 3 * bmp_head.height_px * bmp_head.width_px;
    printf("Pixel %x Data:\t%x, %x, %x\n", last + 138, Pixel_Data[last].B, Pixel_Data[last].G, Pixel_Data[last].R);

    free(HEX_DATA);
    free(Pixel_Data);
    return 0;
}