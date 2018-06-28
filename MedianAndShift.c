#include <stdio.h>
#include "RobustSR.h"
// MedianAndShift.c: Calculo de mediana y factor de normalizacion por pixel
/*
typedef struct MY_TYPE {
	boolean flag;
	short int value;
	double stuff;
	} MY_TYPE;

	void function(void) {
		MY_TYPE a;
		...
		a = { true, 15, 0.123 };
		}
float Z[HRsize[1]][HRsize[2]] = {
	 { 0, 0, 0},
	 { 0, 0, 0},
	 { 0, 0, 0}
	 };
*/
/*
Inputs:
LR  - The sequence of low resolution image
D   - The displacement vector for each frame
HRsize - The size of the HR image
Dres - The resolution scale factor

Outputs:
Z - Mediana de los LRs
A - Factor de Normalizacion de cada pixel
*/
TipoIniciales MedianandShift(TipoImagenes LR, TipoImagenSR D, TipoRango HRsize, int Dres)
{
 int largo = HRsize[0];
 int ancho = HRsize[1];
 double Z[largo][ancho]; // Inicializar a CERO
 double A[largo][ancho]; // Inicializar a UNO

 double S[RESOLUTIO][RESOLUTIO];   // Inicializar a CERO

 TipoIniciales ZyA;
 
 int x,y,i;
 TipoImagen I;
 for (x=RESOLUTIO; x<=2*RESOLUTIO-1 ; x++)
 {
	 for (y=RESOLUTIO; y<=2*RESOLUTIO-1 ; y++)
	 {
		 I = D(:,1)==x & D(:,2)==y; // Comparacion de fila de matriz D contra enteros x e y y luego operados logicamente
		 len = length(find(I=true)); // busqueda cuando I sea true

		 if (len > 0)
		 {
			 S[x-RESOLUTIO+1][y-RESOLUTIO+1] = 1;

			 Z[y:RESOLUTIO:size(Z,1),x:RESOLUTIO:size(Z,2)]=median(LR(:,:,I),3); // Poner mediana en Z
			 A[y:RESOLUTIO:size(Z,1),x:RESOLUTIO:size(Z,2)]=len; // Poner largo en fila de A
		 }
	 }
 }

 [X,Y] = find(S==0); // Encontrar under-determined shift

 if (noesvacio(X))
 {
	 Zmed = medfilt2(Z, [RESOLUTIO RESOLUTIO]);

	 for (i=1; i<=largo(X) ; i++)
	 {
		 x = X[i] + RESOLUTIO - 1;
		 y = Y[i] + RESOLUTIO - 1;

		 ZyA.Z[y:RESOLUTIO:size(Z,1),x:RESOLUTIO:size(Z,2)] = Zmed(y:RESOLUTIO:size(Z,1), x:RESOLUTIO:size(Z,2)]; // Asigna promedio a Z
	 }
 }
 ZyA.A = sqrt(ZyA.A); // Se debe devolver Z tambien
	return ZyA.A;
}
