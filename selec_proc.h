#ifndef SELEC_PROC_H
#define SELEC_PROC_H

#include <stddef.h>

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

/* Helper functions */
int read_bmp24(const char *path, Bmp24 *bmp);
int write_bmp24(const char *path, const Bmp24 *bmp, const unsigned char *data);
void make_output_name(const char *prefix, char *out, size_t out_size);

/* Public functions */
void inv_img(const char *output_prefix, const char *input_path);
void desenfoque(const char *input_path, const char *output_path, int kernel_size);

#endif
