# New C implementation

This directory contains the clean C rewrite being developed from the MATLAB reference in `reference/matlab/robust/`.

Initial modules now present:
- `image.c`: dense image allocation, symmetric indexing and filtering.
- `GradientRegulization.c`: regularization gradient.
- `GradientBackProject.c`: residual sign and back-projection for the original ROBUST method.
- `FastGradientBackProject.c`: back-projection for the fast variant.
- `MedianAndShift.c`: median-and-shift initialization.
- `RobustSR.c`: iterative ROBUST solver.
- `FastRobustSR.c`: iterative fast ROBUST solver.
- `robust_sr.h`: explicit C data structures and function interfaces.

The modules compile together with GCC in C99 mode with `-Wall -Wextra -Wpedantic -Werror` enabled. Basic tests cover image allocation/filtering, sign behavior, a constant-image regularization case, and the row/column mapping of median-and-shift displacement phases. These are smoke tests only; numerical equivalence to MATLAB has not yet been established.

Rules:
- Explicit image dimensions and ownership; no implicit MATLAB-style array operations.
- Keep the old attempt isolated in `archive/first-version/`.
- Add numerical tests and verify boundary/indexing behavior before claiming parity.
- Do not describe the port as flight-ready.
