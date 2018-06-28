#include <stdio.h>
#include "RobustSR.h"
// imfilter.c: convolucion entre imagen y filtro
/*
Inputs:
Xn - The current estimate of the deblurred HR image
Hpsh - The PSF function (common to all frames and space invariant)
BoundaryOptions - performs multidimensional filtering

- Boundary Options
	X			Input array values outside the bounds of the array
				are implicitly assumed to have the value X. When no
				boundary option is specified, imfilter uses X = 0.

	'symmetric'	Input array values outside the bounds of the array
				are computed by mirror-reflecting the array across
				the array border.

	'replicate'	Input array values outside the bounds of the array
				are assumed to equal the nearest array border
				value.

	'circular'	Input array values outside the bounds of the array
				are computed by implicitly assuming the input array
				is periodic.
Output:
 N-D filtering of multidimensional images
*/
TipoArreglo imfilter(TipoImagenSR Xn, TipoKernel Hpsf, char* BoundaryOptions)
{
/*
Let's first create a small testimage from the built-in peppers image
original = im2double(imread('peppers.png'));
original = original(1:5,1:8,1);

filter = 1/9 * [-1 -1 -1; -1 17 -1; -1 -1 -1];
s = size(original);
r = zeros(s);

original =padarray(original,[1,1});
for i=2:s(1)	// :s(1) -1
for j=2:s(2)	// :s(2) -1
   temp = original(i-1:i+1,j-1:j+1) .* filter;
   r(i-1,j-1) = sum(temp(:));		// r(i,j) = ...
   end
  end


*/
	return 0;
}
