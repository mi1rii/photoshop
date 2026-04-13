#include "selec_proc_1.h"
#include "selec_proc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void inv_img_color(const char *output_prefix, const char *input_path) {
    Bmp24 bmp;
    char output_path[512];
    int y;
    int x;

    memset(&bmp, 0, sizeof(Bmp24));
    make_output_name(output_prefix, output_path, sizeof(output_path));
    if (output_path[0] == '\0') {
        printf("Error: nombre de salida demasiado largo.\n");
        return;
    }

    if (!read_bmp24(input_path, &bmp)) {
        printf("Error: no se pudo leer BMP en %s (se requiere BMP 24-bit sin compresion).\n", input_path);
        return;
    }

    for (y = 0; y < bmp.height; y++) {
        for (x = 0; x < bmp.width; x++) {
            int idx = y * bmp.row_size + x * 3;
            bmp.data[idx] = (unsigned char)(255 - bmp.data[idx]);
            bmp.data[idx + 1] = (unsigned char)(255 - bmp.data[idx + 1]);
            bmp.data[idx + 2] = (unsigned char)(255 - bmp.data[idx + 2]);
        }
    }

    if (!write_bmp24(output_path, &bmp, bmp.data)) {
        printf("Error: no se pudo escribir %s\n", output_path);
    } else {
        printf("Imagen invertida en color creada: %s\n", output_path);
    }

    free(bmp.preamble);
    free(bmp.data);
}
