#include <stdio.h>
#include <stdlib.h>

#include "../Include/bmp.h"

// Get some head bitches
    // Checking if the file is the correct BMP format, exit otherwise
void get_format(FILE *fptr, struct BMP_head *image) {

    // Checking the file type. Require file type of BM to continue
    image->type[0] = fgetc(fptr);
    image->type[1] = fgetc(fptr);

    if ((image->type[0] != 'B') && (image->type[1] != 'M')) {
        printf("ERROR: Image provided is not of the BMP file format\n");
        exit(-1);
    }
}

    // TODO
void get_head(FILE *fptr, struct BMP_head *image) {
    // TODO
}


