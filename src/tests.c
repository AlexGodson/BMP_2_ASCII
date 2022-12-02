#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Include/bmp.h"
#include "../Include/tests.h"


// Dumps the contents of the image hex to a text file names image_hex.txt
void dump_img_to_txt(struct BMP_HEADER header, char *filename) {
    FILE *fptr = fopen(filename, "rb");

    if (fptr == NULL) {
        printf("Error opening file: %s\n", filename);
        exit(-1);
    }

    // Moving the file data pointer to the start of the image Hex data
    // Image data starts at bmp_head->offset
    int found_seek = fseek(fptr, header.offset, SEEK_SET);

    if (found_seek == -1) {
        printf("ERROR: Could not find bmp image offset in file\n");
        exit(-1);
    }

    FILE *out_fptr = fopen("temp/image_hex.txt", "w");;

    if (out_fptr == NULL) {
        printf("Error opening file: ..temp/image_hex.txt\n");
        exit(-1);
    }    

    uint8_t hex;
    uint8_t *hex_ptr = &hex;
    int width = 0;
    int pos = 0;
    while (pos < (header.image_size_bytes)) {
        fread(hex_ptr, sizeof(uint8_t), 1, fptr);
        fprintf(out_fptr, "%x ", *hex_ptr);
        width++;
        pos++;
        if (header.width_px % 4 != 0 && width == header.width_px * header.height_px * 3) {
            fseek(fptr, 4 - (header.width_px % 4), SEEK_CUR);
            width=0;
        }
        if (width%3 == 0) {
            fprintf(out_fptr, "\n");
        }
    }

    fclose(out_fptr);
    fclose(fptr);
}


// Dumps the read hex data to a text file named produced_hex.txt
void dump_hex_to_txt(struct BMP_HEADER header, uint8_t *hex) {

}


// Dumps the read Pixel data to a textfile named read_pixel_hex.txt
void dump_pixel_to_hex(struct BMP_HEADER header, struct PIXEL24 *pixels) {

}


// For debugging purposes
// Prints all of the meta-data associated with the provided BMP file
void print_head(struct BMP_HEADER image_head) {
    printf("Image: type: %X\n", image_head.type);
    printf("Image: size: %d\n", image_head.size);
    printf("Image: reserved1: %d\n", image_head.reserved1);
    printf("Image: reserved2: %d\n", image_head.reserved2);
    printf("Image: offset: %d\n", image_head.offset);
    printf("Image: offset: %x\n", image_head.offset);
    printf("Image: dib_header_size: %d\n", image_head.dib_header_size);
    printf("Image: width_px: %d\n", image_head.width_px);
    printf("Image: height_px: %d\n", image_head.height_px);
    printf("Image: num_planes: %d\n", image_head.num_planes);
    printf("Image: bits_per_pixel: %d\n", image_head.bits_per_pixel);
    printf("Image: compression: %d\n", image_head.compression);
    printf("Image: image_size_bytes: %d\n", image_head.image_size_bytes);
    printf("Image: x_resolution_ppm: %d\n", image_head.x_resolution_ppm);
    printf("Image: y_resolution_ppm: %d\n", image_head.y_resolution_ppm);
    printf("Image: num_colors: %d\n", image_head.num_colors);
    printf("Image: important_colors: %d\n", image_head.important_colors);
}

