#include <stdio.h>
#include "RobustSR.h"
// FastGradientBackProject.c: Back Projection del signo del error residual
/*
Inputs:
Xn - The current estimate of the deblurred HR image
Z  - The original blurred estimate of the HR image
A - Normalization factor for each pixel
Hpsf - The PSF function (common to all frames and space invariant)

Output:
The backprojection of the sign of the residual error
*/

TipoKernel FastGradientBackProject(TipoImagen Xn, TipoImagen Z, TipoImagen A, TipoKernel Hpsf)
{
	TipoKernel Zn,Gsign,G;

	Zn = imfilter(Xn, Hpsf, 'Symmetric');	// Blur the current HR estimate	

	Gsign = sign(A * (Zn - Z));				// Deblur the normalized sign of Xdiff

	G = imfilter(A * Gsign, flipud(fliplr(Hpsf)), 'Symmetric');  // Unblur the backprojected image

	return G;
}
