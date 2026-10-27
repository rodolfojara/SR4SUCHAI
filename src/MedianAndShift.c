#include "robust_sr.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

static int compare_double(const void *left, const void *right)
{
    double a = *(const double *)left;
    double b = *(const double *)right;
    return (a > b) - (a < b);
}

static double median_of_values(double *values, size_t count)
{
    qsort(values, count, sizeof(double), compare_double);
    if (count % 2 != 0) return values[count / 2];
    return 0.5 * (values[count / 2 - 1] + values[count / 2]);
}

/* MATLAB MedianAndShift.m. Shift coordinates are zero-based offsets. */
int sr_median_and_shift(const SRImageStack *low_resolution,
                        const SRShiftList *shifts,
                        size_t high_rows,
                        size_t high_cols,
                        size_t resolution_factor,
                        SRImage *blurred_initial,
                        SRImage *normalization)
{
    size_t frame, row, col, x, y, r, i;
    unsigned char *known = NULL;
    double *values = NULL;
    SRImage median_filtered = {0, 0, NULL};
    size_t max_frames, value_capacity;

    if (low_resolution == NULL || shifts == NULL || blurred_initial == NULL ||
        normalization == NULL || low_resolution->data == NULL ||
        shifts->items == NULL || low_resolution->frames != shifts->count ||
        resolution_factor == 0 || high_rows == 0 || high_cols == 0) {
        return 0;
    }

    r = resolution_factor;
    max_frames = low_resolution->frames;
    if (r > SIZE_MAX / r) goto fail;
    value_capacity = r * r;
    if (value_capacity < max_frames) value_capacity = max_frames;
    if (value_capacity > SIZE_MAX / sizeof(double)) goto fail;
    values = (double *)malloc(value_capacity * sizeof(double));
    known = (unsigned char *)calloc(r * r, sizeof(unsigned char));
    if (values == NULL || known == NULL) goto fail;

    *blurred_initial = sr_image_create(high_rows, high_cols);
    *normalization = sr_image_create(high_rows, high_cols);
    if (blurred_initial->data == NULL || normalization->data == NULL) goto fail;

    for (i = 0; i < high_rows * high_cols; ++i) normalization->data[i] = 1.0;

    /* Faithful index conversion from MATLAB:
       MATLAB loops x,y = r..2r-1 and uses them as 1-based HR indices.
       Shifts supplied by the C caller are zero-based, so these correspond
       to x,y = r-1..2r-2. Preserve that range rather than reducing phases
       modulo r; the original MATLAB code does not do so. */
    for (x = r - 1; x < 2 * r - 1; ++x) {
        for (y = r - 1; y < 2 * r - 1; ++y) {
            size_t count = 0;
            size_t phase_x = x - (r - 1);
            size_t phase_y = y - (r - 1);
            size_t phase = phase_x * r + phase_y;
            for (frame = 0; frame < shifts->count; ++frame) {
                if (shifts->items[frame].x == x &&
                    shifts->items[frame].y == y) {
                    ++count;
                }
            }
            if (count == 0) continue;
            known[phase] = 1;

            for (row = 0; row < low_resolution->rows; ++row) {
                for (col = 0; col < low_resolution->cols; ++col) {
                    size_t n = 0;
                    for (frame = 0; frame < shifts->count; ++frame) {
                        if (shifts->items[frame].x == x &&
                            shifts->items[frame].y == y) {
                            values[n++] = low_resolution->data[
                                frame * low_resolution->rows * low_resolution->cols +
                                row * low_resolution->cols + col];
                        }
                    }
                    if (y + row * r < high_rows && x + col * r < high_cols) {
                        size_t at = (y + row * r) * high_cols + (x + col * r);
                        blurred_initial->data[at] = median_of_values(values, n);
                        normalization->data[at] = (double)count;
                    }
                }
            }
        }
    }

    /* Fill missing displacement phases with the spatial median filter, as in
       MATLAB medfilt2(Z,[r r]); its out-of-image samples are treated as zero. */
    median_filtered = sr_image_create(high_rows, high_cols);
    if (median_filtered.data == NULL) goto fail;
    for (row = 0; row < high_rows; ++row) {
        for (col = 0; col < high_cols; ++col) {
            size_t n = 0;
            long rr, cc;
            for (rr = (long)row - (long)(r / 2);
                 rr < (long)row - (long)(r / 2) + (long)r; ++rr) {
                for (cc = (long)col - (long)(r / 2);
                     cc < (long)col - (long)(r / 2) + (long)r; ++cc) {
                    if (rr < 0 || cc < 0 || (size_t)rr >= high_rows ||
                        (size_t)cc >= high_cols) {
                        values[n++] = 0.0;
                    } else {
                        values[n++] = blurred_initial->data[
                            (size_t)rr * high_cols + (size_t)cc];
                    }
                }
            }
            median_filtered.data[row * high_cols + col] = median_of_values(values, n);
        }
    }

    for (x = 0; x < r; ++x) {
        for (y = 0; y < r; ++y) {
            size_t phase = x * r + y;
            if (known[phase]) continue;
            for (row = y + r - 1; row < high_rows; row += r) {
                for (col = x + r - 1; col < high_cols; col += r) {
                    blurred_initial->data[row * high_cols + col] =
                        median_filtered.data[row * high_cols + col];
                }
            }
        }
    }

    for (i = 0; i < high_rows * high_cols; ++i) {
        normalization->data[i] = sqrt(normalization->data[i]);
    }

    free(values);
    free(known);
    sr_image_destroy(&median_filtered);
    return 1;

fail:
    free(values);
    free(known);
    sr_image_destroy(&median_filtered);
    if (blurred_initial != NULL) sr_image_destroy(blurred_initial);
    if (normalization != NULL) sr_image_destroy(normalization);
    return 0;
}
