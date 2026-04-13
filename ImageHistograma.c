#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PIXELS 1000000

int main()
{
    FILE *image, *output;
    image = fopen("sample.bmp", "rb");          //Imagen original
    output = fopen("histograma.txt", "w");      //Archivo histograma en ASCII

    unsigned char r, g, b;
    int pixelCount = 0;
    
    //Arrays para almacenar los píxeles
    unsigned char *arrayR = (unsigned char*)malloc(MAX_PIXELS * sizeof(unsigned char));
    unsigned char *arrayG = (unsigned char*)malloc(MAX_PIXELS * sizeof(unsigned char));
    unsigned char *arrayB = (unsigned char*)malloc(MAX_PIXELS * sizeof(unsigned char));
    
    //Arrays para contar frecuencias (histograma)
    int histR[256] = {0};
    int histG[256] = {0};
    int histB[256] = {0};

    //Salta la cabecera BMP (54 bytes)
    for(int i = 0; i < 54; i++) fgetc(image);

    //Lee píxeles y los almacena en los arrays
    while(!feof(image) && pixelCount < MAX_PIXELS) {
        b = fgetc(image);
        if(feof(image)) break;
        g = fgetc(image);
        if(feof(image)) break;
        r = fgetc(image);
        if(feof(image)) break;
        
        arrayB[pixelCount] = b;
        arrayG[pixelCount] = g;
        arrayR[pixelCount] = r;
        
        histB[b]++;
        histG[g]++;
        histR[r]++;
        
        pixelCount++;
    }

    //Escribe el histograma en el archivo ASCII
    fprintf(output, "HISTOGRAMA DE IMAGEN\n");
    fprintf(output, "====================\n\n");
    fprintf(output, "Total de pixeles: %d\n\n", pixelCount);
    
    fprintf(output, "HISTOGRAMA CANAL RED (R)\n");
    fprintf(output, "------------------------\n");
    for(int i = 0; i < 256; i++) {
        if(histR[i] > 0) {
            fprintf(output, "Valor %3d: %d pixeles\n", i, histR[i]);
        }
    }
    
    fprintf(output, "\nHISTOGRAMA CANAL GREEN (G)\n");
    fprintf(output, "-------------------------\n");
    for(int i = 0; i < 256; i++) {
        if(histG[i] > 0) {
            fprintf(output, "Valor %3d: %d pixeles\n", i, histG[i]);
        }
    }
    
    fprintf(output, "\nHISTOGRAMA CANAL BLUE (B)\n");
    fprintf(output, "------------------------\n");
    for(int i = 0; i < 256; i++) {
        if(histB[i] > 0) {
            fprintf(output, "Valor %3d: %d pixeles\n", i, histB[i]);
        }
    }

    //Libera la memoria
    free(arrayR);
    free(arrayG);
    free(arrayB);
    
    fclose(image);
    fclose(output);
    
    printf("Histograma guardado en histograma.txt\n");
    return 0;
}
