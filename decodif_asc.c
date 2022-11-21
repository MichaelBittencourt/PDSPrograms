/******************************************************************
 * Arquivo:decodif_asc.c                                           *
 *                                                                 *
 * Reconstrução de sinal quantizado vetorialmente.                 *
 *                                                                 *
 * Autor: Francisco Madeiro Bernardino Junior                      *
 *                                                                 *
 * Modificado por: Waslon Terllizzie Araújo Lopes                  *
 *                                                                 *
 * Data: 18/04/2001.                                               *
 *                                                                 *
 ******************************************************************/


#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "inout.h"
#define cnome 50 /* comprimento max. de nome de arquivo,incluindo diretorio */

void decodifica_arquivo(char entrada[], char saida[], double **x, int N, int K);
double distancia(double *pta, double *ptb, int K);

int main()
{
    char dicion[cnome], codi[cnome], decod[cnome];
    int   K, N;
    double **pta;


    printf("Qual a dimensão do quantizador?\n");
    scanf("%d", &K);

    printf("Qual o número de níveis do dicionário?\n");
    scanf("%d", &N);

    printf("Qual o nome do arquivo que contém o dicionário ?\n");
    scanf("%s", dicion);

    printf("Qual o nome do arquivo de símbolos a serem decodificados ?\n");
    scanf("%s", codi);

    printf("Qual o nome do arquivo de símbolos decodificados ?\n");
    scanf("%s", decod);


    //Alocação de memória -- vetor N x K

    pta = ler_arquivo(dicion,N,K); 

    decodifica_arquivo(codi,decod, pta, N, K);

    return (0);     

} /* fim do main */

void decodifica_arquivo(char entrada[], char saida[], double **x, int N, int K){
    /******************************************************************
     * Função que os índices armazenados no arquivo de nomes "entrada",*
     * de acordo com o dicionário "x" de dimensão N,K. Os dados de saí-*
     * são escritos no arqbuivo de nome "saida".                       *
     ******************************************************************/

    FILE *entradap, *saidap;
    int i, j;

    entradap = fopen(entrada,"r");
    if (!entradap) {
        printf("\nErro na abertura do arquivo dos dados a serem codificados\n");
        exit(1);
    }

    saidap = fopen(saida,"w");
    if (!saidap) {
        printf("\nErro na abertura do arquivo dos dados codificados\n");
        exit(1);
    }

    for(;!feof(entradap);){
        fscanf(entradap,"%d",&i);
        if( feof(entradap) ) break;
        for(j=0;j<K;j++)
            fprintf(saidap,"%f\n",*(x[i]+j));
    }

    fclose(saidap);
    fclose(entradap);

    return;
}

