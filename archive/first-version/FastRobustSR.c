#include <stdio.h>
#include "RobustSR.h"
// FastRobustSR.c: Metodo SR Robust FAST
/*
Inputs:
 LR - A sequence of low resolution images
 D  - The translational motion for each LR frame
 resFactor - The resolution increment factor
 Hpsf - The PSF function (common to all frames and space invariant)
 props - Property structure used to control the algorithm parameters; Definidos en el archivo RobustSR.h

Output:
 The estimated HR image
*/
TipoImagenSR FastRobustSR(TipoImagenes LR, TipoImagenSR D, int resFactor, TipoKernel Hpsf)
{
// Calculo inicial de emborrocimiento
 TipoIniciales ZyA;
 ZyA  = MedainAndShift(LR, D, Rango, RESOLUTIO);

// Inicializar HR con lo obtenido de MediaAndShift
 TipoImagenHR Gback,Greg;
 TipoImagenSR HR = ZyA.Z;
 int iter = 1;

// Iteracion donde se va generando HR
 while (iter < MAXITER)
 {
	Despliegue(iter/MAXITER);

	// Calculo del Gradiente de la parte de energia de la funcion de costo
	Gback = FastGradienteBckProject(HR, Z, A, Hpsf);

	// Calculo del Gradiente de la parte del filtro bilateral de la funcion de costo
	Greg  = GradientRegulization(HR, PVENTANA, ALPHA);

	// Se realiza un paso SD simple
	HR    = HR - BETA * (Gback + LAMBDA * Greg);

	iter++;
 }
	
 return HR;
}
