#ifndef IMG_OPS_H
#define IMG_OPS_H

int bmp_black_white(const char *input_path, const char *output_path);
int bmp_grayscale(const char *input_path, const char *output_path);
int bmp_flip_horizontal(const char *input_path, const char *output_path);
int bmp_flip_vertical(const char *input_path, const char *output_path);
int bmp_blur(const char *input_path, const char *output_path, int kernel_size);

#endif
