/* Programa para gerar um sinal senoidal */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rand.h"
#define pi 3.14159265359


int i,npt,j;

FILE *saida;

void main(void){

    if((saida=fopen("gera.dat","w"))==NULL)
    {
        printf("Nao foi possivel abrir o arquivo de saida de dados.\n");
        exit(1);
    }

    printf("\nDigite o numero de pontos a serem gerados: ");
    scanf("%d",&npt);

    for(i=-npt;i<npt;i++)
        //fprintf(saida,"%+2.2f\n",1.0*i); // rampa
        //fprintf(saida,"%.10f \n",gauss(0,0.013124)); // seno
        fprintf(saida,"%.10f  0.0 \n",sin(2*pi*2*1000*i/1000000)); // seno

    fclose(saida);

    return;

}   
