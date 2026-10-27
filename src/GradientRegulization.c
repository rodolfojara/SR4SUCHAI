#include "robust_sr.h"

#include <limits.h>
#include <math.h>
#include <stdlib.h>

/*
 * Translation of reference/matlab/robust/GradientRegulization.m.
 *
 * The MATLAB implementation pads the image symmetrically before making each
 * shifted copy. Its shifted sign image is then padded with zeros and shifted
 * in the opposite direction. The loops below express those same operations
 * directly, avoiding temporary padded matrices.
 */
int sr_gradient_regularization(const SRImage *image,
                              size_t radius,
                              double alpha,
                              SRImage *gradient)
{
    size_t row, col;
    long l, m;
    double weight;

    if (image == NULL || gradient == NULL || image->data == NULL ||
        image->rows == 0 || image->cols == 0 ||
        radius > (size_t)LONG_MAX) {
        return 0;
    }

    *gradient = sr_image_create(image->rows, image->cols);
    if (gradient->data == NULL) return 0;

    for (l = -(long)radius; l <= (long)radius; ++l) {
        for (m = -(long)radius; m <= (long)radius; ++m) {
            weight = pow(alpha, (double)(labs(l) + labs(m)));

            for (row = 0; row < image->rows; ++row) {
                for (col = 0; col < image->cols; ++col) {
                    size_t index = row * image->cols + col;
                    double shifted = sr_image_get_symmetric(
                        image, (long)row - l, (long)col - m);
                    double sign_here = sr_sign(image->data[index] - shifted);

                    /* Xsignpad is zero outside the unpadded image. */
                    long other_row = (long)row + l;
                    long other_col = (long)col + m;
                    double sign_shifted = 0.0;
                    if (other_row >= 0 && other_col >= 0 &&
                        (size_t)other_row < image->rows &&
                        (size_t)other_col < image->cols) {
                        double other = sr_image_get_symmetric(
                            image, other_row - l, other_col - m);
                        sign_shifted = sr_sign(
                            image->data[(size_t)other_row * image->cols +
                                        (size_t)other_col] - other);
                    }
                    gradient->data[index] += weight *
                        (sign_here - sign_shifted);
                }
            }
        }
    }
    return 1;
}
