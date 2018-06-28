#include <stdio.h>
#include "RobustSR.h"
// fliplr.c: Alterna las posiciones de sus filas
/*
Flipud: flip matrix in up/down direction.
   flipud(X) retorns X with columns preserved and rows in the up/down direction.
   X = 1 4		flipud(X) = 3 6
	   2 5					2 5
	   3 6					1 4
*/
TipoKernel flipud(TipoKernel X)
{
	TipoKernel Y;
    int MaxFilas    = length(X);
	int MaxColumnas = largocolumnas(X);

	for (i=0; i<MaxFilas ; i++)
	{
		for (j=MaxColumnas-1; j>=0; j--)
		{
			Y[i][j-MaxColumnas] = X[i][j];
		}
	}	
	return Y;
}
