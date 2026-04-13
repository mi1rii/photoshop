#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "selec_proc.h"
#include "selec_proc_1.h"

int main(){
    FILE *archivo;
    char data[80] = "arc1.txt";
    archivo = fopen(data, "w");
    if (archivo == NULL){
        printf("Error al abrir el archivo");
        return 1;
    }
    fprintf(archivo, "Ejemplo escribir\n");
    fprintf(archivo, "Alejandro Kong\n");
    fclose(archivo); // hola
    inv_img("inv_1","images.bmp");
    inv_img_color("inv_1_color","images.bmp");
    desenfoque("images.bmp","desenfoque.bmp", 47);
    return 0;
}