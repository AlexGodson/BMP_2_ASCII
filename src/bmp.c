#include <stdio.h>
#include <stdlib.h>

#include "../Include/bmp.h"

// Get some head bitches
void get_BMP_head(char *filepath, struct BMP_HEADER *bmp_head) {
    FILE *fptr = fopen(filepath, "rb");

    if (fptr == NULL) {
        printf("Error opening file: %s\n", filepath);
        exit(-1);
    }

    printf("size of the stuct is: %ld", sizeof(*bmp_head));

    fread(bmp_head, sizeof(*bmp_head), 1, fptr);

    if (bmp_head->type != 0x4D42) {
        printf("ERROR: Invalid file, not a BMP\n");
        exit(-1);
    }

    fclose(fptr);
}


