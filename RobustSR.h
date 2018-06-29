/*
Definiciones globales a ser utilizadas en ROBUST
*/
#define MAXITER	 20
#define LAMBDA    0.04	
#define PVENTANA  2
#define ALPHA	  0.7
#define BETA	  1
#define RESOLUTIO 2
#define PSFKERNELSIZE	3
#define PSFSIGMA		1

#define MAXSIZELR 100

#define MaxsizeHR	( RESOLUTIO * MAXSIZELR )

#define ANCHOIMAGEN		100
#define ALTOIMAGEN		100
#define NUMIMAGENES		 15

typedef double element;

typedef int TipoImagenLR[MAXSIZELR][MAXSIZELR];
typedef int TipoImagenHR[MaxsizeHR][MaxsizeHR];
typedef int TipoKernel[PSFKERNELSIZE][PSFKERNELSIZE];

typedef struct TipoIniciales { // Se genera Z y A 
	TipoImagenHR	Z;
	TipoImagenHR	A;
};

