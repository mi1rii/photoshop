#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "img_ops.h"   

#define NUM_THREADS 6

int main() {
    FILE *fptr;
    char data[80] = "results/arc1.txt";
    system("mkdir -p results");
    fptr = fopen(data, "w");
    if (fptr == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    fprintf(fptr, "Ejemplo escribir en un archivo");
    fclose(fptr);

    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            inv_img("results/inv_1", "./img/images.bmp");
            #pragma omp section
            inv_img_grey_horizontal("results/espejo_gris", "./img/images.bmp");
            #pragma omp section
            inv_img_color("results/inv_color_1", "./img/images.bmp");
            #pragma omp section
            inv_img_bn_vertical("results/inv_bn_vertical", "./img/images.bmp");
            #pragma omp section
            desenfoque("./img/images.bmp", "results/desenfoque", 27);
            #pragma omp section
            desenfoque_gris("./img/images.bmp", "results/desenfoque_bn", 27);
            #pragma omp section
            inv_img_color_horizontal("results/espejo_color", "./img/images.bmp");
        }
    }
    return 0;
}