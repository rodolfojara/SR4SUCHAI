#include <stdio.h>
#include "RobustSR.h"
// GradientBackProjection.c: Back projection del signo del error residual
/*
Inputs:
Xn - The current estimate of the deblurred HR image
LR - A sequence of low resolution image
Fmot - The tranlational motion for each LR frame
Hpsf - The PSF function (common to all frames and space invariant)
Dres - The resolution increment factor

Output:
The backprojection of the sign of the residual error
*/

TipoImagenHR imfilter(TipoImagenHR Xn, TipoKernel Hpsf, char *tipo);

TipoImagenSR sign(TipoImagenSR X);

TipoKernel GradientBackProject(TipoImagen Xn, TipoImagenLR LR, TipoImagenLR Fmot, TipoKernel Hpsf, int Dres)
{
  
  TipoImagenLR HRsd, Zn, G;
  // The shift and blur are comutative, so to improve runtime, we first filter the HR image
  Zn = imfilter(Xn, Hpsf, 'Symmetric'); // Blur the current HR estimate	

  // Allocate shifted and decimated HR image
  HRsd = zeros(size(LR)); // matriz de ceros

  int kMax = size(LR,3); // Numero de imagenes

  for (int k=1; k<=kMax; k++ )
  {
	  HRsd(:,:,k) = Zn(Fmot(k,2):RESOLUTIO(size(LR,1)-1)*RESOLUTIO+Fmot(k,2), Fmot(k,1):RESOLUTIO:(size(LR,2)-1)*RESOLUTIO+Fmot(k,1));
  }
  // Compute the sign between HRsd-LR
  Gsign = sign(HRsd-LR); // Deblur the normalized sign of Xdiff

  HRsd = zeros( [size(Xn) size(LR,3)] );

  // Back project Gsign to HR space
  for (k=1; k<=kMax ; k++)
  {
	  HRsd(Fmot(k,2):RESOLUTIO:(size(LR,1)-1)*RESOLUTIO+Fmot(k,2),Fmot(k,1):RESOLUTIO:(size(LR,2)-1)*RESOLUTIO+Fmot(k,1),k) = Gsign(:,:,k);
  }
  // Unblur the backprojected image
  G = imfilter(HRsd, flipud(fliplr(Hpsf)), 'Symmetric');  // Unblur the backprojected image

  // compute the sum over k of the backprojected gradient
  G = sum(G, 3);

  return G;
}
