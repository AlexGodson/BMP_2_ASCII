#include <stdio.h>
#include <stdlib.h>

#include "../Include/bmp.h"

struct BMP {
    char* type;
};

FILE *_fopen(char *filename) {
    char filepath[15] = "INPUTS/";
    strcat(filepath, filename);
    FILE *fptr = fopen(filepath, "r");
    if (fptr == NULL) {
        printf("ERROR: could not open file");
        printf("Location: shunt.c, _fopen()\n");
        exit(-1);
    }
    return fptr;
}