#include <stdio.h>
#include "RobustSR.h"
// flipud.c: Alterna las posiciones de sus columnas
/*
Fliplr: flip matrix in left/right direction.
   flipud(X) retorns X with row preserved and columns flipped.
   X = 1 2 3		flipud(X) = 3 2 1
	   4 5 6					6 5 4
*/
TipoKernel flipud(TipoKernel X)
{
	TipoKernel Y;
    int MaxFilas    = length(X);
	int MaxColumnas = largocolumnas(X);

	for (i=MaxFilas-1; i>=0 ; i++)
	{
		for (j=0; j<MaxColumnas; j--)
		{
			Y[i-MaxFilas][j] = X[i][j];
		}
	}	
	return Y;
}
