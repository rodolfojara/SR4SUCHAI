#ifndef SR4SUCHAI_ROBUST_SR_H
#define SR4SUCHAI_ROBUST_SR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dense, row-major, single-channel image using double precision. */
typedef struct {
    size_t rows;
    size_t cols;
    double *data;
} SRImage;

/* A stack of low-resolution frames stored as contiguous image planes. */
typedef struct {
    size_t rows;
    size_t cols;
    size_t frames;
    double *data;
} SRImageStack;

/* Frame displacement in zero-based C coordinates. */
typedef struct {
    size_t x;
    size_t y;
} SRShift;

typedef struct {
    size_t count;
    SRShift *items;
} SRShiftList;

typedef struct {
    size_t max_iter;
    size_t regularization_radius;
    double alpha;
    double beta;
    double lambda;
} SRRobustProps;

SRImage sr_image_create(size_t rows, size_t cols);
void sr_image_destroy(SRImage *image);
SRImage sr_image_clone(const SRImage *image);
double sr_image_get_symmetric(const SRImage *image, long row, long col);
int sr_image_filter_symmetric(const SRImage *image, const SRImage *kernel, SRImage *filtered);
double sr_sign(double value);

/* MATLAB GradientRegulization.m, translated to explicit C loops. */
int sr_gradient_regularization(const SRImage *image,
                               size_t radius,
                               double alpha,
                               SRImage *gradient);

/* MATLAB GradientBackProject.m. Shifts are zero-based sample offsets. */
int sr_gradient_back_project(const SRImage *estimate,
                             const SRImageStack *low_resolution,
                             const SRShiftList *shifts,
                             const SRImage *psf,
                             size_t resolution_factor,
                             SRImage *gradient);

/* MATLAB RobustSR.m. */
int sr_robust_sr(const SRImageStack *low_resolution,
                 const SRShiftList *shifts,
                 const SRImage *initial_estimate,
                 size_t resolution_factor,
                 const SRImage *psf,
                 const SRRobustProps *props,
                 SRImage *estimate);

/* MATLAB FastGradientBackProject.m. */
int sr_fast_gradient_back_project(const SRImage *estimate,
                                  const SRImage *blurred_initial,
                                  const SRImage *normalization,
                                  const SRImage *psf,
                                  SRImage *gradient);

#ifdef __cplusplus
}
#endif
#endif
