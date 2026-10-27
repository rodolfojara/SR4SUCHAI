# SR4SUCHAI

## Super-resolution algorithms for resource-constrained nanosatellite applications

This repository contains a work-in-progress C implementation/port of super-resolution algorithms evaluated in the context of the **SUCHAI / CubeSat** project.

The original work explored how super-resolution could improve image resolution while considering the constraints of a nanosatellite environment.

## Objective

Evaluate and implement super-resolution approaches suitable for a resource-constrained space platform, considering factors such as:

- image quality;
- resolution increase;
- memory requirements;
- computational/runtime requirements.

The work compared alternative algorithms and selected **ROBUST** as the most promising candidate for the intended nanosatellite use case.

## Algorithm evaluation and selection

The thesis, *Implementación y evaluación de algoritmos de súper resolución para imágenes tomadas con nano satélites* (Universidad de Chile, July 2018), evaluated three spatial-domain approaches:

- **IBP** — Iterative Back Projection;
- **POCS** — Projection Onto Convex Sets;
- **ROBUST** — the method by Farsiu, Robinson, Elad and Milanfar.

The defense presentation describes the selection in the context of CubeSat constraints, rather than image quality alone. Its suggested-use-case table reports approximate resolution values of **2 for IBP, 3 for POCS and 4 for ROBUST**, with low-computing-capacity and response-time constraints on the order of seconds; ROBUST is assigned the in-orbit scenario (defense presentation, slide 26).

The evaluation also considered PSNR-based image-quality comparisons, runtime, memory use and visual inspection (slides 19–24). The available presentation text does not provide the underlying numerical results for every comparison, so this README does not claim a specific PSNR margin or a quantified memory/runtime advantage.

The next engineering step identified in the defense was to port the remaining ROBUST functions to C for potential use in **SUCHAI 2 and later missions**. The presentation calls out two particularly challenging areas: calculating filter and cost-function gradients, and combining their weighted contributions. Image-matrix handling and JPEG input/output were also identified as implementation needs (slides 27 and 29).

## Algorithms and implementation

The repository contains C source files related to:

- robust super-resolution (`RobustSR.c`);
- fast robust super-resolution (`FastRobustSR.c`);
- gradient back-projection (`GradientBackProject.c`);
- regularization (`GradientRegulization.c`);
- median filtering and image alignment (`Median.c`, `Medfilt2.c`, `MedianAndShift.c`);
- image operations such as flipping and filtering.

The implementation follows an iterative estimation approach in which a high-resolution estimate is updated using gradient/back-projection and regularization terms.

A simplified view of the processing flow is:

```text
Low-resolution image sequence
            |
            v
   Alignment / initial HR
            |
            v
 Gradient Back-Projection
            +
       Regularization
            |
            v
     Iterative HR update
            |
            v
    High-resolution estimate
```

## Current status

**Work in progress / incomplete port.**

The repository preserves the state of the C implementation developed during the project. Some components still contain incomplete or MATLAB-like constructs and should not be interpreted as production-ready CubeSat flight software.

The repository is therefore presented as an engineering and research artifact rather than as a finished flight implementation.

## Relation to the research work

This repository is associated with the thesis *Implementación y evaluación de algoritmos de súper resolución para imágenes tomadas con nano satélites* (2018). The work evaluated algorithms and considered how their computational demands and achievable resolution related to the constraints of nanosatellite imaging.

The choice of ROBUST is documented here as the thesis project's engineering selection. It should not be read as a claim that the current C files form a completed, flight-qualified implementation.

## Why this repository is relevant

This project demonstrates work at the intersection of:

- Space Systems Engineering
- Image Processing
- Super-Resolution
- C programming
- Algorithm evaluation
- Resource-constrained computing
- Technical trade-off and selection

## Future work

Possible next steps include completing the C port, validating it against reference results, benchmarking memory/runtime requirements, documenting the test methodology, and adding reproducible examples. The defense also identified evaluating convolutional neural networks and studying limits of super-resolution as possible future research directions.
