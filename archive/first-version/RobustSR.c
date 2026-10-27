#include <stdio.h>
#include "RobustSR.h"
// RobustSR.c: Metodo SR Robust
/*
Inputs:
LR - A sequence of low resolution images
D  - The translational motion for each LR frame
HR - An initial guess for the HR image
resFactor - The resolution increment factor
Hpsf - The PSF function (common to all frames and space invariant)
props - Property structure used to control the algorithm parameters

Output:
The estimated HR image
*/
TipoImagenSR RobustSR(TipoImagenes LR, TipoImagenSR D, TipoImagenSR HR, int resFactor, TipoKernel Hpsf, TipoPropiedades props)
{
 int iter = 1;

// Iteracion donde se va generando HR
 while (iter < MAXITER)
 {
	Despliegue(iter/MAXITER);

	// Calculo del Gradiente de la parte de energia de la funcion de costo
	Gback = GradienteBackProject(HR, Z, A, Hpsf, RESOLUTIO);

	// Calculo del Gradiente de la parte del filtro bilateral de la funcion de costo
	Greg  = GradientRegulization(HR, props.P, ALPHA);

	// Se realiza un paso SD simple
	HR    = HR - BETA * (Gback + LAMBDA * Greg);

	iter++;
 }
	
 return HR;
}
