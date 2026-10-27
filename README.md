# SR4SUCHAI

## Super-resolution algorithms for resource-constrained nanosatellite applications

This repository documents work to evaluate super-resolution algorithms for nanosatellite imagery and to explore a C implementation suitable for a resource-constrained platform.

The thesis *Implementación y evaluación de algoritmos de súper resolución para imágenes tomadas con nano satélites* (Universidad de Chile, July 2018) evaluated IBP, POCS and ROBUST. The defense presentation selected ROBUST as the most promising candidate for the described in-orbit use case, considering image-resolution gain and CubeSat computing constraints.

## Current rewrite plan

The current rewrite branch starts from the original MATLAB algorithm sources and builds a new C implementation from scratch.

- `reference/matlab/robust/`: authoritative MATLAB reference sources.
- `src/`: clean C implementation being developed from those references.
- `archive/first-version/`: earlier C translation attempts, preserved only as historical reference.

The archived C files are not part of the new implementation. We will compare the old and new attempts only after the translation is complete, rather than spending time repairing the old attempt file by file.

## Engineering principles

- Preserve the original algorithm's intent and operation order where possible.
- Make dimensions, indexing, image boundaries and memory ownership explicit in C.
- Record any necessary difference from MATLAB rather than silently changing behavior.
- Compile and test modules before treating them as complete.
- Do not describe this code as flight-ready or validated for an actual spacecraft.

## Algorithm outline

ROBUST iteratively updates a high-resolution estimate using two terms: a back-projected sign of the residual between simulated and observed low-resolution images, and a regularization gradient. The update follows steepest descent using the parameters defined by the MATLAB implementation.

The thesis presentation identified gradient calculations and weighted gradient contributions as challenging parts of the C port. It also identified explicit image-matrix handling and JPEG input/output as engineering requirements.

## Historical evaluation

A concise visual summary of the thesis evidence, including sample runtime and memory measurements and the reason ROBUST was selected, is available in [`docs/robust-selection-results.md`](docs/robust-selection-results.md). The page includes purpose-made SVG figures styled for this repository; the figures summarize the thesis presentation rather than claim new experimental results.

The defense presentation reports approximate resolution values of 2 for IBP, 3 for POCS and 4 for ROBUST in its suggested-use-case table, and associates ROBUST with in-orbit processing under the stated low-compute / seconds-scale response constraints. The presentation also describes PSNR, runtime, memory and visual comparisons. This README does not claim unreported numerical margins.

## Status

**Initial C port — smoke-tested, not yet numerically validated.** The legacy C sources have been moved to `archive/first-version/`. The clean C rewrite includes image storage/filtering, the regularization gradient, both back-projection gradients, the median-and-shift initializer, and both solver loops. GitHub Actions compiles the modules with strict warnings and runs small tests for core operations. These tests do not establish full numerical equivalence to MATLAB; treat this as a documented prototype, not flight-ready software.
