#include "robust_sr.h"

#include <stddef.h>

/* Translation of reference/matlab/robust/FastGradientBackProject.m. */
int sr_fast_gradient_back_project(const SRImage *estimate,
                                  const SRImage *blurred_initial,
                                  const SRImage *normalization,
                                  const SRImage *psf,
                                  SRImage *gradient)
{
    SRImage blurred = {0, 0, NULL};
    SRImage weighted_sign = {0, 0, NULL};
    SRImage flipped_psf = {0, 0, NULL};
    size_t row, col;

    if (estimate == NULL || blurred_initial == NULL || normalization == NULL ||
        psf == NULL || gradient == NULL || estimate->data == NULL ||
        blurred_initial->data == NULL || normalization->data == NULL ||
        psf->data == NULL ||
        estimate->rows != blurred_initial->rows ||
        estimate->cols != blurred_initial->cols ||
        estimate->rows != normalization->rows ||
        estimate->cols != normalization->cols) {
        return 0;
    }

    if (!sr_image_filter_symmetric(estimate, psf, &blurred)) return 0;
    weighted_sign = sr_image_create(estimate->rows, estimate->cols);
    flipped_psf = sr_image_create(psf->rows, psf->cols);
    if (weighted_sign.data == NULL || flipped_psf.data == NULL) goto fail;

    for (row = 0; row < estimate->rows; ++row) {
        for (col = 0; col < estimate->cols; ++col) {
            size_t i = row * estimate->cols + col;
            double residual = blurred.data[i] - blurred_initial->data[i];
            weighted_sign.data[i] = normalization->data[i] * sr_sign(residual);
        }
    }
    for (row = 0; row < psf->rows; ++row) {
        for (col = 0; col < psf->cols; ++col) {
            flipped_psf.data[row * psf->cols + col] =
                psf->data[(psf->rows - 1 - row) * psf->cols +
                          (psf->cols - 1 - col)];
        }
    }

    if (!sr_image_filter_symmetric(&weighted_sign, &flipped_psf, gradient)) {
        goto fail;
    }

    sr_image_destroy(&blurred);
    sr_image_destroy(&weighted_sign);
    sr_image_destroy(&flipped_psf);
    return 1;

fail:
    sr_image_destroy(&blurred);
    sr_image_destroy(&weighted_sign);
    sr_image_destroy(&flipped_psf);
    return 0;
}
