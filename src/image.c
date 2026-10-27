#include "robust_sr.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

SRImage sr_image_create(size_t rows, size_t cols)
{
    SRImage image = { rows, cols, NULL };
    if (rows == 0 || cols == 0 || rows > SIZE_MAX / cols ||
        rows * cols > SIZE_MAX / sizeof(double)) {
        image.rows = image.cols = 0;
        return image;
    }
    image.data = (double *)calloc(rows * cols, sizeof(double));
    if (image.data == NULL) {
        image.rows = image.cols = 0;
    }
    return image;
}

void sr_image_destroy(SRImage *image)
{
    if (image != NULL) {
        free(image->data);
        image->data = NULL;
        image->rows = image->cols = 0;
    }
}

SRImage sr_image_clone(const SRImage *image)
{
    SRImage copy;
    if (image == NULL || image->data == NULL) {
        SRImage empty = { 0, 0, NULL };
        return empty;
    }
    copy = sr_image_create(image->rows, image->cols);
    if (copy.data != NULL) {
        memcpy(copy.data, image->data,
               image->rows * image->cols * sizeof(double));
    }
    return copy;
}

/* MATLAB imfilter(...,'symmetric') repeats the edge pixel on reflection.
   This helper extends that convention to any signed index. */
static size_t symmetric_index(long index, size_t length)
{
    long n = (long)length;
    if (length <= 1) return 0;
    while (index < 0 || index >= n) {
        if (index < 0) index = -index - 1;
        if (index >= n) index = 2 * n - index - 1;
    }
    return (size_t)index;
}

double sr_image_get_symmetric(const SRImage *image, long row, long col)
{
    if (image == NULL || image->data == NULL ||
        image->rows == 0 || image->cols == 0) {
        return 0.0;
    }
    row = (long)symmetric_index(row, image->rows);
    col = (long)symmetric_index(col, image->cols);
    return image->data[(size_t)row * image->cols + (size_t)col];
}

double sr_sign(double value)
{
    return (value > 0.0) - (value < 0.0);
}


/* MATLAB imfilter(..., 'symmetric') uses correlation for its default kernel
   orientation. Callers reverse the kernel explicitly when required. */
int sr_image_filter_symmetric(const SRImage *image,
                              const SRImage *kernel,
                              SRImage *filtered)
{
    size_t row, col, kr, kc;
    long row_anchor, col_anchor;

    if (image == NULL || kernel == NULL || filtered == NULL ||
        image->data == NULL || kernel->data == NULL ||
        image->rows == 0 || image->cols == 0 ||
        kernel->rows == 0 || kernel->cols == 0) {
        return 0;
    }

    *filtered = sr_image_create(image->rows, image->cols);
    if (filtered->data == NULL) return 0;

    row_anchor = (long)((kernel->rows - 1) / 2);
    col_anchor = (long)((kernel->cols - 1) / 2);
    for (row = 0; row < image->rows; ++row) {
        for (col = 0; col < image->cols; ++col) {
            double sum = 0.0;
            for (kr = 0; kr < kernel->rows; ++kr) {
                for (kc = 0; kc < kernel->cols; ++kc) {
                    long source_row = (long)row + (long)kr - row_anchor;
                    long source_col = (long)col + (long)kc - col_anchor;
                    sum += sr_image_get_symmetric(image, source_row, source_col) *
                           kernel->data[kr * kernel->cols + kc];
                }
            }
            filtered->data[row * image->cols + col] = sum;
        }
    }
    return 1;
}
