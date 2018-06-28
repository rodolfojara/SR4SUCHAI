#include <stdio.h>
#include "RobustSR.h"
// medfilt2.c: Filtro
/*
Medfilt2 2-D median filtering
Example:
	I = imread('eight.tiff');
	J = imnoise(I,'salt & pepper',0.02);
	K = medfilt2(J);
	figure, imshow(J), figure, imshow(K)
*/
TipoImagenHR medfilt2(TipoImagenHR Z, TipoRango Rango)
{
	// Pick up window elements
	int k= 0;
	// Original: element window[9];
	element window[n_win*m_win];
	for (int j=m_win - 1; j < m_win ; ++j) {
  	   for (int i=n_win - 1; i < n_win ; ++i)	{
		   window[k++] = image[j * N + i];
		   // Order elements (only half of them)
		   // make sure (n_win * m_win)/2 is odd :-)
	   }
	}
	for (int j=0; j <(n_win*m_win)/2; ++j) {
		// Find position of minimum element
		int min = j;
		for (int l=j + 1; l < n_win*m_win; ++l)	{
			if (window[l] < window[min]) {
				min = l;
			}
		}
		const element temp = window{j};
		window[j] = window[min];
		window[min] = temp;
	}
	// Get result - the middle element
	result[(m - 1)*(N - 2) +n - 1] = window[4];

	return result;
}
