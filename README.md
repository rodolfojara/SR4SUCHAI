# SR4SUCHAI

## Super-resolution algorithms for resource-constrained nanosatellite applications

This repository contains a work-in-progress C implementation/port of super-resolution algorithms evaluated in the context of the **SUCHAI / CubeSat** project.

The original work explored how super-resolution could improve image resolution while considering the constraints of a nanosatellite environment.

## Objective

Evaluate and implement super-resolution approaches suitable for a resource-constrained space platform, considering factors such as:

- image quality;
- resolution increase factor;
- memory requirements;
- computational/runtime requirements.

The algorithmic work included the comparison of alternatives and the selection of an approach with lower resource requirements for the intended CubeSat context.

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

The project is related to the thesis work on the **implementation and evaluation of super-resolution algorithms for images acquired by nanosatellites**. The engineering problem was not only algorithmic: the alternatives had to be considered in the context of the limited computational resources available on a CubeSat.

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

Possible next steps include completing the C port, validating the implementation against reference results, benchmarking memory/runtime requirements, and documenting the selected algorithm and test methodology in greater detail.
