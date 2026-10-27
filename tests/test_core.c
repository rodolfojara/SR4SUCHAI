#include "robust_sr.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define CHECK(condition, message) do { \
    if (!(condition)) { \
        fprintf(stderr, "FAIL: %s (line %d)\n", message, __LINE__); \
        return 1; \
    } \
} while (0)

static int near(double a, double b)
{
    return fabs(a - b) < 1e-12;
}

int main(void)
{
    SRImage image = sr_image_create(2, 2);
    SRImage identity = sr_image_create(1, 1);
    SRImage filtered = {0, 0, NULL};
    SRImage regularized = {0, 0, NULL};
    SRImageStack stack = {0, 0, 0, NULL};
    SRShift shift = {0, 0};
    SRShiftList shifts = {1, &shift};
    SRImage z = {0, 0, NULL};
    SRImage a = {0, 0, NULL};
    SRImage z2 = {0, 0, NULL};
    SRImage a2 = {0, 0, NULL};
    SRShift phase_shifts[4] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
    SRShiftList phases = {4, phase_shifts};
    double phase_pixels[4] = {10.0, 20.0, 30.0, 40.0};
    SRImageStack phase_stack = {1, 1, 4, phase_pixels};
    size_t i;

    CHECK(image.data != NULL && identity.data != NULL, "image allocation");
    image.data[0] = 1.0;
    image.data[1] = 2.0;
    image.data[2] = 3.0;
    image.data[3] = 4.0;
    identity.data[0] = 1.0;

    CHECK(sr_image_filter_symmetric(&image, &identity, &filtered),
          "identity filter succeeds");
    for (i = 0; i < 4; ++i) {
        CHECK(near(filtered.data[i], image.data[i]), "identity filter preserves pixels");
    }

    CHECK(near(sr_sign(-4.0), -1.0) && near(sr_sign(0.0), 0.0) &&
          near(sr_sign(4.0), 1.0), "sign matches MATLAB sign for finite values");

    for (i = 0; i < 4; ++i) image.data[i] = 7.0;
    CHECK(sr_gradient_regularization(&image, 1, 0.5, &regularized),
          "regularization succeeds");
    for (i = 0; i < 4; ++i) {
        CHECK(near(regularized.data[i], 0.0),
              "constant image has zero regularization gradient");
    }

    stack.rows = 2;
    stack.cols = 2;
    stack.frames = 1;
    stack.data = image.data;
    CHECK(sr_median_and_shift(&stack, &shifts, 2, 2, 1, &z, &a),
          "single-phase median-and-shift succeeds for scale one");
    for (i = 0; i < 4; ++i) {
        CHECK(near(z.data[i], 7.0), "scale-one median initializer preserves pixels");
        CHECK(near(a.data[i], 1.0), "single-frame normalization is one");
    }

    CHECK(sr_median_and_shift(&phase_stack, &phases, 2, 2, 2, &z2, &a2),
          "two-times median initializer succeeds for four displacement phases");
    CHECK(near(z2.data[0], 10.0) && near(z2.data[1], 20.0) &&
          near(z2.data[2], 30.0) && near(z2.data[3], 40.0),
          "displacement x maps to columns and y maps to rows");
    for (i = 0; i < 4; ++i) {
        CHECK(near(a2.data[i], 1.0), "each unique phase has unit normalization");
    }

    sr_image_destroy(&image);
    sr_image_destroy(&identity);
    sr_image_destroy(&filtered);
    sr_image_destroy(&regularized);
    sr_image_destroy(&z);
    sr_image_destroy(&a);
    sr_image_destroy(&z2);
    sr_image_destroy(&a2);
    puts("All core tests passed.");
    return 0;
}
