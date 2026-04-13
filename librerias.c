#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "selec_proc.h"
#include "selec_proc_1.h"

#define NUM_THREADS 6

int main(){
    FILE *archivo;
    char data[80] = "arc1.txt";

    omp_set_num_threads(NUM_THREADS);

    archivo = fopen(data, "w");
    if (archivo == NULL){
        printf("Error al abrir el archivo");
        return 1;
    }
    fprintf(archivo, "Ejemplo escribir\n");
    fprintf(archivo, "Estefania Antonio\n");
    fclose(archivo);

    #pragma omp parallel sections
    {
        #pragma omp section
        inv_img("inv_1","prueba1.bmp");

        #pragma omp section
        inv_img_lado("inv_1_lado","prueba1.bmp");

        #pragma omp section
        inv_img_color("inv_1_color","prueba1.bmp");

        #pragma omp section
        inv_img_color_lado("inv_1_color_lado","prueba1.bmp");

        #pragma omp section
        desenfoque_bn("prueba1.bmp","desenfoque_bn.bmp", 47);

        #pragma omp section
        desenfoque_color("prueba1.bmp","desenfoque_color.bmp", 47);
    }

    return 0;
}
