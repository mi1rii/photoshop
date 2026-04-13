#include "selec_proc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned char header[54];
    unsigned char *preamble;
    int pixel_offset;
    int width;
    int height;
    int row_size;
    int data_size;
    unsigned char *data;
} Bmp24;

static int read_bmp24(const char *path, Bmp24 *bmp) {
    FILE *f = fopen(path, "rb");
    int bits_per_pixel;
    int compression;

    if (f == NULL) {
        return 0;
    }

    if (fread(bmp->header, 1, 54, f) != 54) {
        fclose(f);
        return 0;
    }

    if (bmp->header[0] != 'B' || bmp->header[1] != 'M') {
        fclose(f);
        return 0;
    }

    bmp->width = *(int *)&bmp->header[18];
    bmp->height = *(int *)&bmp->header[22];
    bmp->pixel_offset = *(int *)&bmp->header[10];
    bits_per_pixel = *(short *)&bmp->header[28];
    compression = *(int *)&bmp->header[30];

    if (bmp->width <= 0 || bmp->height == 0 || bits_per_pixel != 24 || compression != 0 || bmp->pixel_offset < 54) {
        fclose(f);
        return 0;
    }

    if (bmp->height < 0) {
        bmp->height = -bmp->height;
    }

    bmp->row_size = ((bmp->width * 3 + 3) / 4) * 4;
    bmp->data_size = bmp->row_size * bmp->height;
    bmp->preamble = (unsigned char *)malloc((size_t)bmp->pixel_offset);
    if (bmp->preamble == NULL) {
        fclose(f);
        return 0;
    }
    fseek(f, 0, SEEK_SET);
    if (fread(bmp->preamble, 1, (size_t)bmp->pixel_offset, f) != (size_t)bmp->pixel_offset) {
        free(bmp->preamble);
        bmp->preamble = NULL;
        fclose(f);
        return 0;
    }

    bmp->data = (unsigned char *)malloc((size_t)bmp->data_size);
    if (bmp->data == NULL) {
        free(bmp->preamble);
        bmp->preamble = NULL;
        fclose(f);
        return 0;
    }

    if (fread(bmp->data, 1, (size_t)bmp->data_size, f) != (size_t)bmp->data_size) {
        free(bmp->preamble);
        bmp->preamble = NULL;
        free(bmp->data);
        bmp->data = NULL;
        fclose(f);
        return 0;
    }

    fclose(f);
    return 1;
}

static int write_bmp24(const char *path, const Bmp24 *bmp, const unsigned char *data) {
    FILE *f = fopen(path, "wb");
    if (f == NULL) {
        return 0;
    }

    if (fwrite(bmp->preamble, 1, (size_t)bmp->pixel_offset, f) != (size_t)bmp->pixel_offset) {
        fclose(f);
        return 0;
    }
    if (fwrite(data, 1, (size_t)bmp->data_size, f) != (size_t)bmp->data_size) {
        fclose(f);
        return 0;
    }

    fclose(f);
    return 1;
}

static void make_output_name(const char *prefix, char *out, size_t out_size) {
    if (snprintf(out, out_size, "%s.bmp", prefix) >= (int)out_size) {
        out[0] = '\0';
    }
}

void inv_img(const char *output_prefix, const char *input_path) {
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
            unsigned char b = bmp.data[idx];
            unsigned char g = bmp.data[idx + 1];
            unsigned char r = bmp.data[idx + 2];
            unsigned char gray = (unsigned char)(0.114f * b + 0.587f * g + 0.299f * r);
            unsigned char bw = (gray >= 128) ? 255 : 0;

            bmp.data[idx] = bw;
            bmp.data[idx + 1] = bw;
            bmp.data[idx + 2] = bw;
        }
    }

    if (!write_bmp24(output_path, &bmp, bmp.data)) {
        printf("Error: no se pudo escribir %s\n", output_path);
    } else {
        printf("Imagen blanco/negro creada: %s\n", output_path);
    }

    free(bmp.preamble);
    free(bmp.data);
}

void inv_img_lado(const char *output_prefix, const char *input_path) {
    Bmp24 bmp;                    // Estructura donde se guarda la informacion del BMP
    unsigned char *out_data;      // Arreglo donde se construira la nueva imagen ya transformada
    char output_path[512];        // Nombre completo del archivo de salida
    int y;                        // Recorre las filas de la imagen
    int x;                        // Recorre las columnas de la imagen

    memset(&bmp, 0, sizeof(Bmp24));                                // Inicializa en cero la estructura bmp
    make_output_name(output_prefix, output_path, sizeof(output_path)); // Forma el nombre final de salida con extension .bmp
    if (output_path[0] == '\0') {
        printf("Error: nombre de salida demasiado largo.\n");
        return;
    }

    if (!read_bmp24(input_path, &bmp)) {
        printf("Error: no se pudo leer BMP en %s (se requiere BMP 24-bit sin compresion).\n", input_path);
        return;
    }

    out_data = (unsigned char *)malloc((size_t)bmp.data_size);     // Reserva memoria para guardar la imagen de salida
    if (out_data == NULL) {
        free(bmp.preamble);                                         // Libera la cabecera extendida si fallo la reserva
        free(bmp.data);                                             // Libera los pixeles leidos si fallo la reserva
        printf("Error: no hay memoria para inv_img_lado.\n");
        return;
    }

    memcpy(out_data, bmp.data, (size_t)bmp.data_size);             // Copia la imagen original para conservar padding y base

    for (y = 0; y < bmp.height; y++) {
        for (x = 0; x < bmp.width; x++) {
            int src_idx = y * bmp.row_size + x * 3;                // Posicion del pixel original dentro del arreglo de bytes
            int dst_idx = y * bmp.row_size + (bmp.width - 1 - x) * 3; // Posicion espejo: mismo renglon, columna invertida
            unsigned char b = bmp.data[src_idx];                   // Componente azul del pixel original
            unsigned char g = bmp.data[src_idx + 1];               // Componente verde del pixel original
            unsigned char r = bmp.data[src_idx + 2];               // Componente rojo del pixel original
            unsigned char gray = (unsigned char)(0.114f * b + 0.587f * g + 0.299f * r); // Convierte el pixel a gris
            unsigned char bw = (gray >= 128) ? 255 : 0;            // Si el gris es alto queda blanco, si no queda negro

            out_data[dst_idx] = bw;                                // Guarda azul en blanco o negro en la posicion invertida
            out_data[dst_idx + 1] = bw;                            // Guarda verde en blanco o negro en la posicion invertida
            out_data[dst_idx + 2] = bw;                            // Guarda rojo en blanco o negro en la posicion invertida
        }
    }

    if (!write_bmp24(output_path, &bmp, out_data)) {
        printf("Error: no se pudo escribir %s\n", output_path);
    } else {
        printf("Imagen blanco/negro invertida de lado creada: %s\n", output_path);
    }

    free(out_data);                                                 // Libera la memoria de la imagen de salida
    free(bmp.preamble);                                             // Libera la memoria de la parte inicial del BMP
    free(bmp.data);                                                 // Libera la memoria de la imagen original
}

void desenfoque_bn(const char *input_path, const char *output_path, int kernel_size) {
    Bmp24 bmp;
    unsigned char *out_data;
    int radius;
    int y;
    int x;

    if (kernel_size < 1) {
        kernel_size = 1;
    }
    if (kernel_size % 2 == 0) {
        kernel_size += 1;
    }
    radius = kernel_size / 2;

    memset(&bmp, 0, sizeof(Bmp24));
    if (!read_bmp24(input_path, &bmp)) {
        printf("Error: no se pudo leer BMP en %s (se requiere BMP 24-bit sin compresion).\n", input_path);
        return;
    }

    out_data = (unsigned char *)malloc((size_t)bmp.data_size);
    if (out_data == NULL) {
        free(bmp.data);
        printf("Error: no hay memoria para desenfoque.\n");
        return;
    }
    memcpy(out_data, bmp.data, (size_t)bmp.data_size);

    for (y = 0; y < bmp.height; y++) {
        for (x = 0; x < bmp.width; x++) {
            int ky;
            int kx;
            int count = 0;
            int sum_gray = 0;
            int out_idx = y * bmp.row_size + x * 3;

            for (ky = -radius; ky <= radius; ky++) {
                int ny = y + ky;
                if (ny < 0 || ny >= bmp.height) {
                    continue;
                }
                for (kx = -radius; kx <= radius; kx++) {
                    int nx = x + kx;
                    int in_idx;
                    unsigned char b;
                    unsigned char g;
                    unsigned char r;
                    unsigned char gray;
                    if (nx < 0 || nx >= bmp.width) {
                        continue;
                    }
                    in_idx = ny * bmp.row_size + nx * 3;
                    b = bmp.data[in_idx];
                    g = bmp.data[in_idx + 1];
                    r = bmp.data[in_idx + 2];
                    gray = (unsigned char)(0.114f * b + 0.587f * g + 0.299f * r);
                    sum_gray += gray;
                    count++;
                }
            }

            if (count > 0) {
                unsigned char gray_out = (unsigned char)(sum_gray / count);
                out_data[out_idx] = gray_out;
                out_data[out_idx + 1] = gray_out;
                out_data[out_idx + 2] = gray_out;
            }
        }
    }

    if (!write_bmp24(output_path, &bmp, out_data)) {
        printf("Error: no se pudo escribir %s\n", output_path);
    } else {
        printf("Imagen desenfocada en blanco y negro creada: %s (kernel=%d)\n", output_path, kernel_size);
    }

    free(out_data);
    free(bmp.preamble);
    free(bmp.data);
}

void desenfoque_color(const char *input_path, const char *output_path, int kernel_size) {
    Bmp24 bmp;
    unsigned char *out_data;
    int radius;
    int y;
    int x;

    if (kernel_size < 1) {
        kernel_size = 1;
    }
    if (kernel_size % 2 == 0) {
        kernel_size += 1;
    }
    radius = kernel_size / 2;

    memset(&bmp, 0, sizeof(Bmp24));
    if (!read_bmp24(input_path, &bmp)) {
        printf("Error: no se pudo leer BMP en %s (se requiere BMP 24-bit sin compresion).\n", input_path);
        return;
    }

    out_data = (unsigned char *)malloc((size_t)bmp.data_size);
    if (out_data == NULL) {
        free(bmp.data);
        printf("Error: no hay memoria para desenfoque color.\n");
        return;
    }
    memcpy(out_data, bmp.data, (size_t)bmp.data_size);

    for (y = 0; y < bmp.height; y++) {
        for (x = 0; x < bmp.width; x++) {
            int ky;
            int kx;
            int count = 0;
            int sum_b = 0;
            int sum_g = 0;
            int sum_r = 0;
            int out_idx = y * bmp.row_size + x * 3;

            for (ky = -radius; ky <= radius; ky++) {
                int ny = y + ky;
                if (ny < 0 || ny >= bmp.height) {
                    continue;
                }
                for (kx = -radius; kx <= radius; kx++) {
                    int nx = x + kx;
                    int in_idx;
                    if (nx < 0 || nx >= bmp.width) {
                        continue;
                    }
                    in_idx = ny * bmp.row_size + nx * 3;
                    sum_b += bmp.data[in_idx];
                    sum_g += bmp.data[in_idx + 1];
                    sum_r += bmp.data[in_idx + 2];
                    count++;
                }
            }

            if (count > 0) {
                out_data[out_idx] = (unsigned char)(sum_b / count);
                out_data[out_idx + 1] = (unsigned char)(sum_g / count);
                out_data[out_idx + 2] = (unsigned char)(sum_r / count);
            }
        }
    }

    if (!write_bmp24(output_path, &bmp, out_data)) {
        printf("Error: no se pudo escribir %s\n", output_path);
    } else {
        printf("Imagen desenfocada a color creada: %s (kernel=%d)\n", output_path, kernel_size);
    }

    free(out_data);
    free(bmp.preamble);
    free(bmp.data);
}

void desenfoque(const char *input_path, const char *output_path, int kernel_size) {
    desenfoque_color(input_path, output_path, kernel_size);
}
