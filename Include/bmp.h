#ifndef _BMP_
#define _BMP_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct BMP_head {
    int type[2];
};

struct PIXEL {
    uint8_t alpha;
    uint8_t B;
    uint8_t G;
    uint8_t R;
};

// Get some head bitches
    // Checking if the file is the correct BMP format, exit otherwise
void get_format(FILE *fptr, struct BMP_head *image);
    // TODO
void get_head(FILE *fptr, struct BMP_head *image);

#endif
