#ifndef SELEC_PROC_H
#define SELEC_PROC_H

void inv_img(const char *output_prefix, const char *input_path);
void inv_img_lado(const char *output_prefix, const char *input_path);
void desenfoque_bn(const char *input_path, const char *output_path, int kernel_size);
void desenfoque_color(const char *input_path, const char *output_path, int kernel_size);
void desenfoque(const char *input_path, const char *output_path, int kernel_size);

#endif
