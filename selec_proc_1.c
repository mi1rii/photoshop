#include "selec_proc_1.h"

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

void inv_img_color(const char *output_prefix, const char *input_path) {
    Bmp24 bmp;
    unsigned char *out_data;
    char output_path[512];
    int y;

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

    out_data = (unsigned char *)malloc((size_t)bmp.data_size);
    if (out_data == NULL) {
        free(bmp.preamble);
        free(bmp.data);
        printf("Error: no hay memoria para inv_img_color.\n");
        return;
    }

    memcpy(out_data, bmp.data, (size_t)bmp.data_size);

    for (y = 0; y < bmp.height; y++) {
        int src_row = y * bmp.row_size;
        int dst_row = (bmp.height - 1 - y) * bmp.row_size;
        memcpy(out_data + dst_row, bmp.data + src_row, (size_t)bmp.row_size);
    }

    if (!write_bmp24(output_path, &bmp, out_data)) {
        printf("Error: no se pudo escribir %s\n", output_path);
    } else {
        printf("Imagen invertida en color creada: %s\n", output_path);
    }

    free(out_data);
    free(bmp.preamble);
    free(bmp.data);
}

void inv_img_color_lado(const char *output_prefix, const char *input_path) {
    Bmp24 bmp;
    unsigned char *out_data;
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

    out_data = (unsigned char *)malloc((size_t)bmp.data_size);
    if (out_data == NULL) {
        free(bmp.preamble);
        free(bmp.data);
        printf("Error: no hay memoria para inv_img_color_lado.\n");
        return;
    }

    memcpy(out_data, bmp.data, (size_t)bmp.data_size);

    for (y = 0; y < bmp.height; y++) {
        for (x = 0; x < bmp.width; x++) {
            int src_idx = y * bmp.row_size + x * 3;
            int dst_idx = y * bmp.row_size + (bmp.width - 1 - x) * 3;

            out_data[dst_idx] = bmp.data[src_idx];
            out_data[dst_idx + 1] = bmp.data[src_idx + 1];
            out_data[dst_idx + 2] = bmp.data[src_idx + 2];
        }
    }

    if (!write_bmp24(output_path, &bmp, out_data)) {
        printf("Error: no se pudo escribir %s\n", output_path);
    } else {
        printf("Imagen invertida de lado a color creada: %s\n", output_path);
    }

    free(out_data);
    free(bmp.preamble);
    free(bmp.data);
}
