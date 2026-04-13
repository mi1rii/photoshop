#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *image, *outputImage, *lecturas;
    image = fopen("sample.bmp","rb");          //Imagen original a transformar
    outputImage = fopen("imgROJO.bmp","wb");    //Imagen transformada

    unsigned char r, g, b;               //Pixel

    for(int i=0; i<54; i++) fputc(fgetc(image), outputImage);   //Copia cabecera a nueva imagen
    while(!feof(image)){                                        //Azules
       b = fgetc(image);
       g = fgetc(image);
       r = fgetc(image);

       unsigned char intensity = 0.21*r+0.72*g+0.07*b;
       fputc(0, outputImage);              // B
       fputc(0, outputImage);              // G
       fputc(r, outputImage);              // R
    }

    fclose(image);
    fclose(outputImage);
    return 0;
}
