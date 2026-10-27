#include "robust_sr.h"

#include <stdlib.h>

/*
 * Translation of reference/matlab/robust/GradientBackProject.m.
 * Shifts are zero-based C sample offsets corresponding to MATLAB's 1-based
 * Fmot values after subtracting one.
 */
int sr_gradient_back_project(const SRImage *estimate,
                             const SRImageStack *low_resolution,
                             const SRShiftList *shifts,
                             const SRImage *psf,
                             size_t resolution_factor,
                             SRImage *gradient)
{
    SRImage blurred = {0, 0, NULL};
    SRImage flipped_psf = {0, 0, NULL};
    size_t frame, row, col, i;

    if (estimate == NULL || low_resolution == NULL || shifts == NULL ||
        psf == NULL || gradient == NULL || estimate->data == NULL ||
        low_resolution->data == NULL || shifts->items == NULL ||
        psf->data == NULL || resolution_factor == 0 ||
        low_resolution->frames != shifts->count ||
        low_resolution->rows == 0 || low_resolution->cols == 0) {
        return 0;
    }

    if (!sr_image_filter_symmetric(estimate, psf, &blurred)) return 0;
    flipped_psf = sr_image_create(psf->rows, psf->cols);
    if (flipped_psf.data == NULL) {
        sr_image_destroy(&blurred);
        return 0;
    }
    for (row = 0; row < psf->rows; ++row) {
        for (col = 0; col < psf->cols; ++col) {
            flipped_psf.data[row * psf->cols + col] =
                psf->data[(psf->rows - 1 - row) * psf->cols +
                          (psf->cols - 1 - col)];
        }
    }

    *gradient = sr_image_create(estimate->rows, estimate->cols);
    if (gradient->data == NULL) {
        sr_image_destroy(&blurred);
        sr_image_destroy(&flipped_psf);
        return 0;
    }

    for (frame = 0; frame < low_resolution->frames; ++frame) {
        SRImage sparse = sr_image_create(estimate->rows, estimate->cols);
        SRImage backprojected = {0, 0, NULL};
        const double *lr = low_resolution->data +
            frame * low_resolution->rows * low_resolution->cols;

        if (sparse.data == NULL) goto fail;
        for (row = 0; row < low_resolution->rows; ++row) {
            for (col = 0; col < low_resolution->cols; ++col) {
                size_t hr_row = shifts->items[frame].y + row * resolution_factor;
                size_t hr_col = shifts->items[frame].x + col * resolution_factor;
                if (hr_row >= estimate->rows || hr_col >= estimate->cols) {
                    sr_image_destroy(&sparse);
                    goto fail;
                }
                sparse.data[hr_row * estimate->cols + hr_col] =
                    sr_sign(blurred.data[hr_row * blurred.cols + hr_col] -
                            lr[row * low_resolution->cols + col]);
            }
        }

        if (!sr_image_filter_symmetric(&sparse, &flipped_psf, &backprojected)) {
            sr_image_destroy(&sparse);
            goto fail;
        }
        for (i = 0; i < estimate->rows * estimate->cols; ++i) {
            gradient->data[i] += backprojected.data[i];
        }
        sr_image_destroy(&sparse);
        sr_image_destroy(&backprojected);
    }

    sr_image_destroy(&blurred);
    sr_image_destroy(&flipped_psf);
    return 1;

fail:
    sr_image_destroy(&blurred);
    sr_image_destroy(&flipped_psf);
    sr_image_destroy(gradient);
    return 0;
}
