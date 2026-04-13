#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *image, *outputImage, *lecturas;
    image = fopen("kong.bmp","rb");          //Imagen original a transformar
    outputImage = fopen("imgAZUL.bmp","wb");    //Imagen transformada

    unsigned char r, g, b;               //Pixel

    for(int i=0; i<54; i++) fputc(fgetc(image), outputImage);   //Copia cabecera a nueva imagen
    while(!feof(image)){                                        //Azules
       b = fgetc(image);
       g = fgetc(image);
       r = fgetc(image);

       unsigned char intensity = 0.21*r+0.72*g+0.07*b;
       fputc(b, outputImage);              // B
       fputc(0, outputImage);              // G
       fputc(0, outputImage);              // R
    }

    fclose(image);
    fclose(outputImage);
    return 0;
}
