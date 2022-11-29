#include <stdio.h>
#include <stdlib.h>

#include "../Include/bmp.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("ERROR: incorrect usage\n");
        printf("Asciify <filepath>\n");
        exit(-1);
    }

    FILE *fptr = fopen(argv[1], "r");

    if (fptr == NULL) {
        printf("Error opening file: %s\n", argv[1]);
        exit(-1);
    }

    struct BMP_head image_head;
    get_format(fptr, &image_head);

    printf("DATA TYPE: %c, %c\n\n", image_head.type[0], image_head.type[1]);


    // Closing the BMP file
    fclose(fptr);
    return 0;
}