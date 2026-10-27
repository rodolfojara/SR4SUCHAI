# MATLAB reference: ROBUST super-resolution

The source of truth for the rewrite is the original MATLAB implementation from the thesis project archive supplied by Rodolfo Jara.

Keep the MATLAB algorithm and operation order as close as practical. Record differences where C requires explicit choices about array dimensions, boundary handling, memory ownership, or MATLAB indexing.

Core files to preserve and translate:
- `RobustSR.m`
- `GradientBackProject.m`
- `GradientRegulization.m`
- `FastRobustSR.m`
- `FastGradientBackProject.m`
- `MedianAndShift.m`

Supporting files can be added as their dependencies are identified.