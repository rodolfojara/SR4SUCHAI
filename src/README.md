# New C implementation

This directory is reserved for a clean C implementation written from the MATLAB reference in `reference/matlab/robust/`.

Rules:
- C99 unless a target requirement says otherwise.
- Explicit image dimensions and ownership; no implicit MATLAB-style array operations.
- Small functions with documented inputs, outputs, and error handling.
- No dependency on files in `archive/first-version/`.
- Verify compilation and tests before describing any module as complete.