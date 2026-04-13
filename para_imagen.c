# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "selec_proc.h"
# include "selec_proc_1.h"

int main(){
    FILE *fptr;
    char data[80] = "arc1.txt";
    fptr = fopen(data, "w");
    if (fptr == NULL){
        printf("Error\n");
        exit(1);
    }
    fprintf(fptr, "Ejemplo escribir\n");
    fprintf(fptr, "Miranda Eugenia Colorado Arróniz\n");
    fclose(fptr);

    inv_img("inv_1", "./img/sample1.bmp");
    inv_img_color("inv_color_1", "./img/sample1.bmp");
    desenfoque("./img/sample1.bmp", "desenfoque", 47);
    return 0;
}