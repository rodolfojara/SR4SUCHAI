#include <stdio.h>
#include <math.h>
#include "RobustSR.h"
// GradientRegulization.c: Back projection del signo del error residual
/*
Inputs:
Xn - The current estimate of the deblurred HR image
P  - The spatial window size (radius)
alpha - The exponential decay coefficient

Output:
The backprojection of the sign of the residual error
*/

TipoKernel GradientRegulization(TipoImagen Xn, int PVENTANA, int ALPHA)
{
	tamano =sizeof(Xn);
	double G[tamano][tamano];

	Xpad = padarray(Xn, [PVENTANA PVENTANA], 'symmetric'); // crea una version inflada de Xn asi la operacion de desplazamiento es mas simple

	for (l=-PVENTANA;l<=PVENTANA ; l++)
	{
		for (m=-PVENTANA;m<=PVENTANA ; m++)
		{
			Xshift = Xpad(1+PVENTANA-1:end-PVENTANA-1, 1+PVENTANA-m:end-PVENTANA-m); // Shift HR by l and m

			Xsign = sign(Xn - Xshift); // Subtract from HR image and compute sign

			Xsignpad = padarray(Xsign, [PVENTANA PVENTANA], 0); // Shift Xsign back -l and -m

			Xshift   = Xsignpad(1+PVENTANA+l:end-PVENTANA+l, 1+PVENTANA+m:end-PVENTANA+m);

		//	G = G + alpha^(abs(l)+abs(m))*(Xsign-Xshift);		MATLAB
			G = G + pow(ALPHA, abs(l)+abs(m) )*(Xsign - Xshift);
		}
	}

	return G;
}
