#include <stdio.h>
#include "img_ops.h"

int main() {
    if (!bmp_black_white("images.bmp", "bn.bmp")) {
        printf("Error generando bn.bmp\n");
        return 1;
    }
    if (!bmp_grayscale("images.bmp", "grises.bmp")) {
        printf("Error generando grises.bmp\n");
        return 1;
    }
    if (!bmp_flip_horizontal("images.bmp", "flip_h.bmp")) {
        printf("Error generando flip_h.bmp\n");
        return 1;
    }
    if (!bmp_flip_vertical("images.bmp", "flip_v.bmp")) {
        printf("Error generando flip_v.bmp\n");
        return 1;
    }
    if (!bmp_blur("images.bmp", "blur.bmp", 47)) {
        printf("Error generando blur.bmp\n");
        return 1;
    }
    printf("Listo. Archivos generados correctamente.\n");
    return 0;
}