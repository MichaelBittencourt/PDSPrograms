/******************************************************************
 *	Programa quantizador.c                                         *
 *                                                                 *
 *  Transforma um arquivo texto de aúdio (-1 <= x < +1) em aúdio   *
 *  quantizado usando b bits.                                      *
 *                                                                 *
 *  Autor: Waslon Terllizzie Araújo Lopes                          *
 *  Data: 31/01/2017                                               *
 *                                                                 *
 *  Uso: quantizador.out entrada.dat saida.dat n_bits              *
 ******************************************************************/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "inout.h"
#define cnome 100 /* comprimento max de nome de arquivo */


double *quantizador_uniforme(double *x, int n_amos, int nbits);

int main (int argc, char **argv){

    FILE *f;
    char entrada[cnome],quant[cnome],*str;
    int n_amos,nbits,i;
    double *x,*y;

    /* Abre os arquivos de entrada e saída */
    if(argc!=4){
        printf("Qual o nome do arquivo original (*.dat)?\n");
        scanf("%s", entrada); 
        x = ler_vetor_arquivo_treino(entrada,&n_amos);

        printf("Qual o nome do arquivo de quantizado (*.dat)?\n");
        scanf("%s", quant);
        f = fopen(quant, "w");

        printf("Qual o número de bits para quantizar as amostras?\n");
        scanf("%d",&nbits);
    } 
    else {
        x = ler_vetor_arquivo_treino(argv[1],&n_amos);
        f = fopen(argv[2], "w");
        str = argv[3];
        nbits = atoi(str);
    }


    y = quantizador_uniforme(x, n_amos, nbits);

    for(i=0;i<n_amos;i++)
        fprintf(f,"%.10f\n",y[i]);

    free(x);
    free(y);
    fclose(f);
    return (1);
}


double *quantizador_uniforme(double *x, int n_amos, int nbits){
    double *y,d,e;
    int i, niveis;

    y = aloca_vetor_double(n_amos);

    niveis = (int)pow(2,nbits);
    d = 2.0/niveis;

    for(i=0;i<n_amos;i++){
        if(x[i]>=0){
            e=0;
            while(x[i]-d>=e)
                e+=d;
            y[i]=e+d/2; 
        }
        else
        {
            e=0;
            while(x[i]+d<=e)
                e-=d;
            y[i]=e-d/2;
        }
    }
    return(y);
}
