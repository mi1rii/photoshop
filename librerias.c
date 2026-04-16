#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "img_ops.h"   

#define NUM_THREADS 6

int main() {
    clock_t start_time = clock();
    
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

    // Array de imágenes a procesar
    const char *images[] = {
        "./img/miri.bmp",
        "./img/kong.bmp",
        "./img/fany.bmp",
        "./img/sofi.bmp"
    };
    int num_images = 4;

    // Procesar cada imagen
    for (int img_idx = 0; img_idx < num_images; img_idx++) {
        // Extraer nombre base de la imagen (sin ruta ni extensión)
        char img_name[50];
        const char *filename = strrchr(images[img_idx], '/') + 1;  // Obtener nombre después del /
        strcpy(img_name, filename);
        char *dot = strrchr(img_name, '.');
        if (dot) *dot = '\0';  // Remover extensión .bmp
        
        #pragma omp parallel
        {
            #pragma omp sections
            {
                #pragma omp section
                {
                    char output[100];
                    sprintf(output, "results/%s_inv", img_name);
                    inv_img(output, images[img_idx]);
                }
                #pragma omp section
                {
                    char output[100];
                    sprintf(output, "results/%s_espejo_gris", img_name);
                    inv_img_grey_horizontal(output, images[img_idx]);
                }
                #pragma omp section
                {
                    char output[100];
                    sprintf(output, "results/%s_inv_color", img_name);
                    inv_img_color(output, images[img_idx]);
                }
                #pragma omp section
                {
                    char output[100];
                    sprintf(output, "results/%s_inv_bn_vertical", img_name);
                    inv_img_bn_vertical(output, images[img_idx]);
                }
                #pragma omp section
                {
                    char output[100];
                    sprintf(output, "results/%s_desenfoque", img_name);
                    desenfoque(images[img_idx], output, 27);
                }
                #pragma omp section
                {
                    char output[100];
                    sprintf(output, "results/%s_desenfoque_bn", img_name);
                    desenfoque_gris(images[img_idx], output, 27);
                }
                #pragma omp section
                {
                    char output[100];
                    sprintf(output, "results/%s_espejo_color", img_name);
                    inv_img_color_horizontal(output, images[img_idx]);
                }
            }
        }
    }
    
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecucion: %.2f segundos\n", elapsed_time);
    
    return 0;
}