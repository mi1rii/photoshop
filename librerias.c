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
    fprintf(archivo, "Estefania Antonio\n");
    fclose(archivo);

    inv_img("inv_1","prueba1.bmp");
    inv_img_lado("inv_1_lado","prueba1.bmp");
    inv_img_color("inv_1_color","prueba1.bmp");
    inv_img_color_lado("inv_1_color_lado","prueba1.bmp");
    desenfoque_bn("prueba1.bmp","desenfoque_bn.bmp", 47);
    desenfoque_color("prueba1.bmp","desenfoque_color.bmp", 47);
    return 0;
}
