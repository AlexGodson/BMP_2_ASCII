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
    struct BMP_INFO bmp_info = {
        bmp_head.image_size_bytes,
        bmp_head.width_px,
        bmp_head.height_px,
    };

    if (HEX_DATA == NULL) {
        printf("ERROR: Image data not loaded correctly\n");
        printf("Exiting Program\n");
        return -1;
    }

    print_head(bmp_head);

    return 0;
}