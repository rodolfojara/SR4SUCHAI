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

typedef double element;

typedef int TipoImagenLR[MAXSIZELR][MAXSIZELR];
typedef int TipoImagenHR[MaxsizeHR][MaxsizeHR];
typedef int TipoKernel[PSFKERNELSIZE][PSFKERNELSIZE];

typedef struct TipoIniciales { // Se genera Z y A 
	TipoImagenHR	Z;
	TipoImagenHR	A;
};

/*struct TipoImagenLR { // Se define el arreglo que contiene a la imagen LR

	int imagen[MAXSIZELR][MAXSIZELR];
};

struct TipoImagenHR { // Se define el arreglo que contiene a la imagen HR

	int imagen[MaxsizeHR][MaxsizeHR];
};

struct TipoKernel { // Se define el arreglo que contiene el filtro

	int filtro[PSFKERNELSIZE][PSFKERNELSIZE];
};
*/
/*
struct s_alumno { // se define la estructura alumno
		char nombre[alpha],
		short edad;
};

typedef struct s_alumno ALUMNO; // ALUMNO es un nuevo tipo de variable
typedef struct s_alumno *ALUMNOPTR;

struct  clase {
		ALUMNO alumnos[beta];
		char   nom_profesor[Gamma];
};

typedef struct clase CLASE;
typedef struct clase *CLASEPTR;
*/
/* declaracion de estructura tiempo */
struct tiempo { int hora, minuto, segundo;};

/* declaracion de varibles de tipoe tiempo */
struct tiempo t1, t2;
t1.hora = 12;
t1.minuto = 0;
t1.segundo = 0;

struct estructura_amigo {
	char nombre[30];
	char apellido[40];
	char telefono[10];
	int edad;
};
struct estructura_amigo amigo;
/*
 printf("Escribe el nombre del amigo: ");
 scanf("%s", &amigo.nombre);
 printf("Escribe el apellido del amigo: ");
 scanf("%s", &amigo.apellido);

*/