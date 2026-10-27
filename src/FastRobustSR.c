#include "robust_sr.h"

#include <stdint.h>

/* Translation of reference/matlab/robust/FastRobustSR.m. */
int sr_fast_robust_sr(const SRImageStack *low_resolution,
                      const SRShiftList *shifts,
                      size_t resolution_factor,
                      const SRImage *psf,
                      const SRRobustProps *props,
                      SRImage *estimate)
{
    size_t high_rows, high_cols, iteration, i, pixels;
    SRImage blurred_initial = {0, 0, NULL};
    SRImage normalization = {0, 0, NULL};
    SRImage current = {0, 0, NULL};

    if (low_resolution == NULL || shifts == NULL || psf == NULL ||
        props == NULL || estimate == NULL || low_resolution->data == NULL ||
        resolution_factor == 0 || props->max_iter == 0) {
        return 0;
    }

    if (low_resolution->rows + 1 > SIZE_MAX / resolution_factor ||
        low_resolution->cols + 1 > SIZE_MAX / resolution_factor) {
        return 0;
    }
    high_rows = (low_resolution->rows + 1) * resolution_factor - 1;
    high_cols = (low_resolution->cols + 1) * resolution_factor - 1;

    if (!sr_median_and_shift(low_resolution, shifts, high_rows, high_cols,
                             resolution_factor, &blurred_initial,
                             &normalization)) {
        return 0;
    }

    current = sr_image_clone(&blurred_initial);
    if (current.data == NULL) goto fail;
    pixels = current.rows * current.cols;

    /* MATLAB starts at iter=1 and loops while iter < maxIter. */
    for (iteration = 1; iteration < props->max_iter; ++iteration) {
        SRImage backprojection = {0, 0, NULL};
        SRImage regularization = {0, 0, NULL};

        if (!sr_fast_gradient_back_project(&current, &blurred_initial,
                                           &normalization, psf, &backprojection) ||
            !sr_gradient_regularization(&current,
                                        props->regularization_radius,
                                        props->alpha, &regularization)) {
            sr_image_destroy(&backprojection);
            sr_image_destroy(&regularization);
            goto fail;
        }

        for (i = 0; i < pixels; ++i) {
            current.data[i] -= props->beta *
                (backprojection.data[i] + props->lambda * regularization.data[i]);
        }
        sr_image_destroy(&backprojection);
        sr_image_destroy(&regularization);
    }

    *estimate = current;
    sr_image_destroy(&blurred_initial);
    sr_image_destroy(&normalization);
    return 1;

fail:
    sr_image_destroy(&current);
    sr_image_destroy(&blurred_initial);
    sr_image_destroy(&normalization);
    return 0;
}
