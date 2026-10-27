#include <stdio.h>
// Despliegue.c: Despliega el avance de la iteracion donde este incluida
/*
Input:
Avance - Numero entre 0 y 1, que indica estado de avance de la iteracion

Output:
No tiene
*/
void Despliegue(float Avance)
{
	int max = 10;
	char despliegue[] = "          ";
	int estadoactual = (int) (max * Avance);

    for (int i=0; i<=estadoactual ; i++)
    {
		despliegue[i] ='X';
    }

	printf("%s",despliegue);
	
}
