#include "robust_sr.h"

#include <stddef.h>

/* Translation of reference/matlab/robust/RobustSR.m. */
int sr_robust_sr(const SRImageStack *low_resolution,
                 const SRShiftList *shifts,
                 const SRImage *initial_estimate,
                 size_t resolution_factor,
                 const SRImage *psf,
                 const SRRobustProps *props,
                 SRImage *estimate)
{
    size_t iteration, i, pixels;
    SRImage current = {0, 0, NULL};

    if (low_resolution == NULL || shifts == NULL || initial_estimate == NULL ||
        psf == NULL || props == NULL || estimate == NULL ||
        initial_estimate->data == NULL || props->max_iter == 0) {
        return 0;
    }

    current = sr_image_clone(initial_estimate);
    if (current.data == NULL) return 0;
    pixels = current.rows * current.cols;

    /* MATLAB starts at iter=1 and loops while iter < maxIter. */
    for (iteration = 1; iteration < props->max_iter; ++iteration) {
        SRImage backprojection = {0, 0, NULL};
        SRImage regularization = {0, 0, NULL};

        if (!sr_gradient_back_project(&current, low_resolution, shifts, psf,
                                      resolution_factor, &backprojection) ||
            !sr_gradient_regularization(&current,
                                        props->regularization_radius,
                                        props->alpha, &regularization)) {
            sr_image_destroy(&backprojection);
            sr_image_destroy(&regularization);
            sr_image_destroy(&current);
            return 0;
        }

        for (i = 0; i < pixels; ++i) {
            current.data[i] -= props->beta *
                (backprojection.data[i] + props->lambda * regularization.data[i]);
        }
        sr_image_destroy(&backprojection);
        sr_image_destroy(&regularization);
    }

    *estimate = current;
    return 1;
}
